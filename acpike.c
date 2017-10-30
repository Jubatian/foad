/*
 *  Dragon - Pikeman actor
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
** Processes the passed pikeman actor. Returns the health of the actor: if
** this is zero, the actor is destroyed.
*/
auint acpike_process(mapact_t* actor)
{
 auint  vis;
 asint  vy;
 asint  vx;
 auint  d0 = actor->d0;
 auint  d1 = actor->d1;
 uint16 axp = actor->spr.xpos;
 uint16 ayp = actor->spr.ypos;

 /*
 ** Pikeman's layout:
 **
 ** d0: bits 0-5: Animation frame & action timer
 **     bit    6: If set, after attack timeout
 **     bit    7: If set, on fire
 ** d1: bits 0-7: Health (negated as starting value is zero)
 **
 ** Behaviour:
 **
 ** Only active if the dragon is detectable for him. If the dragon is roughly
 ** level with him, tries to charge. Damage depends on velocity difference:
 ** the dragon can impale himself on a pike, effective against a careless
 ** rush (which otherwise is a good strategy to get past archers).
 */

 actor->spr.xvel = 0;

 if       ((d0 & 0x80U) != 0U){ /* On fire: Flee! (don't caring about ledges or whatever) */

  if (dragon_spr.xpos < axp){
   actor->spr.xvel =  2;
  }else{
   actor->spr.xvel = -2;
  }

 }else if ((d0 & 0x40U) != 0U){ /* After attack */

  if ((d0 & 0x1FU) == 0x00U){
   d0 &= ~0x40U;
  }

 }else{ /* Look out for the dragon */

  vis = acsupp_isvisible(&(actor->spr), &dragon_spr);

  if (vis || (!dragon_issilent())){

   vx = 0;

   if ( ((dragon_spr.ypos + 32U) > (ayp)) &&
        ((dragon_spr.ypos) < (ayp + 32U)) ){ /* Roughly level with dragon */

    if       ((dragon_spr.xpos + 4U) < (axp)){ /* Run towards dragon (charge) */
     vx = -2;
    }else if ((dragon_spr.xpos) > (axp + 4U)){
     vx =  2;
    }else{}

   }

   actor->spr.xvel = vx;
   acsupp_haltatledge(&(actor->spr));

   if (acsupp_iscordindragon(axp, ayp)){ /* Within dragon: hit! */

    vx -= dragon_spr.xvel;
    if (vx < 0){ vx = -vx; }
    vy  = actor->spr.yvel;
    vy -= dragon_spr.yvel;
    if (vy < 0){ vy = -vy; }
    if (vx < vy){ vx = vy; } /* Attack power depends on velocity diff. */

    dragon_mod(DRAGON_STA_HP | DRAGON_P_SUB, ((random_get() & 3U) + 4U) * vx);

    d0 &= 0x0FU; /* After attack, at 0x20 - 0x2F in animation, */
    d0 |= 0x60U; /* so 0x20 - 0x11 until next attack */

   }

  }

 }

 return acsupp_procfin_fire(actor, ((uint16)(d1) << 8) | d0, 0x1930U, 0x80C0U);
}



/*
** Renders the passed pikeman actor.
*/
void  acpike_render(mapact_t* actor)
{
 auint fra;
 auint d0  = actor->d0;
 auint vab;

 if (actor->spr.xvel > 0){ vab =  (actor->spr.xvel); }
 else                    { vab = -(actor->spr.xvel); }

 if       (vab != 0U){ /* Run / Charge */
  if      ((d0 & 0x7U) <  3U){ fra = 0x9EU; }
  else if ((d0 & 0x7U) <  6U){ fra = 0x9FU; }
  else                       { fra = 0xA0U; }
 }else{                /* Stand */
  fra = 0x9DU;
 }

 acsupp_rendernpc(actor, fra, d0);
}
