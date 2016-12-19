/*
 *  Dragon - Game state
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
** Main game state variables
*/


#ifndef GSTAT_H
#define GSTAT_H


#include "types.h"


/*
** Current game score. Should be modified using gstat_score_add() and
** gstat_score_sub().
*/
extern uint16 gstat_score;


/*
** Current game time. Decrements approximately once per second. May be
** incremented by gstat_time_add().
*/
extern auint  gstat_time;


/*
** Adds to the game score with saturation.
*/
void gstat_score_add(uint16 val);


/*
** Subtracts from the game score with saturation.
*/
void gstat_score_sub(uint16 val);


/*
** Adds to the game time with saturation.
*/
void gstat_time_add(auint val);


/*
** Outputs current score as a five digit decimal number with leading zeros.
** "addr" is the target memory address, needs 5 bytes. "zch" is the '0'
** character, assuming numeric characters come incrementally after this.
** The score needs to be supplied in "val", so other scores may be displayed
** with it than gstat_score.
**
** Assembly notes: Outputs addr + 5 in X; clobbers r19-r25. There is also a
** gstat_score_disp_x variant which takes addr in X.
*/
void gstat_score_disp(uint16 addr, uint16 val, auint zch);


#endif
