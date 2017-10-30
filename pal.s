/*
 *  Dragon - Palette render
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



/* Sky coloration variations (14 bytes each) */
sky_data:

	.byte  (0x2 << 0) + (0x3 << 3) + (0x3 << 6) /* Normal daylight blue */
	.byte  (0x2 << 0) + (0x4 << 3) + (0x3 << 6)
	.byte  (0x3 << 0) + (0x4 << 3) + (0x3 << 6)
	.byte  (0x3 << 0) + (0x5 << 3) + (0x3 << 6)
	.byte  (0x4 << 0) + (0x5 << 3) + (0x3 << 6)
	.byte  (0x4 << 0) + (0x6 << 3) + (0x3 << 6)
	.byte  (0x5 << 0) + (0x6 << 3) + (0x3 << 6)
	.byte  (0x5 << 0) + (0x7 << 3) + (0x3 << 6)
	.byte  (0x6 << 0) + (0x7 << 3) + (0x3 << 6)
	.byte  (0x6 << 0) + (0x7 << 3) + (0x2 << 6)
	.byte  (0x6 << 0) + (0x6 << 3) + (0x2 << 6)
	.byte  (0x5 << 0) + (0x6 << 3) + (0x1 << 6)
	.byte  (0x5 << 0) + (0x5 << 3) + (0x1 << 6)
	.byte  (0x4 << 0) + (0x5 << 3) + (0x1 << 6)

	.byte  (0x3 << 0) + (0x2 << 3) + (0x2 << 6) /* Dawn / Dusk */
	.byte  (0x4 << 0) + (0x2 << 3) + (0x2 << 6)
	.byte  (0x4 << 0) + (0x3 << 3) + (0x2 << 6)
	.byte  (0x5 << 0) + (0x3 << 3) + (0x2 << 6)
	.byte  (0x5 << 0) + (0x4 << 3) + (0x2 << 6)
	.byte  (0x6 << 0) + (0x4 << 3) + (0x2 << 6)
	.byte  (0x6 << 0) + (0x5 << 3) + (0x2 << 6)
	.byte  (0x7 << 0) + (0x5 << 3) + (0x2 << 6)
	.byte  (0x7 << 0) + (0x6 << 3) + (0x2 << 6)
	.byte  (0x6 << 0) + (0x4 << 3) + (0x2 << 6)
	.byte  (0x5 << 0) + (0x3 << 3) + (0x1 << 6)
	.byte  (0x4 << 0) + (0x2 << 3) + (0x1 << 6)
	.byte  (0x3 << 0) + (0x1 << 3) + (0x0 << 6)
	.byte  (0x2 << 0) + (0x0 << 3) + (0x0 << 6)




/*
** Renders the sky. The parameter is the sky type to render.
**
** Inputs:
**     r24: sky (type of sky to render)
*/
.global pal_sky_render
pal_sky_render:

	ldi   r20,     0x00
	; Simply fall through with zero for color fading ratio



/*
** Renders the sky with fading. The "sky" parameter is the sky type to render.
** "tg" is the target color to fade towards, "rat" is the ratio; 0: no fading,
** 255: maximal fading.
**
** Inputs:
**     r24: sky (type of sky to render)
**     r22: target color for fading
**     r20: fade ratio
*/
.global pal_sky_render_fade
pal_sky_render_fade:

	lds   r25,     levelscr_y + 0
	andi  r25,     0x07    ; Low 7 bits of map Y location
	ldi   XH,      hi8(M74_COL0_OFF)
	mov   XL,      r25     ; Start position for 185 rows
	subi  XL,      0xD0    ; Add 48 for first row (rows 6 - 29 are the game field)

	ldi   ZH,      hi8(sky_data)
	ldi   ZL,      lo8(sky_data)
	ldi   r25,     14
	mul   r25,     r24     ; Select sky type
	add   ZL,      r0
	adc   ZH,      r1

	ldi   r24,     13
	andi  r20,     0xE0    ; Only 8 levels of fading
	brne  skfade           ; Nonzero fade ratio: do fading
	lpm   r1,      Z+

