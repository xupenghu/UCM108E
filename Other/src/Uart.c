#define	__UART_C_

#include <string.h>
#include "Uart.h"
#include "pulpino.h"
#include "Function.h"
#include "string_lib.h"

#define	REG_CLK_PLL_INT			*((volatile U32 *)0x1A104210)
#define	REG_CLK_PLL_FRAC		*((volatile U32 *)0x1A104214)

void parse_at_cmd_sys(U08 *atCont, const U08 npara, const S16 atLen, U08 *paralen, U16 *parahead);
void split_at_cmd(const U08 *atCont, const S16 atLen, U08 *npara, U08 *paralen, U16 *paraHeadIdx);

void InitUart(IN ENUM_UART_BSP eBsp)
{
	SetBsp(eBsp);
	g_tQueUartTx.sRead = 0;
	g_tQueUartTx.sWrite = 0;
	
	g_tQueUartRx.sRead = 0;
	g_tQueUartRx.sWrite = 0;
	
	g_hSemUartTx = rt_sem_create("tx", 0, 0);
	g_hSemUartRx = rt_sem_create("rx", 0, 0);
	
	if(NULL == g_hSemUartTx || NULL == g_hSemUartRx)
		printf("Uart Tx/Rx Semaphore create failed!\r\n");
	
	REG_UART_IER = (REG_UART_IER & 0xF0) | 0x01; // set IER (interrupt enable register) on UART
}

void SetBsp(IN ENUM_UART_BSP eBsp)
{
	U16 usClkDiv;
	const U32 nClk = 1625000;	// clk / 16, clk = 26MHz
	U32 nClkFrac, nClkInt;
	F32 fSysClk;
	
	nClkInt = REG_CLK_PLL_INT & 0x1F;
	nClkFrac = (REG_CLK_PLL_FRAC >> 8) & 0x7FFFFF;
	fSysClk = nClk * (nClkInt + nClkFrac / 8388608.0f);
	usClkDiv = (U16)(fSysClk / eBsp) - 1;		// clk_div = sysclk / bsp / 16 - 1, auto set bsp
	
	CGREG |= (1 << CGUART);		// don't clock gate UART
	REG_UART_LCR = 0x83;		// sets 8N1 and set DLAB to 1
	REG_UART_DLM = (usClkDiv >> 8) & 0xFF;
	REG_UART_DLL = usClkDiv & 0xFF;
	REG_UART_FCR = 0xA7;		// enables 16byte FIFO and clear FIFOs
	REG_UART_LCR = 0x03;		// sets 8N1 and set DLAB to 0
	m_eBsp = eBsp;
}

void UartSend(IN const U08 *pData, IN U16 usLen)
{
	U16 usLenTmp;
	
	usLenTmp = (g_tQueUartTx.sWrite >= g_tQueUartTx.sRead) ? (g_tQueUartTx.sWrite - g_tQueUartTx.sRead) : (UART_BUF_LEN + g_tQueUartTx.sWrite - g_tQueUartTx.sRead);
	
	if((usLenTmp + usLen) > UART_BUF_LEN)
	{
		printf("uart print over flow\r\n");
		return;
	}
	
	int_disable();
	
	if((g_tQueUartTx.sWrite + usLen) <= UART_BUF_LEN)
	{
		MemoryCopy(g_tQueUartTx.ucBuf + g_tQueUartTx.sWrite, pData, usLen);
	}
	else
	{
		register U16 usTmp;
		
		usTmp = UART_BUF_LEN - g_tQueUartTx.sWrite;
		MemoryCopy(g_tQueUartTx.ucBuf + g_tQueUartTx.sWrite, pData, usTmp);
		MemoryCopy(g_tQueUartTx.ucBuf, pData + usTmp, usLen - usTmp);
	}
	
	g_tQueUartTx.sWrite = (g_tQueUartTx.sWrite + usLen) % UART_BUF_LEN;
	int_enable();
	rt_sem_release(g_hSemUartTx);
}

