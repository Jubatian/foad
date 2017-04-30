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



#include "hiscore.h"
#include "seq.h"
#include "eeprom.h"
#include "text.h"
#include "display.h"
#include "global.h"
#include "gstat.h"
#include "sprite.h"
#include "spriteid.h"
#include "music.h"
#include "sound.h"
#include <uzebox.h>
#include <avr/pgmspace.h>



#define HISCORE_BTNC_LO    global_shared[0]
#define HISCORE_BTNC_HI    global_shared[1]
#define HISCORE_DRG_FRAME  global_shared[2]
#define HISCORE_ENTRY      global_shared[3]
#define HISCORE_CPOS       global_shared[4]
#define HISCORE_SHIFT      global_shared[5]
#define HISCORE_ITICK      global_shared[6]

#define HISCORE_EADDR      (LOC_RAMTILES_OFF + 0x600U)
#define HISCORE_EADDR_T    (LOC_RAMTILES_OFF + 0x600U + 36U)



/* Default name ("Dragon") */
static const char hiscore_name[10] PROGMEM = {
 3U, 17U + 26U, 0U + 26U, 6U + 26U, 14U + 26U, 13 + 26U, 63U, 63U, 63U, 63U
};



/*
** Extracts Little Endian 16 bit number from RAM.
*/
uint16 hiscore_getval(uint16 addr)
{
 return ((uint16)(((uint8*)(addr))[0])) +
        ((uint16)(((uint8*)(addr))[1]) << 8);
}



/*
** Frame routine of the high score screen
*/
static void hiscore_frame(void)
{
 auint  i;
 auint  hen;
 uint16 btp;
 uint16 btn;
 auint  ch;

 /* When completed (indicated by cursor >= 10), give as much time for EEPROM
 ** save as possible */

 if       (HISCORE_CPOS == 10U){

  HISCORE_CPOS = 11U;
  eeprom_save(HISCORE_EADDR, HISCORE_EADDR_T);
  M74_Halt();

 }else if (HISCORE_CPOS >  10U){

  seq_new(1U);

 }else{}

 /* Graphics */

 global_process();

 M74_VramRestore();

 global_genpal();

 global_setxshift(0U);

 sprite_blit(spriteid_conv(0x1DU + HISCORE_DRG_FRAME), 96U, 24U,
             M74_SPR_I3 | M74_SPR_FLIPX, REC_DRAGON);

 if ((global_framectr & 0x03U) == 0U){
  HISCORE_DRG_FRAME += 2U;
  if (HISCORE_DRG_FRAME == 14U){
   HISCORE_DRG_FRAME = 0U;
  }
 }

 /* Display current high score state */

 hen = HISCORE_ENTRY;
 text_disp_ram(
     HISCORE_EADDR + hen,
     LOC_INTXTVRAM_OFF + (6 * 32U) + 7U);
 text_disp_dots(
     LOC_INTXTVRAM_OFF + (6 * 32U) + 7U);
 gstat_score_disp(
     LOC_INTXTVRAM_OFF + (6 * 32U) + 20U,
     hiscore_getval(HISCORE_EADDR + 10U + hen),
     0x70U);

 /* Clear & Display cursor */

 for (i = 0U; i < 10U; i ++){
  ((uint8*)(LOC_INTXTVRAM_OFF + (5 * 32U) + 7U))[i] = 0x60; /* ' ' */
 }
 for (i = 0U; i < 10U; i ++){
  ((uint8*)(LOC_INTXTVRAM_OFF + (7 * 32U) + 7U))[i] = 0x60; /* ' ' */
 }
 ((uint8*)(LOC_INTXTVRAM_OFF + (5 * 32U) + 7U))[HISCORE_CPOS] = 0x3C; /* '|' */
 ((uint8*)(LOC_INTXTVRAM_OFF + (7 * 32U) + 7U))[HISCORE_CPOS] = 0x3C; /* '|' */

 /* Input logic */

 btn = ReadJoypad(0);
 btp = HISCORE_BTNC_LO | ((uint16)(HISCORE_BTNC_HI) << 8);
 HISCORE_BTNC_LO = btn & 0xFFU;
 HISCORE_BTNC_HI = btn >> 8;
 if (((btn ^ btp) & btn) != 0U){ /* Something was pressed */
  HISCORE_ITICK = 0U;
 }

 if ( (btn != 0U) && (HISCORE_ITICK == 0U) ){ /* Accept a button press */

  if ((btn & BTN_LEFT)  != 0U){ if (HISCORE_CPOS > 0U){ HISCORE_CPOS --; }}
  if ((btn & BTN_RIGHT) != 0U){ if (HISCORE_CPOS < 9U){ HISCORE_CPOS ++; }}

  ch = ((uint8*)(HISCORE_EADDR + hen))[HISCORE_CPOS];
  if      (ch < 26U){ HISCORE_SHIFT = 1U; } /* Uppercase letter: Force shifted */
  else if (ch < 52U){ HISCORE_SHIFT = 0U; } /* Lowercase letter: Force unshifted */
  else              {}                      /* Preserve shift state */

  if ((btn & BTN_DOWN) != 0U){
   ch = (ch - 1U) & 0x3FU;
   if (ch == 25U){ /* 'a' (26) => ' ' (63) */
    ch = 63U;
   }
   if (ch == 51U){ /* '0' (52) => 'Z' (25) if shifted */
    if (HISCORE_SHIFT != 0U){
     ch = 25U;
    }
   }
  }

  if ((btn & BTN_UP)   != 0U){
   ch = (ch + 1U) & 0x3FU;
   if (ch == 26U){ /* 'Z' (25) => '0' (52) */
    ch = 52U;
   }
   if (ch ==  0U){ /* ' ' (63) => 'a' (26) if unshifted */
    if (HISCORE_SHIFT == 0U){
     ch = 26U;
    }
   }
  }

  if ((btn & (BTN_SR | BTN_SL | BTN_X | BTN_A | BTN_Y | BTN_B) ) != 0U){
   HISCORE_SHIFT ^= 1U;
   if      (ch < 26U){ ch += 26U; } /* 'a' - 'z' => 'A' - 'Z' */
   else if (ch < 52U){ ch -= 26U; } /* 'A' - 'Z' => 'a' - 'z' */
   else              {}
  }

  ((uint8*)(HISCORE_EADDR + hen))[HISCORE_CPOS] = ch;

  if ((btn & (BTN_START | BTN_SELECT) ) != 0U){ /* Completed */
   global_fadecolor = 0x00U;
   global_palctr = GLOBAL_FADE_ALLV | GLOBAL_FADE_INC;
   sound_effect(SOUND_ITEM, 0x40U);
  }else{
   sound_effect(SOUND_TICK, 0x30U);
  }

 }

 if (global_fadectr == 0xFFU){
  HISCORE_CPOS = 10U; /* Done, indicate request for saving the scores */
 }

 HISCORE_ITICK = (HISCORE_ITICK + 1U) & 0x0FU;

 M74_Halt();
}



