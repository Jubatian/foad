/*
 *  Dragon - Rock fall actors
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


/*
** Logic and Render for rock fall actors.
**
** There are 6 types of rock fall actors: two rock sizes and three positions
** relative to the dragon (before, after, over). They work by causing a rock
** to fall when the dragon is at the appropriate location relative to them,
** the rock is released from the ceiling above. They all release one rock,
** then clear themselves.
*/



#include "acrfall.h"
#include "dragon.h"
#include "eproj.h"
#include "random.h"
#include "passable.h"



/* Rock fall actor types */
#define ACT_SM_L     0x10U
#define ACT_SM_R     (ACT_SM_L + 0x01U)
#define ACT_SM_M     (ACT_SM_L + 0x02U)
#define ACT_LG_L     (ACT_SM_L + 0x03U)
#define ACT_LG_R     (ACT_SM_L + 0x04U)
#define ACT_LG_M     (ACT_SM_L + 0x05U)



/*
** Processes the passed rockfall actor. Returns the health of the actor: if
** this is zero, the actor is destroyed.
*/
auint acrfall_process(mapact_t* actor, auint typ)
{
 auint  rnd = random_get() & 0x0FU;
 uint16 axp = actor->spr.xpos;
 uint16 ayp = actor->spr.ypos;
 uint16 trg;
 auint  rf  = 0U;
 asint  xv;
 auint  rtp;

 /*
 ** Rockfall actor layout:
 **
 ** d0: Unused
 ** d1: Unused
 */

 if ( ((dragon_spr.ypos + 64U) > (ayp)) && /* Dragon should be reasonably level with actor */
      ((dragon_spr.ypos) < (ayp + 64U)) ){

  switch (typ){

   case ACT_SM_L:
   case ACT_LG_L:

    /* Dragon triggers rockfall when on the left of the actor */

    if ( ((dragon_spr.xpos) < (axp + 16U)) &&
         (rnd == 0x3U) ){ rf = 1U; }
    break;

   case ACT_SM_R:
   case ACT_LG_R:

    /* Dragon triggers rockfall when on the right of the actor */

    if ( ((dragon_spr.xpos + 16U) > (axp)) &&
         (rnd == 0x7U) ){ rf = 1U; }
    break;

   case ACT_SM_M:
   case ACT_LG_M:

    /* Dragon triggers rockfall when over the actor */

    if ( ((dragon_spr.ypos) < (ayp + 32U)) &&
         ((dragon_spr.xpos + 64U) > (axp)) &&
         ((dragon_spr.xpos) < (axp + 64U)) &&
         (rnd == 0x9U) ){ rf = 1U; }
    break;

   default:

    break;

  }

 }

 /* Process rockfall */

 if (rf){

  /* Find ceiling. If there is no ceiling, launch from "sky", useful for
  ** in-cave setting (with cave ceiling). */

  ayp = (ayp - 9U) & 0xFFF8U;
  if (ayp < 200U){ trg = 0xFFF8U; }
  else           { trg = ayp - 208U; }
  while (ayp != trg){
   if (!passable(axp, ayp)){ break; }
   ayp -= 8U;
  }

  if ((ayp + 16U) < dragon_spr.ypos){ /* Launch if above dragon */

   ayp += 12U;    /* Certainly passable (tile below the ceiling) */

   xv = 0;
   if ((typ == ACT_SM_L) || (typ == ACT_LG_L)){ xv = -2; }
   if ((typ == ACT_SM_R) || (typ == ACT_LG_R)){ xv =  2; }
   rtp = EPROJ_SROCK;
   if ((typ == ACT_LG_L) || (typ == ACT_LG_R) || (typ == ACT_LG_M)){ rtp = EPROJ_LROCK; }

   eproj_new(axp, ayp, xv, 0, rtp); /* Launch rock */

  }else{

   rf = 0U;       /* Couldn't launch */

  }

 }

 /* If a rock was launched, clear actor */

 if (rf){ return 0x00U; }
 else   { return 0xFFU; }

}



/*
** Renders the passed rockfall actor.
*/
void  acrfall_render(mapact_t* actor, auint typ)
{
}
