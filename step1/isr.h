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
 * isr.h
 *
 *  Created on: Jan 21, 2021
 *      Author: ogruber
 */

#ifndef ISR_H_
#define ISR_H_

/*
 * Versatile Application Baseboard for ARM926EJ-S User Guide HBI-0118
 *   (https://developer.arm.com/documentation/dui0225/latest)
 * Programmer's Reference
 *   4.9 Interrupt Controllers
 * Hardware Description,
 *   3.10 Interrupts
 *   3.18 UART Interface
 */

/*
 * The maximum number of interrupts
 * handled by the VIC
 */
#define NIRQS 32

/*
 * UART Interrupts
 */
#define UART0_IRQ 12
#define UART0_IRQ_MASK (1 << UART0_IRQ)

#define UART1_IRQ 13
#define UART1_IRQ_MASK (1 << UART1_IRQ)

#define UART2_IRQ 14
#define UART2_IRQ_MASK (1 << UART2_IRQ)

/*
 * Timers:
 */
#define TIMER3_IRQ ???
#define TIMER3_IRQ_MASK ???

#define TIMER2_IRQ ???
#define TIMER2_IRQ_MASK ???

#define TIMER1_IRQ ???
#define TIMER1_IRQ_MASK ???

#define TIMER0_IRQ ???
#define TIMER0_IRQ_MASK ???

/*
 * Core interrupt-related behavior:
 */
void core_enable_irqs();
void core_disable_irqs();
void core_halt(void);

/*
 * VIC Setup: setup the Interrupt Service Routine
 * data structure for handlers and cookies.
 */
void vic_setup_irqs();

/*
 * VIC enables the given interrupt,
 * like UART0_IRQ
 */
void vic_enable_irq(uint32_t irq,void(*callback)(uint32_t,void*),void*cookie);

/*
 * VIC disable the given interrupt,
 * like UART0_IRQ
 */
void vic_disable_irq(uint32_t irq);

#endif /* ISR_H_ */
