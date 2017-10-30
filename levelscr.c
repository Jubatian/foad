/*
 *  Dragon - Level data - VRAM interfacing
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



#include "levelscr.h"
#include "parallax.h"
#include "random.h"
#include "m74cfg.h"
#include "global.h"
#include <uzebox.h>



/* Internal flag: Redraw map */
#define LEVELSCR_REDRAW   0x80U



/* Map flags */
static auint  levelscr_flg;
/* Map right boundary in pixels */
static uint16 levelscr_bx;
/* Map bottom boundary in pixels */
static uint16 levelscr_by;

/* Scroll target X in pixels */
static uint16 levelscr_tx;
/* Scroll target Y in pixels */
static uint16 levelscr_ty;

/* Current upper left X in pixels, global for external reads */
uint16 levelscr_x;
/* Current upper left Y in pixels, global for external reads */
uint16 levelscr_y;

/* Previous scroll amount X (2's complement) */
static auint levelscr_psx;
/* Previous scroll amount Y (2's complement) */
static auint levelscr_psy;

/* Current screen shake magnitude */
static auint levelscr_mag;




/*
** Sets map parameters to use
**
** Note that this doesn't initialize the VRAM etc., just sets up the map to
** use. It resets screen.
**
** The flags (flg) specify what to use on the upper / lower parallax regions.
** (Will be defined later)
*/
void levelscr_setmap(uint16 map, auint wdt, auint hgt, auint flg)
{
 levelscr_bx  = ((uint16)(wdt) * 128U) - 192U;
 levelscr_by  = ((uint16)(hgt) *  64U) + 192U - 192U; /* (adds 128 for the top / 64 for the bottom parallax) */
 levelscr_flg = flg & 0x77U;
 global_palsel = (global_palsel & 0xF0U) | (flg >> 7); /* Add sky */
 levelscr_mag = 0U;
 level_set(map, wdt, hgt);
 levelscr_reset();
}



/*
** Set location (upper left corner)
**
** Sets the upper left visible corner of the map in pixels (0:128) is for
** displaying the upper left corner of the map. This resets screen, so
** everything is redrawn.
*/
void levelscr_setloc(uint16 x, uint16 y)
{
 levelscr_settarget(x, y);

 levelscr_x = levelscr_tx;
 levelscr_y = levelscr_ty;
 levelscr_psx = 0U;
 levelscr_psy = 0U;

 levelscr_reset();
}



/*
** Resets screen
**
** Calling this ensures that next time everything is redrawn. Normally the
** VRAM is not altered unless necessary (when scrolling it is copied) so VRAM
** effects are possible.
*/
void levelscr_reset(void)
{
 levelscr_flg |= LEVELSCR_REDRAW;
}



/*
** Set scroll target location
**
** Set the target location of scrolling. The location is clipped to map
** boundaries. Negatives (2's complement) are possible, they will be clipped
** proper.
**
** The scrolling is smoothened to make it look aesthetically nice, so no
** related considerations are necessary when setting the target.
*/
void levelscr_settarget(uint16 x, uint16 y)
{
 y += 128U;

 if      (x >=     0xFF00U){ levelscr_tx = 0U;          }
 else if (x >= levelscr_bx){ levelscr_tx = levelscr_bx; }
 else                      { levelscr_tx = x;           }

 if      (y >=     0xFF00U){ levelscr_ty = 0U;          }
 else if (y >= levelscr_by){ levelscr_ty = levelscr_by; }
 else                      { levelscr_ty = y;           }
}



/*
** Adds a shake effect, to be used for bombs, destruction etc.
**
** The parameter is the magnitude and timespan of the effect
*/
void levelscr_shake(auint mag)
{
 levelscr_mag = mag;
}



