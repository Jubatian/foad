/*
 *  Dragon - Sprite to Level blitting
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
** Blits sprites by map coordinates onto game level, properly determining
** whether they are on-screen.
*/



#ifndef SPRITELV_H
#define SPRITELV_H


#include "types.h"
#include "physics.h"



/*
** Blits the given sprite. The level (levelscr) has to be set up properly as
** it blits by the position included.
** frame is the sprite image to blit (by spriteid_conv())
** flg is the Mode 74 flags to use for the blit (as required by sprite_blit())
** rep is the color replacement table to use, for REC_FLAT_0 it will select
** the bright variant if the sprite is in cave.
*/
void spritelv_blit(physics_spr_t const* spr, auint frame, auint flg, auint rep);


/*
** Like spritelv_blit() but with positions instead of sprite structure.
*/
void spritelv_blit_xy(uint16 x, uint16 y, auint frame, auint flg, auint rep);


#endif
