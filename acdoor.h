/*
 *  Dragon - Door actor
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


/*
** Logic and Render for the Door actor.
**
** Stationary object which can be burned down by the dragon. When it happens,
** it replaces the L0 underneath. The door is wider than a tile column, it
** should be applied on L0 blocks which have a proper impassable visually
** apparent door at its location (the actor itself has no graphic, but adds
** fires as the dragon burns it).
*/


#ifndef ACDOOR_H
#define ACDOOR_H



#include "types.h"
#include "mapact.h"



/*
** Processes the passed door actor. Returns the health of the actor: if this
** is zero, the actor is destroyed.
*/
auint acdoor_process(mapact_t* actor);


/*
** Renders the passed door actor.
*/
void  acdoor_render(mapact_t* actor);


#endif
