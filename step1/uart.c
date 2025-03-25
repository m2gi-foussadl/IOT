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
#include "uart-mmio.h"

struct uart {
  uint8_t uartno; // the UART numéro
  void* bar;      // base address register for this UART
};

static
struct uart uarts[NUARTS];

static
void uart_init(uint32_t uartno, void* bar) {
  struct uart*uart = &uarts[uartno];
  uart->uartno = uartno;
  uart->bar = bar;
  // no hardware initialization necessary
  // when running on QEMU, the UARTs are
  // already initialized, as long as we
  // do not rely on interrupts.
}

void uarts_init() {
  uart_init(UART0,UART0_BASE_ADDRESS);
  uart_init(UART1,UART1_BASE_ADDRESS);
  uart_init(UART2,UART2_BASE_ADDRESS);
}

void uart_enable(uint32_t uartno) {
  struct uart*uart = &uarts[uartno];
  // nothing to do here, as long as
  // we do not rely on interrupts
}

void uart_disable(uint32_t uartno) {
  struct uart*uart = &uarts[uartno];
  // nothing to do here, as long as
  // we do not rely on interrupts
}

void uart_receive(uint8_t uartno, char *pt) {
    struct uart* uart = &uarts[uartno];
    volatile uint32_t* uart_dr = (volatile uint32_t*)((uintptr_t)uart->bar + UART_DR);
    volatile uint32_t* uart_fr = (volatile uint32_t*)((uintptr_t)uart->bar + UART_FR);

    // Attendre jusqu'à ce qu'il y ait des données disponibles
    while (*uart_fr & (1 << 4)) {
        // Le bit 4 du registre de statut (FR) indique si le FIFO RX est vide
    }

    // Lire le caractère du registre de données (DR)
    *pt = (char)(*uart_dr & 0xFF);
}

/**
 * Sends a character through the given uart, this is a blocking call
 * until the character has been sent.
 */
void uart_send(uint8_t uartno, char s) {
    struct uart* uart = &uarts[uartno];
    volatile uint32_t* uart_dr = (volatile uint32_t*)((uintptr_t)uart->bar + UART_DR);
    volatile uint32_t* uart_fr = (volatile uint32_t*)((uintptr_t)uart->bar + UART_FR);

    // Attendre jusqu'à ce qu'il y ait de la place dans le FIFO de transmission
    while (*uart_fr & (1 << 5)) {
        // Le bit 5 du registre de statut (FR) indique si le FIFO TX est plein
    }

    // Écrire le caractère dans le registre de données (DR)
    *uart_dr = (uint32_t)s;
}

/**
 * This is a wrapper function, provided for simplicity,
 * it sends a C string through the given uart.
 */
void uart_send_string(uint8_t uartno, const char *s) {
  while (*s != '\0') {
    uart_send(uartno, *s);
    s++;
  }
}

