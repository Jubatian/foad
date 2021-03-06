/*
 *  Dragon - Death screen logic
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
#include "sound.h"
#include "seq.h"
#include <uzebox.h>
#include <avr/pgmspace.h>



#define DEATH_EXIT global_shared[2]
#define DEATH_TOUT global_shared[3]

/* Initial value of timeout */
#define TOUT_INIT  10U



/*
** Small prison cell
*/
static uint8 const death_cell[] PROGMEM = {
 0x54U, 0x54U, 0x54U, 0x55U, 0x54U, 0x54U, 0x54U, 0x56U,
 0x56U, 0x52U, 0x53U, 0x52U, 0x52U, 0xFAU, 0x52U, 0x56U,
 0x56U, 0x53U, 0x52U, 0x52U, 0x52U, 0xFBU, 0x57U, 0x56U,
 0x56U, 0x52U, 0x52U, 0x52U, 0x53U, 0x52U, 0x58U, 0x56U,
 0x54U, 0x54U, 0x54U, 0x54U, 0x54U, 0x54U, 0x55U, 0x56U
};



/*
** Frame routine of the death screen
*/
static void death_frame(void)
{
 uint8* vram = ((uint8*)(LOC_INTXTVRAM_OFF));
 auint  dval;
 auint  tout;
 auint  cred;
 auint  pres;

 global_process();

 M74_VramRestore();

 torch_prep();

 global_genpal();

 global_setxshift(0U);

 sprite_blit(spriteid_conv(0x0CU + ((global_framectr >> 4) & 2U)), 90U, 32U,
             M74_SPR_I3 | M74_SPR_MASK | M74_SPR_FLIPX, REC_DRAGON);

 torch_render();

 /* Countine display's counters */

 tout = DEATH_TOUT;
 cred = global_credits;

 if (tout < 10U){ dval = tout; }
 else           { dval = 9U; }
 vram[32U * 7U + 19U] = dval + '0' + 0x40U;
 if (cred < 10U){ dval = cred; }
 else           { dval = 9U; }
 vram[32U * 7U + 22U] = dval + '0' + 0x40U;

 /* Logic */

 pres = global_ispress();
 if (pres || (tout == 0U)){
  if (cred != 0U){
   sound_effect(SOUND_ITEM, 0x40U);
  }
  global_fadecolor = 0x00U;
  global_palctr = GLOBAL_FADE_ALLV | GLOBAL_FADE_INC;
  DEATH_EXIT = 1U;
 }

 if (tout != 0U){   /* Timeout controls */
  if (pres){
   tout = TOUT_INIT;
  }
  if ((global_framectr & 0x3FU) == 0U){
   tout --;
  }
 }
 DEATH_TOUT = tout;

 if ((DEATH_EXIT != 0U) && (global_fadectr == 0xFFU)){
  if ((tout == 0U) || (cred == 0U)){
   hiscore_enter(); /* Exit game */
  }else{
   global_credits = cred - 1U;
   seq_reset();     /* Continue game */
  }
 }

 M74_Halt();
}



/*
** Initializes and starts a death screen. Calls seq_new() when it is over.
*/
void death_enter(void)
{
 uint16 i16;
 uint16 t16;
 uint8  i8;
 uint8  j8;
 uint8* vram;

 /* Make sure nothing would be visible */

 global_hide();

 /* Init for press detection */

 global_initpress();

 /* Init variables */

 DEATH_EXIT = 0U;
 DEATH_TOUT = TOUT_INIT;

 /* Clear VRAM */

 display_clear(DISPLAY_INTRO);

 /* Add text */

 text_add_line(TXT_OVER_POS, 5U);
 text_add_line(TXT_CONTINUE_POS, 7U);

 /* Add small prison cell */

 vram = ((uint8*)(M74_VRAM_OFF));
 i16 = (0U * 25U) + 8U;
 t16 = (uint16)(&death_cell[0]);
 for (j8 = 0U; j8 < 5U; j8 ++){
  for (i8 = 0U; i8 < 8U; i8 ++){
   vram[i16] = pgm_read_byte(t16);
   t16 ++;
   i16 ++;
  }
  i16 += 25U - 8U;
 }

 /* Reset VRAM restore list (for sprite blitting) */

 M74_ResReset();

 /* Control fading: Fade in from whatever was set before */

 global_palctr = GLOBAL_FADE_ALLV;

 /* Start music */

 music_start_id(0U);

 /* Sets display for intro mode */

 display_set(DISPLAY_INTRO, (uint16)(&death_frame));
}
