/*
 *  Dragon - Torch tile logic
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
** Prepares torch tiles. This must be called before starting to render: it
** fills in RAM tile masks for the torch tiles.
*/
.global torch_prep
torch_prep:

	ldi   XL,      lo8(M74_RAMMASKIDX_OFF + 56)
	ldi   XH,      hi8(M74_RAMMASKIDX_OFF + 56)
	ldi   r24,     0xFE
	st    X+,      r24
	st    X+,      r24
	st    X+,      r24
	st    X+,      r24
	ret



/*
** Animates torch tiles. This may be called anywhere, should possibly be
** called last (this is the least important, so no problem if it doesn't fit
** in the frame).
*/
.global torch_render
torch_render:

	; Torch tiles are in the 1bpp charset area (RES_CHARSET_00_OFF), the
	; last 64 bytes of every row contains 2 torch tiles.

	push  YL
	push  YH
	push  r16

	ldi   YL,      lo8(RES_CHARSET_00_OFF + 192)
	ldi   YH,      hi8(RES_CHARSET_00_OFF + 192)
	lds   r24,     global_framectr
	lsr   r24
	lsr   r24
	andi  r24,     0x03
	add   YH,      r24
	ldi   r16,     ((LOC_RAMTILES_OFF >> 5) + 56)

	rcall rren
	subi  YH,      0xFC    ; +4 (+1K)
	rcall rren
	subi  YH,      0x04    ; -4 (-1K)
	subi  YL,      0xE0    ; +32
	rcall rren
	subi  YH,      0xFC    ; +4 (+1K)
	rcall rren

	pop   r16
	pop   YH
	pop   YL
	ret

rren:
	movw  r24,     YL
	mov   r22,     r16
	call  M74_RamTileFillRom
	subi  r16,     0xFF
	ret
