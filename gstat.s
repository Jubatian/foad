/*
 *  Dragon - Game state
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



#include <avr/io.h>
; (Needed only for register names for X, Y and Z)



.section .bss


/*
** Current game score. Should be modified using gstat_score_add() and
** gstat_score_sub().
*/
.global gstat_score
	gstat_score: .space 2


/*
** Current game time. Decrements approximately once per second. May be
** incremented by gstat_time_add().
*/
.global gstat_time
	gstat_time: .space 1



.section .text



/*
** Adds to the game score with saturation.
**
** r25:r24: Amount to increment score with
*/
.global gstat_score_add
gstat_score_add:

	lds   r22,     gstat_score + 0
	lds   r23,     gstat_score + 1
	add   r22,     r24
	adc   r23,     r25
	brcc  .+4
	ldi   r22,     0xFF
	ldi   r23,     0xFF
	sts   gstat_score + 0, r22
	sts   gstat_score + 1, r23

	ret



/*
** Subtracts from the game score with saturation.
**
** r25:r24: Amount to decrement score with
*/
.global gstat_score_sub
gstat_score_sub:

	lds   r22,     gstat_score + 0
	lds   r23,     gstat_score + 1
	sub   r22,     r24
	sbc   r23,     r25
	brcc  .+4
	ldi   r22,     0x00
	ldi   r23,     0x00
	sts   gstat_score + 0, r22
	sts   gstat_score + 1, r23

	ret



/*
** Adds to the game time with saturation.
**
**     r24: Amount to increment time with
*/
.global gstat_time_add
gstat_time_add:

	lds   r25,     gstat_time
	add   r25,     r24
	brcc  .+2
	ldi   r25,     0xFF
	sts   gstat_time, r25

	ret



/*
** Outputs current score as a five digit decimal number with leading zeros.
** "addr" is the target memory address, needs 5 bytes. "zch" is the '0'
** character, assuming numeric characters come incrementally after this.
** The score needs to be supplied in "val", so other scores may be displayed
** with it than gstat_score.
**
** Inputs:
** r25:r24: addr
** r23:r22: val
**     r20: zch
** Outputs:
**       X: The bar's total width added to r25:r24
** Clobbers:
** r25, r24, r23, r22, r21, r19
*/
.global gstat_score_disp
gstat_score_disp:

	movw  XL,      r24

.global gstat_score_disp_x
gstat_score_disp_x:

	; Convert input to BCD into r21:r25:r24

	clr   r21
	clr   r25
	clr   r24

	lsl   r22              ; Step 1 (1)
	rol   r23
	rol   r24              ; r24 <= 1

	lsl   r22              ; Step 2 (3)
	rol   r23
	rol   r24              ; r24 <= 3

	lsl   r22              ; Step 3 (7)
	rol   r23
	rol   r24              ; r24 <= 7, correction may be necessary

	ldi   r19,     3       ; Step 4 (15), 5 (31), 6 (63)
u16l0:
	cpi   r24,     0x05    ; Digit larger or equal to 5?
	brhs  .+2
	subi  r24,     0xFD    ; Add 3
	lsl   r22
	rol   r23
	rol   r24
	dec   r19
	brne  u16l0

	ldi   r19,     3       ; Step 7 (127), 8 (255), 9 (511)
u16l1:
	cpi   r24,     0x05    ; Digit larger or equal to 5?
	brhs  .+2
	subi  r24,     0xFD    ; Add 3
	cpi   r24,     0x50    ; Digit larger or equal to 5?
	brcs  .+2
	subi  r24,     0xD0    ; Add 3
	lsl   r22
	rol   r23
	rol   r24
	rol   r25
	dec   r19
	brne  u16l1

	ldi   r19,     4       ; Step 10 (1023), 11 (2047), 12 (4095), 13 (8191)
u16l2:
	cpi   r24,     0x05    ; Digit larger or equal to 5?
	brhs  .+2
	subi  r24,     0xFD    ; Add 3
	cpi   r24,     0x50    ; Digit larger or equal to 5?
	brcs  .+2
	subi  r24,     0xD0    ; Add 3
	cpi   r25,     0x05    ; Digit larger or equal to 5?
	brhs  .+2
	subi  r25,     0xFD    ; Add 3
	lsl   r22
	rol   r23
	rol   r24
	rol   r25
	dec   r19
	brne  u16l2

	ldi   r19,     3       ; Step 14 (16383), 15 (32767), 16 (65535)
u16l3:
	cpi   r24,     0x05    ; Digit larger or equal to 5?
	brhs  .+2
	subi  r24,     0xFD    ; Add 3
	cpi   r24,     0x50    ; Digit larger or equal to 5?
	brcs  .+2
	subi  r24,     0xD0    ; Add 3
	cpi   r25,     0x05    ; Digit larger or equal to 5?
	brhs  .+2
	subi  r25,     0xFD    ; Add 3
	cpi   r25,     0x50    ; Digit larger or equal to 5?
	brcs  .+2
	subi  r25,     0xD0    ; Add 3
	lsl   r22
	rol   r23
	rol   r24
	rol   r25
	rol   r21
	dec   r19
	brne  u16l3

	; Output it

	add   r21,     r20
	st    X+,      r21
	mov   r21,     r25
	swap  r21
	andi  r21,     0x0F
	add   r21,     r20
	st    X+,      r21
	andi  r25,     0x0F
	add   r25,     r20
	st    X+,      r25
	mov   r21,     r24
	swap  r21
	andi  r21,     0x0F
	add   r21,     r20
	st    X+,      r21
	andi  r24,     0x0F
	add   r24,     r20
	st    X+,      r24

	; Done

	ret
