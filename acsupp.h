/*
 *  Dragon - Actor support functions
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



#ifndef ACSUPP_H
#define ACSUPP_H



#include "types.h"
#include "physics.h"



/*
** Line of sight test between two sprites (typically the actor and the
** dragon). This is a crude test returning whether the line of sight between
** the two passed sprites is unobstructed.
*/
auint acsupp_isvisible(physics_spr_t const* spr1, physics_spr_t const* spr2);


/*
** Ledge test. Returns whether there is a ledge in front of the sprite at the
** given distance (negative for left, positive for right) which may be used
** to prevent it walking into a chasm.
*/
auint acsupp_isatledge(physics_spr_t const* spr, asint dist);


/*
** Returns the suggested Y velocity for a projectile to get it travelling from
** a location to a sprite with the given X velocity. Returns 0x7F if it can't
** be done (such as if the X velocity points the wrong way).
*/
asint acsupp_getshotvy(uint16 x, uint16 y, physics_spr_t const* spr, asint xv);


/*
** Simple coordinate nearness test. A pair of coordinates are nearby to each
** if one is within 8 pixels distance to the other on X, and within 16 pixels
** on Y (in accordance with typical sprite heights).
*/
auint acsupp_iscordnear(uint16 x1, uint16 y1, uint16 x2, uint16 y2);


#endif
