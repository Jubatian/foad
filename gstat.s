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
** r23, r22, r21
*/
.global gstat_score_disp
gstat_score_disp:

	movw  XL,      r24

.global gstat_score_disp_x
gstat_score_disp_x:

	dec   r20

	; Digit 4 (10000s)

	mov   r21,     r20
	inc   r21
	subi  r22,     lo8(10000)
	sbci  r23,     hi8(10000)
	brcc  .-8
	subi  r22,     lo8(-10000)
	sbci  r23,     hi8(-10000)
	st    X+,      r21

	; Digit 3 (1000s)

	mov   r21,     r20
	inc   r21
	subi  r22,     lo8(1000)
	sbci  r23,     hi8(1000)
	brcc  .-8
	subi  r22,     lo8(-1000)
	sbci  r23,     hi8(-1000)
	st    X+,      r21

	; Digit 2 (100s)

	mov   r21,     r20
	inc   r21
	subi  r22,     lo8(100)
	sbci  r23,     hi8(100)
	brcc  .-8
	subi  r22,     lo8(-100)
	sbci  r23,     hi8(-100)
	st    X+,      r21

	; Digit 1 (10s)

	mov   r21,     r20
	inc   r21
	subi  r22,     lo8(10)
	brcc  .-6
	subi  r22,     lo8(-10)
	st    X+,      r21

	; Digit 0 (1s)

	inc   r20
	add   r22,     r20
	st    X+,      r22

	; Done

	ret
