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

#include<uc_evtc.h>

/******************************************************************************
 * xyang
 * enable evtc
 * 20210917
 */
void evtc_enable(EVTC_TYPE *EVTC, EVTC_TYPE_t evtc_type)
{
	CHECK_PARAM(PARAM_EVTC(EVTC));
	
	//error event type
	if((evtc_type < RTC_ALARM_EVTC) || (evtc_type > TIMER1_COMP_EVTC))
	{
		return;
	}
	
	EVTC->EE |= (1<<evtc_type);
}

/******************************************************************************
 * xyang
 * disable evtc
 * 20210922
 */
void evtc_disable(EVTC_TYPE *EVTC, EVTC_TYPE_t evtc_type)
{
	CHECK_PARAM(PARAM_EVTC(EVTC));
	
	//error event type
	if((evtc_type < RTC_ALARM_EVTC) || (evtc_type > TIMER1_COMP_EVTC))
	{
		return;
	}
	
	EVTC->EE &= ~(1<<evtc_type);
}

/******************************************************************************
 * xyang
 * set event status
 * 20210922
 */
void evtc_set_pending(EVTC_TYPE *EVTC, EVTC_TYPE_t evtc_type)
{
	CHECK_PARAM(PARAM_EVTC(EVTC));
	
	//error event type
	if((evtc_type < RTC_ALARM_EVTC) || (evtc_type > TIMER1_COMP_EVTC))
	{
		return;
	}
	
	EVTC->EP |= (1<<evtc_type);
}

/******************************************************************************
 * xyang
 * set event status
 * 20210922
 */
void evtc_set_unpend(EVTC_TYPE *EVTC, EVTC_TYPE_t evtc_type)
{
	CHECK_PARAM(PARAM_EVTC(EVTC));
	
	//error event type
	if((evtc_type < RTC_ALARM_EVTC) || (evtc_type > TIMER1_COMP_EVTC))
	{
		return;
	}
	
	EVTC->EP &= ~(1<<evtc_type);
}

/******************************************************************************
 * xyang
 * get event status
 * 20210922
 */
uint8_t evtc_get_status(EVTC_TYPE *EVTC, EVTC_TYPE_t evtc_type)
{
	uint8_t ret = 0;
	
	CHECK_PARAM(PARAM_EVTC(EVTC));
	
	//error event type
	if((evtc_type < RTC_ALARM_EVTC) || (evtc_type > TIMER1_COMP_EVTC))
	{
		return ret;
	}
	
	if(EVTC->EP & (1<<evtc_type))
	{
		ret = EVENT_PENDING;
	}
	else
	{
		ret = EVENT_UNPENDING;
	}
	return ret;
}

/******************************************************************************
 * xyang
 * triger event
 * 20210922
 */
void evtc_trig_event(EVTC_TYPE *EVTC, EVTC_TYPE_t evtc_type)
{
	CHECK_PARAM(PARAM_EVTC(EVTC));
	
	//error event type
	if((evtc_type < RTC_ALARM_EVTC) || (evtc_type > TIMER1_COMP_EVTC))
	{
		return;
	}
	
	EVTC->ES |= (1<<evtc_type);
}

/******************************************************************************
 * xyang
 * shileld event
 * 20210922
 */
void evtc_shield_masks(EVTC_TYPE *EVTC, EVTC_TYPE_t evtc_type)
{
	CHECK_PARAM(PARAM_EVTC(EVTC));
	
	//error event type
	if((evtc_type < RTC_ALARM_EVTC) || (evtc_type > TIMER1_COMP_EVTC))
	{
		return;
	}
	
	EVTC->EM |= (1<<evtc_type);
}

/******************************************************************************
 * xyang
 * unblock event
 * 20210922
 */
void evtc_unshield_masks(EVTC_TYPE *EVTC, EVTC_TYPE_t evtc_type)
{
	CHECK_PARAM(PARAM_EVTC(EVTC));
	
	//error event type
	if((evtc_type < RTC_ALARM_EVTC) || (evtc_type > TIMER1_COMP_EVTC))
	{
		return;
	}
	
	EVTC->EM &= ~(1<<evtc_type);
}

