/*
 *  Dragon - Door actor
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



#include "acdoor.h"
#include "acsupp.h"
#include "fireball.h"
#include "levelscr.h"
#include "gstat.h"
#include "sound.h"
#include <avr/pgmspace.h>
#include <uzebox.h>



/*
** Processes the passed door actor. Returns the health of the actor: if this
** is zero, the actor is destroyed.
*/
auint acdoor_process(mapact_t* actor)
{
 auint  d0 = actor->d0;
 auint  d1 = actor->d1;
 auint  fid;
 auint  dl0;
 uint16 xt = actor->spr.xpos >> 3;
 uint16 yt = actor->spr.ypos >> 3;

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
  d1 += 0x10U;
  if (d1 < 0x10U){ d1 = 0xFFU; }
  if (d1 == 0xFFU){
   level_repadd(xt, yt);
   levelscr_reset();
   levelscr_shake(3U);
   gstat_score_add(50U); /* Destroyed door */
   sound_effect(SOUND_BOOM, 0x80U);
  }
  d0 += 0x20U;
  if (d0 < 0x20U){ d0 = 0xFFU; }

 }else{

  dl0 = level_getl0(xt, yt);
  if (pgm_read_byte(RES_L0REPD_OFF | dl0) == dl0){ /* No replacement tile (already open) */
   d1 = 0xFFU; /* Door was destroyed somehow (such as by bomb) */
   gstat_score_add(50U); /* Score is added for this, too */
  }

 }

 if (d0 > 0U){ d0 --; }

 return acsupp_procfin(actor, d0, d1);
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
