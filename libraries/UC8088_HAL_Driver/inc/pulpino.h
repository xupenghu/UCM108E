// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

/**
 * @file
 * @brief Register mapping for PULPino peripherals.
 *
 * Contains event register mappings for the PULPino SOC as
 * well as some general definitions for the overall system.
 *
 * @author Florian Zaruba
 *
 * @version 1.0
 *
 * @date 2/10/2015
 *
 */
#ifndef PULPINO_H
#define PULPINO_H

#include "uc_core.h" 
#include "stdint.h"

#define PULPINO_BASE_ADDR             0x10000000

/** SOC PERIPHERALS */
#define SOC_PERIPHERALS_BASE_ADDR     ( PULPINO_BASE_ADDR + 0xA100000 )

#define UART0_BASE_ADDR               ( SOC_PERIPHERALS_BASE_ADDR + 0x0000 )
#define UART1_BASE_ADDR               ( SOC_PERIPHERALS_BASE_ADDR + 0x0020 )
#define GPIO_BASE_ADDR                ( SOC_PERIPHERALS_BASE_ADDR + 0x1000 )
#define SPIM_BASE_ADDR                ( SOC_PERIPHERALS_BASE_ADDR + 0x2000 )
#define TIMER0_BASE_ADDR              ( SOC_PERIPHERALS_BASE_ADDR + 0x3000 )
#define TIMER1_BASE_ADDR              ( SOC_PERIPHERALS_BASE_ADDR + 0x3010 )
#define EVENT_UNIT_BASE_ADDR          ( SOC_PERIPHERALS_BASE_ADDR + 0x4000 )
#define EVTC_UNIT_BASE_ADDR           ( SOC_PERIPHERALS_BASE_ADDR + 0x4000 )
#define PMU_BASE_ADDR                 ( SOC_PERIPHERALS_BASE_ADDR + 0x4200 )
#define RTC_BASE_ADDR                 ( SOC_PERIPHERALS_BASE_ADDR + 0x4300 )
#define WATCHDOG_BASE_ADDR            ( SOC_PERIPHERALS_BASE_ADDR + 0x4400 )
#define I2C_BASE_ADDR                 ( SOC_PERIPHERALS_BASE_ADDR + 0x5000 )
#define FLL_BASE_ADDR                 ( SOC_PERIPHERALS_BASE_ADDR + 0x6000 )
#define SOC_CTRL_BASE_ADDR            ( SOC_PERIPHERALS_BASE_ADDR + 0x7000 )
#define SMARTCARD_BASE_ADDR           ( SOC_PERIPHERALS_BASE_ADDR + 0x8000 )
#define ADDC_BASE_ADDR		          ( SOC_PERIPHERALS_BASE_ADDR + 0x9000 )


#define PWM_BASE_ADDR                 ( SOC_PERIPHERALS_BASE_ADDR + 0xB000 )
#define SPI_BASE_ADDR                 ( SOC_PERIPHERALS_BASE_ADDR + 0xC000 )

/** STDOUT */
#define STDOUT_BASE_ADDR              ( SOC_PERIPHERALS_BASE_ADDR + 0x10000 )
#define FPUTCHAR_BASE_ADDR            ( STDOUT_BASE_ADDR + 0x1000 )
#define FILE_CMD_BASE_ADDR            ( STDOUT_BASE_ADDR + 0x2000 )
#define STREAM_BASE_ADDR              ( STDOUT_BASE_ADDR + 0x3000 )

/** Instruction RAM */
#define INSTR_RAM_BASE_ADDR           ( 0x00       )
#define INSTR_RAM_START_ADDR          ( 0x80       )

/** ROM */
#define ROM_BASE_ADDR                 ( 0x00200000 )

/** Data RAM */
#define DATA_RAM_BASE_ADDR            ( 0x00300000 )

/** CCE SHADOW MEM */
#define CCE_RAM_BANK0_BASE_ADDR       ( 0x00380000 )
#define CCE_RAM_BANK1_BASE_ADDR       ( 0x00390000 )
#define CCE_REG_BASE_ADDR             ( 0x003A0000 )
#define TABROM_ADDR                   ( 0x003C0000 )

#define SYSTEM_CLK                    ( 131000000)

///** Registers and pointers */
#define REGP(x) ((volatile unsigned int*)(x))
#define REG(x) (*((volatile unsigned int*)(x)))
#define REGP_8(x) (((volatile uint8_t*)(x)))

