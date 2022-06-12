/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-11-26     RT-Thread    first version
 */

#include <rtthread.h>
#include <board.h>
#include <rtdevice.h>

#define NAV_LG          (1)
#if NAV_LG
    #define	__MAIN_C_

	#include "Uart.h"
	#include "string_lib.h"
	#include "cce_firmware_comm.h"
#ifdef _WTG_OPEN_
	#include "uc_watchdog.h"
#endif
	
	rt_thread_t g_hTaskUartTx, g_hTaskUartRx;
	
	#define	TSK_STACK_SIZE_UART_TX		(1024)
	#define	TSK_STACK_SIZE_UART_RX		(2048)
#endif

void cce_handler()
{
	TicIsr();
}

void get_pos(STU_RTC rtc, PSTU_USR_PVT pv)
{
	printf("RTC: year %d, day %d, hour %d, min %d, sec %.2o\r\n", rtc.sYear, rtc.ucDay, rtc.ucHour, rtc.ucMin, *((int *)&rtc.fSec));
	printf("POS: ST %d, tLon %o, tLat %o, fAlt: %o\r\n",pv->eState, *((int *)&pv->tLla.tLon), *((int *)&pv->tLla.tLat), *((int *)&pv->tLla.fAlt));
}

int main(void)
{
	int_disable();
	REG_INT_PEND = 0x0;
#ifdef _WTG_OPEN_
	wdt_init(UC_WATCHDOG, 5000);
	wdt_enable(UC_WATCHDOG);
#endif
	InitUart(UART_BSP_115200);
	
	
	GnssStart(get_pos, 0x7f, FALSE, NULL);
	
	g_hTaskUartTx = rt_thread_create("Task Uart Tx", TaskUartTx, NULL, TSK_STACK_SIZE_UART_TX, 10, 10);
	
	if(g_hTaskUartTx == RT_NULL)
		printf("tx task create failed!\r\n");
	else
	{
		rt_thread_startup(g_hTaskUartTx);
		printf("tx task is start!\r\n");
	}
	
	g_hTaskUartRx = rt_thread_create("Task Uart Rx", TaskUartRx, NULL, TSK_STACK_SIZE_UART_RX, 10, 10);
	
	if(g_hTaskUartRx == RT_NULL)
		printf("rx task create failed!\r\n");
	else
	{
		rt_thread_startup(g_hTaskUartRx);
		printf("rx task is start!\r\n");
	}
#ifdef _WTG_OPEN_
	wdt_feed(UC_WATCHDOG);
#endif




}

