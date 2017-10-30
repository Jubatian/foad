/*
 *  Dragon - Parallax scrolling components
 *  Copyright (C) 2017 Sandor Zsuga (Jubatian)
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
** Produces top & bottom parallax scrolling regions for the map.
*/


#ifndef PARALLAX_H
#define PARALLAX_H


#include "types.h"



/*
** Render visible parallax regions. 'x' is the X position in pixels, 'ty' is
** the Y position in tiles (!), both in accordance with levelscr_scroll(). flg
** is the type of parallax to use, also the same as in levelscr_setmap(). hgt
** is the map height in L1 blocks.
**
** Parallax types (flg):
** Low nybble: bottom:
** bit 0-1: Graphics:
**          0: Cave water (dark)
**          1: Grass
**          2: Cave floor
**          3: Surface water (blue)
** bit 2:   Layer drifts if clear (mostly for flowing water)
** High nybble: top:
** bit 0-1: Graphics:
**          0: Sky with many clouds
**          1: Sky with a few clouds
**          2: Cave ceiling
**          3: Nothing (mostly for cloudless sky)
** bit 2:   Layer is a sky if clear (with drifting clouds if any)
*/
void parallax_render(uint16 x, uint16 ty, auint flg, auint hgt);


#endif
