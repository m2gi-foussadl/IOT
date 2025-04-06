/*
 * isr.h
 *
 *  Created on: Jan 21, 2021
 *      Author: ogruber
 */

#ifndef ISR_MMIO_H_
#define ISR_MMIO_H_

/*
 * Versatile Application Baseboard for ARM926EJ-S User Guide
 * DUI0225D
 * Programmer's Reference, 4.9 Interrupt Controllers
 */

#define VIC_BASE_ADDR 0x10140000

/*
 * PrimeCell Vectored Interrupt Controller (PL190) Technical Reference Manual
 * https://developer.arm.com/documentation/ddi0183/latest/)
 */

/*
 * Shows the status of the interrupts after masking by
 * the VICINTENABLE and VICINTSELECT Registers.
 * A HIGH bit indicates that the interrupt is active,
 * and generates an IRQ interrupt to the processor.
 */
#define VICIRQSTATUS 0x000

/*
 * Shows the status of the interrupts after masking by
 * the VICINTENABLE and VICINTSELECT Registers.
 * A HIGH bit indicates that the interrupt is active,
 * and generates an FIQ interrupt to the processor.
 */
#define VICFIQSTATUS 0x004
/*
 * Shows the status of the interrupts before masking by
 * the enable registers. A HIGH bit indicates that
 * the appropriate interrupt request is active before masking.
 */
#define VICRAWSTATUS 0x008
/*
 * [31:0] Selects the type of interrupt for interrupt requests:
 *   1 = FIQ interrupt
 *   0 = IRQ interrupt.
 */
#define VICINTSELECT 0x00C
/*
 * Enables the interrupt request lines:
 *   1 = Interrupt enabled. Enables interrupt request to processor.
 *   0 = Interrupt disabled.
 * On reset, all interrupts are disabled.
 * On writes, a HIGH bit sets the corresponding bit in
 * the VICINTENABLE Register, while a LOW bit has no effect.
 */
#define VICINTENABLE 0x010
/*
 * Clears bits in the VICINTENABLE Register.
 * On writes, a HIGH bit clears the corresponding bit in the
 * VICINTENABLE Register, while a LOW bit has no effect.
 */
#define VICINTCLEAR 0x014

#endif /* ISR_MMIO_H_ */
