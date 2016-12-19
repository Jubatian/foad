/*
 *  Dragon - Actor support functions
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



.section .text



/*
** Line of sight test between two sprites (typically the actor and the
** dragon). This is a crude test returning whether the line of sight between
** the two passed sprites is unobstructed.
**
** Inputs:
** r25:r24: Sprite 1 pointer
** r23:r22: Sprite 2 pointer
** Outputs:
** r25:r24: 1 if line of sight is unobstructed
*/
.global acsupp_isvisible
acsupp_isvisible:

	movw  XL,      r24
	ld    r18,     X+
	ld    r19,     X+      ; Sprite 1 posx
	ld    r20,     X+
	ld    r21,     X+      ; Sprite 1 posy
	movw  XL,      r22
	ld    r22,     X+
	ld    r23,     X+      ; Sprite 2 posx
	ld    r24,     X+
	ld    r25,     X+      ; Sprite 2 posy

	sub   r22,     r18
	sbc   r23,     r19     ; Vector X from Sprite 1 to Sprite 2
	sub   r24,     r20
	sbc   r25,     r21     ; Vector Y from Sprite 1 to Sprite 2

	movw  XL,      r22
	subi  XL,      0x80
	sbci  XH,      0x00
	cpi   XH,      0xFF
	brne  isv_e0           ; Vector X wasn't between 0xFF80 and 0x007F (128 pixels range)

	movw  ZL,      r24
	subi  ZL,      0x80
	sbci  ZH,      0x00
	cpi   ZH,      0xFF
	brne  isv_e0           ; Vector Y wasn't between 0xFF80 and 0x007F (128 pixels range)

	subi  XL,      0x70
	brcs  isv_t
	subi  XL,      0x20
	brcc  isv_t            ; Between 0xFFF0 and 0x000F: Always visible (if Y is so too)
	subi  ZL,      0x70
	brcs  isv_t
	subi  ZL,      0x20
	brcs  isv_e1           ; Between 0xFFF0 and 0x000F: Always visible

isv_t:

	; Neither too far, neither too near. Line of sight is determined by
	; sampling 3 points between the two sprites for passability. Crude,
	; but doesn't take much CPU time.

	asr   r25
	ror   r24
	asr   r25
	ror   r24
	asr   r23
	ror   r22
	asr   r23
	ror   r22

	subi  r20,     0x0C
	sbci  r21,     0x00    ; Apply 12 pixels above sprite bottom. Sort of "eye level"

	push  r2
	push  r3
	push  r4
	push  r5
	push  r6
	push  r7
	push  r8
	push  r9
	push  r17

	movw  r2,      r18
	movw  r4,      r20
	movw  r6,      r22
	movw  r8,      r24

	ldi   r17,     3
isv_l:
	add   r2,      r6
	adc   r3,      r7
	add   r4,      r8
	adc   r5,      r9
	movw  r24,     r2
	movw  r22,     r4
	call  passable         ; r25:r24 return is the same as needed here
	cpi   r24,     0
	breq  .+4              ; Impassable: No visibility
	dec   r17
	brne  isv_l

	pop   r17
	pop   r9
	pop   r8
	pop   r7
	pop   r6
	pop   r5
	pop   r4
	pop   r3
	pop   r2
	ret

isv_e0:
	clr   r24
	clr   r25
	ret
isv_e1:
	ldi   r24,     0x01
	clr   r25
	ret



/*
** Ledge test. Returns whether there is a ledge in front of the sprite at the
** given distance (negative for left, positive for right) which may be used
** to prevent it walking into a chasm.
**
** Inputs:
** r25:r24: Sprite pointer
**     r22: Distance to check
** Outputs:
** r25:r24: 1 if there is a ledge there
*/
.global acsupp_isatledge
acsupp_isatledge:

	movw  XL,      r24
	mov   r20,     r22
	ld    r24,     X+
	ld    r25,     X+      ; Sprite posx
	ld    r22,     X+
	ld    r23,     X+      ; Sprite posy

	clr   r21
	sbrc  r20,     7
	ldi   r21,     0xFF    ; Sign extend

	add   r24,     r20
	adc   r25,     r21     ; Add distance to posx

	sbrs  r20,     7
	neg   r20              ; Take inverted absolute

	sub   r22,     r20
	sbci  r23,     0xFF    ; Do an inverted add to posy with it (no reliable zero around)

	jmp   passable         ; Finish with a passability check at 45 deg angle down forth



