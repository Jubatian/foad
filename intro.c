/*
 *  Dragon - Intro screen logic
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
#include "torch.h"
#include "text.h"
#include "eeprom.h"
#include "sprite.h"
#include "spriteid.h"
#include "music.h"
#include <uzebox.h>
#include <avr/pgmspace.h>



#define INTRO_BTNC_LO   global_shared[0]
#define INTRO_BTNC_HI   global_shared[1]
#define INTRO_DRG_FRAME global_shared[2]
#define INTRO_EXIT      global_shared[3]
#define INTRO_TIM_LO    global_shared[4]
#define INTRO_TIM_HI    global_shared[5]
#define INTRO_STAT      global_shared[6]

#define INTRO_EADDR     (LOC_RAMTILES_OFF + 0x600U)
#define INTRO_EADDR_T   (LOC_RAMTILES_OFF + 0x600U + 36U)


/*
** Extracts Little Endian 16 bit number from RAM.
*/
uint16 intro_getval(uint16 addr)
{
 return ((uint16)(((uint8*)(addr))[0])) +
        ((uint16)(((uint8*)(addr))[1]) << 8);
}



/*
** Clears text VRAM.
*/
static void intro_txclr()
{
 uint16 i16;
 uint8* vram;

 vram = ((uint8*)(LOC_INTXTVRAM_OFF));
 for (i16 = 0U; i16 < (12U * 32U); i16 ++){ vram[i16] = 0x60U; }
}



/*
** Clears text VRAM and adds selected text to it.
*/
static void intro_txadd(uint16 txpos, auint ypos, auint height)
{
 uint16 i16;
 uint8  j8;
 uint8  i8;
 uint8  ch;
 uint8* vram;

 intro_txclr();

 vram = ((uint8*)(LOC_INTXTVRAM_OFF));
 i16 = ((uint16)(ypos) * 32U);
 for (j8 = 0U; j8 < height; j8 ++){
  for (i8 = 1U; i8 < 31U; i8 ++){
   ch = text_rom_getc(txpos);
   txpos ++;
   if (ch == 0x3FU){ break; }
   vram[i16 + i8] = ch;
  }
  i16 += 32;
 }
}



/*
** Frame routine of the intro screen
*/
static void intro_frame(void)
{
 auint  i;
 uint16 a16;
 uint16 btn = INTRO_BTNC_LO | ((uint16)(INTRO_BTNC_HI) << 8);

 global_process();

 M74_VramRestore();

 global_genpal();

 global_setxshift(0U);

 sprite_blit(spriteid_conv(0x1CU + INTRO_DRG_FRAME), 96U, 24U,
             M74_SPR_I3, REC_DRAGON);

 if ((global_framectr & 0x03U) == 0U){
  INTRO_DRG_FRAME += 2U;
  if (INTRO_DRG_FRAME == 14U){
   INTRO_DRG_FRAME = 0U;
  }
 }

 if (global_ispress()){
  global_fadecolor = 0x00U;
  global_palctr = GLOBAL_FADE_ALLV | GLOBAL_FADE_INC;
  INTRO_EXIT = 1U;
 }
 if ((INTRO_EXIT != 0U) && (global_fadectr == 0xFFU)){
  seq_next();
 }

 /* Intro state machine looping through screens */

 INTRO_TIM_LO ++;
 if (INTRO_TIM_LO == 0U){ INTRO_TIM_HI ++; }

 if (INTRO_EXIT == 0U){

  switch (INTRO_STAT){

   case 0x00U: /* Flight of a Dragon intro text, init */

    intro_txadd(TXT_TITLE_POS, 5U, 3U);
    goto state_tran;

   case 0x01U: /* Flight of a Dragon intro text */

    if (INTRO_TIM_HI == 3U){ goto state_fout; }
    break;

   case 0x02U: /* Transition to High scores */

    goto state_fin;

   case 0x03U: /* High scores, init */

    intro_txclr();
    eeprom_load(INTRO_EADDR, INTRO_EADDR_T);
    a16 = LOC_INTXTVRAM_OFF + (4U * 32U) + 7U;
    for (i = 0U; i < 36U; i += 12U){
     text_disp_ram(
         INTRO_EADDR + i,
         a16);
     text_disp_dots(
         a16);
     gstat_score_disp(
         a16 + 13U,
         intro_getval(INTRO_EADDR + 10U + i),
         0x70U);
     a16 += 64U;
    }
    goto state_tran;

   case 0x04U: /* High scores */

    if (INTRO_TIM_HI == 2U){ goto state_fout; }
    break;

   case 0x05U: /* Transition to Author text */

    goto state_fin;

   case 0x06U: /* Author text, init */

    intro_txadd(TXT_AUTH_POS, 5U, 3U);
    goto state_tran;

   case 0x07U: /* Author text */

    if (INTRO_TIM_HI == 2U){ goto state_fout; }
    break;

   case 0x08U: /* Transition to intro text */

    goto state_fin;

   default:    /* Reset state machine */

    INTRO_STAT = 0U;
    break;

state_fout:

    global_fadecolor = 0x00U;
    global_palctr = GLOBAL_FADE_TOP | GLOBAL_FADE_INC;
    INTRO_TIM_HI = 0U;

state_tran:

    INTRO_STAT ++;
    break;

state_fin:

    if (global_fadectr == 0xFFU){
     global_palctr = GLOBAL_FADE_TOP;
     INTRO_STAT ++;
    }
    break;

  }

 }

 M74_Halt();
}



/*
** Initializes and starts intro screen. Calls seq_next() when it is over.
** The parameter requests whether to start with the score display (1) or with
** the title (0).
*/
void intro_enter(auint hs)
{
 /* Init state as requested */

 if (hs != 0U){
  INTRO_STAT = 3U; /* High score display */
 }else{
  INTRO_STAT = 0U; /* Game title display */
 }

 /* Make sure nothing would be visible */

 global_hide();

 /* Init variables */

 INTRO_BTNC_LO   = 0U;
 INTRO_BTNC_HI   = 0U;
 INTRO_DRG_FRAME = 0U;
 INTRO_EXIT      = 0U;
 INTRO_TIM_LO    = 0U;
 INTRO_TIM_HI    = 0U;

 /* Clear VRAM */

 display_clear(DISPLAY_INTRO);

 /* Reset VRAM restore list (for sprite blitting) */

 M74_ResReset();

 /* Control fading: Fade in from whatever was set before */

 global_palctr = GLOBAL_FADE_ALLV;

 /* Start music */

 music_start_id(0U);

 /* Sets display for intro mode */

 display_set(DISPLAY_INTRO, (uint16)(&intro_frame));
}
