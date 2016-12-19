/*
 *  Dragon - Text materials
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
** Provides support for text materials.
**
** ROM text:
**
** Text is stored 7 bits / character, so 7 bytes encoding 8 characters. High
** bit comes first. The character order accords with the 1bpp charset's first
** 128 entries. String end is 0x3F (corresponding to 0x7F, DEL in normal
** ASCII). There is no new line character.
**
** On-screen formatting of larger text blocks is done directly (30 columns
** used).
**
** RAM text:
**
** This is used for high score entry / retrieval. An entry is up to 10 chars
** long of the following charset: [A-Z], [a-z], [0-9], '-', ' ': 64
** characters. This is stored 6 bits / character in EEPROM, using 8 bytes
** (allowing 3 high score entries on the available 30 bytes).
**
** The 64 characters are laid out in the above order (so first the uppercase
** letters appear).
*/



#ifndef TEXT_H
#define TEXT_H


#include "types.h"



/*
** Fetches character from ROM text storage, from the given character position
** in it.
*/
uint8 text_rom_getc(uint16 pos);


/*
** Depacks RAM text loaded at the given position (10 chars on 8 bytes) to the
** given position (10 bytes, 0-63).
*/
void  text_x_ram(uint16 src, uint16 dst);


/*
** Packs RAM text from the given position (10 bytes, 0-63) to the given
** position (8 bytes).
*/
void  text_c_ram(uint16 src, uint16 dst);


/*
** Converts 6 bit text in RAM (10 chars, 0-63, on 10 bytes) to display format.
*/
void  text_disp_ram(uint16 src, uint16 dst);


/*
** Replaces trailing part of 12 char text to dots (for score display).
*/
void  text_disp_dots(uint16 addr);


#endif
