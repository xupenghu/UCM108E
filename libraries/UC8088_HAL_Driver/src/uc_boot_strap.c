#include <uc_spi.h>
#include <uc_utils.h>
#include <pulpino.h>
#include <uc_event.h>
#include <stdint.h>
#include <sectdefs.h>


#define __critical __attribute__((section(".critical"))) 
#define __critical_64 __attribute__((section(".critical"), aligned(64))) 
#define __critical_128 __attribute__((section(".critical"), aligned(128))) 
#define __critical_256 __attribute__((section(".critical"), aligned(256))) 
#define __critical_512 __attribute__((section(".critical"), aligned(512))) 


#define REG_XIP_CTRL    0x1A10C02C
#define REG_XIP_CMD     0x1A10C030 
#define SEG_TBL_SIZE    20
#define REG_MTR_CFG_0   0x1A107040
#define REG_MTR_CFG_1   0x1A107044
#define REG_MTR_CFG_2   0x1A107048
#define REG_MTR_CFG_3   0x1A10704C
#define REG_MTR_CFG_4   0x1A107050
#define REG_MTR_CFG_5   0x1A107054
#define REG_MTR_CFG_6   0x1A107058
#define REG_MTR_CFG_7   0x1A10705C

extern uint32_t _stbl_end ;
extern uint32_t _sdata, edata;
extern uint32_t _rom_start;
uint16_t auto_dummy = 6;

#define reg_xip_ctrl    ((volatile uint32_t *) 0x1a10c02c)
#define reg_xip_cmd     ((volatile uint32_t *) 0x1A10C030)
#define reg_spi_cmd     ((volatile uint32_t *) 0x1a10c008)
#define reg_spi_addr    ((volatile uint32_t *) 0x1a10c00c)
#define reg_spi_len     ((volatile uint32_t *) 0x1a10c010)
#define reg_spi_status  ((volatile uint32_t *) 0x1a10c000)
#define reg_spi_rxfifo  ((volatile uint32_t *) 0x1a10c020)
#define reg_spi_txfifo  ((volatile uint32_t *) 0x1a10c018)
#define reg_spi_dummy   ((volatile uint32_t *) 0x1a10c014)

#define SPI_START(cmd)  (*reg_spi_status = (1<<(SPI_CSN0+8))|(1<<(cmd))); //start 
#define WAIT_XIP_FREE    while((*reg_xip_ctrl)&0x1)
#define WAIT_SPI_IDLE    while((*reg_spi_status)!=1)    
#define FLASH_QRD        0x11101011
#define FLASH_FRD        0x0B000000
#define FLASH_RD         0x03000000
#define FLASH_WE         0x06000000
#define FLASH_WSR        0x01000000
#define FLASH_RSR        0x05000000

//#define	SYS_CLK					(131072000)
#define	SYS_CLK					(163840000)


//__critical int flash_read_sr()
//{
//  uint32_t data;
//  *reg_spi_cmd = FLASH_RSR; //read sr 
//  *reg_spi_len = 0x200008;   
//  WAIT_XIP_FREE;
//  SPI_START(SPI_CMD_RD);
//  while(((*(reg_spi_status)>>16)&0xFF)==0);
//  data = *(reg_spi_rxfifo);
//  return data;
//}

static inline void deley_us(int n)
{
	for(int i=0;i<9*n;i++)//9 for DCXO
	{
		asm volatile ("nop");
	}
}

