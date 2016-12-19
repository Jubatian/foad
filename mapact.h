/*
 *  Dragon - Map actors (enemies and objects), backend
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
** Manages map actors belonging to the level.
**
** Up to 256 actors may be defined of which up to 12 may be active at once on
** the map. There are two priorities, the higher priority actors always appear
** if there is any unused or low priority slot available.
**
** Actor activity relates to their distance from the dragon. If they are more
** than 192 pixels distant, they are removed from the active actor pool.
**
** Actors may be destroyed. If this happens, they will no longer appear. This
** can be requested for either of the 256 actors individually.
**
** RAM storage:
** byte 0: Flags
**         bit   7: Occupation (1: Occupied)
**         bit   6: Priority (part of actor type)
**         bit 0-5: Type
** byte 1: ID
** byte 2: Level coordinate, b0
** byte 3: Level coordinate, b1
** byte 4: Level coordinate, b2
** byte 5: X velocity (signed)
** byte 6: Y velocity (signed)
** byte 7: User data 0
** byte 8: User data 1
**
** ROM source:
** For each actor type a separate list is provided. Only the format of the
** individual lists is specified here for this module.
** Byte 0: bit   7: Set: Relative position (only 1 byte)
**         bit 0-3: Signed X position (-8 to 7: -128 to 112 px)
**         bit 4-6: Signed Y position (-4 to 3: -64 to 48 px)
**         bit   7: Clear: Absolute position (3 bytes except if end of list)
**         bit 0-6: If all set, then end of list marker.
**                  Otherwise Y absolute pixel position bits 8-14.
** Byte 1: bit 0-7: X absolute pixel position bits 8-15.
** Byte 2: bit 4-7: Y absolute pixel position bits 4-7.
**         bit 0-3: X absolute pixel position bits 4-7.
** The low 4 bits of X and Y are provided as constants (set up to align well
** with typical L0 block arrangement). Single sprite wide actors should be set
** up to have their sprite at X = -4, Y = 0, which will align with the tile
** boundary.
**
** RAM usage:
** 108 bytes for the RAM storage of 12 actors.
** 32  bytes for the actor availabilities (32 x 8 = 256 bits).
*/


#ifndef MAPACT_H
#define MAPACT_H



#include "types.h"
#include "physics.h"



/* Total count of active actors */
#define MAPACT_ACN  12U



/* Map actor structure (expanded) */
typedef struct{
 physics_spr_t spr;
 auint         typ; /* User type (including priority) */
 auint         d0;  /* User data 0 */
 auint         d1;  /* User data 1 */
 auint         id;  /* Actor's unique ID */
}mapact_t;


/*
** Initializes the storage with all active actors cleared (free) and all
** non-destroyed. The dragon sprite is used to set up initial dragon location.
*/
void  mapact_init(void);


/*
** Polls a ROM source for actors which should be activated, and activates them
** as necessary or possible. The ROM source is specified by its start address,
** the type of actors it should provide and the ID of the first actor within.
** Activated actors start with their User Data being zero. Returns the number
** of actors within the source added to the passed ID.
*/
auint mapact_pollsrc(auint id, auint typ, uint16 src);


/*
** Fills up actor structure from an actor (from active actor storage). Returns
** nonzero if the actor does not exist (free slot). Fields in the spr member
** not used by the actor structure are left unchanged.
*/
auint mapact_get(auint no, mapact_t* actor);


/*
** Destroys actor by ID. If the actor is active, it is removed from the active
** actor storage as well.
*/
void  mapact_destroy(auint id);


/*
** Modifies actor by the given structure. Anything may be altered. If the new
** location is too far from the dragon, the actor is inactivated. If the given
** actor number is free, nothing happens.
*/
void  mapact_set(auint no, mapact_t const* actor);


/*
** Updates actors in relation to the dragon's changed location, removing
** actors which ended up too far from it.
*/
void  mapact_update(void);


#endif
