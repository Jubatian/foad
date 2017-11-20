/*
 *  Dragon - Low level physics
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
** Realizes tile level physics.
**
** The tiles in the RES_TILEFLG_OFF have three flags for this module:
** - bit 0-1: Low half passability
** - bit 2-3: High half passability
**            0: Impassable
**            1: Passable
**            2: Ramp from lower right to upper left
**            3: Ramp from upper right to lower left
** - bit 4: Ladder tile; 1 if ladder (climbers may climb it)
**
** Sprites are handled using bounding boxes, by the following sprite
** structures (other modules might generate these):
**
** - 16 bits X pixel position on map
** - 16 bits Y pixel position on map
** - 8 bits X bounding box size
** - 8 bits Y bounding box size
** - 8 bits (signed) X velocity (pixels)
** - 8 bits (signed) Y velocity (pixels)
** - 8 bits Acceleration return (return value only)
** - 8 bits flags:
**   bit 0: If set, climbs if possible (tile allows it)
**   bit 1: If set, moves on the ground if possible
**
** The sprite bounding box:
**
** +---------------+
** |               |
** |               | Y size
** |    Position   |
** +-------O-------+
**  X size   X size
**
** Gravity causes an Y velocity decrement after every PHYSICS_G_DIV frames
** (physics_frame()) on processed sprites unless they stand on ground or
** climbing a ladder.
**
** Ladder may mix with any passability, it takes precedence when the climbs
** flag is set.
**
** The Acceleration return is generated if the sprite moving with the given
** velocity hits an obstacle indicating impact force. It does not necessarily
** correspond to the actual change of velocity in the frame, rather the change
** which would occur if it hit the obstacle from 0 pixels distance (so it
** corresponds to the impact force, to be used to determine effect on the
** affected sprite, not the graphical movement).
**
** Map boundaries are treated as Impassable tiles.
*/



#ifndef PHYSICS_H
#define PHYSICS_H


#include "types.h"


/* Gravity divider */
#define PHYSICS_G_DIV   3U

/* Flags. The powered (PWR) flags indicate the motion is powered, so gravity
** or ground follow shouldn't permanently alter velocity. The HK (hook) flags
** are set (either the left or right) if only the left or right edge keeps the
** sprite grounded (which may be used to display alternate sprites). The climb
** flag is removed (original plan was to have ladders for NPCs). */
#define PHYSICS_F_CLIMB 0x01U
#define PHYSICS_F_GRND  0x02U
#define PHYSICS_F_PWR_X 0x04U
#define PHYSICS_F_PWR_Y 0x08U
#define PHYSICS_F_HK_L  0x10U
#define PHYSICS_F_HK_R  0x20U


/* Sprite structure */
typedef struct{
 uint16 xpos;
 uint16 ypos;
 auint  xbs;
 auint  ybs;
 asint  xvel;
 asint  yvel;
 asint  xacc;
 asint  yacc;
 auint  flg;
}physics_spr_t;


/*
** Advances a frame.
**
** This is used for counting the frames for generating gravity, so call once
** in each logic frame.
*/
void  physics_frame(void);


/*
** Returns frames until a gravity frame (in which in-air object y velocities
** increase towards ground). This may be used to get deterministic results for
** actions involving gravity.
*/
auint physics_gravrem(void);


/*
** Processes a sprite.
**
** Pass the sprite structure to process physics for the frame on the sprite.
** - Writes xpos and ypos according to the new position.
** - Writes xvel and yvel according to the constraints.
** - Writes xacc and yacc according to any acceleration (due to impact).
** - Writes flg according to the environment.
**
** In flg it so returns whether the sprite is standing on ground or is
** falling, and whether it succesfully hangs on a climbable tile.
*/
void  physics_proc(physics_spr_t* spr);


#endif
