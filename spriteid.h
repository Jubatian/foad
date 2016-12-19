/*
 *  Dragon - Sprite ID to address conversion
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
** Converts sprite frame IDs (up to 256) to sprite addresses which can be
** used later with sprite_blit().
*/



#ifndef SPRITEID_H
#define SPRITEID_H


#include "types.h"



/*
** Converts the given sprite frame ID to sprite address for sprite_blit().
**
** For assembler calls Z is clobbered.
*/
uint16 spriteid_conv(auint sid);


#endif
