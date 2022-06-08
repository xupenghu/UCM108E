/*
 * Copyright (C) 2020 UCCHIP CO., LTD. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *    1. Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *    2. Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the
 *       distribution.
 *    3. Neither the name of UCCHIP CO., LTD. nor the names of
 *       its contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _EVTC_H_
#define _EVTC_H_

#include <pulpino.h>


#define PARAM_EVTC(EVTC)              (EVTC==UC_EVTC)

typedef enum
{
	RTC_ALARM_EVTC        = 0,
	CCE_EVTC,
	PULSE_COUNT_EVTC,
	UDMA_TANS_DONE_EVTC,
	ACCESS_DENIED_EVTC,
	
	ADC_WATERLINE_EVTC    = 20,
	DAC_WATERLINE_EVTC,
	I2C_EVTC,
	UART0_EVTC,
	UART1_EVTC,
	GPIO_EVTC,
	APIM_WATERLINE_EVTC,
	SPIM_TANS_DONE_EVTC,
	TIMER0_OVERFLOW_EVTC,
	TIMER0_COMP_EVTC,
	TIMER1_OVERFLOW_EVTC,
	TIMER1_COMP_EVTC,
}EVTC_TYPE_t;

typedef enum
{
	EVENT_UNPENDING  = 0,
	EVENT_PENDING,
}EVTC_STATUS_t;

extern void evtc_enable(EVTC_TYPE *EVTC, EVTC_TYPE_t evtc_type);
extern void evtc_disable(EVTC_TYPE *EVTC, EVTC_TYPE_t evtc_type);
extern void evtc_set_pending(EVTC_TYPE *EVTC, EVTC_TYPE_t evtc_type);
extern void evtc_set_unpend(EVTC_TYPE *EVTC, EVTC_TYPE_t evtc_type);
extern uint8_t evtc_get_status(EVTC_TYPE *EVTC, EVTC_TYPE_t evtc_type);
extern void evtc_trig_event(EVTC_TYPE *EVTC, EVTC_TYPE_t evtc_type);
extern void evtc_shield_masks(EVTC_TYPE *EVTC, EVTC_TYPE_t evtc_type);
extern void evtc_unshield_masks(EVTC_TYPE *EVTC, EVTC_TYPE_t evtc_type);


#endif