void uart0_handler()
{
	switch(REG_UART_IIR & 0xF)
	{
	case UART_INT_ERR:	// receiver line status
		break;
	case UART_INT_DAT:	// received data available
	{
		register S16 sLen, sRxLen, i;
		register U08 *pBuf = g_tQueUartRx.ucBuf + g_tQueUartRx.sWrite;
		
		sRxLen = UART_RX_LVL;
		sLen = UART_BUF_LEN - g_tQueUartRx.sWrite;
		
		if(sLen >= sRxLen)
		{
			for(i = 0; i < sRxLen; i++)
				*pBuf++ = REG_UART_RBR;
		}
		else
		{
			for(i = 0; i < sLen; i++)
				*pBuf++ = REG_UART_RBR;
			
			pBuf = g_tQueUartRx.ucBuf;
			
			for(i = sLen; i < sRxLen; i++)
				*pBuf++ = REG_UART_RBR;
		}
		
		g_tQueUartRx.sWrite = (g_tQueUartRx.sWrite + sRxLen) % UART_BUF_LEN;
		rt_sem_release(g_hSemUartRx);
		break;
	}
	case UART_INT_TMT:	// character timeout
		while(REG_UART_LSR & 0x1)
		{
			g_tQueUartRx.ucBuf[g_tQueUartRx.sWrite++] = REG_UART_RBR;
			g_tQueUartRx.sWrite = g_tQueUartRx.sWrite % UART_BUF_LEN;
		}
		
		rt_sem_release(g_hSemUartRx);
		break;
	case UART_INT_TSP:	// transmitter holding register empty
		rt_sem_release(g_hSemUartTx);
		break;
	case UART_INT_MDM:	// modem status
		break;
	default:
		break;
	}
	
	REG_INT_CLEAR |= (1 << MASK_BIT_UART0);
}

void TaskUartTx(void *pvParameters)
{
	while(1)
	{
		if(rt_sem_take(g_hSemUartTx, RT_WAITING_FOREVER) == RT_EOK)
		{
			while(g_tQueUartTx.sWrite != g_tQueUartTx.sRead)
			{
				if((REG_UART_LSR & 0x20) == 0x20)
				{
					REG_UART_THR = *(g_tQueUartTx.ucBuf + g_tQueUartTx.sRead);
					g_tQueUartTx.sRead = (g_tQueUartTx.sRead + 1) % UART_BUF_LEN;
				}
			}
		}
	}
}

void TaskUartRx(void *pvParameters)
{
	REG_INT_ENABLE |= (1 << MASK_BIT_UART0);	// enable CCE interrupt for PULPino event handler
	
	while(1)
	{
		if(rt_sem_take(g_hSemUartRx, 1000) == RT_EOK)
		{
			while(g_tQueUartRx.sWrite != g_tQueUartRx.sRead)
			{
				U08 ucBuf[256];
				S16 i, sLen, sIdx, sDataLen;
				
				sLen = (g_tQueUartRx.sWrite >= g_tQueUartRx.sRead) ? (g_tQueUartRx.sWrite - g_tQueUartRx.sRead) : (g_tQueUartRx.sWrite + UART_BUF_LEN - g_tQueUartRx.sRead);
				
				if(sLen <= 10)
					break;
				
				sIdx = g_tQueUartRx.sRead;
				ucBuf[0] = g_tQueUartRx.ucBuf[sIdx];
				
				if(ucBuf[0] == '$')
				{
					BOOL bTrue = FALSE;
					
					sDataLen = 1;
					g_tQueUartRx.sRead = (sIdx + 1) % UART_BUF_LEN;
					
					for(i = 1; i < sLen; i++)
					{
						switch(g_tQueUartRx.ucBuf[g_tQueUartRx.sRead])
						{
						case '$':
							sIdx = g_tQueUartRx.sRead;
							sDataLen = 0;
							break;
						default:
							break;
						}
						
						ucBuf[sDataLen++] = g_tQueUartRx.ucBuf[g_tQueUartRx.sRead++];
						g_tQueUartRx.sRead = g_tQueUartRx.sRead % UART_BUF_LEN;
						
						if(sDataLen > 5 && ucBuf[sDataLen - 2] == '\r' && ucBuf[sDataLen - 1] == '\n')
						{
							bTrue = TRUE;
							break;
						}
					}
					
					if(FALSE == bTrue)
						g_tQueUartRx.sRead = sIdx;
					else
					{
						U08 npara, paralen[MAX_AT_PNUM];
						U16 parahead[MAX_AT_PNUM];
						
						split_at_cmd(ucBuf, sDataLen - 2, &npara, paralen, parahead);
						parse_at_cmd_sys(ucBuf, npara, sDataLen, paralen, parahead);
					}
				}
				else
					g_tQueUartRx.sRead = (sIdx + 1) % UART_BUF_LEN;
				
				rt_thread_mdelay(1);
			}
		}
	}
}
