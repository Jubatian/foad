/*
 *  Dragon - Level data interfacing
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
** Interfaces static (non object) level data.
**
** Level layout:
**
** Innermost L0 blocks are 4 x 4 tiles. There are up to 256 blocks. This also
** defines the logic of the block.
**
** L0 blocks may be replaced by a replacement list in RAM, detailed later
** below.
**
** L1 blocks are 16 x 8 tiles large (horizontally wide). They contain 8 L0
** blocks, taking 8 bytes each.
**
** The map is built from L1 blocks, up to 64 on X, 128 on Y (so up to 256 L0
** blocks in either dimensions). It can use at most 256 L1 blocks (so the L0
** replacement map fits in its 2048 bit storage). On the X dimension the L1
** count must be even.
**
** The replacement map is a simple bitmap mapping all L0 blocks of the level,
** indicating whether the replacement L0 should be used.
**
** L1 layout (L0 blocks within):
**
** 0123
** 4567
**
** The replacement map doesn't follow the L1 layout (it addresses L0 blocks
** directly, high bits corresponding to blocks on the left).
*/



#ifndef LEVEL_H
#define LEVEL_H


#include "types.h"
#include "leveldef.h"



/*
** Clears replacements.
*/
void level_repclear(void);


/*
** Adds L0 block to replacements.
**
** Marks the given L0 block as replaced. Marks by x:y locations (low 2 bits
** unused as they index within the L0. If it is already marked, nothing
** happens.
*/
void level_repadd(uint16 x, uint16 y);


/*
** Removes L0 block from replacements.
**
** Removes replaced mark from an L0. If it isn't marked, nothing happens.
*/
void level_reprem(uint16 x, uint16 y);


/*
** Set map by its L1 index table and width.
**
** The L1 index table (map) is in ROM starting at arbitrary location. Only map
** width is used here to serve as pitch. Width and height is to be interpreted
** in L1 blocks (so 16 x 8 tile blocks).
**
** The replacement map is cleared upon calling this.
*/
void level_set(uint16 map, auint wdt, auint hgt);


/*
** Retrieve tile from map.
**
** Fetches the tile at a given tile location on the map, to be used for
** filling and scrolling the VRAM. Returns the tile's index.
**
** For assembly routine calls XH:XL is not clobbered.
*/
auint level_gettile(uint16 x, uint16 y);


/*
** Retrieve L0 from map.
**
** Fetches the L0 block at the given tile location on the map. The low 2 bits
** of the location are not used (they specify offset within the L0). When an
** out of map coordinate is passed, return is generated according to the
** related defines in leveldef.h.
**
** For assembly routine calls XH:XL is not clobbered.
*/
auint level_getl0(uint16 x, uint16 y);


/*
** Map width (in L1 blocks: 16 tiles wide). Read only!
*/
extern auint level_w;


/*
** Map height (in L1 blocks: 8 tiles tall). Read only!
*/
extern auint level_h;


#endif
