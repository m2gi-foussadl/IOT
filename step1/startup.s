/*
 * Copyright: Olivier Gruber (olivier dot gruber at acm dot org)
 *
 * This program is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <https://www.gnu.org/licenses/>.
 */

 /*
  * Standard definitions of Mode bits and Interrupt (I & F) flags in CPSR
  * (Current Program Status Register).
  */
    .equ    CPSR_USR_MODE,       0x10
    .equ    CPSR_FIQ_MODE,       0x11
    .equ    CPSR_IRQ_MODE,       0x12
    .equ    CPSR_SVC_MODE,       0x13
    .equ    CPSR_ABT_MODE,       0x17
    .equ    CPSR_UND_MODE,       0x1B
    .equ    CPSR_SYS_MODE,       0x1F

    .equ    CPSR_IRQ_FLAG,         0x80      /* when set, IRQs are disabled, at the core. */
    .equ    CPSR_FIQ_FLAG,         0x40      /* when set, FIQs are disabled, at the core. */

.global _reset_handler
_reset_handler:
   /*
    * Initialize the Current Program Status Register,
    * Set the core in the SYS_MODE, with all interrupts disabled,
    */
	msr     cpsr_c,#(CPSR_SYS_MODE | CPSR_IRQ_FLAG | CPSR_FIQ_FLAG)
	/*
     * Set the stack for the current mode (SYS_MODE)
     * Find the definition of the symbol "stack_top"
     * in the linker script used to link this kernel.
	 */
	ldr     sp,=stack_top             /* set the C stack pointer */

	/*-------------------------------------------
	 * Clear out the bss section, located from _bss_start to _bss_end.
	 * Check in the linker script used to link this kernel that
	 * both symbols are 4-byte aligned.
	 *
	 * This is a C convention, the GNU GCC compiler will group
	 * all global variables that need to be zeroed on startup
	 * in the bss section of the executable ELF produced by the
	 * linker..
	 *-------------------------------------------*/
	ldr	r4, =_bss_start
	ldr	r9, =_bss_end
	mov	r5, #0
1:
	stmia	r4!, {r5} 
	cmp	r4, r9
	blo	1b

 	/*
 	 * Set the GCC frame-pointer to null, indicating the top
 	 * of the stack, which is necessary for GDB tracing the
 	 * call stack correctly, assuming the code is generated
 	 * with a calling convention that uses frame pointers.
 	 */
    eor r11, r11, r11
    /*
     * It is time to upcall the C entry function:
     *    _start(void)
     * defined in the source file "main.c".
     */
 	ldr r3,=_start
    blx r3
    // in case the function "_start" returns...
    // loop for ever.
halt:
    b .
