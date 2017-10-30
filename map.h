/*
 *  Dragon - Maps (complete levels)
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
** Manages complete levels.
**
** (Naming hiccup: this module should have been called "level", not that
** managing the map itself. At the point when it was noticed there was no easy
** way to fix it, so it remained)
**
** The level data need not be positioned on any boundary.
**
** Format:
**
** Header:
**
** byte  0: Initial dragon X location, low
** byte  1: Initial dragon X location, high
** byte  2: Initial dragon Y location, low
** byte  3: Initial dragon Y location, high
** byte  4: Map (level) data pointer, low (to be passed to level.h)
** byte  5: Map (level) data pointer, high
** byte  6: Map (level) width in L1 blocks
** byte  7: Map (level) height in L1 blocks
** byte  8: Parallax region config & sky (as required by levelscr_setmap())
** byte  9: Dragon healing on entry (amount of health restored)
** byte 10: Initial remaining time
**
** Object list:
**
** Each list entry is 2 bytes, first the low, then the high byte of the ROM
** pointer of the object list (to be passed to mapact.h). Bit 15 of the
** pointer marks the end of the list (if set), Bit 14 is the priority (if set,
** high priority). The high 2 bits of the pointer are always set, so
** addressing the uppermost 16K of the ROM (so they have to be located here).
**
** Object types (identified by address in the object list):
**
** See mapobjp.c
*/


#ifndef MAP_H
#define MAP_H



#include "types.h"



/*
** Initializes the passed level by the passed pointer to its structure.
*/
void  map_init(uint16 map);


/*
** Polls map objects to generate active actors as the dragon moves around.
** Should be called periodically.
*/
void  map_pollobjs(void);



#endif
