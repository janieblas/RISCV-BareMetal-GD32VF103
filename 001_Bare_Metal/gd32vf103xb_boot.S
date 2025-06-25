/*
 * GD32VF103 Boot Assembly - Standalone Version
 * No external headers required - all CSR definitions inline
 */

/* ================================================================
 * RISC-V CSR (Control and Status Register) Definitions
 * ================================================================ */
#define CSR_MSTATUS     0x300   /* Machine Status Register */
#define CSR_MTVEC       0x305   /* Machine Trap Vector Base Address */
#define CSR_MTVT        0x307   /* Machine Trap Vector Table (N200 specific) */

/* MSTATUS Register Bit Definitions */
#define MSTATUS_MIE     0x00000008   /* Machine Interrupt Enable bit */

/*
 * Main vector table entries.
 */
.global vtable
.type vtable, %object
.section .vector_table,"a",%progbits
vtable:
  j reset_handler
  .align 2
  .word 0
  .word 0
  .word eclic_msip_handler
  .word 0
  .word 0
  .word 0
  .word eclic_mtip_handler

  /*
   * Weak aliases to point each exception handler to the
   * 'default_interrupt_handler', unless the application defines
   * a function with the same name to override the reference.
   */
  .weak eclic_msip_handler
  .set  eclic_msip_handler,default_interrupt_handler
  .weak eclic_mtip_handler
  .set  eclic_mtip_handler,default_interrupt_handler

/*
 * A 'default' interrupt handler, in case an interrupt triggers
 * without a handler being defined.
 */
.section .text.default_interrupt_handler,"ax",%progbits
default_interrupt_handler:
    default_interrupt_loop:
      j default_interrupt_loop

/*
 * Assembly 'reset handler' function to initialize core CPU registers.
 */
.global reset_handler
.type reset_handler,@function
reset_handler:
  // Disable interrupts until they are needed.
  csrc CSR_MSTATUS, MSTATUS_MIE
  // Move from 0x00000000 to 0x08000000 address space if necessary.
  la   a0, in_address_space
  li   a1, 1
  slli a1, a1, 27
  bleu a1, a0, in_address_space
  add  a0, a0, a1
  jr   a0
  in_address_space:
  // Load the initial stack pointer value.
  la   sp, _sp
  
  // Initialize .data section (copy from Flash to RAM)
  la   t0, _sidata    // source in flash
  la   t1, _sdata     // dest in ram  
  la   t2, _edata     // end of data
data_init_loop:
  beq  t1, t2, data_init_done
  lw   t3, 0(t0)
  sw   t3, 0(t1)
  addi t0, t0, 4
  addi t1, t1, 4
  j    data_init_loop
data_init_done:

  // Clear .bss section (zero-initialize)
  la   t0, _sbss      // start of bss
  la   t1, _ebss      // end of bss
bss_init_loop:
  beq  t0, t1, bss_init_done
  sw   zero, 0(t0)
  addi t0, t0, 4
  j    bss_init_loop
bss_init_done:

  // Set the vector table's base address.
  la   a0, vtable
  csrw CSR_MTVT, a0
  // Set non-vectored interrupts to use the default handler.
  // (That will gracefully crash the program,
  //  so only use vectored interrupts for now.)
  la   a0, default_interrupt_handler
  csrw CSR_MTVEC, a0
  // Call 'main(0,0)' (.data/.bss sections already initialized)
  li   a0, 0
  li   a1, 0
  call main