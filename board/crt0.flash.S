// Copyright 2016 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License. You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

 
#define EXCEPTION_STACK_SIZE 96

#include "rtconfig.h"

#ifdef ARCH_RISCV_FPU
#define FSTORE                  fsw
#define FLOAD                   flw
#define FREGBYTES               4  	 
#endif

/* ========================================================= [ entry ] === */
.section .reset  
reset_handler:
  csrw mtvec, x0
  csrci  mstatus, 0x08
  /* set all registers to zero */
  mv  x1, x0
  mv  x2, x1
  mv  x3, x1
  mv  x4, x1
  mv  x5, x1
  mv  x6, x1  
  mv  x7, x1 
  mv  x8, x1  
  mv  x9, x1 
  mv x10, x1
  mv x11, x1
  mv x12, x1
  mv x13, x1
  mv x14, x1
  mv x15, x1
  mv x16, x1
  mv x17, x1
  mv x18, x1
  mv x19, x1
  mv x20, x1
  mv x21, x1
  mv x22, x1
  mv x23, x1
  mv x24, x1
  mv x25, x1
  mv x26, x1
  mv x27, x1
  mv x28, x1
  mv x29, x1
  mv x30, x1
  mv x31, x1

#ifdef ARCH_RISCV_FPU
  fssr    x0
  fmv.s.x f0, x0
  fmv.s.x f1, x0
  fmv.s.x f2, x0
  fmv.s.x f3, x0
  fmv.s.x f4, x0
  fmv.s.x f5, x0
  fmv.s.x f6, x0
  fmv.s.x f7, x0
  fmv.s.x f8, x0
  fmv.s.x f9, x0
  fmv.s.x f10,x0
  fmv.s.x f11,x0
  fmv.s.x f12,x0
  fmv.s.x f13,x0
  fmv.s.x f14,x0
  fmv.s.x f15,x0
  fmv.s.x f16,x0
  fmv.s.x f17,x0
  fmv.s.x f18,x0
  fmv.s.x f19,x0
  fmv.s.x f20,x0
  fmv.s.x f21,x0
  fmv.s.x f22,x0
  fmv.s.x f23,x0
  fmv.s.x f24,x0
  fmv.s.x f25,x0
  fmv.s.x f26,x0
  fmv.s.x f27,x0
  fmv.s.x f28,x0
  fmv.s.x f29,x0
  fmv.s.x f30,x0
  fmv.s.x f31,x0
#endif

  /* stack initilization */
  la   x2, _stack_start


_start:
  .global _start
  
  /* clear BSS */
  la x26, _bss_start
  la x27, _bss_end

  bge x26, x27, zero_loop_end

zero_loop:
  sw x0, 0(x26)
  addi x26, x26, 4
  ble x26, x27, zero_loop
zero_loop_end:

  /* Run global initialization functions */
  li a0, 1                      /* set app mode */
  call  set_program_type
  
  call  boot_noop
  call  boot_strap
  call  __libc_init_array
  j main_entry
.section .crt0, "ax" 

main_entry:
  addi   x10, x0, 0
  /* Baud Rate 156250 
  *clock divider, SYSCLK/156250/16-1
  *5MHZ 1; 50MHZ 19
  * 103.68Mhz clk, 115200 sv model 89
  * 19.6608Mhz clk, VHD model, value 4. VHD
  * 196/2Mhz   VHD model value 84 for 115200
  */
  //addi   x11, x0, 84 //98Mhz, 1152000 for sim
  addi   x11, x0, 70  //131.072Mhz, 115200 for sim
  //addi   x11, x0, 22 //26M DCXO, just leave it here, not necessary
  //jal  uart_set_cfg 
  
  /* jump to main program entry point (argc = argv = 0) */
  addi x10, x0, 0
  addi x11, x0, 0
  jal x1, entry

  jal  uart_wait_tx_done;

  /* if program exits call exit routine from library */
  jal  x1, exit



/* ================== [ all interrupt handler] =========== */
.weak default_int_handler
default_int_handler:
  ret

ISR_ALL_ASM:
  // save all from thread context
    addi sp, sp, -32 * 4

    sw x1,   1 * 4(sp)
    sw x5,   5 * 4(sp)
    li t0, 0x80
    sw t0,   2 * 4(sp)

    sw x4,   4 * 4(sp)
    //sw x5,   5 * 4(sp)
    sw x6,   6 * 4(sp)
    sw x7,   7 * 4(sp)
    sw x8,   8 * 4(sp)
    sw x9,   9 * 4(sp)
    sw x10, 10 * 4(sp)
    sw x11, 11 * 4(sp)
    sw x12, 12 * 4(sp)
    sw x13, 13 * 4(sp)
    sw x14, 14 * 4(sp)
    sw x15, 15 * 4(sp)
    sw x16, 16 * 4(sp)
    sw x17, 17 * 4(sp)
    sw x18, 18 * 4(sp)
    sw x19, 19 * 4(sp)
    sw x20, 20 * 4(sp)
    sw x21, 21 * 4(sp)
    sw x22, 22 * 4(sp)
    sw x23, 23 * 4(sp)
    sw x24, 24 * 4(sp)
    sw x25, 25 * 4(sp)
    sw x26, 26 * 4(sp)
    sw x27, 27 * 4(sp)
    sw x28, 28 * 4(sp)
    sw x29, 29 * 4(sp)
    sw x30, 30 * 4(sp)
    sw x31, 31 * 4(sp)

