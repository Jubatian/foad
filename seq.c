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



#include "seq.h"
#include "story.h"
#include "game.h"
#include "death.h"
#include "gameend.h"
#include "intro.h"
#include "hiscore.h"
#include "res.h"
#include "global.h"
#include "gstat.h"
#include "dragon.h"
#include <avr/pgmspace.h>
#include <uzebox.h>



/* Story sequence position */
static auint seq_pos;



/*
** Starts a new game by the intro. The parameter requests whether to start
** with the score display (1) or with the title (0).
*/
void seq_new(auint hs)
{
 /* Story starts from the beginning */

 seq_pos = 0U;

 /* The dragon's initial status */

 dragon_mod(DRAGON_CAP_HP |
            DRAGON_CAP_EN |
            DRAGON_CAP_FI,
            1U);
 dragon_mod(DRAGON_STA_HP |
            DRAGON_STA_EN |
            DRAGON_STA_FI,
            1U);

 /* Initial score is zero */

 gstat_score = 0U;

 /* Initial fade setup: faded out */

 global_fadecolor = 0x00U;
 global_fadectr   = 0xFFU;
 global_palctr    = GLOBAL_FADE_ALL | GLOBAL_FADE_INC;

 /* Start by the intro */

 intro_enter(hs);
}



/*
** Called by screens, this requests the sequencer to sequence the subsequent
** phase of the game.
*/
void seq_next(void)
{
 auint d0 = pgm_read_byte(&(res_sequence[seq_pos]));
 auint d1;
 auint d2;

 if ((d0 & 0x80U) == 0x00U){ /* Story panel */

  d1 = pgm_read_byte(&(res_sequence[seq_pos + 1U]));
  d2 = pgm_read_byte(&(res_sequence[seq_pos + 2U]));
  seq_pos += 3U;
  story_enter(((uint16)(d0) << 8) | d1, d2);

 }else{

  switch (d0){

   case 0x80U: /* Game level */

    d0 = pgm_read_byte(&(res_sequence[seq_pos + 1U]));
    seq_pos += 2U;
    game_enter(d0);
    break;

   case 0x81U: /* End of sequence */

    gameend_enter();
    break;

   default:

    break;

  }

 }

 M74_Halt();
}



/*
** Called by screens, this requests the sequencer to handle game over.
*/
void seq_over(void)
{
 death_enter();
}
