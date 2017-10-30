/*
 *  Dragon - Swordsman actor
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



#include "acpike.h"
#include "acsupp.h"
#include "dragon.h"
#include "spritelv.h"
#include "passable.h"
#include "fireball.h"
#include "random.h"
#include "gstat.h"



/*
** Processes the passed swordsman actor. Returns the health of the actor: if
** this is zero, the actor is destroyed.
*/
auint acswords_process(mapact_t* actor)
{
 auint  vis;
 asint  vx;
 auint  d0 = actor->d0;
 auint  d1 = actor->d1;
 uint16 axp = actor->spr.xpos;
 uint16 ayp = actor->spr.ypos;

 /*
 ** Swordsman's layout:
 **
 ** d0: bits 0-5: Animation frame & action timer
 **     bit    6: If set, attacking
 **     bit    7: If set, on fire
 ** d1: bits 0-7: Health (negated as starting value is zero)
 **
 ** Behaviour:
 **
 ** Only active if the dragon is detectable for him. Walks towards him, then
 ** attacks. Fire doesn't scare him, and has good armor against it.
 */

 actor->spr.xvel = 0;

 if ((d0 & 0x40U) != 0U){ /* Attack animation */

  if ((d0 & 0x1FU) == 0x00U){
   d0 &= ~0x40U;
  }

 }else{ /* Look out for the dragon */

  vis = acsupp_isvisible(&(actor->spr), &dragon_spr);

  if (vis || (!dragon_issilent())){

   vx = 0;

   if       ((dragon_spr.xpos + 8U) < (axp)){ /* Walk towards dragon */
    vx = -1;
   }else if ((dragon_spr.xpos) > (axp + 8U)){
    vx =  1;
   }else{}

   actor->spr.xvel = vx;
   acsupp_haltatledge(&(actor->spr));

   if (acsupp_iscordindragon(axp, ayp)){ /* Within dragon: attack! */

    dragon_mod(DRAGON_STA_HP | DRAGON_P_SUB, (random_get() & 7U) + 8U);

    d0 &= 0x80U; /* Preserve fire if any */
    d0 |= 0x70U; /* Attack + Animation */

   }

  }

 }

 return acsupp_procfin_fire(actor, ((uint16)(d1) << 8) | d0, 0x1910U, 0xB0FFU);
}



/*
** Renders the passed swordsman actor.
*/
void  acswords_render(mapact_t* actor)
{
 auint fra;
 auint d0  = actor->d0;
 auint vab;

 if (actor->spr.xvel > 0){ vab =  (actor->spr.xvel); }
 else                    { vab = -(actor->spr.xvel); }

 if       ((d0 & 0x40U) != 0U){ /* Attack */
  if      ((d0 & 0xFU) <  4U){ fra = 0xA5U; }
  else if ((d0 & 0xFU) <  8U){ fra = 0xA6U; }
  else if ((d0 & 0xFU) < 12U){ fra = 0xA7U; }
  else                       { fra = 0xA8U; }
 }else if (vab == 1U){          /* Walk */
  if      ((d0 & 0xFU) <  5U){ fra = 0xA2U; }
  else if ((d0 & 0xFU) < 10U){ fra = 0xA3U; }
  else                       { fra = 0xA4U; }
 }else{                         /* Stand */
  fra = 0xA1U;
 }

 acsupp_rendernpc(actor, fra, d0);
}
