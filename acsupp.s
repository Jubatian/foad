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


/* Count of fireballs. Note: Keep in sync with that in fireball.h! */
#define FIREBALL_N 12


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
** Ledge halt. If sprite is at ledge, halts it (xvel), preventing it walking
** or running into a chasm.
**
** Inputs:
** r25:r24: Sprite pointer
*/
.global acsupp_haltatledge
acsupp_haltatledge:

	movw  ZL,      r24
	ldd   r23,     Z + 6   ; spr->xvel
	cpi   r23,     0
	brmi  hal_0            ; xvel < 0
	breq  hal_1            ; xvel == 0 (do nothing)
	ldi   r22,     0x04    ; Test for ledge at +4
	rjmp  .+2
hal_0:
	ldi   r22,     0xFC    ; Test for ledge at -4
	push  ZL
	push  ZH
	rcall acsupp_isatledge
	pop   ZH
	pop   ZL
	ldi   r23,     0
	cpse  r24,     r23
	std   Z + 6,   r23     ; spr->xvel = 0 if at ledge
hal_1:
	ret



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
** Tests if a given coordinate pair is in the dragon. Returns 1 if so.
**
** Inputs:
** r25:r24: x
** r23:r22: y
** Outputs:
**     r24: 1 if the coordinate is in the dragon
*/
.global acsupp_iscordindragon
acsupp_iscordindragon:

	lds   r21,     dragon_spr + 4 ; dragon_spr.xbs
	lds   r20,     dragon_spr + 5 ; dragon_spr.ybs
	                              ; Fall through to acsupp_iscordneardragon



/*
** Tests if a given coordinate pair is near to the dragon. Returns 1 if so.
** dxy is the distances to test: X on high 8 bits, Y on low 8 bits.
**
** Inputs:
** r25:r24: x
** r23:r22: y
** r21:r20: dxy (r21: dx; r20: dy)
** Outputs:
**     r24: 1 if the coordinate is near the dragon
*/
.global acsupp_iscordneardragon
acsupp_iscordneardragon:

	ldi   ZL,      lo8(dragon_spr)
	ldi   ZH,      hi8(dragon_spr)
	ld    r18,     Z+      ; dragon_spr.xpos
	ld    r19,     Z+      ; dragon_spr.xpos >> 8
	ld    XL,      Z+      ; dragon_spr.ypos
	ld    XH,      Z+      ; dragon_spr.ypos >> 8

	sub   r24,     r18
	sbc   r25,     r19     ; x -= dragon_spr.xpos
	clr   r0
	add   r24,     r21
	adc   r25,     r0      ; x += dx
	lsl   r21
	rol   r0
	cp    r24,     r21
	cpc   r25,     r0      ; x < 2 * dx
	brcc  icn_0

	sub   r22,     XL
	sbc   r23,     XH      ; y -= dragon_spr.ypos
	clr   r0
	add   r22,     r20
	adc   r23,     r0      ; y += dy
	lsl   r20
	rol   r0
	cp    r22,     r20
	cpc   r23,     r0      ; y < 2 * dy
	brcc  icn_0

	ldi   r24,     1       ; Near
	ret

icn_0:
	ldi   r24,     0       ; Not near
	ret



/*
** Renders a non-player-character sprite. Such a sprite faces the dragon when
** not moving, and may be on fire. This can be used for typical 2x1 tile human
** characters. The fra parameter is the frame to draw, fir is whether the
** sprite should be on fire (bit 7 set; then bits 0-4 are the frame).
**
** Inputs:
** r25:r24: actor
**     r22: fra
**     r20: fir
*/
.global acsupp_rendernpc
acsupp_rendernpc:

	push  r20              ; Put away fire, will only be needed later

	ldi   r20,     0x50    ; M74_SPR_I1 | M74_SPR_MASK
	movw  ZL,      r24
	ldd   r23,     Z + 6   ; actor->spr.xvel
	cpi   r23,     0
	breq  rnp_0            ; actor->spr.xvel == 0
	brmi  rnp_1            ; actor->spr.xvel < 0
	ori   r20,     0x01    ; M74_SPR_FLIPX

