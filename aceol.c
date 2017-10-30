/*
 *  Dragon - End of level actor
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
#include "spritelv.h"
#include "gstat.h"
#include "global.h"
#include "sound.h"
#include <uzebox.h>



/*
** Processes the passed end of level actor. Returns the health of the actor:
** if this is zero, the actor is destroyed.
*/
auint aceol_process(mapact_t* actor)
{
 if (acsupp_iscordneardragon(actor->spr.xpos, actor->spr.ypos, 0x0810U)){

  gstat_score_add(100U); /* Level end reached */
  sound_effect(SOUND_ITEM, 0x40U);
  global_shared[7] = 1U; /* Mark level end */

  return 0U;             /* Destroyed */

 }

 return 1U;
}



/*
** Renders the passed end of level actor.
*/
void  aceol_render(mapact_t* actor)
{
 spritelv_blit(&(actor->spr), 0x9CU, M74_SPR_I2 | M74_SPR_MASK, REC_FLAT_0);
}
