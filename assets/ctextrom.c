/*
**  Converts Text data to the format used in the ROM text storage.
**
**  Note: Changed to produce assembly data due to the impossibility of
**  maintaining strict variable order with more recent GCC versions.
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
 unsigned int  ptb[256U];
 unsigned int  ptp = 0U;
 int           beg = 1;

 ptb[0] = 0U;
 ptp = 1U;

 while (1){

  if (fread(&cin, 1U, 1U, stdin) == 0U){ break; }

  if       (cin == '\\'){

   if (fread(&cin, 1U, 1U, stdin) == 0U){ break; }
   if (cin ==  'p'){
    printf("\n; Position: %u (byte: %u, bit %u)", pos, osi, bit - 1U);
    beg = 1;
    cin = 0xFFU;
    ptb[ptp] = pos;
    ptp ++;
   }else{
    if      (cin ==  'e'){ cin = 0x3FU; }
    else if (cin == '\\'){ cin = '\\' - 0x40U; }
    else if (cin == '\n'){ cin = 0xFFU; }
    else{                  cin = 0xFFU; }
   }

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
     if ((osi & 0x07U) == 0U){ beg = 1; }
     if (beg){ printf("\n\t.byte "); beg = 0; }
     else    { printf(", "); }
     printf("0x%02X", cou);
     cou = 0U;
     osi ++;
    }
   }
   pos ++;

  }

 }

 if (bit != 8U){
  if ((osi & 0x07U) == 0U){ beg = 1; }
  if (beg){ printf("\n\t.byte "); }
  else    { printf(", "); }
  printf("0x%02X", cou);
  cou = 0U;
  osi ++;
 }

 printf("\n");
 printf("; Size: %u\n", osi);

 printf("; Position map (Little Endian words):\n");

 for (i = 0U; i < ptp; i++){
  printf(" 0x%02X, 0x%02X,\n", ptb[i] & 0xFFU, ptb[i] >> 8);
 }

 return 0;
}