rnp_0:

	lds   r23,     dragon_spr + 0 ; dragon_spr.xpos
	ldd   r21,     Z + 0   ; actor->spr.xpos
	cp    r23,     r21
	lds   r23,     dragon_spr + 1 ; dragon_spr.xpos >> 8
	ldd   r21,     Z + 1   ; actor->spr.xpos >> 8
	cpc   r23,     r21
	brcs  .+2              ; dragon_spr.xpos < actor->spr.xpos
	ori   r20,     0x01    ; M74_SPR_FLIPX

rnp_1:

	ldi   r18,     REC_FLAT_0
	push  ZL
	push  ZH
	call  spritelv_blit
	pop   ZH
	pop   ZL

	pop   r20
	sbrs  r20,     7
	ret                    ; No fire

	ld    r24,     Z+
	ld    r25,     Z+      ; actor->spr.xpos
	ld    r22,     Z+
	ld    r23,     Z+      ; actor->spr.ypos
	subi  r22,     0x08
	sbci  r23,     0x00    ; actor->spr.ypos - 8
	com   r20
	andi  r20,     0x1F    ; (0xFF - fir) & 0x1F
	jmp   fireball_render_fire



/*
** Interacts an actor with a fireball and returns, assuming the most common
** scenario for actor layout:
** d0 is laid out as follows:
** bit   7: On fire marker, cleared on frame 0x1F (~1sec burn).
** bit   6: Not touched.
** bit 0-5: Frame counter which is incremented.
** Writes back d0 and d1 into the actor and returns 0 if health is depleted
** (d1 = 0xFF) and 1 if not (d1 not 0xFF).
** d1d0: d1 is high, d0 is low
** shp: Kill penalty score (high) & Health decrement for actor (low)
** d0m: Masks for d0 (high: OR, low: AND) when fireball hits
**
** Inputs:
** r25:r24: actor
** r23:r22: d1d0 (r23: d1; r22: d0)
** r21:r20: shp (r21: sub; r20: hpp)
** r19:r18: d0m (r19: d0or; r18: d0and)
** Outputs:
**     r24: 1 if d1 was not 0xFF, 0 otherwise
*/
.global acsupp_procfin_fire
acsupp_procfin_fire:

	; Calculate d0 for next frame; increment low 6 bits (frame counter)

	bst   r22,     6
	andi  r22,     0xBF    ; Clear bit 6 so a carry-over may only set it
	inc   r22
	bld   r22,     6

	; Clear on fire marker if low 5 bits are zero

	mov   ZL,      r22
	andi  ZL,      0x1F
	brne  .+2
	andi  r22,     0x7F

	; Fireball interaction

	push  r25
	push  r24
	push  YH
	push  YL
	push  r17
	push  r16
	push  r15
	push  r14
	movw  r14,     r18     ; d0or:d0and
	movw  YL,      r20     ; sub:hpp
	movw  r16,     r22     ; d1:d0
	call  fireball_getat
	cpi   r24,     FIREBALL_N
	breq  pff_0

	; There is a fireball hitting the actor

	ldi   r22,     0x20    ; Aging of the fireball
	call  fireball_age
	add   r17,     YH      ; d1 += sub
	brcc  .+2
	ldi   r17,     0xFF
	cpi   r17,     0xFF
	brne  .+6
	mov   r24,     YL      ; hpp
	call  gstat_score_sub
	or    r16,     r15
	and   r16,     r14

pff_0:

	; Interaction done, restore stuff

	mov   r22,     r16     ; d0 for acsupp_procfin
	mov   r20,     r17     ; d1 for acsupp_procfin
	pop   r14
	pop   r15
	pop   r16
	pop   r17
	pop   YL
	pop   YH
	pop   r24              ; actor for acsupp_procfin
	pop   r25              ; Fall through to acsupp_procfin



/*
** Simple return depending on the value of d1 (0xFF: 0; others: 1), writes
** back d0 and d1.
**
** Inputs:
** r25:r24: actor
**     r22: d0
**     r20: d1
** Outputs:
**     r24: 1 if d1 was not 0xFF, 0 otherwise
*/
.global acsupp_procfin
acsupp_procfin:

	movw  ZL,      r24
	std   Z + 12,  r22     ; actor->d0 = d0
	std   Z + 13,  r20     ; actor->d1 = d1
	cpi   r20,     0xFF    ; C clear if 0xFF
	sbc   r24,     r24     ; r24 = 0 if 0xFF, r24 = 0xFF otherwise
	andi  r24,     1
	ret
