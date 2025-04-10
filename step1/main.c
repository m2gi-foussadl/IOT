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
#include "main.h"
#include "uart.h"
#include "isr.h"
#include "ring_buffer.h"

extern uint32_t irq_stack_top;
extern uint32_t stack_top;

void check_stacks() {
  void *memsize = (void*)MEMORY;
  void *addr;
  addr = &stack_top;
  if (addr >= memsize)
    panic();
/*
  addr = &irq_stack_top;
  if (addr >= memsize)
    panic();
*/
}
/**This is the interrupt handler for the UART0
 * It is called when there is an interrupt on the UART0
 * It will call the shell function to handle the input
 * Must be short because no interrupts are allowed in the handler
 * and we don't want to block the system in order to don't loose some bytes
*/
void uart_handler(uint32_t irq, void *cookie) {
    uint8_t code;
    uart_receive(UART0,&code);
    while (code) {
        ring_put(code);
        uart_receive(UART0, &code);
    }
    //Réactivation de l'interruption de reception
    uart_interrupt_ack();
}
/**
 * This is the C entry point,
 * upcalled once the hardware has been setup properly
 * in assembly language, see the startup.s file.
 */
void _start(void) {
    check_stacks();
    vic_setup_irqs();
    uarts_init();
    uart_enable(UART0);
    vic_enable_irq(UART0_IRQ, uart_handler, NULL);
    core_enable_irqs();
    for (;;) {
        process_ring();
        core_disable_irqs();
        if(ring_empty()) {
            core_halt();
        }
    }
}


void panic() {
  for(;;)
    ;
}
