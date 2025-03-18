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

#ifndef MAIN_H_
#define MAIN_H_

#include <stdint.h>
#include <stddef.h>

typedef uint8_t bool_t;

void panic();
void kprintf(const char *fmt, ...);

__inline__
__attribute__((always_inline))
uint32_t mmio_read8(void* bar, uint8_t offset) {
  return *((uint8_t*)(bar+offset));
}

__inline__
__attribute__((always_inline))
void mmio_write8(void* bar, uint32_t offset, uint8_t value) {
  *((uint8_t*)(bar+offset)) = value;
}

__inline__
__attribute__((always_inline))
uint16_t mmio_read16(void* bar, uint32_t offset) {
  return *((uint16_t*)(bar+offset));
}

__inline__
__attribute__((always_inline))
void mmio_write16(void* bar, uint32_t offset, uint16_t value) {
  *((uint16_t*)(bar+offset)) = value;
}

__inline__
__attribute__((always_inline))
uint32_t mmio_read32(void* bar, uint32_t offset) {
  return *((uint32_t*)(bar+offset));
}

__inline__
__attribute__((always_inline))
void mmio_write32(void* bar, uint32_t offset, uint32_t value) {
  *((uint32_t*)(bar+offset)) = value;
}

__inline__
__attribute__((always_inline))
void mmio_set(void* bar, uint32_t offset, uint32_t bits) {
  uint32_t value = *((uint32_t*)(bar+offset));
  value |= bits;
  *((uint32_t*)(bar+offset)) = value;
}

__inline__
__attribute__((always_inline))
void mmio_clear(void* bar, uint32_t offset, uint32_t bits) {
  uint32_t value = *((uint32_t*)(bar+offset));
  value &= ~bits;
  *((uint32_t*)(bar+offset)) = value;
}

#endif /* MAIN_H_ */
