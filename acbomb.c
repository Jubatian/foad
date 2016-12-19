/*
 *  Dragon - Bomb actor
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



#include "acbomb.h"
#include "fireball.h"
#include "levelscr.h"
#include "gstat.h"
#include "global.h"
#include "random.h"
#include "spritelv.h"
#include "sound.h"
#include <uzebox.h>



/*
** Processes the passed bomb actor. Returns the health of the actor: if this
** is zero, the actor is destroyed.
*/
auint acbomb_process(mapact_t* actor)
{
 auint  d0 = actor->d0;
 auint  d1 = actor->d1;
 uint16 rpx;
 uint16 rpy;
 auint  fid;

 /*
 ** Bomb's layout:
 **
 ** d0: bits 0-7: Animation frame (burning)
 ** d1: bits 0-7: Health (negated as starting value is zero)
 **
 ** Behaviour:
 **
 ** Stationary, fireballs can set it off (need a few, but not too much).
 ** When destroyed, it changes five L0 blocks around itself.
 */

 fid = fireball_getat(&(actor->spr));

 if (fid != FIREBALL_N){
  fireball_age(fid, 0x20U);
  if (d1 < 0xC8U){
   d1 += 0x38U;
  }else{
   d1  = 0xFFU;
  }
  if (d0 < 0xE0U){ d0 += 0x20U; }
  else{            d0  = 0xFFU; }
 }

 if (d0 > 0U){
  d0 --;
  if (d1 != 0xFFU){ d1 ++; } /* Slowly burn towards explosion */
 }

 if (d1 == 0xFFU){
  rpx = actor->spr.xpos;
  rpy = actor->spr.ypos;
  fireball_new(rpx, rpy, (asint)(random_get() & 1U) - 5, -1);
  fireball_new(rpx, rpy, (asint)(random_get() & 1U) + 4, -1);
  fireball_new(rpx, rpy, (asint)(random_get() & 1U) - 4, -2);
  fireball_new(rpx, rpy, (asint)(random_get() & 1U) + 3, -2);
  fireball_new(rpx, rpy, (asint)(random_get() & 1U) - 3, -3);
  fireball_new(rpx, rpy, (asint)(random_get() & 1U) + 2, -3);
  rpx = rpx >> 3;
  rpy = rpy >> 3;
  level_repadd(rpx, rpy);
  level_repadd(rpx - 1U, rpy);
  level_repadd(rpx + 1U, rpy);
  level_repadd(rpx, rpy - 1U);
  level_repadd(rpx, rpy + 1U);
  levelscr_reset();
  levelscr_shake(8U);
  global_fadecolor = 0xBFU;
  global_palctr = GLOBAL_FADE_FLASH | GLOBAL_FADE_2X | GLOBAL_FADE_INC | GLOBAL_FADE_GAME;
  gstat_score_add(50U); /* Destroyed bomb */
  sound_effect(SOUND_BOOM, 0xFFU);
 }

 actor->d0 = d0;
 actor->d1 = d1;

 if (d1 == 0xFFU){ return 0U; }
 else            { return 1U; }
}



/*
** Renders the passed bomb actor.
*/
void  acbomb_render(mapact_t* actor)
{
 auint  d0 = actor->d0;
 auint  d1 = actor->d1;
 auint  fra;

 /* Bomb sprite */

 if (d1 == 0U){ fra = 0x94U; }
 else         { fra = 0x95U; }
 spritelv_blit(&(actor->spr), fra, M74_SPR_I2 | M74_SPR_MASK, REC_STRAIGHT);

 /* Fire */

 if (d0 > 0x00U){
  fireball_render_fire(
      (actor->spr.xpos),
      (actor->spr.ypos) - 5U,
      d0 - 0x01U);
 }
}
