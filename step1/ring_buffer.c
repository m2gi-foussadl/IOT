//
// Created by lance on 08/04/2025.
//

#include "ring_buffer.h"
#include "stddef.h"
#include "uart.h"
#include "uart-mmio.h"

uint32_t tail = 0;
uint32_t head = 0;
char buffer[MAX_CHARS];

bool ring_empty() {
    return (head == tail);
}
bool ring_full() {
    int next = (head + 1) % MAX_CHARS;
    return (next == tail);
}
void ring_put(uint8_t value) {
    if (!ring_full()){
        buffer[head] = value;
        head = (head + 1) % MAX_CHARS;
    }
}
uint8_t ring_get(){
    if (!ring_empty()){
        uint8_t value = buffer[tail];
        tail = (tail + 1) % MAX_CHARS;
        return value;
    }
    return 0;
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
void shell(){
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

char line[MAX_CHARS];
uint32_t nchars = 0;

void process_ring() {
    uint8_t code;
    while (!ring_empty()) {
        code = ring_get();
        line[nchars++] = (char) code;
    }
    shell();
    nchars = 0;
}