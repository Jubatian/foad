/*
 *  Dragon - Supply actors
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



#include "acsupply.h"
#include "acsupp.h"
#include "dragon.h"
#include "spritelv.h"
#include "gstat.h"
#include "sound.h"
#include <uzebox.h>



/* Supply actor types */
#define ACT_STAR     0x05U
#define ACT_MEAT     (ACT_STAR + 0x01U)
#define ACT_HGLASS   (ACT_STAR + 0x02U)
#define ACT_LIFE     (ACT_STAR + 0x03U)
#define ACT_FIRE     (ACT_STAR + 0x04U)
#define ACT_ENERGY   (ACT_STAR + 0x05U)



/*
** Processes the passed supply actor. Returns the health of the actor: if
** this is zero, the actor is destroyed.
*/
auint acsupply_process(mapact_t* actor, auint typ)
{
 if (acsupp_iscordneardragon(actor->spr.xpos, actor->spr.ypos, 0x0810U)){

  switch (typ){

   case ACT_STAR:
    gstat_score_add(50U);  /* Collected star */
    break;

   case ACT_MEAT:
    gstat_score_add(10U);  /* Collected meat */
    dragon_mod(DRAGON_STA_HP | DRAGON_P_ADD, 32U);
    break;

   case ACT_HGLASS:
    gstat_score_add(10U);  /* Collected hourglass */
    gstat_time_add(48U);
    break;

   case ACT_LIFE:
    gstat_score_add(100U); /* Collected life capacity increment */
    dragon_mod(DRAGON_CAP_HP | DRAGON_P_ADD, 1U);
    break;

   case ACT_FIRE:
    gstat_score_add(100U); /* Collected fire capacity increment */
    dragon_mod(DRAGON_CAP_FI | DRAGON_P_ADD, 1U);
    break;

   case ACT_ENERGY:
    gstat_score_add(100U); /* Collected energy capacity increment */
    dragon_mod(DRAGON_CAP_EN | DRAGON_P_ADD, 1U);
    break;

   default:
    break;

  }

  sound_effect(SOUND_ITEM, 0x40U);

  return 0U;            /* Destroyed */

 }

 return 1U;
}



/*
** Renders the passed supply actor.
*/
void  acsupply_render(mapact_t* actor, auint typ)
{
 spritelv_blit(&(actor->spr), 0x96U + typ - ACT_STAR, M74_SPR_I2 | M74_SPR_MASK, REC_FLAT_0);
}
