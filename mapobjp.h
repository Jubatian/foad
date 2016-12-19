/*
 *  Dragon - Map objects (actors, projectiles), processing
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
** Processes map objects expect the dragon.
**
** This is a logic frame operation, must be performed after the dragon was
** processed. It uses the current pool of objects only, processing physics and
** interactions among them. It also launches object specific logic as needed
** to handle individual objects.
*/


#ifndef MAPOBJP_H
#define MAPOBJP_H



#include "types.h"



/*
** Initializes map objects. Should be called upon map initialization.
*/
void  mapobjp_init(void);


/*
** Processes map objects expect the dragon.
*/
void  mapobjp_process(void);


/*
** Renders map objects expect the dragon.
*/
void  mapobjp_render(void);


#endif
