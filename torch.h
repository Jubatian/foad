/*
 *  Dragon - Torch tile logic
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
** Torches are animated tiles using 4 RAM tiles (0x57 - 0x59). This module
** contains the processing of these tiles.
*/



#ifndef TORCH_H
#define TORCH_H


#include "types.h"



/*
** Prepares torch tiles. This must be called before starting to render: it
** fills in RAM tile masks for the torch tiles.
*/
void torch_prep(void);


/*
** Animates torch tiles. This may be called anywhere, should possibly be
** called last (this is the least important, so no problem if it doesn't fit
** in the frame).
*/
void torch_render(void);


#endif
