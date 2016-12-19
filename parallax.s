/*
 *  Dragon - Parallax scrolling components
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



.section .bss

.equ	v_flow,        MEM_LOC_4  ; Flowing or drifting state

.section .text



/* Data for various parallax fields */
.balign 16
data_sky_clouds:
	.byte  0x00, 0x30, 0x32, 0x33, 0x00, 0x00, 0x00, 0x00
	.byte  0x00, 0x00, 0x00, 0x00, 0x30, 0x31, 0x32, 0x33
	.byte  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x31
	.byte  0x32, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
data_sky_clear:
	.byte  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte  0x00, 0x00, 0x00, 0x00, 0x30, 0x32, 0x33, 0x00
	.byte  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte  0x00, 0x30, 0x31, 0x32, 0x33, 0x00, 0x00, 0x00
	.byte  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
data_gnd_water:
	.byte  0x34, 0x34, 0x35, 0x34, 0x35, 0x35, 0x34, 0x35
	.byte  0x35, 0x34, 0x34, 0x35, 0x34, 0x35, 0x34, 0x35
data_gnd_grass:
	.byte  0x03, 0x03, 0x04, 0x03, 0x04, 0x04, 0x03, 0x04
	.byte  0x04, 0x03, 0x03, 0x04, 0x03, 0x04, 0x03, 0x04
data_sky_cave:
data_gnd_cave:
	.byte  0x0C, 0x0C, 0x0D, 0x0C, 0x0D, 0x0D, 0x0C, 0x0D
	.byte  0x0D, 0x0C, 0x0C, 0x0D, 0x0C, 0x0D, 0x0C, 0x0D



/*
** Render visible parallax regions. 'x' is the X position in pixels, 'ty' is
** the Y position in tiles (!), both in accordance with levelscr_scroll(). flg
** is the type of parallax to use, also the same as in levelscr_setmap(). hgt
** is the map height in L1 blocks.
**
** r25:r24: x
** r23:r22: ty
**     r20: flg
**     r18: hgt
*/
.global parallax_render
parallax_render:

	; Step flow for parallax elements drifting or flowing

	lsl   r24
	rol   r25
	lsl   r24
	rol   r25              ; X = X * 4
	push  r16
	push  r17
	lds   r16,     v_flow + 0
	lds   r17,     v_flow + 1
	subi  r16,     0xFF
	sbci  r17,     0xFF
	sts   v_flow + 0, r16
	sts   v_flow + 1, r17
	ldi   ZL,      3
	mul   r17,     ZL
	mov   r17,     r0
	mul   r16,     ZL
	mov   r16,     r0
	add   r17,     r1      ; Drift = Drift * 3
	add   r16,     r24
	adc   r17,     r25

	; Adjust Y with 128 pixels

	subi  r22,     16
	sbci  r23,     0

	; Set up for looping

	ldi   XL,      lo8(M74_VRAM_OFF)
	ldi   XH,      hi8(M74_VRAM_OFF)
	ldi   r21,     2       ; For addressing row map
	ldi   r19,     0       ; For addressing X shift map

	; Main output loop

renl0:
	sbrc  r23,     7       ; Y (tiles) below zero?
	rjmp  rentp            ; If so, then top parallax rows

	ldi   ZL,      8
	mul   r18,     ZL      ; Map height in tiles
	sub   r0,      r22
	sbc   r1,      r23
	brcs  renbp            ; If position is larger, then bottom parallax
	breq  renbp            ; Also if equal (after last valid map row)

	clr   r1
	rjmp  rennp

	; Bottom parallax rows

renbp:
	ldi   ZH,      9
	sub   ZH,      r0
	sbrs  r20,     2
	rjmp  .+8
	mul   ZH,      r25     ; Non drifting
	mov   ZL,      r0
	mul   ZH,      r24
	rjmp  .+6
	mul   ZH,      r17     ; Drifting
	mov   ZL,      r0
	mul   ZH,      r16
	add   r1,      ZL
	lsl   r0
	rol   r1
	lsl   r0
	rol   r1
	lsl   r0
	rol   r1
	mov   r0,      r1      ; r0: Pixel offset for parallaxing

	rcall renxsh           ; Set up X shift & Prepare for tiles

	sbrs  r20,     1
	rjmp  .+6
	sbrc  r20,     0
	rjmp  renbt3
	rjmp  renbt2
	sbrc  r20,     0
	rjmp  renbt1
