/*
 *  Dragon - Sound effects
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



#ifndef SOUND_H
#define SOUND_H



#include "types.h"



/* Sound effects */
#define SOUND_BOOM    0U
#define SOUND_FIRE    1U
#define SOUND_ITEM    2U
#define SOUND_ARROW   3U
#define SOUND_TICK    7U



/*
** Plays sound effect.
*/
void sound_effect(auint eff, auint vol);


#endif
