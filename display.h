/*
 *  Dragon - Display setup
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



#ifndef DISPLAY_H
#define DISPLAY_H


#include "types.h"



#define DISPLAY_GAME   0U
#define DISPLAY_STORY  1U
#define DISPLAY_INTRO  2U



/*
** Sets up a display mode along with a reset address, and halts execution (so
** after generating a frame with the new mode, it returns onto that reset
** address). The reset address must be specified as a progmem address
** (converting a function pointer to int is OK for this).
*/
void  display_set(auint mode, uint16 reset);


/*
** Clears VRAM for a given display mode
*/
void  display_clear(auint mode);


#endif
