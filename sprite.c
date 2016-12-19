/*
 *  Dragon - Sprite blitter
 *  Copyright (C) 2016 Sandor Zsuga (Jubatian)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#include "sprite.h"
#include <avr/pgmspace.h>
#include <uzebox.h>



/*
** Blits a sprite
**
** spo: Sprite descriptor offset
** x:   X position on screen (VRAM), wraps
** y:   Y position on screen (VRAM), wraps
** flg: Flags (same as used for Mode 74, RAM source & Y flip not supported)
** col: Recoloring. 0: Off.
*/
void  sprite_blit(uint16 spo, auint x, auint y, auint flg, auint col)
{
 auint  b0;
 auint  b1;
 auint  b2;
 auint  b3;
 auint  xp;
 auint  yp;
 auint  cp;
 auint  fp;
 uint16 spp;

 /* Drop unsupported flags */

 flg &= M74_SPR_FLIPX |
        M74_SPR_MASK |
        M74_SPR_I3 |
        M74_SPR_I2 |
        M74_SPR_I1 |
        M74_SPR_I0;

 /* Process list */

 do{

  b0 = pgm_read_byte(spo++);
  b1 = pgm_read_byte(spo++);

  if ( ((b0 & 0x01U) == 0U) &&
       ((b1 & 0x80U) == 0U) ){ /* Jump command */

   spo  = b0 | ((uint16)(b1 | 0x80U) << 8);
   b0   = 0x00U;            /* Don't terminate loop */

  }else{

   if ((b0 & 0x01U) == 0U){ /* Reuse command (b1 bit 7 is set then) */
    spp = b0 | ((uint16)(b1) << 8);
    b0 = pgm_read_byte(spp++);
    b1 = pgm_read_byte(spp++);
    b2 = pgm_read_byte(spp++);
    b3 = pgm_read_byte(spp++);
   }else{
    b2 = pgm_read_byte(spo++);
    b3 = pgm_read_byte(spo++);
   }

   if ((b0 & 0x04U) == 0U){ /* Sprite component render */

    if (flg & M74_SPR_FLIPX){ b2 = 0U - 8U - b2; }

    b2 = b2 + x + 8U;
    b3 = b3 + y + 8U - 7U;  /* The '7' is for adjusting to tiles. */

    spp  = (b0 & 0xE0U) | ((uint16)(b1) << 8);

    if (col != 0U){
     M74_BlitSpriteCol(spp, b2, b3, flg, col);
    }else{
     M74_BlitSprite(spp, b2, b3, flg);
    }

   }else{                   /* Particle generator */

    cp = b0 >> 4;
    fp = (flg & (M74_SPR_MASK)) | M74_SPR_I1;

    xp = (b1 >> 4);         /* 1st particle */
    yp = (b1 & 0x0FU);
    if (flg & M74_SPR_FLIPX){ xp = 16U - xp; }
    xp = xp + x;
    yp = yp + y;
    M74_PutPixel(cp, xp, yp, fp);

    if (b2 != b1){          /* 2nd particle (only if differs from first) */
     xp = (b2 >> 4);
     yp = (b2 & 0x0FU);
     if (flg & M74_SPR_FLIPX){ xp = 16U - xp; }
     xp = xp + x;
     yp = yp + y;
     M74_PutPixel(cp, xp, yp, fp);
    }

    if (b3 != b2){          /* 3rd particle (only if differs from second) */
     xp = (b3 >> 4);
     yp = (b3 & 0x0FU);
     if (flg & M74_SPR_FLIPX){ xp = 16U - xp; }
     xp = xp + x;
     yp = yp + y;
     M74_PutPixel(cp, xp, yp, fp);
    }

   }
  }

 }while ((b0 & 0x02U) == 0U);
}
