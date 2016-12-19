/*
 *  Dragon - Level (map) coordinate encoding & decoding
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
** Encodes and decodes level (map) coordinates.
**
** The maximal possible dimension of a map on an axis is 8192 pixels (256 L0
** blocks). The same time assuming allowing at most 2048 L0 blocks (requiring
** 256 bytes of replacement map RAM), 2M pixels is the maximal map area. This
** requires 21 coordinate bits, which allows the coordinates to be represented
** in 3 bytes (instead of 4 which would be used if they were uint16s).
**
** Using the map height (level_h) this performs such encoding.
*/



#ifndef LEVELCOR_H
#define LEVELCOR_H


#include "types.h"



/*
** Encodes coordinates into dst (3 bytes).
** For assembly calls, ZL, r1:r0 and r19:r18 are not clobbered.
** For assembly calls, XH:XL upon return is set dst + 3.
*/
void levelcor_encode(uint8* dst, uint16 x, uint16 y);


/*
** Decodes coordinates from src (3 bytes).
** For assembly calls, r1:r0 and r19:r18 are not clobbered.
** For assembly calls, XH:XL upon return is set src + 3.
*/
void levelcor_decode(uint8 const* src, uint16* x, uint16* y);


/*
** Note: For assembler uses, another routine is provided, producing results
** into registers instead of memory locations: levelcor_decode_asm.
**
** Inputs:
** XH:XL:   src (3 bytes)
**
** Outputs:
** r23:r22: X
** r21:r20: Y
** XH:XL:   src + 3
*/

#endif