/*
** Performs a scroll step
**
** VRAM must be restored before calling this. It carries out a frame of
** scrolling as necessary. After this, sprites may be blitted.
*/
void levelscr_scroll(void)
{
 uint16 ptx;
 uint16 pty;
 uint16 tx;
 uint16 ty;
 uint8* vram = (uint8*)(M74_VRAM_OFF);
 uint8* rows = (uint8*)(RSL_GAME);
 uint8* xshm = (uint8*)(M74_XSHIFT_OFF);
 auint  i;
 auint  j;
 auint  t;
 auint  n;
 auint  s;
 asint  sx;
 asint  sy;
 uint16 t16;
 uint16 u16;


 /* Prepare previous tile locations */

 ptx = levelscr_x >> 3;
 pty = levelscr_y >> 3;


 /* Scroll. Tricky for smoothening the scroll, also limiting to 8px at most.
 ** Note an exploit of C conversion rules between signed and unsigned. */

 t16 = levelscr_tx - levelscr_x;
 t   = levelscr_psx;
 i   = (auint)(t16 & 0xFFU);
 if ((t16 & 0x8000U) == 0U){ /* Right scroll */
  if (t16 > 0x0080U){ i = 0x80U; }
  i += 15U;
  i >>= 4;
  t ++;
  if ((asint)(t) >= (asint)(i)){ t = i; }
 }else{                      /* Left scroll */
  if (t16 < 0xFF80U){ i = 0x80U; }
  i >>= 4;
  i |= 0xF0U;
  t --;
  if ((asint)(t) <= (asint)(i)){ t = i; }
 }
 levelscr_psx = t;
 if (levelscr_mag != 0U){    /* Screen shaking */
  i = random_get();
  if (levelscr_mag > 4U){ i &= 0xFU; j = 8U; }
  else                  { i &= 0x7U; j = 4U; }
  t = (t + i) - j;
  if ((t & 0x80U) != 0U){
   if (t < 0xF8U){ t = 0xF8U; }
  }else{
   if (t > 0x08U){ t = 0x08U; }
  }
  t16 = (uint16)((asint)(t));
  levelscr_x += t16;
  if      (levelscr_x >=     0xFF00U){ levelscr_x = 0U; }
  else if (levelscr_x >= levelscr_bx){ levelscr_x = levelscr_bx; }
  else                               {}
 }else{                      /* Normal (no shaking) output */
  t16 = (uint16)((asint)(t));
  levelscr_x += t16;
  if      (levelscr_x >=     0xFF00U){ levelscr_x = 0U; }
  else if (levelscr_x >= levelscr_bx){ levelscr_x = levelscr_bx; }
  else                               {}
 }

 t16 = levelscr_ty - levelscr_y;
 t   = levelscr_psy;
 i   = (auint)(t16 & 0xFFU);
 if ((t16 & 0x8000U) == 0U){ /* Down scroll */
  if (t16 > 0x0080U){ i = 0x80U; }
  i += 15U;
  i >>= 4;
  t ++;
  if ((asint)(t) >= (asint)(i)){ t = i; }
 }else{                      /* Up scroll */
  if (t16 < 0xFF80U){ i = 0x80U; }
  i >>= 4;
  i |= 0xF0U;
  t --;
  if ((asint)(t) <= (asint)(i)){ t = i; }
 }
 levelscr_psy = t;
 if (levelscr_mag != 0U){    /* Screen shaking */
  i = random_get();
  if (levelscr_mag > 4U){ i &= 0xFU; j = 8U; }
  else                  { i &= 0x7U; j = 4U; }
  t = (t + i) - j;
  if ((t & 0x80U) != 0U){
   if (t < 0xF8U){ t = 0xF8U; }
  }else{
   if (t > 0x08U){ t = 0x08U; }
  }
  t16 = (uint16)((asint)(t));
  levelscr_y += t16;
  if      (levelscr_y >=     0xFF00U){ levelscr_y = 0U; }
  else if (levelscr_y >= levelscr_by){ levelscr_y = levelscr_by; }
  else                               {}
  levelscr_mag --;
 }else{                      /* Normal (no shaking) output */
  t16 = (uint16)((asint)(t));
  levelscr_y += t16;
 }


 /* Prepare tile locations */

 tx = levelscr_x >> 3;
 ty = levelscr_y >> 3;


 /* Set up row selectors & X shift map, now only assuming normal map (no
 ** parallax parts) */

 n = 11U + 8U - ((auint)(levelscr_y) & 7U);  /* Next row */
 s = ((auint)(levelscr_x) & 7U);             /* X scroll */
 rows[0] = 11U;                              /* Game screen always begins at line 11 (above is status) */
 rows[1] = ((auint)(levelscr_y) & 7U) + 48U; /* First partial row */
 rows[2] = s;
 xshm[0] = s;
 t = 8U + 48U;
 i = 3U;
 j = 1U;
 do{
  rows[i] = n; i ++; /* Scanline to enter at */
  rows[i] = t; i ++; /* Tile row to render there */
  rows[i] = s; i ++; /* X shift */
  xshm[j] = s; j ++; /* X shift for sprites */
  t += 8U;
  n += 8U;
 }while (j < 24U);


 /* Draw necessary elements on the map. Here the top & bottom parallax region
 ** is not generated, simply written over by wrapping map content. These
 ** regions will be filled proper further below. */

 if ((levelscr_flg & LEVELSCR_REDRAW) != 0U){ /* Need complete redraw */

  /* Notes here: A normal frame seems not to be enough for this... Maybe
  ** splitting will be necessary! */

  levelscr_flg &= (auint)(~(auint)(LEVELSCR_REDRAW));

  t16 = 0U;
  u16 = ty - 16U;
  for (j = 0U; j < 24U; j ++){
   for (i = 0U; i < 25U; i ++){
    vram[t16 + i] = level_gettile(tx + i, u16);
   }
   t16 += 25U;
   u16 ++;
  }

 }else{ /* Only scroll (a row and a column might have to be produced) */

  sx = 0;
  sy = 0;
  if (ptx < tx){       sx = -1; } /* Scrolls to the right */
  else if (ptx != tx){ sx =  1; } /* Scrolls to the left */
  if (pty < ty){       sy = -1; } /* Scrolls down */
  else if (pty != ty){ sy =  1; } /* Scrolls up */

  M74_VramMove(sx, sy);

  t =  0U;
  if (sx  < 0){ t = 24U; }
  if (sx != 0){
   t16 = 0U;
   u16 = ty - 16U;
   for (j = 0U; j < 24U; j ++){
    vram[t16 + t] = level_gettile(tx + t, u16);
    t16 += 25U;
    u16 ++;
   }
  }

  t =  0U;
  if (sy  < 0){ t = 23U; }
  if (sy != 0){
   t16 = (uint16)(t) * 25U;
   u16 = ty - 16U + t;
   for (i = 0U; i < 25U; i ++){
    vram[t16 + i] = level_gettile(tx + i, u16);
   }
  }

 }

 /* Top / bottom parallax regions */

 parallax_render(levelscr_x, ty, levelscr_flg, level_h);
}
