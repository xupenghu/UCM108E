#define __UART_C_

#include <string.h>
#include "Uart.h"
#include "pulpino.h"
#include "Function.h"
#include "string_lib.h"

#define REG_CLK_PLL_INT  *((volatile U32 *)0x1A104210)
#define REG_CLK_PLL_FRAC *((volatile U32 *)0x1A104214)

void InitUart(IN ENUM_UART_BSP eBsp)
{
    SetBsp(eBsp);
    REG_UART_IER = (REG_UART_IER & 0xF0) | 0x01;  // set IER (interrupt enable register) on UART
}

void SetBsp(IN ENUM_UART_BSP eBsp)
{
    U16       usClkDiv;
    const U32 nClk = 1625000;  // clk / 16, clk = 26MHz
    U32       nClkFrac, nClkInt;
    F32       fSysClk;

    nClkInt  = REG_CLK_PLL_INT & 0x1F;
    nClkFrac = (REG_CLK_PLL_FRAC >> 8) & 0x7FFFFF;
    fSysClk  = nClk * (nClkInt + nClkFrac / 8388608.0f);
    usClkDiv = (U16)(fSysClk / eBsp) - 1;  // clk_div = sysclk / bsp / 16 - 1, auto set bsp

    CGREG |= (1 << CGUART);  // don't clock gate UART
    REG_UART_LCR = 0x83;     // sets 8N1 and set DLAB to 1
    REG_UART_DLM = (usClkDiv >> 8) & 0xFF;
    REG_UART_DLL = usClkDiv & 0xFF;
    REG_UART_FCR = 0xA7;  // enables 16byte FIFO and clear FIFOs
    REG_UART_LCR = 0x03;  // sets 8N1 and set DLAB to 0
    m_eBsp       = eBsp;
}