renbt0:
	ori   ZL,      lo8(data_gnd_water)
	ldi   ZH,      hi8(data_gnd_water)
	rjmp  renl1
renbt3:
renbt1:
	ori   ZL,      lo8(data_gnd_grass)
	ldi   ZH,      hi8(data_gnd_grass)
	rjmp  renl1
renbt2:
rentt2:
	ori   ZL,      lo8(data_gnd_cave)
	ldi   ZH,      hi8(data_gnd_cave)
	rjmp  renl1


	; Top parallax rows

rentp:
	sbrc  r20,     6
	rjmp  rentr0
	ldi   ZH,      7       ; Sky, distant effect
	sub   ZH,      r22
	lsl   ZH
;	lsl   ZH
;	lsl   ZH
	mul   ZH,      r16
	mov   ZL,      r1
	mul   ZH,      r17
	add   r0,      ZL      ; r0: Pixel offset for parallaxing
	rjmp  rentre
rentr0:
	ldi   ZH,      9       ; Ceiling
	sub   ZH,      r22
	mul   ZH,      r25
	mov   ZL,      r0
	mul   ZH,      r24
	add   r1,      ZL
	lsl   r0
	rol   r1
	lsl   r0
	rol   r1
	lsl   r0
	rol   r1
	mov   r0,      r1      ; r0: Pixel offset for parallaxing
rentre:

	rcall renxsh           ; Set up X shift & Prepare for tiles

	mov   ZH,      r22     ; Prepare for multi row skies
	swap  ZH
	andi  ZH,      0x30
	sbrs  r20,     5       ; Normal content preparation
	rjmp  .+6
	sbrc  r20,     4
	rjmp  rentt3
	rjmp  rentt2
	sbrc  r20,     4
	rjmp  rentt1
rentt0:
	ori   ZL,      lo8(data_sky_clouds)
	add   ZL,      ZH
	ldi   ZH,      hi8(data_sky_clouds)
	adc   ZH,      r0
	rjmp  renl1
rentt3:
rentt1:
	ori   ZL,      lo8(data_sky_clear)
	add   ZL,      ZH
	ldi   ZH,      hi8(data_sky_clear)
	adc   ZH,      r0
renl1:
	lpm   r0,      Z
	st    X+,      r0
	subi  ZL,      0xFF
	brhs  .+2
	subi  ZL,      0x10
	dec   r1
	brne  renl1
	sbiw  XL,      25

	; No parallaxing

rennp:
	adiw  XL,      25
	subi  r22,     0xFF
	sbci  r23,     0xFF
	subi  r21,     0xFD
	inc   r19
	cpi   r19,     24
	breq  .+2
	rjmp  renl0

	; Done, return

	pop   r17
	pop   r16
	ret

	; Small internal routine to set up X shift and to do the common part
	; of preparing for tile output

renxsh:
	ldi   ZL,      0x7     ; Set up X shift
	mov   r1,      ZL
	and   r1,      r0
	mov   ZL,      r21
	clr   ZH
	subi  ZL,      lo8(-RSL_GAME)
	sbci  ZH,      hi8(-RSL_GAME)
	st    Z,       r1
	mov   ZL,      r19
	clr   ZH
	subi  ZL,      lo8(-M74_XSHIFT_OFF)
	sbci  ZH,      hi8(-M74_XSHIFT_OFF)
	st    Z,       r1

	ldi   ZL,      25      ; Prepare for tiles
	mov   r1,      ZL
	lsr   r0
	lsr   r0
	lsr   r0               ; r0: Tile source offset
	mov   ZL,      r22
	add   ZL,      r0
	andi  ZL,      0x0F
	clr   r0

	ret
