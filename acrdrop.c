/*
 *  Dragon - Rock dropper actor
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
** Logic and Render for rock dropper actor.
**
** This is a man with a pile of rock who is supposed to sit at a ledge,
** pushing rocks onto the dragon. Initial logic puts it on the nearest
** ledge, then if the dragon is on the appropriate side, he would
** periodically push out a rock.
*/



#include "acrdrop.h"
#include "acsupp.h"
#include "dragon.h"
#include "spritelv.h"
#include "passable.h"
#include "eproj.h"
#include "fireball.h"
#include "random.h"
#include "gstat.h"
#include <uzebox.h>



/*
** Processes the passed rock dropper actor. Returns the health of the actor:
** if this is zero, the actor is destroyed.
*/
auint acrdrop_process(mapact_t* actor)
{
 auint  rnd = random_get();
 auint  vis;
 asint  vx;
 asint  lx;
 auint  d0 = actor->d0;
 auint  d1 = actor->d1;
 uint16 axp = actor->spr.xpos;
 uint16 ayp = actor->spr.ypos;
 uint16 t16;

 /*
 ** Rock dropper's layout:
 **
 ** d0: bits 0-5: Animation frame & action timer
 **     bit    6: If set, pushing, otherwise idle
 **     bit    7: If set, on fire
 ** d1: bits 0-7: Health (negated as starting value is zero)
 **               0x00 indicates uninitialized (need to put it on a ledge).
 **
 ** Behaviour:
 **
 ** Only active if the dragon is detectable for him and it is in a suitable
 ** direction. Pushes (small) rocks over him.
 */

 /* Initialize rock dropper, moving it to a nearby ledge */

 if (d1 == 0x00U){

  d1  = 0x01U; /* Initialized */
  vis = 8U;
  while (1){
   if (passable(axp - vis, ayp + 8U)){
    axp = axp - (auint)(vis - 8U);
    actor->spr.xpos = axp;
    break;
   }
   if (passable(axp + vis, ayp + 8U)){
    axp = axp + (auint)(vis - 8U);
    actor->spr.xpos = axp;
    break;
   }
   vis += 8U;
   if (vis == 32U){
    d1 = 0xFFU; /* Assume broken */
    break;
   }
  }

 }

 /* Pre-check dragon location */

 vx = 0;
 if ((axp + 16U) < (dragon_spr.xpos)){ vx =  1; }
 if ((axp) > (dragon_spr.xpos + 16U)){ vx = -1; }
 lx = 0;
 if (passable(axp + 8U, ayp + 8U)){ lx =  6; }
 if (passable(axp - 8U, ayp + 8U)){ lx = -6; }
 if (lx == 0){ d1 = 0xFFU; } /* Assume broken (should not normally happen) */

 /* Process */

 if ((d0 & 0x40U) != 0U){ /* Pushing */

  if ((d0 & 0x1FU) == 0x08U){ /* Creates rock at this point */
   eproj_new(axp + lx, ayp, vx, 0, EPROJ_SROCK);
  }
  if ((d0 & 0x1FU) == 0x1FU){ /* Ends pushing so actor may push another rock */
   d0 &= ~0x40U;
  }

 }else{ /* Waiting for opportunity */

  actor->spr.xpos += lx; /* "lean over" the ledge while checking for the dragon */
  vis = acsupp_isvisible(&(actor->spr), &dragon_spr);
  actor->spr.xpos -= lx;

  t16 = dragon_spr.ypos;
  if ( (vis) &&
       (ayp < t16) &&
       ((rnd & 0x0FU) == 0x05U) ){
   t16 -= ayp;
   if ( ((lx > 0) && (dragon_spr.xpos >= axp) && ((dragon_spr.xpos - axp) <= t16)) ||
        ((lx < 0) && (dragon_spr.xpos <= axp) && ((axp - dragon_spr.xpos) <= t16)) ){
    d0 |= 0x40U; /* New rock */
    d0 &= 0xC0U;
   }
  }

 }

 return acsupp_procfin_fire(actor, ((uint16)(d1) << 8) | d0, 0x0F18U, 0x80C0U);
}



/*
** Renders the passed rock dropper actor.
*/
void  acrdrop_render(mapact_t* actor)
{
 auint fr1;
 auint fr2;
 auint flg = M74_SPR_I1 | M74_SPR_MASK;
 auint d0  = actor->d0;

 if ( ((d0 & 0x40U) != 0U) &&
      ((d0 & 0x3FU) <  0x08U) ){
  fr1 = 0x8BU; /* Archer run1 gives the human (pushing) */
  fr2 = 0xAAU; /* Rocks (pushed) */
 }else{
  fr1 = 0x8AU; /* Archer run0 gives the human */
  fr2 = 0xA9U; /* Rocks */
 }

 if (passable(actor->spr.xpos + 8U, actor->spr.ypos + 8U)){
  flg |= M74_SPR_FLIPX; /* Rock pile faces the ledge */
 }

 acsupp_rendernpc(actor, fr1, d0);
 spritelv_blit(&(actor->spr), fr2, flg, REC_ROCK);
}
