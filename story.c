/*
 *  Dragon - Story screen logic
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
#include "text.h"
#include "music.h"
#include <uzebox.h>



#define STORY_TEXTPOS_LO global_shared[2]
#define STORY_TEXTPOS_HI global_shared[3]
#define STORY_TEXTHEIGHT global_shared[4]
#define STORY_STAT       global_shared[5]
#define STORY_TXE_LO     global_shared[6]
#define STORY_TXE_HI     global_shared[7]



/*
** Adds text to the VRAM, as much as STORY_TXE permits. Returns 1 if all text
** could be output.
*/
static auint story_txadd(void)
{
 uint8* vram = ((uint8*)(M74_VRAM_OFF));
 uint16 txp = STORY_TEXTPOS_LO | ((uint16)(STORY_TEXTPOS_HI) << 8);
 auint  py  = (16U - STORY_TEXTHEIGHT) >> 1;
 auint  pye = py + STORY_TEXTHEIGHT;
 auint  px  = 2U;
 auint  ch;

 do{

  if (txp == ( (STORY_TEXTPOS_LO | ((uint16)(STORY_TEXTPOS_HI) << 8)) +
               (STORY_TXE_LO | ((uint16)(STORY_TXE_HI) << 8)) ) ){ return 0U; }

  ch = text_rom_getc(txp);
  txp ++;

  if ((ch == 0x3FU) || (px == 30U)){
   py ++;
   px = 2U;
  }

  if (ch != 0x3FU){
   vram[((uint16)(py) * 32U) + px] = ch;
   px ++;
  }

 }while (py != pye);

 return 1U;
}



/*
** Frame routine of the story screen
*/
static void story_frame(void)
{
 global_process();
 global_genpal();

 /* If a press happened, step the state machine */

 if (global_ispress()){
  STORY_STAT ++;
 }

 /* Process according to state machine */

 switch (STORY_STAT){

  case 0x00U: /* Incrementally add text */

   STORY_TXE_LO ++;
   if (STORY_TXE_LO == 0U){ STORY_TXE_HI ++; }
   if (STORY_TXE_HI == 2U){
    STORY_STAT ++;
   }else if (story_txadd()){
    STORY_STAT ++;
   }else{}
   break;

  case 0x01U: /* Add full text */

   STORY_TXE_LO = 0U;
   STORY_TXE_HI = 2U;
   STORY_STAT ++;
   story_txadd();
   break;

  case 0x02U: /* Wait for user */

   break;

  case 0x03U: /* Fade out */

   global_fadecolor = 0x52U;
   global_palctr = GLOBAL_FADE_ALLV | GLOBAL_FADE_INC;
   if (global_fadectr == 0xFFU){
    STORY_STAT ++;
   }
   break;

  default:    /* Transfer */

   seq_next();

 }

 M74_Halt();
}



/*
** Initializes and starts a story screen. Calls seq_next() when it is over.
*/
void story_enter(uint16 textpos, auint textheight)
{
 /* Make sure nothing would be visible */

 global_hide();

 /* Inits control variables */

 global_initpress();
 STORY_TEXTPOS_LO = textpos & 0xFFU;
 STORY_TEXTPOS_HI = textpos >> 8;
 STORY_TEXTHEIGHT = textheight;
 STORY_STAT       = 0U;
 STORY_TXE_LO     = 0U;
 STORY_TXE_HI     = 0U;

 /* Clear VRAM */

 display_clear(DISPLAY_STORY);

 /* Control fading: Fade in from whatever was set before */

 global_palctr = GLOBAL_FADE_ALLV;

 /* Start music */

 music_start_id(1U);

 /* Sets display for story mode */

 display_set(DISPLAY_STORY, (uint16)(&story_frame));
}