/* pointer to mem of apb pulpino unit - PointerSocCtrl */
#define __PSC__(a) *(unsigned volatile int*) (SOC_CTRL_BASE_ADDR + a)

/** Peripheral Glock gating */
#define CGREG __PSC__(0x04)

/** Glock gate SPI */
#define CGSPI     0x00
/** Glock gate UART */
#define CGUART    0x01
/** Glock gate GPIO */
#define CGGPIO    0x02
/** Glock gate SPI Master */
#define CGGSPIM   0x03
/** Glock gate Timer */
#define CGTIM     0x04
/** Glock gate Event Unit */
#define CGEVENT   0x05
/** Glock gate I2C */
#define CGGI2C    0x06
/** Glock gate FLL */
#define CGFLL     0x07

/** Boot address register */
#define BOOTREG     __PSC__(0x08)

#define RES_STATUS  __PSC__(0x14)



typedef enum {
    ENABLE = 1,
    DISABLE = 0
}FunctionalState;


typedef struct {
   union {
   __I  uint8_t  RBR;
   __O  uint8_t  THR;
   __IO uint8_t  DLL;
        uint32_t RESERVED0;
   };
   __IO uint8_t  DLM;
   __IO uint8_t  RESERVED1[3];
   __IO uint8_t  FCR;
   __IO uint8_t  RESERVED2[3];
   __IO uint8_t  LCR;
   __IO uint8_t  RESERVED3[3];
   __IO uint8_t  MCR;
   __IO uint8_t  RESERVED4[3];
   __IO uint8_t  LSR;
   __IO uint8_t  RESERVED5[3];
   __IO uint8_t  MSR;
   __IO uint8_t  RESERVED6[3];
   __IO uint8_t  SCR;
   __IO uint8_t  RESERVED7[3];
   __IO uint8_t  DIR;
   __IO uint8_t  RESERVED8[3];     /* */
   __IO uint8_t  RCR;
   __IO uint8_t  RESERVED9[3];     /* rst pin control reg */
}SMARTCARD_TYPE;


typedef struct{
    __IO    uint32_t    CTRL;
    __I     uint32_t    TIM0;
    __I     uint32_t    TIM1;
    __IO    uint32_t    TS0;
    __IO    uint32_t    TS1;
    __IO    uint32_t    AS0;
    __IO    uint32_t    AS1;
    __IO    uint32_t    ACTRL;
        
}RTC_TypeDef;                   /* RTC Struct */


typedef struct {
   union {
   __I  uint8_t  RBR;
   __O  uint8_t  THR;
   __IO uint8_t  DLL;
        uint32_t RESERVED0;
   };
   union {
   __IO uint8_t  DLM;
   __IO uint8_t  IER;
        uint32_t RESERVED1;
   };
   union {
   __IO uint8_t  IIR;
   __IO uint8_t  FCR;
        uint32_t RESERVED2;
   };
   __IO uint8_t  LCR;
   __IO uint8_t  RESERVED3[3];
   __IO uint8_t  MCR;
   __IO uint8_t  RESERVED4[3];
   __IO uint8_t  LSR;
   __IO uint8_t  RESERVED5[3];
   __IO uint8_t  MSR;
   __IO uint8_t  RESERVED6[3];
   __IO uint8_t  SCR;
   __IO uint8_t  RESERVED7[3];
}UART_TYPE;


typedef struct {
   __IO uint32_t  CPR;
   __IO uint32_t  CTR;
   __O  uint32_t  RXR;
   __I  uint32_t  STR;
   __IO uint32_t  TXR;
   __IO uint32_t  CDR;
}I2C_TYPE;


typedef struct {
   __IO uint32_t  TRR;
   __IO uint32_t  CTR;
   __IO uint32_t  CMP;
}TIMER_TYPE;

typedef struct {
   __IO uint32_t  CTR;
   __IO uint32_t  WIV;
   __IO uint32_t  WFD;
}WDG_TYPE;

//yangxiong -- 20210918
typedef struct {
   __IO uint32_t  EE;
   __IO uint32_t  EP;
   __IO uint32_t  ES;
   __IO uint32_t  EM;
}EVTC_TYPE;

