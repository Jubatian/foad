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



#include "m74cfg.h"
#include <avr/io.h>
; (Needed only for register names for X, Y and Z)



.section .text



/*
** Fetches character from ROM text storage, from the given character position
** in it.
**
** Inputs:
** r25:r24: Character position
** Outputs:
** r25:r24: Character code (0 - 127 of charset)
** Clobbers:
** r0, r1(zero), ZL, ZH
*/
.global text_rom_getc
text_rom_getc:

	; Get bit position in ZH:ZL (and set r1 = 7)

	ldi   ZL,      7
	mul   r25,     ZL
	mov   ZH,      r0
	mul   r24,     ZL
	add   ZH,      r1
	mov   r1,      ZL
	mov   ZL,      r0

	; Get byte position in Z and start bit in r25 as mask

	ldi   r25,     0x80
	lsr   ZH
	ror   ZL
	brcc  .+2
	lsr   r25
	lsr   ZH
	ror   ZL
	brcc  .+4
	lsr   r25
	lsr   r25
	lsr   ZH
	ror   ZL
	brcc  .+2
	swap  r25

	; Add text material offset

	subi  ZL,      lo8(-(RES_TEXT_OFF))
	sbci  ZH,      hi8(-(RES_TEXT_OFF))

	; Generate character into r24 (r1 is 7 for counting bits)

	lpm   r0,      Z+
	lpm   ZL,      Z
	clr   r24
romgl:
	mov   ZH,      r0
	and   ZH,      r25
	neg   ZH               ; Sets carry if nonzero
	rol   r24              ; Collect bits (high first)
	lsr   r25
	brne  .+4
	ldi   r25,     0x80
	mov   r0,      ZL
	dec   r1
	brne  romgl

	; Done (r1 zero)

	clr   r25
	ret



/*
** Depacks RAM text loaded at the given position (10 chars on 8 bytes) to the
** given position (10 bytes, 0-63).
**
** Assembler entry point is also provided working X => Z, incrementing
** pointers appropriately.
**
** r25:r24: Source address
** r23:r22: Destination address
*/
.global text_x_ram
text_x_ram:

	movw  XL,      r24
	movw  ZL,      r22

.global text_x_ram_xz
text_x_ram_xz:

	ldi   r22,     2       ; 2 blocks of 3 byte <=> 4 chars

xra0:

	ld    r24,     X+
	clr   r25
	lsr   r24
	ror   r25
	lsr   r24
	ror   r25
	st    Z+,      r24

	ld    r24,     X+
	mov   r23,     r24
	swap  r24
	andi  r24,     0x0F
	lsr   r25
	lsr   r25
	or    r25,     r24
	st    Z+,      r25

	ld    r24,     X+
	mov   r25,     r24
	lsl   r25
	rol   r23
	lsl   r25
	rol   r23
	andi  r23,     0x3F
	st    Z+,      r23

	andi  r24,     0x3F
	st    Z+,      r24

	dec   r22
	brne  xra0

	ld    r24,     X+      ; 2 normal characters
	andi  r24,     0x3F
	st    Z+,      r24

	ld    r24,     X+
	andi  r24,     0x3F
	st    Z+,      r24

	ret



/*
** Packs RAM text from the given position (10 bytes, 0-63) to the given
** position (8 bytes).
**
** Assembler entry point is also provided working X => Z, incrementing
** pointers appropriately.
**
** r25:r24: Source address
** r23:r22: Destination address
*/
.global text_c_ram
text_c_ram:

	movw  XL,      r24
	movw  ZL,      r22

.global text_c_ram_xz
text_c_ram_xz:

	ldi   r22,     2       ; 2 blocks of 3 byte <=> 4 chars

cra0:

	ld    r24,     X+

	ld    r25,     X+
	lsl   r25
	lsl   r25
	lsl   r25
	rol   r24
	lsl   r25
	rol   r24
	st    Z+,      r24

	ld    r24,     X+
	lsl   r24
	lsl   r24
	swap  r24
	mov   r23,     r24
	andi  r23,     0x0F
	or    r25,     r23
	st    Z+,      r25

	ld    r25,     X+
	andi  r24,     0xC0
	andi  r25,     0x3F
	or    r24,     r25
	st    Z+,      r24

	dec   r22
	brne  cra0

	ld    r24,     X+      ; 2 normal characters
	andi  r24,     0x3F
	st    Z+,      r24

	ld    r24,     X+
	andi  r24,     0x3F
	st    Z+,      r24

	ret



