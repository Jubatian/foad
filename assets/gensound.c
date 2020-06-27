/*
**  Generates waveforms.
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
*/



#include <stdio.h>
#include <stdlib.h>



/* 1st quarter for the wave sine table */
static const unsigned char sinqt[64] = {
 0x81U, 0x84U, 0x87U, 0x8AU, 0x8EU, 0x91U, 0x94U, 0x97U,
 0x9AU, 0x9DU, 0xA0U, 0xA3U, 0xA6U, 0xA9U, 0xACU, 0xAFU,
 0xB2U, 0xB5U, 0xB8U, 0xBAU, 0xBDU, 0xC0U, 0xC3U, 0xC5U,
 0xC8U, 0xCAU, 0xCDU, 0xCFU, 0xD2U, 0xD4U, 0xD6U, 0xD9U,
 0xDBU, 0xDDU, 0xDFU, 0xE1U, 0xE3U, 0xE5U, 0xE7U, 0xE9U,
 0xEBU, 0xECU, 0xEEU, 0xEFU, 0xF1U, 0xF2U, 0xF4U, 0xF5U,
 0xF6U, 0xF7U, 0xF8U, 0xF9U, 0xFAU, 0xFBU, 0xFCU, 0xFCU,
 0xFDU, 0xFEU, 0xFEU, 0xFEU, 0xFFU, 0xFFU, 0xFFU, 0xFFU
};



/* Normalization divisor for the noise */
#define NOISE0_NORM   79U
/* Offset removal for the noise */
#define NOISE0_DCADD   9U
#define NOISE0_DCREM   0U


/* Normalization divisor for the noise */
#define NOISE1_NORM   64U
/* Offset removal for the noise */
#define NOISE1_DCADD   0U
#define NOISE1_DCREM   0U



int main(void)
{
 unsigned int  i;
 unsigned int  j;
 unsigned int  r;
 unsigned int  s;
 unsigned int  f;

 /* Sine */

 for (i = 0U; i < 0x100U; i++){
  if ((i & 0x7U) == 0x0U){ printf("\n\t.byte"); }
  else                   { printf(","); }
  if      (i < 0x40U){ printf(" 0x%02X", (sinqt[i - 0x00U] - 0x080U)); }
  else if (i < 0x80U){ printf(" 0x%02X", (sinqt[0x7FU - i] - 0x080U)); }
  else if (i < 0xC0U){ printf(" 0x%02X", (0x17FU - sinqt[i - 0x80U])); }
  else               { printf(" 0x%02X", (0x17FU - sinqt[0xFFU - i])); }
 }

 printf("\n");

 /* Noise 0 */

 r = 4U;
 f = 0x8000U;
 for (i = 0U; i < 0x100U; i++){

  if (r < 0x80U){
   s = r;
   s = (s * s) >> 7;
  }else{
   s = 0xFFU - r;
   s = (s * s) >> 7;
   s = 0xFFU - s;
  }
  f = ((f * 0xE0U) + ((s << 8) * 0x20U)) >> 8;
  s = (f / NOISE0_NORM) - (0x8000U / NOISE0_NORM) + NOISE0_DCADD - NOISE0_DCREM;
  if ((s & 0x8000U) != 0U){
   s = s + 0x100U;
   if (s <  0x80U){ s = 0x80U; } /* Saturate negative */
  }else{
   if (s >= 0x80U){ s = 0x7FU; } /* Saturate positive */
  }

  if ((i & 0x7U) == 0x0U){ printf("\n\t.byte"); }
  else                   { printf(","); }
  printf(" 0x%02X", s);

  r = (((r >> 7) + (r + r) + 0xBBU) ^ 0x7FU) & 0xFFU;
 }

 printf("\n");

 /* Triangle */

 for (i = 0U; i < 0x100U; i++){
  if ((i & 0x7U) == 0x0U){ printf("\n\t.byte"); }
  else                   { printf(","); }
  if      (i < 0x40U){ printf(" 0x%02X", (i - 0x00U) * 2U     ); }
  else if (i < 0x80U){ printf(" 0x%02X", (0x7FU - i) * 2U + 1U); }
  else if (i < 0xC0U){ printf(" 0x%02X", (0xFFU - i) * 2U + 1U); }
  else               { printf(" 0x%02X", (i - 0x80U) * 2U     ); }
 }

 printf("\n");

 /* Noise 1, distorted sine */

 r = 4U;
 f = 0x8000U;
 for (i = 0U; i < 0x100U; i++){

  if (r < 0x80U){
   s = r;
   s = (s * s) >> 7;
  }else{
   s = 0xFFU - r;
   s = (s * s) >> 7;
   s = 0xFFU - s;
  }

  j = (i + 0x1BU) & 0xFFU; /* Offset sine to align with filter */
  if      (j < 0x40U){ s = (((0x00U + sinqt[j - 0x00U]) * 4U) + (s * 12U)) >> 4; }
  else if (j < 0x80U){ s = (((0x00U + sinqt[0x7FU - j]) * 4U) + (s * 12U)) >> 4; }
  else if (j < 0xC0U){ s = (((0xFFU - sinqt[j - 0x80U]) * 4U) + (s * 12U)) >> 4; }
  else               { s = (((0xFFU - sinqt[0xFFU - j]) * 4U) + (s * 12U)) >> 4; }

  f = ((f * 0xF8U) + ((s << 8) * 0x08U)) >> 8;
  s = (f / NOISE1_NORM) - (0x8000U / NOISE1_NORM) + NOISE1_DCADD - NOISE1_DCREM;
  if ((s & 0x8000U) != 0U){
   s = s + 0x100U;
   if (s <  0x80U){ s = 0x80U; } /* Saturate negative */
  }else{
   if (s >= 0x80U){ s = 0x7FU; } /* Saturate positive */
  }

  if ((i & 0x7U) == 0x0U){ printf("\n\t.byte"); }
  else                   { printf(","); }
  printf(" 0x%02X", s);

  r = (((r >> 7) + (r + r) + 0xBBU) ^ 0x7FU) & 0xFFU;
 }

 printf("\n");

 return 0;
}
