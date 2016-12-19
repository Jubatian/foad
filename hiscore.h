/*
 *  Dragon - High score screen logic
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


#ifndef HISCORE_H
#define HISCORE_H


#include "types.h"



/*
** Initializes and starts a high score screen. Calls seq_new() when it is
** over (this is the last screen of a game session). This screen is skipped
** (passes over to seq_new) if the score isn't in the top three.
*/
void hiscore_enter(void);


#endif
