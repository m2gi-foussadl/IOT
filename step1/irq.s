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

 /* Standard definitions of Mode bits and Interrupt (I & F) flags in PSRs */

    .equ    CPSR_USR_MODE,       0x10
    .equ    CPSR_FIQ_MODE,       0x11
    .equ    CPSR_IRQ_MODE,       0x12
    .equ    CPSR_SVC_MODE,       0x13
    .equ    CPSR_ABT_MODE,       0x17
    .equ    CPSR_UND_MODE,       0x1B
    .equ    CPSR_SYS_MODE,       0x1F

    .equ    CPSR_IRQ_FLAG,         0x80      /* when set, IRQs are disabled, at the core level */
    .equ    CPSR_FIQ_FLAG,         0x40      /* when set, FIQs are disabled, at the core level */

/*
  * Cortex-A8 use the instruction wfi with a memory barrier to ensure
 * that the write pipeline is flushed before the core goes in powered
 * down mode, waiting for interrupts.
*/
.global _wfi
	.func _wfi
_wfi:
    dsb
    wfi
	mov pc,lr
    .size   _wfi, . - _wfi
	.endfunc

/*
 * Initial setup for handling interrupts on the processor
 * It is about setting up the stack for the interrupt mode.
 */
.global _irqs_setup
	.func _irqs_setup
_irqs_setup:
    /* get Program Status Register */
    mrs r0, cpsr
    /* go in IRQ mode */
    bic r1, r0, #CPSR_SYS_MODE  /* 0x1F */
    orr r1, r1, #CPSR_IRQ_MODE  /* 0x12 */
    msr cpsr, r1
    /* set IRQ stack */
    ldr sp, =irq_stack_top
    /* go back to the mode the processor was in
     * when this function was called, normally,
     * it should be the Supervisor mode */
    msr cpsr, r0
    mov pc,lr
    .size   _irqs_setup, . - _irqs_setup
	.endfunc

/*
 * Enable all interrupts at the processor.
 */
.global _irqs_enable
	.func _irqs_enable
_irqs_enable:
    /* get Program Status Register */
    mrs r0, cpsr
    /* Enable IRQs on the processor by clearing the mask bit
     * The BIC (BIt Clear) instruction performs an AND operation
     * with the complements of the given contants #0x80.
     */
    bic r0, r0, #CPSR_IRQ_FLAG /*0x80*/
    /* set Program Status Register */
    msr cpsr, r0
    mov pc,lr
    .size   _irqs_enable, . - _irqs_enable
	.endfunc

/*
 * Disable all interrupts at the processor and
 * at the processor only, the VIC is still managing
 * interrupts. When the interrupts will be turned back
 * on, the VIC will interrupt the processor if they are
 * pending interrupts.
 */
.global _irqs_disable
	.func _irqs_disable
_irqs_disable:
    /* get Program Status Register */
    mrs r0, cpsr
    /* Disable IRQs on the processor */
    orr r0, r0, #CPSR_IRQ_FLAG /*0x80*/
    /* set Program Status Register */
    msr cpsr, r0
    mov pc,lr
    .size   _irqs_disable, . - _irqs_disable
	.endfunc


