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


/*
** Logic and Render for the Pikeman actor.
**
** Troop without much armor charging at the dragon, doing larger damage.
** Fireballs can make them flee.
*/


#ifndef ACPIKE_H
#define ACPIKE_H



#include "types.h"
#include "mapact.h"



/*
** Processes the passed pikeman actor. Returns the health of the actor: if
** this is zero, the actor is destroyed.
*/
auint acpike_process(mapact_t* actor);


/*
** Renders the passed pikeman actor.
*/
void  acpike_render(mapact_t* actor);


#endif
