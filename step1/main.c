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
size_t strlen(const char *str) {
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}
void exit(char status) {
    // Code pour quitter le programme
    // On va juste boucler ici pour simuler un exit
    // On peut éventuellement afficher le code de sortie
    uart_send_string(UART0, "Ctrl-C pressed, exiting with status: ");
    uart_send(UART0, status);
    uart_send_string(UART0, "\n");
    uart_disable(UART0);
    uart_disable(UART1);
    uart_disable(UART2);
    while (1) {
    }
}
void uart_handler(uint32_t irq, void *cookie) {
    static char buffer[256];
    static int pos = 0;
    static int escape_seq = 0;
    char c;
    uart_receive(UART0, &c);

    if (escape_seq == 1) {
        if (c == '[') {
            escape_seq = 2;
        } else {
            escape_seq = 0;
        }
        return;
    } else if (escape_seq == 2) {
        if (c == 'D') {
            // Flèche gauche
            if (pos > 0) {
                pos--;
                uart_send_string(UART0, "\x1B[D");
            }
        } else if (c == 'C') {
            // Flèche droite
            if (pos < strlen(buffer)) {
                pos++;
                uart_send_string(UART0, "\x1B[C");
            }
        }
        escape_seq = 0;
        return;
    }

    if (c == 0x1B) {
        escape_seq = 1;
        return;
    }

    // Si c'est Ctrl-C, exit le programme
    if (c == 0x03) {
        exit('0');

    }
    //Si c'est un retour chariot, on termine la ligne
    if (c == '\r' || c == '\n') {
        uart_send_string(UART0, "\n");
        pos = 0;
        buffer[0] = '\0'; // Réinitialiser le buffer
        return;
    }

    // Sinon, on ajoute le caractère au buffer et on l'affiche
    if (pos < sizeof(buffer) - 1) {
        buffer[pos++] = c;
        buffer[pos] = '\0';
        uart_send(UART0, c);
    }
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
        core_halt();
    }
}

void panic() {
  for(;;)
    ;
}