ISR_ALL_ASM_BAK:
#ifdef ARCH_RISCV_FPU
    addi    sp, sp, -32 * FREGBYTES

    FSTORE  f0, 0 * FREGBYTES(sp)
    FSTORE  f1, 1 * FREGBYTES(sp)
    FSTORE  f2, 2 * FREGBYTES(sp)
    FSTORE  f3, 3 * FREGBYTES(sp)
    FSTORE  f4, 4 * FREGBYTES(sp)
    FSTORE  f5, 5 * FREGBYTES(sp)
    FSTORE  f6, 6 * FREGBYTES(sp)
    FSTORE  f7, 7 * FREGBYTES(sp)
    FSTORE  f8, 8 * FREGBYTES(sp)
    FSTORE  f9, 9 * FREGBYTES(sp)
    FSTORE  f10, 10 * FREGBYTES(sp)
    FSTORE  f11, 11 * FREGBYTES(sp)
    FSTORE  f12, 12 * FREGBYTES(sp)
    FSTORE  f13, 13 * FREGBYTES(sp)
    FSTORE  f14, 14 * FREGBYTES(sp)
    FSTORE  f15, 15 * FREGBYTES(sp)
    FSTORE  f16, 16 * FREGBYTES(sp)
    FSTORE  f17, 17 * FREGBYTES(sp)
    FSTORE  f18, 18 * FREGBYTES(sp)
    FSTORE  f19, 19 * FREGBYTES(sp)
    FSTORE  f20, 20 * FREGBYTES(sp)
    FSTORE  f21, 21 * FREGBYTES(sp)
    FSTORE  f22, 22 * FREGBYTES(sp)
    FSTORE  f23, 23 * FREGBYTES(sp)
    FSTORE  f24, 24 * FREGBYTES(sp)
    FSTORE  f25, 25 * FREGBYTES(sp)
    FSTORE  f26, 26 * FREGBYTES(sp)
    FSTORE  f27, 27 * FREGBYTES(sp)
    FSTORE  f28, 28 * FREGBYTES(sp)
    FSTORE  f29, 29 * FREGBYTES(sp)
    FSTORE  f30, 30 * FREGBYTES(sp)
    FSTORE  f31, 31 * FREGBYTES(sp)
#endif

    // switch to interrupt stack
    move s0, sp
    la   sp, _sp

    // interrupt handle
    call rt_interrupt_enter
    csrr a0, mcause
    csrr a1, mepc
    mv a2, sp
    call default_int_handler
    call rt_interrupt_leave

    // switch to from thread stack
    move sp, s0

    // need to switch new thread
    la   s0, rt_thread_switch_interrupt_flag
    lw   s2, 0(s0)
    beqz s2, spurious_interrupt_all
    sw   zero, 0(s0)

    csrr a0, mepc
#ifdef ARCH_RISCV_FPU
    sw a0,  32 * 4(sp)
#else
    sw a0,  0 * 4(sp)
#endif

    la   s0, rt_interrupt_from_thread
    lw   s1, 0(s0)
    sw   sp, 0(s1)

    la   s0, rt_interrupt_to_thread
    lw   s1, 0(s0)
    lw   sp, 0(s1)

#ifdef ARCH_RISCV_FPU
    lw  a0,  32 * 4(sp)
#else
    lw  a0,  0 * 4(sp)
#endif
    csrw mepc, a0

spurious_interrupt_all:
#ifdef ARCH_RISCV_FPU
    FLOAD   f0, 0 * FREGBYTES(sp)
    FLOAD   f1, 1 * FREGBYTES(sp)
    FLOAD   f2, 2 * FREGBYTES(sp)
    FLOAD   f3, 3 * FREGBYTES(sp)
    FLOAD   f4, 4 * FREGBYTES(sp)
    FLOAD   f5, 5 * FREGBYTES(sp)
    FLOAD   f6, 6 * FREGBYTES(sp)
    FLOAD   f7, 7 * FREGBYTES(sp)
    FLOAD   f8, 8 * FREGBYTES(sp)
    FLOAD   f9, 9 * FREGBYTES(sp)
    FLOAD   f10, 10 * FREGBYTES(sp)
    FLOAD   f11, 11 * FREGBYTES(sp)
    FLOAD   f12, 12 * FREGBYTES(sp)
    FLOAD   f13, 13 * FREGBYTES(sp)
    FLOAD   f14, 14 * FREGBYTES(sp)
    FLOAD   f15, 15 * FREGBYTES(sp)
    FLOAD   f16, 16 * FREGBYTES(sp)
    FLOAD   f17, 17 * FREGBYTES(sp)
    FLOAD   f18, 18 * FREGBYTES(sp)
    FLOAD   f19, 19 * FREGBYTES(sp)
    FLOAD   f20, 20 * FREGBYTES(sp)
    FLOAD   f21, 21 * FREGBYTES(sp)
    FLOAD   f22, 22 * FREGBYTES(sp)
    FLOAD   f23, 23 * FREGBYTES(sp)
    FLOAD   f24, 24 * FREGBYTES(sp)
    FLOAD   f25, 25 * FREGBYTES(sp)
    FLOAD   f26, 26 * FREGBYTES(sp)
    FLOAD   f27, 27 * FREGBYTES(sp)
    FLOAD   f28, 28 * FREGBYTES(sp)
    FLOAD   f29, 29 * FREGBYTES(sp)
    FLOAD   f30, 30 * FREGBYTES(sp)
    FLOAD   f31, 31 * FREGBYTES(sp)

    addi    sp, sp, 32 * FREGBYTES
