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

#include<uc_pmu.h>

/******************************************************************************
 * xyang
 * stop run,continue by event trig
 * 20210917
 */
void pmu_enter_idle(PMU_TYPE *PMU)
{
	CHECK_PARAM(PARAM_PMU(PMU));

	PMU->PC |= (1<<11);
	asm("wfi");//asm("wfe");
}

/******************************************************************************
 * xyang
 * stop run,continue by event irq
 * 20210917
 */
void pmu_enter_sleep(PMU_TYPE *PMU, EX_WAKEUP_MASK_t method)
{
	CHECK_PARAM(PARAM_PMU(PMU));
	
	if(method == EX_WAKEUP_EN)
	{
		PMU->PC &= ~(1<<16);
	}
	
	PMU->PC |= (1<<3);
	PMU->PC |= (1<<0);
	
	asm("wfi");
}

/******************************************************************************
 * xyang
 * stop run,continue by event irq
 * 20210917
 */
void pmu_enter_deepsleep(PMU_TYPE *PMU, EX_WAKEUP_MASK_t method)
{
	CHECK_PARAM(PARAM_PMU(PMU));
	
	if(method == EX_WAKEUP_EN)
	{
		PMU->PC &= ~(1<<16);
	}
	
	PMU->PC |= (1<<3);
	PMU->PC |= (1<<1);
}
