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


#ifndef ACRFALL_H
#define ACRFALL_H



#include "types.h"
#include "mapact.h"



/*
** Processes the passed rockfall actor. Returns the health of the actor: if
** this is zero, the actor is destroyed.
*/
auint acrfall_process(mapact_t* actor, auint typ);


/*
** Renders the passed rockfall actor.
*/
void  acrfall_render(mapact_t* actor, auint typ);


#endif
