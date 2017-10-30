/*
 *  Dragon - Prisoner actor
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



#include "acprison.h"
#include "acsupp.h"
#include "dragon.h"
#include "spritelv.h"
#include "passable.h"
#include "fireball.h"
#include "gstat.h"
#include <uzebox.h>



/*
** Processes the passed prisoner actor. Returns the health of the actor: if
** this is zero, the actor is destroyed.
*/
auint acprison_process(mapact_t* actor)
{
 auint  d0  = actor->d0;
 auint  d1  = actor->d1;
 uint16 axp = actor->spr.xpos;
 uint16 ayp = actor->spr.ypos;

 /*
 ** Prisoner's layout:
 **
 ** d0: bits 0-5: Animation frame & action timer
 **     bit    6: Running direction (1: right)
 **     bit    7: If set, on fire.
 ** d1: Health (negated as starting value is zero)
 **     0x00: In cell.
 **
 ** Behaviour:
 **
 ** Initially stays in cell where he can not be harmed (health: inverted d1
 ** remains 0xFF). The dragon can free him by standing over him (reduces
 ** health by 1). Then he tries to run in an open direction while the dragon's
 ** fire might harm him. He is free when he is off the map.
 */

 if (d1 == 0U){ /* Prisoner in cell: Waits for the dragon to free him */

  actor->spr.xvel = 0;
  d0 = 0x00U;   /* Run to left */

  if (acsupp_iscordneardragon(axp, ayp, 0x0810U)){

   d1 ++;       /* Free the prisoner */
   gstat_score_add(75U); /* Freed a prisoner: add score. */

   if (passable(axp + 128U, ayp - 8U)){
    d0 |= 0x40U;   /* If right is open, run to right (no random choice so multiple prisoners try to flee together) */
   }

  }

 }else{         /* Free prisoner: Runs */

  if (actor->spr.xvel == 0){ /* Escape path was blocked: Turn around */
   d0 ^= 0x40U;
  }

  if (!acsupp_iscordneardragon(axp, ayp, 0x8080)){
   d1 = 0xFFU;            /* Prisoner successfully escaped */
  }

 }

 if (d1 != 0U){ /* Prisoner is not in cell */

  if ((d0 & 0x40U) != 0U){
   actor->spr.xvel =  2;
  }else{
   actor->spr.xvel = -2;
  }

  return acsupp_procfin_fire(actor, ((uint16)(d1) << 8) | d0, 0x4B30U, 0x80C0U);

 }else{

  return 1U; /* Waiting in cell */

 }
}



/*
** Renders the passed prisoner actor.
*/
void  acprison_render(mapact_t* actor)
{
 auint fra;
 auint flg = M74_SPR_I1 | M74_SPR_MASK;
 auint d0  = actor->d0;
 auint d1  = actor->d1;

 if       (d1 == 0U){             /* Under bars */
  fra = 0x93U;
 }else if (actor->spr.xvel == 0){ /* Standing */
  fra = 0x8FU;
 }else{                           /* Running */
  if      ((d0 & 0x7U) <  3U){ fra = 0x90U; }
  else if ((d0 & 0x7U) <  6U){ fra = 0x91U; }
  else                       { fra = 0x92U; }
 }

 if (actor->spr.xvel > 0){
  flg |= M74_SPR_FLIPX;
 }

 spritelv_blit(&(actor->spr), fra, flg, REC_STRAIGHT);

 if ((d0 & 0x80U) != 0U){ /* Fire */
  fireball_render_fire(
      (actor->spr.xpos),
      (actor->spr.ypos) - 8U,
      0x1FU - (d0 & 0x1FU));
 }
}
