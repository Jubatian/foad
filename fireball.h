/*
 *  Dragon - Fireballs (dragon's breath)
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
** Realizes the fireballs breathed by the dragon.
**
** RAM storage:
** Byte 0: Burnout timer, 0: Empty slot
** byte 1: X velocity (signed)
** byte 2: Y velocity (signed)
** Byte 3: Level coordinate, b0
** byte 4: Level coordinate, b1
** byte 5: Level coordinate, b2
**
** Fireballs start to burn out as soon as released. The timer is also a frame
** counter to produce frames of fireball animations.
*/



#ifndef FIREBALL_H
#define FIREBALL_H



#include "types.h"
#include "physics.h"



/* Total count of fireballs. Note: keep in sync with that in firebals.s!
** Note: More is necessary than what the dragon might use since bombs and
** other things might also generate them. */
#define FIREBALL_N 12U

/* Initial fireball age (2.5 secs) */
#define FIREBALL_AGE 75U



/*
** Initializes the storage with all fireballs cleared (free).
*/
void  fireball_init(void);


/*
** Creates a new fireball if possible. If not possible, nothing happens.
*/
void  fireball_new(uint16 x, uint16 y, asint xvel, asint yvel);


/*
** Ages a given fireball, accelerating its burnout. A fireball may be
** destroyed (removed) by bumping its burnout by 0xFF.
*/
void  fireball_age(auint id, auint age);


/*
** Retrieve a fireball structure for reading, possibly used for collisions,
** interactions between sprites. Returns nonzero if the fireball doesn't
** exist.
*/
auint fireball_get(auint id, physics_spr_t* dest);


/*
** Returns whether there is a fireball within a sprite. Returns the fireball's
** ID if there is any or FIREBALL_N if there is none.
*/
auint fireball_getat(physics_spr_t const* spr);


/*
** Processes fireballs (only the physics, not collisions)
*/
void  fireball_process(void);


/*
** Renders fireballs
*/
void  fireball_render(void);


/*
** Generates a single burning fire, may be used for objects on fire. Age is
** the fire's age which should decrement once every render frame until burning
** out.
*/
void  fireball_render_fire(uint16 x, uint16 y, auint age);


#endif
