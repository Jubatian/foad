/*
**  Converts GIMP header to 1bpp Uzebox Mode 74 image data, C header.
**  Adds burning torch images on it (over char. positions 192 - 255).
**
**  By Sandor Zsuga (Jubatian)
**
**  Licensed under GNU General Public License version 3.
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
**  ---
**
**  The input image must be 8 rows tall, the width must be 2048 pixels
**  (8 * 256).
**
**  The palette is ignored. Index 0 will become background (zero), index 1
**  will become foreground (one).
**
**  Produces result onto standard output, redirect into a ".h" file to get it
**  proper.
*/



/*  The GIMP header to use */
#include "chars_1bpp.h"
#include "torch.h"


#include <stdio.h>
#include <stdlib.h>



int main(void)
{
 unsigned int  dlen = width * height;
 unsigned int  sp = 0U;
 unsigned int  row = 0U;
 unsigned int  col = 0U;
 unsigned char c;

 /* Basic tests */

 if (width != 2048U){
  fprintf(stderr, "Input width must be 2048!\n");
  return 1;
 }
 if (height != 8U){
  fprintf(stderr, "Input height must be 8!\n");
  return 1;
 }

 if (t_width != 128U){
  fprintf(stderr, "Torch width must be 128!\n");
  return 1;
 }
 if (t_height != 8U){
  fprintf(stderr, "Torch height must be 8!\n");
  return 1;
 }

 /* Create some heading text */

 printf("\n");
 printf("/* 1bpp image data (%u bytes); row increment: %u bytes (%u) */\n", width, (width >> 3), (width >> 5));
 printf("\n");
 printf("const unsigned char imgdata[] __attribute__ ((section (\".imgdata\"))) = {\n");
 printf(" ");

 /* Process image data */

 while (1){

  row = sp / width;
  col = sp % width;

  if (col < 1536U){

   /* Collect eight pixels, normal char. image */

   c  = (header_data[sp + 0U] & 1U) << 7;
   c |= (header_data[sp + 1U] & 1U) << 6;
   c |= (header_data[sp + 2U] & 1U) << 5;
   c |= (header_data[sp + 3U] & 1U) << 4;
   c |= (header_data[sp + 4U] & 1U) << 3;
   c |= (header_data[sp + 5U] & 1U) << 2;
   c |= (header_data[sp + 6U] & 1U) << 1;
   c |= (header_data[sp + 7U] & 1U) << 0;

  }else{

   /* Collect two pixels, torch image */

   col = (col - 1536U) / 4U;

   c  = (t_header_data[(row * 8U) + ((col >> 6) * 64U) + (((col >> 3) & 7U) * 128U) + (col & 7U) + 0U] & 0xFU) << 4;
   c |= (t_header_data[(row * 8U) + ((col >> 6) * 64U) + (((col >> 3) & 7U) * 128U) + (col & 7U) + 1U] & 0xFU) << 0;

  }

  sp += 8U;

  /* Output it */

  printf("0x%02XU", c);

  /* Check for bounds, line or loop termination */

  if (sp == dlen){
   printf("\n};\n");
   break;
  }

  if ((sp & 0x7FU) == 0U){
   printf(",\n ");
  }else{
   printf(", ");
  }

 }

 return 0;
}