skl0:
	mov   r0,      r1
	lpm   r1,      Z+
	st    X+,      r0
	st    X+,      r0
	st    X+,      r0
	st    X+,      r0
	st    X+,      r1
	st    X+,      r0
	st    X+,      r1
	st    X+,      r0
	st    X+,      r1
	st    X+,      r0
	st    X+,      r1
	st    X+,      r0
	st    X+,      r1
	st    X+,      r1
	dec   r24
	brne  skl0

	st    X+,      r1      ; Line 182
	st    X+,      r1      ; Line 183
	st    X+,      r1      ; Line 184

	clr   r1
	ret

	; With fading

skfade:
	; Calculate target color

	mov   r21,     r22
	andi  r22,     0xC7    ; Blue and Red of target color
	andi  r21,     0x38    ; Green of target color
	mul   r22,     r20
	movw  r22,     r0      ; Result Blue and Red of target color
	mul   r21,     r20
	mov   r21,     r1      ; Result Green of target color

	; Prepare multiplier and first color

	neg   r20              ; Multiplier negated for sky colors
	lpm   r18,     Z+
	mov   r19,     r18
	andi  r18,     0xC7    ; Blue and Red of source color
	andi  r19,     0x38    ; Green of source color
	mul   r18,     r20
	add   r0,      r22
	adc   r1,      r23     ; Combined Blue and Red
	mov   r18,     r1
	andi  r18,     0xC7    ; Masked out Blue and Red
	mul   r19,     r20
	add   r1,      r21     ; Combined Green
	mov   r19,     r1
	andi  r19,     0x38    ; Masked out Green
	add   r18,     r19     ; Combine source & target

	; Output loop

skl1:
	mov   r25,     r18
	lpm   r18,     Z+
	mov   r19,     r18
	andi  r18,     0xC7    ; Blue and Red of source color
	andi  r19,     0x38    ; Green of source color
	mul   r18,     r20
	add   r0,      r22
	adc   r1,      r23     ; Combined Blue and Red
	mov   r18,     r1
	andi  r18,     0xC7    ; Masked out Blue and Red
	mul   r19,     r20
	add   r1,      r21     ; Combined Green
	mov   r19,     r1
	andi  r19,     0x38    ; Masked out Green
	add   r18,     r19     ; Combine source & target
	st    X+,      r25
	st    X+,      r25
	st    X+,      r25
	st    X+,      r25
	st    X+,      r18
	st    X+,      r25
	st    X+,      r18
	st    X+,      r25
	st    X+,      r18
	st    X+,      r25
	st    X+,      r18
	st    X+,      r25
	st    X+,      r18
	st    X+,      r18
	dec   r24
	brne  skl1

	st    X+,      r18     ; Line 182
	st    X+,      r18     ; Line 183
	st    X+,      r18     ; Line 184

	clr   r1
	ret



/*
** Renders the top palette.
**
** Inputs:
**     r24: top palette type (0: Black & White, 1: Yellow & Red)
*/
.global pal_top_render
pal_top_render:

	ldi   r20,     0x00
	; Simply fall through with zero for color fading ratio



/*
** Renders the top palette with fading. "tg" is the target color to fade
** towards, "rat" is the ratio; 0: no fading, 255: maximal fading.
**
** Inputs:
**     r24: top palette type (0: Black & White, 1: Yellow & Red)
**     r22: target color for fading
**     r20: fade ratio
*/
.global pal_top_render_fade
pal_top_render_fade:

	ldi   XH,      hi8(LOC_TOPPAL_OFF + 1)
	ldi   XL,      lo8(LOC_TOPPAL_OFF + 1)

	andi  r20,     0xE0    ; Only 8 levels of fading
	brne  ptfade           ; Nonzero fade ratio: do fading

	ldi   r22,     0xAD    ; Y&R Bg. color (10 101 101)
	sbrs  r24,     0
	ldi   r22,     0x00    ; B&W Bg. color (00 000 000)
	st    X+,      r22
	ldi   r22,     0x0B    ; Y&R Fg. color (00 001 011)
	sbrs  r24,     0
	ldi   r22,     0xFF    ; B&W Fg. color (11 111 111)
	st    X+,      r22
	ret

