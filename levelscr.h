/*
 *  Dragon - Level data - VRAM interfacing
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
** Realizes the scrolling map without sprites.
**
** The scrolling map is a 192x185 area; 24 x 23 tiles + 1 line. Horizontally
** it is constrained to map boundaries, vertically extra lines are visible
** for parallax clouds on the top, parallax water / lava on the bottom.
**
** It also returns coordinates for sprite output, setting up the X Shift map
** proper for blitting.
**
** VRAM effects note:
** If necessary, these should be performed directly on the VRAM due to the
** costs. So no support is provided here except for a row check to see whether
** a given VRAM row (0 - 23) is from the map (not parallax top / bottom).
*/



#ifndef LEVELSCR_H
#define LEVELSCR_H


#include "types.h"
#include "level.h"



/*
** Sets map parameters to use
**
** Note that this doesn't initialize the VRAM etc., just sets up the map to
** use. It resets screen.
**
** The flags (flg) specify what to use on the upper / lower parallax regions.
** bits 0-3: Bottom parallax region type
** bits 4-7: Top parallax region type & Sky select
** See parallax.h. Bit 7 selects sky, if set, dawn/dusk scheme.
*/
void levelscr_setmap(uint16 map, auint wdt, auint hgt, auint flg);


/*
** Set location (upper left corner)
**
** Sets the upper left visible corner of the map in pixels (0:128) is for
** displaying the upper left corner of the map. This resets screen, so
** everything is redrawn.
*/
void levelscr_setloc(uint16 x, uint16 y);


/*
** Resets screen
**
** Calling this ensures that next time everything is redrawn. Normally the
** VRAM is not altered unless necessary (when scrolling it is copied) so VRAM
** effects are possible.
*/
void levelscr_reset(void);


/*
** Set scroll target location
**
** Set the target location of scrolling. The location is clipped to map
** boundaries. Negatives (2's complement) are possible, they will be clipped
** proper.
**
** The scrolling is smoothened to make it look aesthetically nice, so no
** related considerations are necessary when setting the target.
*/
void levelscr_settarget(uint16 x, uint16 y);


/*
** Adds a shake effect, to be used for bombs, destruction etc.
**
** The parameter is the magnitude and timespan of the effect
*/
void levelscr_shake(auint mag);


/*
** Performs a scroll step
**
** VRAM must be restored before calling this. It carries out a frame of
** scrolling as necessary. After this, sprites may be blitted.
*/
void levelscr_scroll(void);


/*
** X location, read only!
**
** Current visible upper left corner, to be used for locating sprites.
*/
extern uint16 levelscr_x;


/*
** Y location, read only!
**
** Current visible upper left corner, to be used for locating sprites.
** To get the real Y, subtract 128 from this value.
*/
extern uint16 levelscr_y;


#endif
