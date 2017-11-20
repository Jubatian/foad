/*
 *  Dragon - Fireballs (dragon's breath), companion asm module
 *  Copyright (C) 2017 Sandor Zsuga (Jubatian)
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



#include <avr/io.h>
; (Needed only for register names for X, Y and Z)


/* Count of fireballs. Note: Keep in sync with that in fireball.h! */
#define FIREBALL_N 12



.section .text



/*
** Returns whether there is a fireball within a sprite. Returns the fireball's
** ID if there is any or FIREBALL_N if there is none. This only returns
** fireballs which aren't inactivated.
**
** Inputs:
** r25:r24: spr (pointer to sprite)
** Outputs:
** r25:r24: Fireball ID within sprite or FIREBALL_N
*/
.global fireball_getat
fireball_getat:

	; Load sprite parameters

	movw  XL,      r24
	ld    ZL,      X+
	ld    ZH,      X+      ; xpos
	ld    r18,     X+
	ld    r19,     X+      ; ypos
	ld    r0,      X+      ; xbs
	ld    r25,     X+      ; ybs
	clr   r1               ; (zero, should be so by C, though)

	; Prepare for iterating the fireballs

	ldi   XL,      lo8(fireball_arr)
	ldi   XH,      hi8(fireball_arr)

	; Enlarge sprite slightly to account for fireball size (more chance to
	; hit). The dragon can't aim for specific sprites, this helps him
	; delivering a hit slightly.

	ldi   r24,     4
	add   r0,      r24     ; X size + 4 (total: 8 px wider)
	subi  r25,     0xFC    ; Y size + 4 (this means 20 pixels tall things in a 24 px tall corridor will always be hit)
	add   ZL,      r0
	adc   ZH,      r1      ; xpos + xbs

	clr   r24              ; ID counter
	rjmp  gatl0e

	; Iterate and check
	; Coordinates from levelcor_decode_asm return as:
	; X: r23:r22
	; Y: r21:r20
	; (X incremented by 3)

gatl0i:
	adiw  XL,      5
gatl0:
	inc   r24
	cpi   r24,     FIREBALL_N
	breq  gatx

gatl0e:

	ld    r20,     X+      ; Fireball's burnout timer (exists?)
	cpi   r20,     0
	breq  gatl0i           ; Zero: Fireball does not exist
	cpi   r20,     64
	brcc  gatl0i           ; Fireball is temporarily inactivated
	adiw  XL,      2       ; Fireball's coordinates
	call  levelcor_decode_asm
	cp    ZL,      r22
	cpc   ZH,      r23     ; (xpos + xbs) - fireball.X
	brcs  gatl0            ; <  0: Fireball is off to the right
	add   r22,     r0
	adc   r23,     r1
	add   r22,     r0
	adc   r23,     r1      ; fireball.X + xbs + xbs
	cp    ZL,      r22
	cpc   ZH,      r23     ; (xpos - xbs) - fireball.X
	brcc  gatl0            ; >= 0: Fireball is off to the left
	cp    r18,     r20
	cpc   r19,     r21     ; ypos - fireball.Y
	brcs  gatl0            ; <  0: Fireball is off to the bottom
	add   r20,     r25
	adc   r21,     r1      ; fireball.Y + ybs
	cp    r18,     r20
	cpc   r19,     r21     ; (ypos - ybs) - fireball.Y
	brcc  gatl0            ; >= 0: Fireball is off to the top

	; Fireball is within the sprite's bounding box: Hit!

gatx:

	clr   r25
	ret