/*
** Converts 6 bit character code into display byte (r24)
*/
text_disp_ram_cv:

	cpi   r24,     26
	brcs  dra0
	cpi   r24,     52
	brcs  dra1
	cpi   r24,     62
	brcs  dra2
	cpi   r24,     63
	brcs  dra3
	ldi   r24,     0x60    ; ' '
	ret
dra0:
	subi  r24,     0xFF    ; [A-Z]
	ret
dra1:
	subi  r24,     0xF9    ; [a-z]
	ret
dra2:
	subi  r24,     0xC4    ; [0-9]
	ret
dra3:
	ldi   r24,     0x6D    ; '-'
	ret



/*
** Converts 6 bit text in RAM (10 chars, 0-63, on 10 bytes) to display format.
**
** r25:r24: Source address
** r23:r22: Destination address
*/
.global text_disp_ram
text_disp_ram:

	movw  XL,      r24
	movw  ZL,      r22

	ldi   r22,     10

dra9:

	ld    r24,     X+
	rcall text_disp_ram_cv
	st    Z+,      r24
	dec   r22
	brne  dra9

	ret



/*
** Replaces trailing part of a 12 char text to dots (for score display).
**
** r25:r24: Address of 12 char text
*/
.global text_disp_dots
text_disp_dots:

	movw  XL,      r24
	adiw  XL,      11

	ldi   r25,     12

dot0:
	ld    r24,     X
	cpi   r24,     0x6E    ; '.'
	breq  dotc
	cpi   r24,     0x60    ; ' '
	brne  dotx
	ldi   r24,     0x6E    ; '.'
	st    X,       r24
dotc:
	sbiw  XL,      1
	dec   r25
	brne  dot0

dotx:
	cpi   r25,     12
	brne  .+2
	ret
	adiw  XL,      1
	ldi   r24,     0x60    ; ' ' (Clear dot after last valid char)
	st    X,       r24
	ret



/*
** Clears text VRAM (use on mixed text / graphics screens).
*/
.global text_clear_vram
text_clear_vram:

	ldi   XL,      lo8(LOC_INTXTVRAM_OFF)
	ldi   XH,      hi8(LOC_INTXTVRAM_OFF)
	ldi   ZL,      lo8(LOC_INTXTVRAM_OFF + (12 * 32))
	ldi   ZH,      hi8(LOC_INTXTVRAM_OFF + (12 * 32))
	ldi   r23,     0x60
tcvl:
	st    X+,      r23
	cp    XL,      ZL
	cpc   XH,      ZH
	brne  tcvl
	ret



/*
** Adds text to VRAM from ROM text storage along with clearing the text VRAM.
** Returns text position after the add.
**
** Inputs:
** r25:r24: Source ROM text address
**     r22: Y position on VRAM
**     r20: Height
** Outputs:
** r25:r24: New text position
*/
.global text_add_clear
text_add_clear:

	rcall  text_clear_vram ; Fall through to text_add



/*
** Adds text to VRAM from ROM text storage.
** Returns text position after the add.
**
** Inputs:
** r25:r24: Source ROM text address
**     r22: Y position on VRAM
**     r20: Height
** Outputs:
** r25:r24: New text position
*/
.global text_add
text_add:

	ldi   r18,     lo8(LOC_INTXTVRAM_OFF + 1)
	ldi   r19,     hi8(LOC_INTXTVRAM_OFF + 1)
	ldi   r23,     32
	mul   r22,     r23     ; Add start position
	add   r18,     r0
	adc   r19,     r1      ; r19:r18: Starting line, X position is 1.
	movw  r22,     r24     ; r23:r22: Character position
txal0:
	movw  XL,      r18
	ldi   r21,     30      ; Up to 30 characters
txal1:
	movw  r24,     r22
	rcall text_rom_getc
	subi  r22,     0xFF
	sbci  r23,     0xFF    ; Next character
	cpi   r24,     0x3F    ; End of line?
	breq  txal1e
	cpi   r21,     0
	breq  txal1e
	st    X+,      r24
	dec   r21
	rjmp  txal1
txal1e:
	subi  r18,     0xE0
	sbci  r19,     0xFF    ; Add 32
	dec   r20
	brne  txal0
	movw  r24,     r22     ; Return text position
	ret
