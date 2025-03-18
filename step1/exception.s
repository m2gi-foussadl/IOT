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

 /* Exception Vector
  * assume this is linked and loaded at 0x0000-0000
  */
     ldr pc, reset_handler_addr
     ldr pc, undef_handler_addr
     ldr pc, swi_handler_addr
     ldr pc, prefetch_abort_handler_addr
     ldr pc, data_abort_handler_addr
     ldr pc, unused_handler_addr
     ldr pc, irq_handler_addr
     ldr pc, fiq_handler_addr

reset_handler_addr: .word _reset_handler
undef_handler_addr: .word _undef_handler
swi_handler_addr: .word _swi_handler
prefetch_abort_handler_addr: .word _prefetch_abort_handler
data_abort_handler_addr: .word _data_abort_handler
unused_handler_addr: .word _unused_handler
irq_handler_addr: .word _isr_handler
fiq_handler_addr: .word _fiq_handler

_isr_handler:
    b .  // unexpected interrupt occurred

_unused_handler:
    b .  // unused interrupt occurred

_fiq_handler:
	b . // unexpected fast interrupt

_undef_handler:
	b . // unexpected trap for an undefined instruction

_swi_handler:
	b .  // unexpected software interrupt

/*
 * The faulty address is [r14,-4],
 * because of the fetch is at the beginning of the pipeline.
*/
_prefetch_abort_handler:
	b .  // unexpected prefetch-abort trap

/*
 * The faulty address is [r14,-8],
 * because of the memory access happens later in the pipeline.
*/
_data_abort_handler:
	b .  // unexpected abort trap

