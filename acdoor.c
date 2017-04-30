/*
 *  Dragon - Door actor
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



#include "acdoor.h"
#include "fireball.h"
#include "levelscr.h"
#include "gstat.h"
#include "sound.h"
#include <uzebox.h>



/*
** Processes the passed door actor. Returns the health of the actor: if this
** is zero, the actor is destroyed.
*/
auint acdoor_process(mapact_t* actor)
{
 auint d0 = actor->d0;
 auint d1 = actor->d1;
 auint fid;

 /*
 ** Door's layout:
 **
 ** d0: bits 0-7: Animation frame (burning)
 ** d1: bits 0-7: Health (negated as starting value is zero)
 **
 ** Behaviour:
 **
 ** Stationary, the dragon can burn it down. When it is destroyed, it changes
 ** the L0 block underneath (door collapsed).
 */

 fid = fireball_getat(&(actor->spr));

 if (fid != FIREBALL_N){
  fireball_age(fid, 0x20U);
  if (d1 < 0xF0U){
   d1 += 0x10U;
  }else{
   d1  = 0xFFU;
  }
  if (d1 == 0xFFU){
   level_repadd((actor->spr.xpos >> 3), (actor->spr.ypos >> 3));
   levelscr_reset();
   levelscr_shake(3U);
   gstat_score_add(50U); /* Destroyed door */
   sound_effect(SOUND_BOOM, 0x80U);
  }
  if (d0 < 0xE0U){ d0 += 0x20U; }
  else{            d0  = 0xFFU; }
 }

 if (d0 > 0U){ d0 --; }

 actor->d0 = d0;
 actor->d1 = d1;

 if (d1 == 0xFFU){ return 0U; }
 else            { return 1U; }
}



/*
** Renders the passed door actor.
*/
void  acdoor_render(mapact_t* actor)
{
 auint  d0 = actor->d0;
 uint16 x  = actor->spr.xpos;
 uint16 y  = actor->spr.ypos;

 /* Fires */

 if (d0 > 0x00U){
  fireball_render_fire(x - 3U, y -  7U, d0 - 0x01U);
 }
 if (d0 > 0x40U){
  fireball_render_fire(x + 0U, y -  1U, d0 - 0x41U);
 }
 if (d0 > 0x80U){
  fireball_render_fire(x + 3U, y - 12U, d0 - 0x81U);
 }
 if (d0 > 0xC0U){
  fireball_render_fire(x + 1U, y - 17U, d0 - 0xC1U);
 }
}
