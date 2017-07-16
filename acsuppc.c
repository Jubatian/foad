/*
 *  Dragon - Actor support functions, C
 *  Copyright (C) 2017 Sandor Zsuga (Jubatian)
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



#include "acsuppc.h"
#include "dragon.h"
#include "fireball.h"
#include "spritelv.h"
#include <uzebox.h>



/*
** Renders a non-player-character sprite. Such a sprite faces the dragon when
** not moving, and may be on fire. This can be used for typical 2x1 tile human
** characters. The fra parameter is the frame to draw, fir is whether the
** sprite should be on fire (bit 7 set; then bits 0-4 are the frame).
*/
void acsuppc_rendernpc(mapact_t* actor, auint fra, auint fir)
{
 auint flg = M74_SPR_I1 | M74_SPR_MASK;

 if (actor->spr.xvel > 0){
  flg |= M74_SPR_FLIPX;
 }else if (actor->spr.xvel == 0){
  if ((dragon_spr.xpos) > (actor->spr.xpos)){
   flg |= M74_SPR_FLIPX;
  }
 }else{}

 spritelv_blit(&(actor->spr), fra, flg, REC_FLAT_0);

 if ((fir & 0x80U) != 0U){ /* Fire */
  fireball_render_fire(
      (actor->spr.xpos),
      (actor->spr.ypos) - 8U,
      0x1FU - (fir & 0x1FU));
 }
}