#endif
    lw x1,   1 * 4(sp)
    
    // Remain in M-mode after mret
    li t0, 0x00001800
    csrs mstatus, t0
    lw t0,   2 * 4(sp)
    csrs mstatus, t0 
    
    lw x4,   4 * 4(sp)
    lw x5,   5 * 4(sp)
    lw x6,   6 * 4(sp)
    lw x7,   7 * 4(sp)
    lw x8,   8 * 4(sp)
    lw x9,   9 * 4(sp)
    lw x10, 10 * 4(sp)
    lw x11, 11 * 4(sp)
    lw x12, 12 * 4(sp)
    lw x13, 13 * 4(sp)
    lw x14, 14 * 4(sp)
    lw x15, 15 * 4(sp)
    lw x16, 16 * 4(sp)
    lw x17, 17 * 4(sp)
    lw x18, 18 * 4(sp)
    lw x19, 19 * 4(sp)
    lw x20, 20 * 4(sp)
    lw x21, 21 * 4(sp)
    lw x22, 22 * 4(sp)
    lw x23, 23 * 4(sp)
    lw x24, 24 * 4(sp)
    lw x25, 25 * 4(sp)
    lw x26, 26 * 4(sp)
    lw x27, 27 * 4(sp)
    lw x28, 28 * 4(sp)
    lw x29, 29 * 4(sp)
    lw x30, 30 * 4(sp)
    lw x31, 31 * 4(sp)

    addi sp, sp, 32 * 4
    mret 
/* ================== [ all interrupt handler end] =========== */

  .global _init
  .global _fini
_init:
_fini:
  # These don't have to do anything since we use init_array/fini_array.
  ret

/* =================================================== [ exceptions ] === */
/* This section has to be down here, since we have to disable rvc for it  */

  .section .vectors, "ax"
  .option norvc;

  // external interrupts are handled by the same callback
  // until compiler supports IRQ routines
  .org 0x00
  j ISR_ALL_ASM //ISR_RTC_ASM
  j ISR_ALL_ASM //ISR_CCE_NOTICE          //CCE
  .rept 18
   nop                       // unused
  .endr
//.global ISA_TA_CMP_ASM  
  j   ISR_ALL_ASM //ISR_ADC_ASM           // 20: ADC
  j   ISR_ALL_ASM //ISR_DAC_ASM           // 21: DAC
  j   ISR_ALL_ASM //ISR_I2C_ASM 	    // 22: i2c
  j   ISR_ALL_ASM //ISR_UART_ASM 	    // 23: uart[1]
  j   ISR_ALL_ASM //ISR_UART_ASM 	    // 24: uart[0]
  j   ISR_ALL_ASM //ISR_GPIO_ASM 	    // 25: gpio
  j   ISR_ALL_ASM //ISR_SPIM0_ASM     // 26: spim	end of transmission
  j   ISR_ALL_ASM //ISR_SPIM1_ASM     // 27: spim R/T finished
  j   ISR_ALL_ASM //ISR_TA_OVF_ASM    // 28: timer A overflow
  j   ISR_ALL_ASM //ISR_TA_CMP_ASM    // 29: timer A compare 
  j   ISR_ALL_ASM //ISR_TB_OVF_ASM    // 30: timer B overflow
  j   ISR_ALL_ASM //ISR_TB_CMP_ASM    // 31: timer B compare


  // reset vector
  .org 0x80
  j   reset_handler

  // illegal instruction exception
  .org 0x84
  illegal_insn_handler:
  j   illegal_insn_handler

  // ecall handler
  /* pulpino doesn't really need this
   * since it's always in M mode.
   * but we do not want system hang...
   */
  .org 0x88
   //should return mepc + 4;...
   ecall_insn_handler:
   j   ecall_insn_handler
  .org 0x8c
   LSU_E:
   j LSU_E
  /* magic word for spi dummy autoadapt */
  .word 0xefbeadde
 