/**************************
*
*DO NOT MOVE AROUND THE FUNCTIONS!
*spi_read_fifo() and flash_read_sr() must be in cache()
*otherwise might deadlock or fetch wrong data for instr
*/
__critical_256 void xip_dummy_detect(uint8_t enable)
{
	uint32_t data;
	register int i;
	
	if(0x00 == enable) return;

	WAIT_XIP_FREE;
#if SYS_CLK == 163840000
	*(volatile int*) (SPI_REG_CLKDIV) = 0x0;
#else
	*(volatile int*) (SPI_REG_CLKDIV) = 0x1;
#endif
	*reg_spi_cmd = FLASH_FRD;  //set cmd
	*reg_spi_len = 0x201808;    //set cmd,add and data len
	*reg_spi_addr = 0x00009000;
	
	for (i=0;i<12;i++)
	{
		*(reg_spi_dummy) = i;
		WAIT_XIP_FREE;
		SPI_START(SPI_CMD_RD);
		//check SPI_RX FIFO
		while(((*(reg_spi_status)>>16)&0xFF)==0);
		data = *(reg_spi_rxfifo);
		
		if (data == 0xdeadbeef)
		{
			auto_dummy = i;
			break;
		}
	}
}
/*
__critical_256 uint32_t flash_read_id(){
  uint32_t data;
  *reg_spi_cmd = 0x9F000000;  //set cmd
  *reg_spi_len = 0x200008;      //set cmd and data len
  WAIT_XIP_FREE;
  SPI_START(SPI_CMD_RD);
  while(((*(reg_spi_status)>>16)&0xFF)==0);
  data = *(reg_spi_rxfifo);
  return data;
}
*/
//enable QSPI at flashcd
//__critical_256 void flash_qspi_en() {
//
//}

/////* this is done in SPI mode for safety */
__critical_256 void fill_mtr(uint8_t enable) {
  uint32_t stbl_flash_addr;
  uint32_t stbls[SEG_TBL_SIZE];
  uint32_t *mtr_addr = (uint32_t *) REG_MTR_CFG_0;
  register int count;
  register int i;
  uint8_t *src, *dest;
  
  if(0x00 == enable) return;
   
 /* load mtr registeres using spi  */
   //warmup read fifo, otherwise might deadlock with code fetch
   WAIT_XIP_FREE;
   spi_read_fifo((int *) stbls,0);
   stbl_flash_addr = (uint32_t)(&_stbl_end) -(uint32_t ) &_rom_start- (SEG_TBL_SIZE<<2);
   *reg_spi_cmd = FLASH_FRD;  //set cmd
   *reg_spi_addr = (stbl_flash_addr << 8);
   *reg_spi_len = 0x1808|((SEG_TBL_SIZE<<5)<<16);  //set cmd,addr and data len
   WAIT_XIP_FREE;
   SPI_START(SPI_CMD_RD);
   WAIT_XIP_FREE;
   spi_read_fifo((int *) stbls,(SEG_TBL_SIZE<<5));
   /* we only have 8 mtr regs now */
   for (i=7;i>=0;i--)
   {
       *(mtr_addr + i) = stbls[i];
   }
   /* now safe to access RO memory area */
   /* cp init data from RO to RAM */
#ifndef _NO_SDATA_
   count = ((uint32_t)&edata - (uint32_t)&_sdata);
   dest = (uint8_t *) &_sdata;
   src = (uint8_t * ) stbls[19];
   for (i=0;i<count;i++)
   {
      *(dest++) = *(src++);
   }
#endif 
}


///* this is done in SPI mode for safety */
__critical_256 void fill_mtr_qspi() {
  uint32_t stbl_flash_addr;
  uint32_t stbls[SEG_TBL_SIZE];
  uint32_t *mtr_addr = (uint32_t *) REG_MTR_CFG_0;
  register int count;
  register int i;
  uint8_t *src, *dest;

 /* load mtr registeres using spi  */
   //warmup read fifo, otherwise might deadlock with code fetch
   spi_read_fifo((int *) stbls,0);
   stbl_flash_addr = (uint32_t)(&_stbl_end) -(uint32_t ) &_rom_start- (SEG_TBL_SIZE<<2);
   *reg_spi_cmd = FLASH_QRD;  //set cmd
   *reg_spi_addr = (stbl_flash_addr << 8);
   *reg_spi_len = 0x1820|((SEG_TBL_SIZE<<5)<<16);  //set cmd,addr and data len
   WAIT_XIP_FREE;
   SPI_START(SPI_CMD_QRD);
   spi_read_fifo((int *) stbls,(SEG_TBL_SIZE<<5));
   /* we only have 8 mtr regs now */
   for (i=7;i>=0;i--)
   {
       *(mtr_addr + i) = stbls[i];
   }
   /* now safe to access RO memory area */
   /* cp init data from RO to RAM */
#ifndef _NO_SDATA_
   count = ((uint32_t)&edata - (uint32_t)&_sdata);
   dest = (uint8_t *) &_sdata;
   src = (uint8_t * ) stbls[19];
   for (i=0;i<count;i++)
   {
      *(dest++) = *(src++);
   }
#endif 
}

