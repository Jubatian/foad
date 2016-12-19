/*
 *  Dragon - Prisoner actor
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
** Logic and Render for the Prisoner actor.
**
** They are locked in their cells until the dragon comes to free them (by
** standing over). Until then they can not be harmed, but after that, they
** may. They try to run off the screen, if they get far enough, they
** disappear. Score is received for freeing them, but removed if they are
** killed.
*/


#ifndef ACPRISON_H
#define ACPRISON_H



#include "types.h"
#include "mapact.h"



/*
** Processes the passed prisoner actor. Returns the health of the actor: if
** this is zero, the actor is destroyed.
*/
auint acprison_process(mapact_t* actor);


/*
** Renders the passed prisoner actor.
*/
void  acprison_render(mapact_t* actor);


#endif