ptfade:

	; Calculate target color

	mov   r25,     r22
	andi  r22,     0xC7    ; Blue and Red of target color
	andi  r25,     0x38    ; Green of target color
	mul   r22,     r20
	movw  r22,     r0      ; Result Blue and Red of target color
	mul   r25,     r20
	mov   r25,     r1      ; Result Green of target color

	; Prepare multiplier

	neg   r20              ; Multiplier negated

	; Bg. color

	ldi   r18,     0x85    ; Y&R Blue and Red of color
	sbrs  r24,     0
	ldi   r18,     0x00    ; B&W Blue and Red of color
	mul   r18,     r20
	add   r0,      r22
	adc   r1,      r23     ; Combined Blue and Red
	mov   r18,     r1
	andi  r18,     0xC7    ; Masked out Blue and Red
	ldi   r19,     0x28    ; Y&R Green of color
	sbrs  r24,     0
	ldi   r19,     0x00    ; B&W Green of color
	mul   r19,     r20
	add   r1,      r25     ; Combined Green
	mov   r19,     r1
	andi  r19,     0x38    ; Masked out Green
	add   r18,     r19     ; Combine source & target
	st    X+,      r18

	; Fg. color

	ldi   r18,     0x03    ; Y&R Blue and Red of color
	sbrs  r24,     0
	ldi   r18,     0xC7    ; B&W Blue and Red of color
	mul   r18,     r20
	add   r22,     r0
	adc   r23,     r1      ; Combined Blue and Red
	andi  r23,     0xC7    ; Masked out Blue and Red
	ldi   r19,     0x08    ; Y&R Green of color
	sbrs  r24,     0
	ldi   r19,     0x38    ; B&W Green of color
	mul   r19,     r20
	add   r25,     r1      ; Combined Green
	andi  r25,     0x38    ; Masked out Green
	add   r25,     r23     ; Combine source & target
	st    X+,      r25

	clr   r1
	ret



/*
** Renders the playfield palette.
*/
.global pal_pf_render
pal_pf_render:

	ldi   r22,     0x00
	; Simply fall through with zero for color fading ratio



/*
** Renders the playfield palette with fading. "tg" is the target color to fade
** towards, "rat" is the ratio; 0: no fading, 255: maximal fading.
**
** Inputs:
**     r24: target color for fading
**     r22: fade ratio
*/
.global pal_pf_render_fade
pal_pf_render_fade:

	ldi   XH,      hi8(LOC_MAINPAL_OFF + 1)
	ldi   XL,      lo8(LOC_MAINPAL_OFF + 1)
	ldi   ZH,      hi8(RES_PAL_00_OFF + 1)
	ldi   ZL,      lo8(RES_PAL_00_OFF + 1)

	ldi   r18,     15      ; Count of colors to produce
	andi  r22,     0xE0    ; Only 8 levels of fading
	brne  pffade           ; Nonzero fade ratio: do fading

pfl0:
	lpm   r0,      Z+
	st    X+,      r0
	dec   r18
	brne  pfl0
	ret

pffade:

	; Calculate target color

	mov   r23,     r24
	andi  r24,     0xC7    ; Blue and Red of target color
	andi  r23,     0x38    ; Green of target color
	mul   r24,     r22
	movw  r24,     r0      ; Result Blue and Red of target color
	mul   r23,     r22
	mov   r23,     r1      ; Result Green of target color

	; Prepare multiplier

	neg   r22              ; Multiplier negated

	; Color generation loop

pfl1:
	lpm   r20,     Z+
	mov   r21,     r20
	andi  r20,     0xC7    ; Blue and Red of color
	mul   r20,     r22
	add   r0,      r24
	adc   r1,      r25     ; Combined Blue and Red
	mov   r20,     r1
	andi  r20,     0xC7    ; Masked out Blue and Red
	andi  r21,     0x38    ; Green of color
	mul   r21,     r22
	add   r1,      r23     ; Combined Green
	mov   r21,     r1
	andi  r21,     0x38    ; Masked out Green
	add   r20,     r21     ; Combine source & target
	st    X+,      r20
	dec   r18
	brne  pfl1

	clr   r1
	ret
