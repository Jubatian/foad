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


#ifndef LEVELDEF_H
#define LEVELDEF_H


#include "m74cfg.h"


/*
** L0 block definition location.
**
** One L0 block is 16 bytes. Must be on a 256 byte boundary.
*/
#define LEVEL_L0_OFF    RES_L0_OFF


/*
** L1 block definition location.
**
** One L1 block is 8 bytes. Must be on a 256 byte boundary.
*/
#define LEVEL_L1_OFF    RES_L1_OFF


/*
** Replacement list data location.
**
** Contains the replacements for each of the tiles. 256 bytes. Must be on a
** 256 byte boundary.
*/
#define LEVEL_RLD_OFF   RES_L0REPD_OFF


/*
** Replacement list location.
**
** A RAM location where the replacement list occurs. 256 bytes for up to 2048
** L0 blocks. Must be on a 256 byte boundary.
*/
#define LEVEL_RL_OFF    LOC_L0REP_OFF


/*
** L0 block to return when dimension X is out of map.
*/
#define LEVEL_RET_XI    3


/*
** L0 block to return when dimension Y is out of map.
*/
#define LEVEL_RET_YI    0


#endif
