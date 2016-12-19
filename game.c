/*
 *  Dragon - Game screen logic
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


#include "story.h"
#include "seq.h"
#include "display.h"
#include "global.h"
#include "gstat.h"
#include "dragon.h"
#include "levelscr.h"
#include "physics.h"
#include "mapact.h"
#include "mapobjp.h"
#include "status.h"
#include "map.h"
#include "res.h"
#include "torch.h"
#include "music.h"
#include "sound.h"
#include <avr/pgmspace.h>
#include <uzebox.h>



/* Performance check on score. Enable this to make score display the kernel's
** "sync_pulse" variable:
** 1: Logic frames
** 2: Render frames (score adjustments make it jitter a bit) */

#define PERF_DBG      0



#define GAME_BTNC_LO  global_shared[0]
#define GAME_BTNC_HI  global_shared[1]

/* Level End: Also see aceol.c which writes 1 here when collecting level end */
#define GAME_LEVEND   global_shared[7]

/* Temp for health & time to run end level score animations */
#define GAME_THEALTH  global_shared[2]
#define GAME_TTIME    global_shared[3]



#if (PERF_DBG != 0)
extern uint8 sync_pulse;
#endif



/*
** Frame routine of the game screen
*/
static void game_frame(void)
{
 auint  cmd;
 auint  die = 0U;
 uint16 btn = GAME_BTNC_LO | ((uint16)(GAME_BTNC_HI) << 8);

 global_process();

 ReadControllers();
 btn |= ReadJoypad(0);


 if ((global_framectr & 0x1U) == 0U){ /* Logic frame */

  if (GAME_LEVEND != 0U){ /* End of level reached */

   if       (gstat_time != 0U){
    gstat_time --;
    gstat_score_add(3U);  /* Each remaining tick of time adds 3 points (normally it is 2 points) */
    if ((global_framectr & 0x7U) == 0U){ sound_effect(SOUND_TICK, 0x30U); }
   }else if (dragon_stat.hp != 0U){
    dragon_stat.hp --;
    gstat_score_add(2U);  /* Each remaining unit of health adds 2 points */
    if ((global_framectr & 0x7U) == 0U){ sound_effect(SOUND_TICK, 0x30U); }
    global_fadecolor = 0x00U;
   }else if (global_fadectr != 0xFFU){
    global_palctr    = GLOBAL_FADE_ALLV | GLOBAL_FADE_INC;
   }else{
    gstat_time     = GAME_TTIME;   /* Restore stuff added to score */
    dragon_stat.hp = GAME_THEALTH;
    seq_next();           /* Level completed */
   }

   dragon_follow(0);

  }else{                  /* End of level not reached: normal processing */

   GAME_THEALTH = dragon_stat.hp;
   GAME_TTIME   = gstat_time;

   /* Global time & death logic */

   if ((global_framectr & 0x3FU) == 0U){
    if (gstat_time != 0U){             /* Remaining time decrements (~1 sec) */
     gstat_time --;
    }else{
     die = 1U;
    }
   }
   if ((global_framectr & 0x1FU) == 0U){
    gstat_score_add(1U);               /* Score increments as time passes (~0.5 sec)*/
   }
   if ((global_framectr & 0xFFU) == 0U){
    dragon_mod(DRAGON_STA_HP | DRAGON_P_ADD, 1U); /* Health slowly increments */
   }

   if (dragon_stat.hp == 0U){
    die = 1U;
   }

  }

  /* Common processing (even at end level, so player may keep moving the dragon around) */

  if (die != 0U){

   /* Player died either by depleted health or time. Fade. */

   global_fadecolor = 0x00U;
   global_palctr    = GLOBAL_FADE_ALLV | GLOBAL_FADE_INC;
   if (global_fadectr == 0xFFU){
    seq_over();
   }

  }else{

   /* No death, normal processing */

   /* Controls */

   cmd = DRAGON_RUN;

   if ((btn & BTN_RIGHT) != 0U){ cmd |= DRAGON_RIGHT | DRAGON_MOVE; }
   if ((btn & BTN_LEFT)  != 0U){ cmd |= DRAGON_LEFT | DRAGON_MOVE; }
   if ((btn & BTN_SR)    != 0U){ cmd &= ~DRAGON_RUN; }
   if ((btn & BTN_Y)     != 0U){ cmd |= DRAGON_FIRE; }
   if ((btn & BTN_B)     != 0U){ cmd |= DRAGON_FIRE; }
   if ((btn & BTN_X)     != 0U){ cmd |= DRAGON_JUMP; }
   if ((btn & BTN_A)     != 0U){ cmd |= DRAGON_JUMP; }
   if ((btn & BTN_DOWN)  != 0U){ cmd |= DRAGON_DOWN; }
   if ((btn & BTN_UP)    != 0U){ cmd |= DRAGON_UP; }
   if ((btn & BTN_SL)    != 0U){ cmd |= DRAGON_UP; }

   GAME_BTNC_LO = 0x00U;
   GAME_BTNC_HI = 0x00U;

   physics_frame();
   dragon_logic(cmd);

   mapobjp_process();

   map_pollobjs();

  }

#if (PERF_DBG == 1)
  gstat_score = sync_pulse;
#endif

  /* All done */

  M74_Halt();

 }


 /* Render frame */

 GAME_BTNC_LO = btn & 0xFFU;
 GAME_BTNC_HI = btn >> 8;

 M74_VramRestore();

 torch_prep();

 levelscr_scroll();

 global_genpal();

 dragon_render();

 mapobjp_render();

 status_render();

 torch_render();

#if (PERF_DBG == 2)
 gstat_score = sync_pulse;
#endif

 M74_Halt();
}



/*
** Initializes and starts a game screen. Calls seq_next() or seq_over() when
** it is over.
*/
void game_enter(auint map)
{
 /* Make sure nothing would be visible */

 global_hide();

 /* Inits control variables */

 GAME_BTNC_LO = 0x00U;
 GAME_BTNC_HI = 0x00U;
 GAME_LEVEND  = 0x00U;

 /* Clear VRAM */

 display_clear(DISPLAY_GAME);

 /* Reset VRAM restore list (for sprite blitting) */

 M74_ResReset();

 /* Initialize map */

 map_init(pgm_read_word(&res_maplist[map]));

 /* Control fading: Fade in from whatever was set before */

 global_palctr = GLOBAL_FADE_ALLV;

 /* Start music */

 music_start_id(2U);

 /* Sets display for game mode */

 display_set(DISPLAY_GAME, (uint16)(&game_frame));
}
