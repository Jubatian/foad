/*
 *  Dragon - Game sequencer
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
** The game sequencer governs the progress of the game. Note that it is in
** circular reference with game.c and story.c, due to the frame reset
** architecture this is a necessity.
*/


#ifndef SEQ_H
#define SEQ_H


#include "types.h"



/*
** Starts a new game by the intro. The parameter requests whether to start
** with the score display (1) or with the title (0).
*/
void seq_new(auint hs);


/*
** Called by screens, this requests the sequencer to sequence the subsequent
** phase of the game.
*/
void seq_next(void);


/*
** Called by screens, this requests restarting a game level. Can only be
** called if it was a game level (to restart a game in which the player died).
*/
void seq_reset(void);


/*
** Called by screens, this requests the sequencer to handle game over.
*/
void seq_over(void);


#endif
