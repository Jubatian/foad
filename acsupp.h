/*
 *  Dragon - Actor support functions
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



#ifndef ACSUPP_H
#define ACSUPP_H



#include "types.h"
#include "physics.h"
#include "mapact.h"



/*
** Line of sight test between two sprites (typically the actor and the
** dragon). This is a crude test returning whether the line of sight between
** the two passed sprites is unobstructed.
*/
auint acsupp_isvisible(physics_spr_t const* spr1, physics_spr_t const* spr2);


/*
** Ledge test. Returns whether there is a ledge in front of the sprite at the
** given distance (negative for left, positive for right) which may be used
** to prevent it walking into a chasm.
*/
auint acsupp_isatledge(physics_spr_t const* spr, asint dist);


/*
** Ledge halt. If sprite is at ledge, halts it (xvel), preventing it walking
** or running into a chasm.
*/
void  acsupp_haltatledge(physics_spr_t const* spr);


/*
** Returns the suggested Y velocity for a projectile to get it travelling from
** a location to a sprite with the given X velocity. Returns 0x7F if it can't
** be done (such as if the X velocity points the wrong way).
*/
asint acsupp_getshotvy(uint16 x, uint16 y, physics_spr_t const* spr, asint xv);


/*
** Tests if a given coordinate pair is in the dragon. Returns 1 if so.
*/
auint acsupp_iscordindragon(uint16 x, uint16 y);


/*
** Tests if a given coordinate pair is near to the dragon. Returns 1 if so.
** dxy is the distances to test: X on high 8 bits, Y on low 8 bits.
*/
auint acsupp_iscordneardragon(uint16 x, uint16 y, uint16 dxy);


/*
** Renders a non-player-character sprite. Such a sprite faces the dragon when
** not moving, and may be on fire. This can be used for typical 2x1 tile human
** characters. The fra parameter is the frame to draw, fir is whether the
** sprite should be on fire (bit 7 set; then bits 0-4 are the frame).
*/
void  acsupp_rendernpc(mapact_t* actor, auint fra, auint fir);


/*
** Interacts an actor with a fireball and returns, assuming the most common
** scenario for actor layout:
** d0 is laid out as follows:
** bit   7: On fire marker, cleared on frame 0x1F (~1sec burn).
** bit   6: Not touched.
** bit 0-5: Frame counter which is incremented.
** Writes back d0 and d1 into the actor and returns 0 if health is depleted
** (d1 = 0xFF) and 1 if not (d1 not 0xFF).
** d1d0: d1 is high, d0 is low
** shp: Kill penalty score (high) & Health decrement for actor (low)
** d0m: Masks for d0 (high: OR, low: AND) when fireball hits
*/
auint acsupp_procfin_fire(mapact_t* actor, uint16 d1d0, uint16 shp, uint16 d0m);


/*
** Simple return depending on the value of d1 (0xFF: 0; others: 1), writes
** back d0 and d1.
*/
auint acsupp_procfin(mapact_t* actor, auint d0, auint d1);


#endif
