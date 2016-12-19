/*
 *  Dragon - Sprite blitter
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
** Sprite blitter for realizing large sprites.
**
** Concepts:
**
** Sprite tiles can be fetched from any ROM location (multiple of 32 bytes, so
** at block boundary).
**
** Sprites are rendered relative to a center point passed as the parameter of
** the sprite renderer. The origin of the coordinate system is the upper left
** corner of the screen. Sprite clipping is not performed (so sprites covering
** more than 64 x 64 pixels can not be produced).
**
** X flipping, masking, importance, and recoloring is supported.
**
** Sprites have a 16 bit offset (into ROM), pointing at a descriptor which
** collects the tiles relating to it.
**
** The descriptor is a list of arbitrary length, specifying sprite components.
** This list is traversed from beginning to end while the sprite parts are
** sent to the display (so later sprite components on it appear on the front).
**
** The descriptor must be on the high (0x8000 - 0xFFFF) half of the program
** memory.
**
** A list entry is normally 4 bytes:
**
** Byte 0: bits 5-7: Low byte of sprite tile address
**         bit  3-4: Ignored
**         bit    2: 0
**         bit    1: If set, end of list
**         bit    0: 1
** Byte 1: High byte of sprite tile address
** Byte 2: X offset of component (signed, right side)
** Byte 3: Y offset of component (signed, bottom)
**
** If Byte 0, bit 0 is clear, then it is either a reuse or a jump command, the
** latter merging the "tail" with another sprite's tail. The commands:
**
** Byte 0: bits 1-7: Low byte of jump target (bit 0 is always zero)
**         bit    0: 0
** Byte 1: bit    7: If set, reuse, if clear, jump
**         bits 0-6: High byte of jump target (bit 7 is always 1)
**
** The reuse command can reuse a 4 byte sprite part or particle description,
** thus saving 2 bytes each time it is used. Note that reusing a terminating
** (end of list) command will end the list.
**
** The jump enables adding for example different head positions, expressions
** over animations with little overhead. It may allow other merges as well.
**
** If Byte 0, bit 2 is set, then it is a particle generator. It is as follows:
**
** Byte 0: bits 4-7: Particle color
**         bit    3: Ignored
**         bit    2: 1
**         bit    1: If set, end of list
**         bit    0: 1
** Byte 1: Particle 0, X:Y
** Byte 2: Particle 1, X:Y
** Byte 3: Particle 2, X:Y
**
** Particles can be offset -8 to 7 pixels relative to the ground point. If a
** particle's offset equals the previous, it is not generated (faster).
** Particle importances are always lowest which adds up.
*/



#ifndef SPRITE_H
#define SPRITE_H


#include "types.h"


/*
** Blits a sprite
**
** spo: Sprite descriptor offset
** x:   X position on screen (VRAM), wraps
** y:   Y position on screen (VRAM), wraps
** flg: Flags (same as used for Mode 74, RAM source & Y flip not supported)
** col: Recoloring. 0: Off.
*/
void  sprite_blit(uint16 spo, auint x, auint y, auint flg, auint col);


#endif
