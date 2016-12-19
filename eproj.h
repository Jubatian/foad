/*
 *  Dragon - Enemy projectiles
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
** Realizes various enemy projectiles
**
** RAM storage:
** Byte 0: Type and type specific data. 0: Unused slot.
** byte 1: X velocity (signed)
** byte 2: Y velocity (signed)
** Byte 3: Level coordinate, b0
** byte 4: Level coordinate, b1
** byte 5: Level coordinate, b2
**
** The enemy projectiles behave in a similar manner to the dragon's fireballs,
** varying by type.
*/



#ifndef EPROJ_H
#define EPROJ_H



#include "types.h"
#include "physics.h"



/* Total count of projectiles */
#define EPROJ_N     8U


/* Projectile types */
/* Arrows. They disappear upon impact. */
#define EPROJ_ARROW 0x02U



/*
** Initializes the storage with all projectiles cleared (free).
*/
void  eproj_init(void);


/*
** Creates a new projectile if possible. If not possible, nothing happens.
*/
void  eproj_new(uint16 x, uint16 y, asint xvel, asint yvel, auint typ);


/*
** Ages (if applicable to the type) or removes a projectile. Aging is
** performed by the passed number. 0xFF removes any projectile.
*/
void  eproj_age(auint id, auint age);


/*
** Retrieve a projectile structure for reading, possibly used for collisions,
** interactions between sprites. Returns nonzero if the projectile doesn't
** exist.
*/
auint eproj_get(auint id, physics_spr_t* dest);


/*
** Processes projectiles and their collisions with the dragon
*/
void  eproj_process(void);


/*
** Renders projectiles
*/
void  eproj_render(void);


#endif
