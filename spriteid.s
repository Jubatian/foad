/*
 *  Dragon - Sprite ID to address conversion
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



#include "m74cfg.h"
#include <avr/io.h>
; (Needed only for register names for X, Y and Z)



.section .text



/*
** Converts the given sprite frame ID to sprite address for sprite_blit().
**
** For assembler calls Z is clobbered.
**
**     r24: sid
**
** Returns in r25:r24.
*/
.global spriteid_conv
spriteid_conv:

	ldi   ZH,      hi8(RES_SPRIDCONV_OFF)
	mov   ZL,      r24
	lpm   r24,     Z
	subi  ZH,      0xFF    ; High bytes are in the bank above
	lpm   r25,     Z       ; (May be halved later to free 256 bytes ROM)
	ret
