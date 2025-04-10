#ifndef IOT_RING_BUFFER_H
#define IOT_RING_BUFFER_H

#include <stdint.h>
#include <stdbool.h>
#define MAX_CHARS 512

bool ring_empty();
bool ring_full();
void ring_put(uint8_t bits);
uint8_t ring_get();
void process_ring();
void exit(char status);
void shell();

#endif // IOT_RING_BUFFER_H