//yangxiong -- 20210916
typedef struct {
   __IO uint32_t  PC;
   __IO uint32_t  DLC;
   __IO uint32_t  CC;
   __IO uint32_t  CC0;
   __IO uint32_t  CC1;
   __IO uint32_t  CC2;
   __IO uint32_t  CC3;
   __IO uint32_t  DC0;
   __IO uint32_t  DC1;
   __IO uint32_t  CHG_C;
   __IO uint32_t  CRC;
   __IO uint32_t  IIC;
   __IO uint32_t  BC;
   __IO uint32_t  RR;
}PMU_TYPE;

typedef struct{
    __IO    uint32_t    STATUS;
    __IO    uint32_t    CLKDIV;
    __IO    uint32_t    SPICMD;
    __IO    uint32_t    SPIADR;
    __IO    uint32_t    SPILEN;
    __IO    uint32_t    SPIDUM;
    __O     uint32_t    TXFIFO;
    __O     uint32_t    RESERVED;
    __IO    uint32_t    RXFIFO;
    __IO    uint32_t    INTCFG;
	__I     uint32_t    INTSTA;
}SPI_TypeDef;

typedef struct{
    __IO    uint32_t    PADDIR;
    __IO    uint32_t    PADIN;
    __IO    uint32_t    PADOUT;
    __IO    uint32_t    INTEN;
    __IO    uint32_t    INTTYPE0;
    __IO    uint32_t    INTTYPE1;
    __IO    uint32_t    INTSTATUS;
}GPIO_TypeDef;


typedef struct{
    __IO   uint32_t     PADMUX;
    const  uint32_t     RESERVED[7];
    __IO   uint32_t     PADCFG;
	
}GPIO_CFG_TypeDef;


typedef struct{
    __IO    uint32_t    CNTMAX;
    __IO    uint32_t    CTRL;
    __IO    uint32_t    DUTY;
}PWM_TypeDef;

typedef struct{
    __IO    uint32_t    ADC_CTRL0;
    __IO    uint32_t    ADC_CTRL1;
    __IO    uint32_t    ADDA_IRQ_CTRL;
    __I     uint32_t    ADC_FIFO_READ;
    __O     uint32_t    DAC_FIFO_WRITE;
    __IO    uint32_t    ADC_FIFO_CTRL;
    __IO    uint32_t    DAC_FIFO_CTRL;
    __IO    uint32_t    DAC_CLK_DIV;
    __IO    uint32_t    AUX_DAC_LV;
}ADDA_TypeDef;

#define UC_SMARTCARD            ((SMARTCARD_TYPE    *) SMARTCARD_BASE_ADDR) 
#define UC_RTC                  ((RTC_TypeDef       *) RTC_BASE_ADDR)
#define UC_SPIM                 ((SPI_TypeDef       *) SPIM_BASE_ADDR)
#define UC_TIMER0               ((TIMER_TYPE        *) TIMER0_BASE_ADDR)
#define UC_TIMER1               ((TIMER_TYPE        *) TIMER1_BASE_ADDR)
#define UC_I2C                  ((I2C_TYPE          *) I2C_BASE_ADDR)
#define UC_WATCHDOG             ((WDG_TYPE          *) WATCHDOG_BASE_ADDR)
#define UC_EVTC                 ((EVTC_TYPE         *) EVTC_UNIT_BASE_ADDR)		//yangxiong -- 20210916
#define UC_PMU                  ((PMU_TYPE          *) PMU_BASE_ADDR)			//yangxiong -- 20210916
#define UC_GPIO                 ((GPIO_TypeDef      *) GPIO_BASE_ADDR)
#define UC_GPIO_CFG             ((GPIO_CFG_TypeDef  *) SOC_CTRL_BASE_ADDR)
#define UC_PWM0                 ((PWM_TypeDef       *) (PWM_BASE_ADDR+0x00))
#define UC_PWM1                 ((PWM_TypeDef       *) (PWM_BASE_ADDR+0x10))
#define UC_PWM2                 ((PWM_TypeDef       *) (PWM_BASE_ADDR+0x20))
#define UC_PWM3                 ((PWM_TypeDef       *) (PWM_BASE_ADDR+0x30))
#define UC_ADDA                 ((ADDA_TypeDef      *) ADDC_BASE_ADDR)
#define UC_UART0                ((UART_TYPE         *) UART0_BASE_ADDR)
#define UC_UART1                ((UART_TYPE         *) UART1_BASE_ADDR)

#endif
