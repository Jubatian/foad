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


/*
** Logic and Render for end of level actor.
**
** Collecting this ends the level.
*/


#ifndef ACEOL_H
#define ACEOL_H



#include "types.h"
#include "mapact.h"



/*
** Processes the passed end of level actor. Returns the health of the actor:
** if this is zero, the actor is destroyed.
*/
auint aceol_process(mapact_t* actor);


/*
** Renders the passed end of level actor.
*/
void  aceol_render(mapact_t* actor);


#endif
