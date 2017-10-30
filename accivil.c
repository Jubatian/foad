/*
 *  Dragon - Civilian actor
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



#include "accivil.h"
#include "acsupp.h"
#include "dragon.h"
#include "spritelv.h"
#include "passable.h"
#include "fireball.h"
#include "gstat.h"
#include "random.h"
#include <uzebox.h>



/*
** Processes the passed civilian actor. Returns the health of the actor: if
** this is zero, the actor is destroyed.
*/
auint accivil_process(mapact_t* actor)
{
 auint  rnd = random_get();
 asint  vx;
 auint  vis;
 auint  d0  = actor->d0;
 auint  d1  = actor->d1;
 uint16 axp = actor->spr.xpos;
 uint16 ayp = actor->spr.ypos;

 /*
 ** Civilian's layout:
 **
 ** d0: bits 0-5: Animation frame & action timer
 **     bit    6: Walking direction, 1: left
 **     bit    7: If set, on fire
 ** d1: bits 1-7: Health (negated as starting value is zero)
 **     bit    0: Standing still if set
 **
 ** Behaviour:
 **
 ** Normally walks around randomly, once the dragon hurts her, she flees.
 */

 vx = actor->spr.xvel;

 if ((d1 & 0xFEU) != 0U){ /* Injured: Fleeing mode: run away */

  if (dragon_spr.xpos < axp){
   actor->spr.xvel =  2;
  }else{
   actor->spr.xvel = -2;
  }

  if (!acsupp_iscordneardragon(axp, ayp, 0x8080U)){
   d1 = 0xFFU;  /* Civilian escaped, destroy (won't appear any more) */
  }

 }else{         /* Normal mode, walk around */

  if ((rnd & 0xF0U) == 0U){ /* Determine new action */

   vis = acsupp_isvisible(&(actor->spr), &dragon_spr);

   vx = 0;
   if ((rnd & 3U) == 0U){ vx =  1; }
   if ((rnd & 3U) == 1U){ vx = -1; }
   if ((vis) && ((rnd & 0xCU) == 0U)){ /* If dragon is visible, sometimes walk towards */
    if ((dragon_spr.xpos + 16U) < (axp)){ vx = -1; }
    if ((dragon_spr.xpos) > (axp + 16U)){ vx =  1; }
   }

   if (vx == 0){
    d1 |= 0x01U;
   }else{
    d1 &= 0xFEU;
    if (vx < 0){ d0 |= 0x40U; }
    else       { d0 &= 0xBFU; }
   }

   actor->spr.xvel = vx;
   acsupp_haltatledge(&(actor->spr));

  }

 }

 return acsupp_procfin_fire(actor, ((uint16)(d1) << 8) | d0, 0x4B30U, 0x80C0U);
}



/*
** Renders the passed civilian actor.
*/
void  accivil_render(mapact_t* actor)
{
 auint fra;
 auint flg = M74_SPR_I1 | M74_SPR_MASK;
 auint d0  = actor->d0;
 auint vab;

 if (actor->spr.xvel > 0){ vab =  (actor->spr.xvel); }
 else                    { vab = -(actor->spr.xvel); }

 if       (vab == 1U){
  if      ((d0 & 0x7U) <  3U){ fra = 0xACU; }
  else if ((d0 & 0x7U) <  6U){ fra = 0xADU; }
  else                       { fra = 0xAEU; }
 }else if (vab == 2U){
  if      ((d0 & 0xFU) <  5U){ fra = 0xACU; }
  else if ((d0 & 0xFU) < 10U){ fra = 0xADU; }
  else                       { fra = 0xAEU; }
 }else{
  fra = 0xABU;
 }

 if (actor->spr.xvel > 0){
  flg |= M74_SPR_FLIPX;
 }
 if (actor->spr.xvel == 0){
  if (actor->spr.xpos < dragon_spr.xpos){
   flg |= M74_SPR_FLIPX;
  }
 }

 spritelv_blit(&(actor->spr), fra, flg, REC_STRAIGHT);

 if ((d0 & 0x80U) != 0U){ /* Fire */
  fireball_render_fire(
      (actor->spr.xpos),
      (actor->spr.ypos) - 8U,
      0x1FU - (d0 & 0x1FU));
 }
}