/*
** Returns the suggested Y velocity for a projectile to get it travelling from
** a location to a sprite with the given X velocity. Returns 0x7F if it can't
** be done (such as if the X velocity points the wrong way).
**
** Inputs:
** r25:r24: X start position
** r23:r22: Y start position
** r21:r20: Sprite pointer
**     r18: Signed X velocity
** Outputs:
** r25:r24: Signed Y velocity (r25 is only sign extension)
*/
.global acsupp_getshotvy
acsupp_getshotvy:

	mov   r0,      r18
	clr   r1

	movw  XL,      r20
	movw  r18,     r24     ; X start position
	movw  r20,     r22     ; Y start position
	ld    r22,     X+
	ld    r23,     X+      ; Sprite posx
	ld    r24,     X+
	ld    r25,     X+      ; Sprite posy

	sub   r22,     r18
	sbc   r23,     r19     ; Vector X from Sprite 1 to Sprite 2
	sub   r24,     r20
	sbc   r25,     r21     ; Vector Y from Sprite 1 to Sprite 2

	cp    r0,      r1
	breq  gsh_e0           ; X velocity is zero

	eor   r0,      r23
	brmi  gsh_e0           ; X velocity and direction sign mismatch
	eor   r0,      r23

	ldi   XL,      0xFF    ; For negative saturations (r1 = 0 used for positives)
	sbrc  r23,     7
	rjmp  gsh_xn
	sbrs  r22,     7
	cpse  r23,     r1
	ldi   r22,     0x7F    ; X positive saturate to 0x7F ( 127) into r22
	rjmp  gsh_xe
gsh_xn:
	sbrc  r22,     7
	cpse  r23,     XL
	ldi   r22,     0x80    ; X negative saturate to 0x80 (-128) into r22
	neg   r0
	neg   r22              ; Get absolute values
gsh_xe:
	sbrc  r25,     7
	rjmp  gsh_yn
	sbrs  r24,     7
	cpse  r25,     r1
	ldi   r24,     0x7F    ; Y positive saturate to 0x7F ( 127) into r24
	rjmp  gsh_ye
gsh_yn:
	sbrc  r24,     7
	cpse  r25,     XL
	ldi   r24,     0x80    ; Y negative saturate to 0x80 (-128) into r24
gsh_ye:

	clr   r19
gsh_a0:
	inc   r19              ; Crude division to get ceil(Vector X / Velocity X) into r19
	sub   r22,     r0
	brpl  gsh_a0

	mov   r25,     r24
	sbrc  r25,     7
	neg   r24              ; Vector Y absolute
	ldi   r21,     0xFF
gsh_b0:
	inc   r21              ; Crude division to get floor(Vector Y / r25)
	sub   r24,     r19
	brpl  gsh_b0

	sbrc  r25,     7
	neg   r19              ; Apply Vector Y sign on gravity correction
	subi  r19,     1
	adc   r19,     r1      ; r1 is zero; subtracted 1 with saturation to zero
	asr   r19
	asr   r19
	sub   r21,     r19     ; Correct for gravity

	cp    r0,      r21
	brcc  .+4
	sbrs  r21,     7       ; Turned negative by gravity correction - skip
	mov   r21,     r0      ; Maximal Y velocity absolute equals max. X absolute
	sbrc  r25,     7
	neg   r21              ; Restore sign for Y velocity

	mov   r24,     r21
	clr   r25
	sbrc  r24,     7
	ldi   r25,     0xFF
	ret

gsh_e0:
	clr   r25
	ldi   r24,     0x7F
	ret



/*
** Simple coordinate nearness test. A pair of coordinates are nearby to each
** if one is within 8 pixels distance to the other on X, and within 16 pixels
** on Y (in accordance with typical sprite heights).
**
** Inputs:
** r25:r24: x1
** r23:r22: y1
** r21:r20: x2
** r19:r18: y2
** Outputs:
** r25:r24: 1 if the coordinates are near to each other
*/
.global acsupp_iscordnear
acsupp_iscordnear:

	sub   r24,     r20
	sbc   r25,     r21
	subi  r24,     0xF8
	sbci  r25,     0xFF    ; Adds 8, so 0 - 16 range can be tested
	subi  r24,     0x10
	sbci  r25,     0x00
	brcc  icn_0

	sub   r22,     r18
	sbc   r23,     r19
	subi  r22,     0xF0
	sbci  r23,     0xFF    ; Adds 16, so 0 - 32 range can be tested
	subi  r22,     0x20
	sbci  r23,     0x00
	brcc  icn_0

	ldi   r24,     1       ; Near
	ldi   r25,     0
	ret

icn_0:
	ldi   r24,     0       ; Not near
	ldi   r25,     0
	ret
