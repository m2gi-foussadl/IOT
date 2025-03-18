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
 * Define the different sections included in the ELF file.
 */
SECTIONS
{
  . = 0x0;
  .text : { 
     build/exception.o(.text) 
     build/startup.o(.text)
  }
  . = 0x100;
  .text : { 
     build/*(.text) 
  }
  . = ALIGN(4); 
  .data : { 
    build/*(.data) 
  }
  /*
   * Include the data sections that must be zeroed upon starting up.
   * Align the section on a 4byte boundary, it is cleaner and more
   * efficient for an ARM processor. Especially that we do not enable
   * unaligned accesses in memory by the ARM processor.
   * Look at the startup code to see how memory is reset to zero
   * between _bss_start and _bss_end.
   */ 
  . = ALIGN(4); 
  _bss_start = .;
  .bss . : {
    build/*(.bss COMMON)
  } 
  . = ALIGN(4); 
  _bss_end = .;
  
  /* 
   * Finally, reserve some memory for the C stack
   * Remember that stacks are growing downward, 
   * so the top of the stack is at the end of 
   * the reserved memory region. 
   * NOTA BENE: 
   *    there are no runtime checks to check that 
   *    your stack does not overflow at runtime when
   *    nesting too many C function calls. 
   */
  . = ALIGN(8);
  . = . + 0x1000; /* 4KB of stack memory */
  stack_top = .;
}