/*
** Initializes and starts a high score screen. Calls seq_new() when it is
** over (this is the last screen of a game session). This screen is skipped
** (passes over to seq_new) if the score isn't in the top three.
*/
void hiscore_enter(void)
{
 auint  i;
 auint  hen;

 /* Load and check if player made it */

 eeprom_load(HISCORE_EADDR, HISCORE_EADDR_T);

 if (gstat_score < hiscore_getval(HISCORE_EADDR + 10U + (2U * 12U))){
  seq_new(0U); /* No high score (smaller than third score) */
 }

 /* Prepare entry */

 hen = 24U;
 do{
  hen -= 12U;

  if (gstat_score >= hiscore_getval(HISCORE_EADDR + 10U + hen)){
   for (i = 0U; i < 12U; i ++){ /* Score was taken over: shift it down */
    ((uint8*)(HISCORE_EADDR + hen + 12U))[i] = ((uint8*)(HISCORE_EADDR + hen))[i];
   }
  }else{
   hen += 12U;
   break;
  }

 }while (hen != 0U);

 for (i = 0U; i < 10U; i ++){
  ((uint8*)(HISCORE_EADDR + hen))[i] = pgm_read_byte(&(hiscore_name[i]));
 }
 ((uint8*)(HISCORE_EADDR + hen))[10] = gstat_score & 0xFFU;
 ((uint8*)(HISCORE_EADDR + hen))[11] = gstat_score >> 8;

 HISCORE_ENTRY = hen;

 /* Make sure nothing would be visible */

 global_hide();

 /* Init variables */

 HISCORE_BTNC_LO   = 0U;
 HISCORE_BTNC_HI   = 0U;
 HISCORE_DRG_FRAME = 0U;
 HISCORE_CPOS      = 0U;
 HISCORE_SHIFT     = 0U;
 HISCORE_ITICK     = 0U;

 /* Clear VRAM */

 display_clear(DISPLAY_INTRO);

 /* Reset VRAM restore list (for sprite blitting) */

 M74_ResReset();

 /* Control fading: Fade in from whatever was set before */

 global_palctr = GLOBAL_FADE_ALLV;

 /* Start music */

 music_start_id(0U);

 /* Sets display for intro mode */

 display_set(DISPLAY_INTRO, (uint16)(&hiscore_frame));
}
