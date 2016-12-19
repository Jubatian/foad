/*
**  Converts Text data to the format used in the ROM text storage.
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
**  Text file structure:
**
**  A new line is converted to a terminator, so normally one line can produce
**  one entry of text.
**  The '\' character can be used for escaping. '\\' produces a '\'. '\e'
**  produces a line end, '\p' emits current position. A new line after '\' is
**  ignored.
**
**  Feed it from standard input, produces result onto standard output.
*/



#include <stdio.h>
#include <stdlib.h>



int main(void)
{
 unsigned char cin;
 unsigned char cou = 0x00U;
 unsigned int  bit = 8U;
 unsigned int  i;
 unsigned int  osi = 0U;
 unsigned int  pos = 0U;

 while (1){

  if (fread(&cin, 1U, 1U, stdin) == 0U){ break; }

  if       (cin == '\\'){

   if (fread(&cin, 1U, 1U, stdin) == 0U){ break; }
   if      (cin ==  'e'){ cin = 0x3FU; }
   else if (cin == '\\'){ cin = '\\' - 0x40U; }
   else if (cin == '\n'){ cin = 0xFFU; }
   else if (cin ==  'p'){
    if ((osi & 0x07U) != 0U){
     printf("\n");
    }
    printf("/* Position: %u (byte: %u, bit %u) */\n", pos, osi, bit - 1U);
    cin = 0xFFU;
   }else{                 cin = 0xFFU; }

  }else if (cin == '\n'){

   cin = 0x3FU;

  }else if ( (cin >= 0x40U) && (cin < 0x80U) ){

   cin = cin - 0x40U;

  }else if ( (cin >= 0x20U) && (cin < 0x40U) ){

   cin = cin + 0x40U;

  }else{

   cin = 0xFFU;

  }

  if (cin != 0xFFU){

   for (i = 0U; i < 7U; i ++){
    bit = bit - 1U;
    cou = cou | (((cin >> (6U - i)) & 1U) << bit);
    if (bit == 0U){
     bit = 8U;
     printf(" 0x%02XU,", cou);
     cou = 0U;
     osi ++;
     if ((osi & 0x07U) == 0U){
      printf("\n");
     }
    }
   }
   pos ++;

  }

 }

 if (bit != 8U){
  printf(" 0x%02XU,", cou);
  cou = 0U;
  osi ++;
  if ((osi & 0x07U) == 0U){
   printf("\n");
  }
 }
 if ((osi & 0x07U) != 0U){
  printf("\n");
 }
 printf("/* Size: %u */\n", osi);

 return 0;
}