__critical_256 void boot_strap()
{
	/* turn off device IRQ */
	//this is done in reset handler now;
	volatile uint8_t enable = 0x00;
	IER = 0;
	IPR = 0;
//	WAIT_XIP_FREE; 
//  *reg_xip_cmd = FLASH_FRD;
//  *reg_xip_ctrl |=  8<<3;
//  *reg_xip_ctrl |= 0x00008000;

	volatile uint32_t * ptr4 = (volatile uint32_t*)(0x1a104204);
	*ptr4 &= (~(0x7<<15));
	*ptr4 |= (0x1<<18);
	*ptr4 &= (~(0x1<<19));
	*ptr4 |= (0xf<<20);

	//1.9v  010
	volatile uint32_t * ptr5 = (volatile uint32_t*)(0x1a10422c);

	*ptr5 &= (~(0x0<<28));
	*ptr5 |= (0x1<<29);
	*ptr5 &= (~(0x0<<30));
	
	xip_dummy_detect(enable);
	fill_mtr(enable);
//  fill_mtr();
//  flash_read_id();

 /* critical function, enable flash QSPI Mode */
//  flash_qspi_en();

//-----------------------------------------------------------------------------
  /*************************  
   *   XIP Controller Defs
   * bit 0 RO - sts, 1=busy;BIT 1, Enable Flash Fetch. CAUTION! default 1
   * bit 2 SPI mode 0=SPI 1=QSPI
   * bit 3:6 Dummy cycles as in specs, must add extra 2 for modebits.see pdf
   * bit 7 rsv 
   * bit 8:13 spi_cmd_len in bits, default 8bits.
   * xip control Enable+QSPI READ+4+2Dummy. 2Extra mode bits as for s256x4.
   * cmd is send as 32bit so on SPI is 8bits. 0xEB is only send out on 
   * sdo0,  cmd expand from 8 to 32...
   * so register valule bit 3-6 is 4b'0110 
   * 0x00002036, 6 dummy,QSPI, 32bit cmd
   * 0x0000203E, 7 dummy, QSPI, 32bit cmd
   * 0x00002046, 8 dymmy, QSPI, 32bit cmdr
   * 0x00002056, 10 dummy,QSPI, 32bit cmd
   **************************
  */
//-----------------------------------------------------------------------------
//  WAIT_XIP_FREE;
//  *reg_xip_ctrl |= 0x00008000;

  *((volatile uint32_t *)0x1a104218) = 0x4FAB5555;
#if SYS_CLK == 163840000
  *((volatile uint32_t *)0x1a104210) = 0x899957E6;
  *((volatile uint32_t *)0x1a104214) = 0xA698D080;
#else
  *((volatile uint32_t *)0x1a104210) = 0x899917E5;
  *((volatile uint32_t *)0x1a104214) = 0x85470D80;
#endif
  /* switch from DCXO clk to PLL clk */
  WAIT_XIP_FREE;
  PM_CPC = 0xf1;
  REG(0x1a10420c) = 0x4100bc88;//power fast on pull up
  deley_us(600);
  REG(0x1a10420c) = 0x4000bc88;//power fast on pull down
  deley_us(100);
  xip_dummy_detect(0x01);
  *((volatile uint32_t *)(REG_XIP_CMD)) = FLASH_FRD;
  //now change XIP controller to us
  *reg_xip_ctrl = (0x000802|auto_dummy<<3);
  fill_mtr(0x01);
//  fill_mtr_qspi();
  return;
}

__critical_256 void boot_noop()
{
    __asm__ ("nop");
}


