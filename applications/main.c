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

#define NAV_LG (1)
#if NAV_LG
#define __MAIN_C_

#include "Uart.h"
#include "string_lib.h"
#include "cce_firmware_comm.h"
#ifdef _WTG_OPEN_
#include "uc_watchdog.h"
#endif

rt_thread_t g_hTaskUartTx, g_hTaskUartRx;

#define TSK_STACK_SIZE_UART_TX (1024)
#define TSK_STACK_SIZE_UART_RX (2048)
#endif

void cce_handler()
{
    TicIsr();
}

void get_pos(STU_RTC rtc, PSTU_USR_PVT pv)
{
    printf("Time: %d/%d/%d %d:%d:%.2o\r\n", rtc.sYear, rtc.ucMon, rtc.ucDay, (rtc.ucHour + 8) % 24, rtc.ucMin,
           *((int *)&rtc.fSec));
    printf("POS: ST %d, tLon %o, tLat %o, fAlt: %o\r\n", pv->eState, *((int *)&pv->tLla.tLon), *((int *)&pv->tLla.tLat),
           *((int *)&pv->tLla.fAlt));
}
#define _WTG_OPEN_

int main(void)
{
    int_disable();
    REG_INT_PEND = 0x0;
#ifdef _WTG_OPEN_
    wdt_init(UC_WATCHDOG, 5000);
    wdt_enable(UC_WATCHDOG);
#endif
    InitUart(UART_BSP_115200);

    printf("hello wrold.\r\n");
    GnssStart(get_pos, 0x7f, TRUE, NULL);
    while (1) {
        rt_thread_delay(1000);
#ifdef _WTG_OPEN_
        wdt_feed(UC_WATCHDOG);
#endif
    }
}
