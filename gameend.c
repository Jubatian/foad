/*
 *  Dragon - End game screen logic
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


#include "death.h"
#include "hiscore.h"
#include "display.h"
#include "global.h"
#include "torch.h"
#include "text.h"
#include "sprite.h"
#include "spriteid.h"
#include "music.h"
#include "levelscr.h"
#include <uzebox.h>
#include <avr/pgmspace.h>



#define GAMEEND_EXIT global_shared[2]
#define GAMEEND_TOUT global_shared[3]
#define GAMEEND_DPOS global_shared[4]
#define GAMEEND_TLO  global_shared[5]
#define GAMEEND_THI  global_shared[6]
#define GAMEEND_SPOS global_shared[7]



/*
** Grassland
*/
static uint8 const gameend_bg[] PROGMEM = {
 0x08U, 0x07U, 0x07U, 0x6EU, 0x07U, 0x6DU, 0x6DU, 0x08U,
 0x07U, 0x08U, 0x08U, 0x6DU, 0x08U, 0x6DU, 0x6DU, 0x07U,
 0x6EU, 0x07U, 0x08U, 0x6DU, 0x07U, 0x07U, 0x08U, 0x08U, 0x00U,

 0x03U, 0x03U, 0x04U, 0x03U, 0x04U, 0x04U, 0x03U, 0x04U,
 0x04U, 0x03U, 0x03U, 0x03U, 0x04U, 0x03U, 0x04U, 0x03U,
 0x03U, 0x04U, 0x03U, 0x03U, 0x03U, 0x04U, 0x04U, 0x03U, 0x00U,

 0x04U, 0x09U, 0x0AU, 0x0AU, 0x0AU, 0x0AU, 0x0AU, 0x0AU,
 0x0AU, 0x0AU, 0x0AU, 0x0AU, 0x0AU, 0x0AU, 0x0AU, 0x0AU,
 0x0AU, 0x0AU, 0x0AU, 0x0AU, 0x0AU, 0x0AU, 0x0BU, 0x04U
};



/*
** Routine to copy off 6 rows of sky to generate backdrop for the top 6 rows
*/
static void gameend_skycp(void)
{
 auint  i;
 uint8* cram = (uint8*)(0x0E00U);

 for (i = 0U; i < 48U; i ++){
  cram[i] = cram[i + 106U];
 }
}



/*
** Routine to add a row of text. Returns new text position. Clears the VRAM
** row it targets.
*/
static uint16 gameend_addtxt(uint16 txpos, auint ypos)
{
 auint  i;
 uint8* vptr = (uint8*)(LOC_INTXTVRAM_OFF + (32U * ypos));

 for (i = 0U; i < 32U; i++){ vptr[i] = 0x60U; } /* Blank tile (space) */

 if (txpos >= TXT_GAMEEND_END){
  return txpos;
 }else{
  return text_add(txpos, ypos, 1U);
 }
}



