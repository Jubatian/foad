/*
 *  Dragon - Actor support functions, C
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



#ifndef ACSUPPC_H
#define ACSUPPC_H



#include "types.h"
#include "mapact.h"



/*
** Renders a non-player-character sprite. Such a sprite faces the dragon when
** not moving, and may be on fire. This can be used for typical 2x1 tile human
** characters. The fra parameter is the frame to draw, fir is whether the
** sprite should be on fire (bit 7 set; then bits 0-4 are the frame).
*/
void acsuppc_rendernpc(mapact_t* actor, auint fra, auint fir);


#endif
