/*
 *  Dragon - Map actor dimensions
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
** Retrieves actor dimensions (xbs and ybs members into a sprite physics
** structure) by the RES_SPRIDSIZE_CONV resource which encodes the
** dimensions in one byte for both x and y.
**
** bits 0-3: X size, 0 - 30 pixels
** bits 4-7: Y size, 0 - 60 pixels
**
** (the effective X size is the duplicate of this, see physics.h).
**
** Retrieval is performed by the 6 bits of the type ID passed as bits 0 - 5
** to the routine. Bit 6 is ignored. Bit 7 if set asks for alternate selection
** (can be used for the dragon). The result ROM address is calculated by
** copying Bit 7 to Bit 6, and setting Bit 7. This allows sharing a 256 byte
** block with the color remappings.
*/



#ifndef ACTORDIM_H
#define ACTORDIM_H


#include "types.h"
#include "physics.h"


/*
** Fills in sprite physics structure's xbs and ybs members by actor type (bits
** 0 - 5). If bit 7 of type is set, alternate xbs & ybs are returned (can be
** used for the dragon & others).
*/
void   actordim_get(auint typ, physics_spr_t* spr);


#endif