/*
** Frame routine of the end game screen
*/
static void gameend_frame(void)
{
 auint  fctr1 = global_framectr;
 auint  fctr2 = global_framectr + 57U;
 uint8* rsel  = (uint8*)(M74_ROWS_OFF); /* Text rows offset */
 uint8* tram  = (uint8*)(LOC_INTXTVRAM_OFF); /* Scrolling text VRAM */
 uint16 i16;

 global_process();

 M74_VramRestore();

 global_genpal();
 gameend_skycp();

 global_setxshift(0U);

 /* Dragon movements */

 if ( (GAMEEND_DPOS < 84U) || (GAMEEND_DPOS >= 224U) ){ /* 1. Dragons approach */

  sprite_blit(spriteid_conv(0x00U + ((fctr1 >> 3) & 6U)),        GAMEEND_DPOS, 48U,
              M74_SPR_I3 | M74_SPR_MASK | M74_SPR_FLIPX, REC_DRAGON);
  sprite_blit(spriteid_conv(0x00U + ((fctr2 >> 3) & 6U)), 192U - GAMEEND_DPOS, 48U,
              M74_SPR_I3 | M74_SPR_MASK,                 REC_DRAGON);
  if ((global_framectr & 0x03U) == 0U){ GAMEEND_DPOS ++; }

 }else if (GAMEEND_DPOS < 160U){ /* 2. Dragons stand facing each other */

  sprite_blit(spriteid_conv(0x08U + ((fctr1 >> 4) & 2U) + (((GAMEEND_DPOS >> 4) & 1U) ^ 1U)),
              84U,        48U, M74_SPR_I3 | M74_SPR_MASK | M74_SPR_FLIPX, REC_DRAGON);
  sprite_blit(spriteid_conv(0x08U + ((fctr2 >> 4) & 2U) + (((GAMEEND_DPOS >> 4) & 1U) ^ 1U)),
              192U - 84U, 48U, M74_SPR_I3 | M74_SPR_MASK,                 REC_DRAGON);
  if ((global_framectr & 0x03U) == 0U){ GAMEEND_DPOS ++; }

 }else{ /* 3. Dragons sit down */

  sprite_blit(spriteid_conv(0x0CU + ((fctr1 >> 4) & 2U)),        84U, 48U,
              M74_SPR_I3 | M74_SPR_MASK | M74_SPR_FLIPX, REC_DRAGON);
  sprite_blit(spriteid_conv(0x0CU + ((fctr2 >> 4) & 2U)), 192U - 84U, 48U,
              M74_SPR_I3 | M74_SPR_MASK,                 REC_DRAGON);
 }

 /* Text region. By default it displays from row 20 to 29, row 19 is added for
 ** the scroll. */

 if ( (GAMEEND_DPOS < 224U) && (GAMEEND_DPOS > 60U) ){

  if ((global_framectr & 0x7U) == 0U){

   GAMEEND_SPOS = (GAMEEND_SPOS + 1U) & 7U;
   *rsel = (19U * 8U) + GAMEEND_SPOS;
   if (GAMEEND_SPOS == 0U){
    for (i16 = 0U; i16 < (32U * 10U); i16 ++){
     tram[i16] = tram[i16 + 32U];
    }
    i16 = (uint16)(GAMEEND_TLO) + (((uint16)(GAMEEND_THI)) << 8);
    i16 = gameend_addtxt(i16, 10U);
    GAMEEND_TLO = (uint8)(i16 & 0xFFU);
    GAMEEND_THI = (uint8)(i16 >> 8);
   }

  }

 }

 /* User interaction & timeout */

 if ((global_framectr & 0x3FU) == 0U){ GAMEEND_TOUT --; } /* Roughly 1 sec / decrement */

 if (global_ispress() || (GAMEEND_TOUT == 0U)){
  global_fadecolor = 0x00U;
  global_palctr = GLOBAL_FADE_ALLV | GLOBAL_FADE_INC;
  GAMEEND_EXIT = 1U;
 }
 if ((GAMEEND_EXIT != 0U) && (global_fadectr == 0xFFU)){
  hiscore_enter();
 }

 M74_Halt();
}



/*
** Initializes and starts end screen. Calls hiscore_enter() when it is over.
*/
void gameend_enter(void)
{
 uint16 i16;
 uint16 t16;
 uint8  i8;
 uint8* vram;

 /* Make sure nothing would be visible */

 global_hide();

 /* Init for press detection */

 global_initpress();

 /* Init variables */

 GAMEEND_EXIT = 0U;
 GAMEEND_TOUT = 45U;
 GAMEEND_DPOS = 224U;
 GAMEEND_SPOS = 0U;

 /* Clear VRAM */

 display_clear(DISPLAY_INTRO);
 gameend_skycp(); /* Copy from a sky which was valid */

 /* Add text */

 t16 = TXT_GAMEEND_POS;
 for (i8 = 0U; i8 < 5U; i8 ++){
  t16 = gameend_addtxt(t16, 5U + i8);
 }
 GAMEEND_TLO = (uint8)(t16 & 0xFFU);
 GAMEEND_THI = (uint8)(t16 >> 8);

 /* Add grassland graphics */

 vram = ((uint8*)(M74_VRAM_OFF));
 i16 = 0U;
 t16 = (uint16)(&gameend_bg[0]);
 for (i8 = 0U; i8 < (25U * 5U); i8 ++){
  vram[i16] = 0U;
  i16 ++;
 }
 for (i8 = 0U; i8 < ((25U * 3U) - 1U); i8 ++){
  vram[i16] = pgm_read_byte(t16);
  t16 ++;
  i16 ++;
 }

 /* Select sky */

 global_palsel = (global_palsel & 0xF0U) | 1U;
 levelscr_y = 0U; /* Fix sky vertical position so it can be copied. */

 /* Memory map notes:
 ** On the intro screen, the color replacement table for tile rows 0 - 5 is
 ** normally the upper end of the VRAM (0xE00 - 0xE2F). This region is not
 ** used on the intro screen layout, so it can be used to copy off the
 ** required section of the sky */

 /* Reset VRAM restore list (for sprite blitting) */

 M74_ResReset();

 /* Control fading: Fade in from whatever was set before */

 global_palctr = GLOBAL_FADE_ALLV;

 /* Start music */

 music_start_id(3U);

 /* Sets display for intro mode */

 display_set(DISPLAY_INTRO, (uint16)(&gameend_frame));
}
