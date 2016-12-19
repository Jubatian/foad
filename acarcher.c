/*
 *  Dragon - Archer actor
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



#include "acarcher.h"
#include "acsupp.h"
#include "dragon.h"
#include "spritelv.h"
#include "passable.h"
#include "eproj.h"
#include "fireball.h"
#include "random.h"
#include "sound.h"
#include "gstat.h"
#include <uzebox.h>



/*
** Processes the passed archer actor. Returns the health of the actor: if this
** is zero, the actor is destroyed.
*/
auint acarcher_process(mapact_t* actor)
{
 auint  rnd = random_get();
 auint  fid;
 auint  vis;
 asint  vy;
 asint  vx;
 auint  d0 = actor->d0;
 auint  d1 = actor->d1;
 uint16 axp = actor->spr.xpos;
 uint16 ayp = actor->spr.ypos;

 /*
 ** Archer's layout:
 **
 ** d0: bits 0-5: Animation frame & action timer
 **     bit    6: If set, shooting, otherwise idle
 **     bit    7: If set, on fire
 ** d1: bits 0-7: Health (negated as starting value is zero)
 **
 ** Behaviour:
 **
 ** Only active if the dragon is detectable for him. Tries to shoot arrows at
 ** him while attempting to stay farther away.
 */

 actor->spr.xvel = 0;

 if       ((d0 & 0x80U) != 0U){ /* On fire: Flee! (don't caring about ledges or whatever) */

  if (dragon_spr.xpos < axp){
   actor->spr.xvel =  2;
  }else{
   actor->spr.xvel = -2;
  }
  if ((d0 & 0x1FU) == 0x00U){
   d0 &= ~0x80U;
  }

 }else if ((d0 & 0x40U) != 0U){ /* Firing */

  if ((d0 & 0x0FU) == 0x00U){
   if (dragon_spr.xpos < axp){ vx = -6 + (asint)(rnd & 1U); }
   else                      { vx =  6 - (asint)(rnd & 1U); }
   vy = acsupp_getshotvy(axp, ayp - 7U, &dragon_spr, vx);
   eproj_new(axp, ayp - 7U, vx, vy, EPROJ_ARROW);
   d0 &= ~0x40U;
   sound_effect(SOUND_ARROW, 0x38U);
  }

 }else{ /* Look out for the dragon */

  vis = acsupp_isvisible(&(actor->spr), &dragon_spr);

  if (vis || (!dragon_issilent())){

   vx = 0;

   if ( (actor->spr.xacc == 0) && /* Not running against a wall */
        ((rnd & 0x3FU) != 0U) ){  /* Occasionally shoot even while moving away */

    if ((dragon_spr.ypos) < (ayp + 16U)){ /* Dragon in level or above: danger! */

     if ((dragon_spr.xpos) < (axp)){

      if       ((dragon_spr.xpos + 80U - (rnd & 15U)) > (axp)){ /* Within 80 px on the left */
       vx =  2;
      }else if ((dragon_spr.xpos + 90U - (rnd & 15U)) < (axp)){ /* Farther than 90 px on the left */
       vx = -1;
      }else{}

     }else{

      if       ((dragon_spr.xpos) < (axp + 80U - (rnd & 15U))){ /* Within 80 px on the right */
       vx = -2;
      }else if ((dragon_spr.xpos) > (axp + 90U - (rnd & 15U))){ /* Farther than 90 px on the right */
       vx =  1;
      }else{}

     }

    }else{

     if ((dragon_spr.ypos) > (ayp + 24U)){ /* Dragon is below: Get as near as possible */

      if       (dragon_spr.xpos < axp){
       vx = -1;
      }else if (dragon_spr.xpos > axp){
       vx =  1;
      }else{}

     }

    }

    if       (vx > 0){
     if (acsupp_isatledge(&(actor->spr),  4)){ vx = 0; }
    }else if (vx < 0){
     if (acsupp_isatledge(&(actor->spr), -4)){ vx = 0; }
    }else{}

   }

   if ( (vx == 0) && vis ){ /* No movement and dragon is visible: Shoot! */
    d0 = 0x40U | 0x01U;
   }else{
    actor->spr.xvel = vx;
   }

  }

 }

 fid = fireball_getat(&(actor->spr));

 if (fid != FIREBALL_N){
  fireball_age(fid, 0x20U);
  if (d1 < 0xE8U){ d1 += 0x18U; }
  else{            d1  = 0xFFU; }
  d0 = 0x80U; /* Cancel firing too */
  if (d1 == 0xFFU){
   gstat_score_sub(25U); /* Killed an archer (kill penalty). */
  }
 }

 d0 = (d0 & (~0x3FU)) | ((d0 + 1U) & 0x3FU);

 actor->d0 = d0;
 actor->d1 = d1;

 if (d1 == 0xFFU){ return 0U; }
 else            { return 1U; }
}



/*
** Renders the passed archer actor.
*/
void  acarcher_render(mapact_t* actor)
{
 auint fra;
 auint flg = M74_SPR_I1 | M74_SPR_MASK;
 auint d0  = actor->d0;
 auint vab;

 if (actor->spr.xvel > 0){ vab =  (actor->spr.xvel); }
 else                    { vab = -(actor->spr.xvel); }

 if       (vab == 1U){
  if      ((d0 & 0xFU) <  5U){ fra = 0x8AU; }
  else if ((d0 & 0xFU) < 10U){ fra = 0x8BU; }
  else                       { fra = 0x8CU; }
 }else if (vab == 2U){
  if      ((d0 & 0x7U) <  3U){ fra = 0x8AU; }
  else if ((d0 & 0x7U) <  6U){ fra = 0x8BU; }
  else                       { fra = 0x8CU; }
 }else if ((d0 & 0x40U) != 0U){
  if      ((d0 & 0xFU) <  4U){ fra = 0x89U; }
  else if ((d0 & 0xFU) < 10U){ fra = 0x8DU; }
  else                       { fra = 0x8EU; }
 }else{
  fra = 0x89U;
 }

 if (actor->spr.xvel > 0){
  flg |= M74_SPR_FLIPX;
 }else if (actor->spr.xvel == 0){
  if ((dragon_spr.xpos) > (actor->spr.xpos)){
   flg |= M74_SPR_FLIPX;
  }
 }else{}

 spritelv_blit(&(actor->spr), fra, flg, REC_FLAT_0);

 if ((d0 & 0x80U) != 0U){ /* Fire */
  fireball_render_fire(
      (actor->spr.xpos),
      (actor->spr.ypos) - 8U,
      0x1FU - (d0 & 0x1FU));
 }
}
