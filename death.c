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
#include <uzebox.h>
#include <avr/pgmspace.h>



#define DEATH_EXIT global_shared[2]
#define DEATH_TOUT global_shared[3]



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
 global_process();

 M74_VramRestore();

 torch_prep();

 global_genpal();

 global_setxshift(0U);

 sprite_blit(spriteid_conv(0x0CU + ((global_framectr >> 4) & 2U)), 90U, 32U,
             M74_SPR_I3 | M74_SPR_MASK | M74_SPR_FLIPX, REC_DRAGON);

 torch_render();

 if ((global_framectr & 0x3FU) == 0U){ DEATH_TOUT --; } /* Roughly 1 sec / decrement */

 if (global_ispress() || (DEATH_TOUT == 0U)){
  global_fadecolor = 0x00U;
  global_palctr = GLOBAL_FADE_ALLV | GLOBAL_FADE_INC;
  DEATH_EXIT = 1U;
 }
 if ((DEATH_EXIT != 0U) && (global_fadectr == 0xFFU)){
  hiscore_enter();
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
 uint8  ch;
 uint8* vram;

 /* Make sure nothing would be visible */

 global_hide();

 /* Init for press detection */

 global_initpress();

 /* Init variables */

 DEATH_EXIT =  0U;
 DEATH_TOUT = 16U;

 /* Clear VRAM */

 display_clear(DISPLAY_INTRO);

 /* Add text */

 t16 = TXT_OVER_POS;
 i16 = (6U * 32U) + 1U;
 vram = ((uint8*)(LOC_INTXTVRAM_OFF));
 for (i8 = 1U; i8 < 31U; i8 ++){
  ch = text_rom_getc(t16);
  if (ch == 0x3FU){ break; }
  vram[i16] = ch;
  t16 ++;
  i16 ++;
 }

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
