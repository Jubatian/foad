/*
 *  Dragon - Map objects (actors, projectiles), processing
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



#include "mapobjp.h"
#include "physics.h"
#include "mapact.h"
#include "actordim.h"
#include "fireball.h"
#include "eproj.h"
#include "acarcher.h"
#include "acdoor.h"
#include "acbomb.h"
#include "acprison.h"
#include "aceol.h"
#include "acsupply.h"
#include "acpike.h"
#include "acswords.h"



/*
** General processing conventions
**
** The actors process only their individual actions without any collisions
** with other actors or objects. They may fire projectiles as needed.
**
** Their processing provides a health info which is used here to destroy them
** (if their health drops to zero).
*/



/* Actor types. Supply actors are in the range 0x05 - 0x0A inclusive. */
#define ACT_EOL      0x00U
#define ACT_ARCHER   0x01U
#define ACT_DOOR     0x02U
#define ACT_BOMB     0x03U
#define ACT_PRISON   0x04U
#define ACT_PIKE     0x0BU
#define ACT_SWORDS   0x0CU



/*
** Initializes map objects. Should be called upon map initialization.
*/
void  mapobjp_init(void)
{
 mapact_init();
 fireball_init();
 eproj_init();
}



/*
** Processes map objects expect the dragon.
*/
void  mapobjp_process(void)
{
 auint    i;
 auint    hlt;
 mapact_t actor;
 auint    typ;

 /* Process fireballs (highest priority) */

 fireball_process();

 /* Process enemy projectiles (next priority) */

 eproj_process();

 /* Process actors (lowest priority) */

 mapact_update(); /* Clear away actors got too far from dragon */

 for (i = 0U; i < MAPACT_ACN; i++){

  if (mapact_get(i, &actor) == 0U){

   /* Actor exists, process it */

   typ = actor.typ & 0x3FU;

   actordim_get(typ, &(actor.spr));
   actor.spr.flg = PHYSICS_F_GRND | PHYSICS_F_PWR_X;
   physics_proc(&(actor.spr));

   /* Actor specifics */

   switch (typ){
    case ACT_ARCHER: hlt = acarcher_process(&actor); break;
    case ACT_DOOR:   hlt = acdoor_process(&actor);   break;
    case ACT_BOMB:   hlt = acbomb_process(&actor);   break;
    case ACT_PRISON: hlt = acprison_process(&actor); break;
    case ACT_EOL:    hlt = aceol_process(&actor);    break;
    case ACT_PIKE:   hlt = acpike_process(&actor);   break;
    case ACT_SWORDS: hlt = acswords_process(&actor); break;
    default:         hlt = acsupply_process(&actor, typ); break;
   }

   /* Done, set new data or destroy if health dropped to zero */

   if (hlt == 0U){
    mapact_destroy(actor.id);
   }else{
    mapact_set(i, &actor);
   }

  }

 }

}



/*
** Renders map objects expect the dragon.
*/
void  mapobjp_render(void)
{
 auint    i;
 mapact_t actor;
 auint    typ;

 /* Render enemy projectiles */

 eproj_render();

 /* Render actors */

 for (i = 0U; i < MAPACT_ACN; i++){

  if (mapact_get(i, &actor) == 0U){

   /* Actor exists, render it */

   typ = actor.typ & 0x3FU;

   switch (typ){
    case ACT_ARCHER: acarcher_render(&actor); break;
    case ACT_DOOR:   acdoor_render(&actor);   break;
    case ACT_BOMB:   acbomb_render(&actor);   break;
    case ACT_PRISON: acprison_render(&actor); break;
    case ACT_EOL:    aceol_render(&actor);    break;
    case ACT_PIKE:   acpike_render(&actor);   break;
    case ACT_SWORDS: acswords_render(&actor); break;
    default:         acsupply_render(&actor, typ); break;
   }

  }

 }

 /* Render fireballs */

 fireball_render();

}
