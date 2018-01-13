/*
 *  Dragon - EEPROM score storage
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
** Supports saving / loading game scores from / to EEPROM, automatically
** handling Uzebox error conditions in a sane manner.
**
** The score data is as follows:
**
** There are 3 high score entries, each entry taking 10 6 bit characters (only
** low 6 bits of the bytes are filled, see text.h for layout) and 2 bytes of
** high score value. So it totals 36 bytes (but due to the compression of the
** text part it fits in the 30 + 2 byte EEPROM block).
**
** The EEPROM block ID is 0xF0AD (For FoaD: Flight of a Dragon), stored in
** Little Endian format in the EEPROM itself (as the Uzebox kernel does this).
*/



#ifndef EEPROM_H
#define EEPROM_H


#include "types.h"



/*
** Loads EEPROM score data to target address. If it doesn't exist, default
** scores are created. 36 bytes of RAM is required for it at addr, tadd is
** used for temporary storage, 32 bytes (can not overlap with addr).
*/
void eeprom_load(uint16 addr, uint16 tadd);


/*
** Saves EEPROM score data from source address (36 bytes at addr), tadd is
** used for temporary storage, 32 bytes (can not overlap with addr). Note
** that it disables Mode 74 for the save (to prevent resets), re-enabling it
** upon return. The state of the frame is not reliable this case (a reset
** might happen any time), so only an M74_Halt() should follow this.
*/
void eeprom_save(uint16 addr, uint16 tadd);


/*
** Load JAMMA dip switch configuration, tadd is used for temporary storage,
** 32 bytes.
*/
uint8 eeprom_loadjdips(uint16 tadd);


#endif
