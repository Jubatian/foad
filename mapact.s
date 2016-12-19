/*
 *  Dragon - Map actors (enemies and objects), backend
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
; (Needed only for register names X, Y and Z)



.section .bss

	v_store:       .space 108 ; Active actor storage (12 x 9 bytes)
	v_avail:       .space 32  ; Availibility bitmap (256 bits)
.equ	v_drx,         MEM_LOC_0  ; Dragon X location + 192 (low, high) (Right of 384 x 384 active region)
.equ	v_dry,         MEM_LOC_2  ; Dragon Y location + 192 (low, high) (Bottom of 384 x 384 active region)

.section .text



/*
** Initializes the storage with all active actors cleared (free) and all
** non-destroyed. The dragon sprite is used to set up initial dragon location.
*/
.global mapact_init
mapact_init:

	; Store dragon location

	ldi   ZL,      lo8(dragon_spr)
	ldi   ZH,      hi8(dragon_spr)
	ldi   XL,      lo8(v_drx)
	ldi   XH,      hi8(v_drx)
	ld    r24,     Z+      ; Dragon X low
	ld    r25,     Z+      ; Dragon X high
	subi  r24,     0x40
	sbci  r25,     0xFF    ; Add 192
	st    X+,      r24
	st    X+,      r25
	ld    r24,     Z+      ; Dragon Y low
	ld    r25,     Z+      ; Dragon Y high
	subi  r24,     0x40
	sbci  r25,     0xFF    ; Add 192
	st    X+,      r24
	st    X+,      r25

	; Clear active actor storage

	ldi   XL,      lo8(v_store)
	ldi   XH,      hi8(v_store)
	clr   r25
	ldi   r24,     12
inicl0:
	st    X,       r25
	adiw  XL,      9
	dec   r24
	brne  inicl0

	; Clear availability map (to all available)

	ldi   XL,      lo8(v_avail)
	ldi   XH,      hi8(v_avail)
	ldi   r25,     0xFF
	ldi   r24,     32
inicl1:
	st    X+,      r25
	dec   r24
	brne  inicl1

	ret



/*
** Polls a ROM source for actors which should be activated, and activates them
** as necessary or possible. The ROM source is specified by its start address,
** the type of actors it should provide and the ID of the first actor within.
** Activated actors start with their User Data being zero. Returns the number
** of actors within the source added to the passed ID.
**
** Inputs:
**     r24: id
**     r22: typ
** r21:r20: src
** Outputs:
** r25:r24: id + no_of_actors (r25 zero)
*/
.global mapact_pollsrc
mapact_pollsrc:

	push  r2
	push  r3
	push  r4
	push  r5
	lds   r2,      v_drx + 0
	lds   r3,      v_drx + 1
	lds   r4,      v_dry + 0
	lds   r5,      v_dry + 1

	movw  ZL,      r20
	ori   r22,     0x80    ; Add occupied flag
	ldi   r18,     0x0C    ; Initial absolute X
	ldi   r19,     0
	ldi   r20,     0x07    ; Initial absolute Y
	ldi   r21,     0
	clr   r1               ; Make sure this is zero (although required by C)
	mov   r23,     r24
	rjmp  psrl0e

psrl0:
	inc   r23              ; Next ID
psrl0e:

	; Load next actor (or exit at end of list)

	lpm   r25,     Z+
	sbrs  r25,     7
	rjmp  psra             ; To absolute position input

	mov   r24,     r25
	andi  r24,     0x70    ; Y position
	add   r20,     r24
	adc   r21,     r1
	subi  r20,     0x40
	sbci  r21,     0       ; Absolute Y acquired

	andi  r25,     0x0F    ; X position
	swap  r25
	add   r18,     r25
	adc   r19,     r1
	subi  r18,     0x80
	sbci  r19,     0       ; Absolute X acquired

psrac:

	; Activity check

	movw  XL,      r2
	sub   XL,      r18
	sbc   XH,      r19     ; Subtract from right edge: should be between 0 - 383 for visibility.
	subi  XL,      0x80
	sbci  XH,      0x01    ; Subtract 384 to check if within the 0 - 383 visibility box
	brcc  psrl0            ; Not in visibility box
	movw  r24,     r4
	sub   r24,     r20
	sbc   r25,     r21     ; Subtract from bottom edge: should be between 0 - 383 for visibility.
	subi  r24,     0x80
	sbci  r25,     0x01    ; Subtract 384 to check if within the 0 - 383 visibility box
	brcc  psrl0            ; Not in visibility box

	; Too near check: a 256 x 256 box within the 384 x 384 visibility box
	; where new actors shouldn't be created (they would pop out of the
	; void).

	subi  XL,      0xC0
	sez                    ; (sbci only clears Z to incidate Z for the whole result)
	sbci  XH,      0xFE    ; Add 320, XH became zero if within the 256 px inner box
	brne  .+8
	subi  r24,     0xC0
	sez                    ; (sbci only clears Z to incidate Z for the whole result)
	sbci  r25,     0xFE    ; Add 320, r25 became zero if within the 256 px inner box
	breq  psrl0            ; If both X and Y is within, then within the 256 x 256 clearance box

	; Availability check

	ldi   r25,     0x80    ; For selecting the bit
	mov   XL,      r23     ; For selecting the byte
	lsr   XL
	brcc  .+2
	lsr   r25
	lsr   XL
	brcc  .+4
	lsr   r25
	lsr   r25
	lsr   XL
	brcc  .+2
	swap  r25
	clr   XH
	subi  XL,      lo8(-(v_avail))
	sbci  XH,      hi8(-(v_avail))
	ld    r24,     X
	and   r25,     r24
	breq  psrl0            ; Actor is destroyed, so skip

	; ID check: If already active, do nothing

	ldi   r24,     13
	ldi   XL,      lo8(v_store - 7)
	ldi   XH,      hi8(v_store - 7)
psrl2:
	adiw  XL,      7
	dec   r24
	breq  psrl2x
	ld    r0,      X+
	ld    r25,     X+
	cpse  r25,     r23
	rjmp  psrl2            ; ID not equal: skip
	sbrs  r0,      7
	rjmp  psrl2            ; Free slot: skip
	rjmp  psrl0            ; ID equal: don't add again
psrl2x:

	; Actor should be active.
	; r19:r18: X absolute position
	; r21:r20: Y absolute position
	; r23: ID
	; r22: Type & Priority (occupation flag added)
	; Velocities, User data starts out zero.

	; Check the RAM storage for the followings:
	; r24: Free slot search
	; r25: A low priority slot (if free slot search fails)

	ldi   r24,     12
	ldi   r25,     0
	ldi   XL,      lo8(v_store)
	ldi   XH,      hi8(v_store)
psrl1:
	ld    r0,      X
	sbrs  r0,      7
	rjmp  psrl1f           ; Free slot: search completed
	sbrs  r0,      6
	mov   r25,     r24     ; Low pri. slot found (r25: How far back it is from end + 1)
	adiw  XL,      9
	dec   r24
	brne  psrl1
	cpi   r25,     0
	breq  psre2            ; No free slot, no low pri. slot: Totally occupied, exit
	sbrs  r22,     6
	rjmp  psre2            ; Input is low priority: No further allocations will happen, exit
	mov   r24,     9
	mul   r25,     r24
	sub   XL,      r0
	sbc   XH,      r1      ; Step back onto low priority slot to occupy it
psrl1f:
	; Free (or taken over Low Pri.) slot available, populate it

	st    X+,      r22     ; Occupation, Type & Priority
	st    X+,      r23     ; ID
	push  r23
	push  r22
	movw  r0,      r20
	movw  r24,     XL
	movw  r22,     r18
	call  levelcor_encode
	movw  r20,     r0
	pop   r22
	pop   r23
	clr   r1
	st    X+,      r1      ; X velocity
	st    X+,      r1      ; Y velocity
	st    X+,      r1      ; User data 0
	st    X+,      r1      ; User data 1

	; Completed, go on with next data

	rjmp  psrl0

psra:

	; Absolute position input (see above at Load next actor)

	cpi   r25,     0x7F
	breq  psre1            ; End of list
	mov   r21,     r25     ; Absolute position input
	lpm   r19,     Z+
	lpm   r18,     Z+
	mov   r20,     r18
	andi  r20,     0xF0    ; Y low part
	ori   r20,     0x07
	andi  r18,     0x0F    ; X low part
	swap  r18
	ori   r18,     0x0C
	rjmp  psrac            ; Go on to activity & availability checks

psre2:

	; Skim over the remaining objects to count them

psrsk:
	inc   r23              ; Next ID
	lpm   r25,     Z+
	sbrc  r25,     7
	rjmp  psrsk            ; Was relative position
	cpi   r25,     0x7F
	breq  psre1            ; End of list
	lpm   r25,     Z+
	lpm   r25,     Z+
	rjmp  psrsk            ; Was absolute position

psre1:
	clr   r25
	mov   r24,     r23
	pop   r5
	pop   r4
	pop   r3
	pop   r2
	ret



/*
** Fills up actor structure from an actor (from active actor storage). Returns
** nonzero if the actor does not exist (free slot). Fields in the spr member
** not used by the actor structure are left unchanged.
**
** Inputs:
**     r24: no
** r23:r22: actor
**
** Outputs:
** r25:r24: 1 if actor did not exist, 0 otherwise.
*/
.global mapact_get
mapact_get:

	cpi   r24,     12
	brcc  getne            ; Invalid actor number
	ldi   r25,     9
	mul   r24,     r25
	movw  XL,      r0
	subi  XL,      lo8(-(v_store))
	sbci  XH,      hi8(-(v_store))
	clr   r1               ; (For proper C interfacing)
	ld    r25,     X+
	sbrc  r25,     7
	rjmp  getex            ; The actor exists
getne:
	ldi   r24,     1
	ldi   r25,     0
	ret
getex:
	; Existing actor: populate the actor structure

	andi  r25,     0x7F    ; Mask off type & priority
	ld    r24,     X+      ; ID
	movw  r18,     r24
	movw  r24,     XL
	movw  ZL,      r22     ; Actor structure
	adiw  ZL,      2
	movw  r20,     ZL
	adiw  ZL,      4       ; Positioned at X velocity
	movw  r0,      ZL
	call  levelcor_decode
	movw  ZL,      r0
	clr   r1
	ld    r23,     X+      ; X velocity
	st    Z+,      r23
	ld    r23,     X+      ; Y velocity
	st    Z+,      r23
	adiw  ZL,      3       ; Skip xacc, yacc and flg members
	st    Z+,      r19     ; Type
	ld    r23,     X+      ; User data 0
	st    Z+,      r23
	ld    r23,     X+      ; User data 1
	st    Z+,      r23
	st    Z+,      r18     ; ID

	; Return existing

	clr   r24
	clr   r25
	ret



/*
** Destroys actor by ID. If the actor is active, it is removed from the active
** actor storage as well.
**
** Inputs:
**     r24: id
*/
.global mapact_destroy
mapact_destroy:

	; Clear availability bit

	ldi   r25,     0x80    ; For selecting the bit
	mov   XL,      r24     ; For selecting the byte
	lsr   XL
	brcc  .+2
	lsr   r25
	lsr   XL
	brcc  .+4
	lsr   r25
	lsr   r25
	lsr   XL
	brcc  .+2
	swap  r25
	clr   XH
	subi  XL,      lo8(-(v_avail))
	sbci  XH,      hi8(-(v_avail))
	ld    r0,      X
	com   r25
	and   r0,      r25
	st    X,       r0      ; Masked off actor availability (destroyed)

	; Find and free actor's slot (or slots in case of some bug)

	ldi   XL,      lo8(v_store - 8)
	ldi   XH,      hi8(v_store - 8)
	ldi   r25,     13
	clr   r1               ; Make sure it is zero (for freeing)
desl0:
	dec   r25
	breq  desl0x
	adiw  XL,      9
	ld    r0,      X
	cpse  r0,      r24
	rjmp  desl0            ; Not the given ID, continue iterating
	sbiw  XL,      1       ; Back onto flags
	st    X+,      r1      ; Free the slot
	rjmp  desl0
desl0x:
	ret



/*
** Modifies actor by the given structure. Anything may be altered. If the new
** location is too far from the dragon, the actor is inactivated. If the given
** actor number is free, nothing happens.
**
** Inputs:
**     r24: no
** r23:r22: actor
*/
.global mapact_set
mapact_set:

	cpi   r24,     12
	brcc  setne            ; Invalid actor number
	ldi   r25,     9
	mul   r24,     r25
	movw  XL,      r0
	subi  XL,      lo8(-(v_store))
	sbci  XH,      hi8(-(v_store))
	clr   r1               ; (For proper C interfacing)
	ld    r25,     X
	sbrc  r25,     7
	rjmp  setex            ; The actor exists
setne:
	ret
setex:
	; Existing actor: populate from the actor structure

	movw  ZL,      r22     ; Actor structure
	adiw  XL,      2       ; Position at pixel locations
	ld    r18,     Z+      ; X pixel location, low
	ld    r19,     Z+      ; X pixel location, high
	ld    r20,     Z+      ; Y pixel location, low
	ld    r21,     Z+      ; Y pixel location, high
	movw  r24,     XL
	movw  r22,     r18
	movw  r0,      r20
	call  levelcor_encode
	movw  r20,     r0
	clr   r1
	adiw  ZL,      2       ; Skip xbs and ybs members
	ld    r23,     Z+      ; X velocity
	st    X+,      r23
	ld    r23,     Z+      ; Y velocity
	st    X+,      r23
	adiw  ZL,      3       ; Skip xacc, yacc and flg members
	ld    r25,     Z+      ; Type
	ld    r23,     Z+      ; User data 0
	st    X+,      r23
	ld    r23,     Z+      ; User data 1
	st    X+,      r23
	ld    r24,     Z+      ; ID
	sbiw  XL,      9       ; Roll back to flags
	ori   r25,     0x80    ; Add occupied flag

	; Check activity and remove if not active

	lds   ZL,      v_drx + 0
	lds   ZH,      v_drx + 1
	sub   ZL,      r18
	sbc   ZH,      r19     ; Subtract from right edge: should be between 0 - 383 for visibility.
	subi  ZL,      0x80
	sbci  ZH,      0x01    ; Subtract 384 to check if within visibility box
	brcc  setcl            ; Not in visibility box
	lds   ZL,      v_dry + 0
	lds   ZH,      v_dry + 1
	sub   ZL,      r20
	sbc   ZH,      r21     ; Subtract from bottom edge: should be between 0 - 383 for visibility.
	subi  ZL,      0x80
	sbci  ZH,      0x01    ; Subtract 384 to check if within visibility box
	brcc  setcl            ; Not in visibility box

	st    X+,      r25     ; Finish writing flags & ID
	st    X+,      r24
	ret                    ; Active, so retain it and return

setcl:
	; Inactive actor: remove it from the active actor list

	st    X,       r1      ; Write zero into flags, so unoccupied
	ret



/*
** Updates actors in relation to the dragon's changed location, removing
** actors which ended up too far from it.
*/
.global mapact_update
mapact_update:

	; Store dragon location

	ldi   ZL,      lo8(dragon_spr)
	ldi   ZH,      hi8(dragon_spr)
	ldi   XL,      lo8(v_drx)
	ldi   XH,      hi8(v_drx)
	ld    r18,     Z+      ; Dragon X low
	ld    r19,     Z+      ; Dragon X high
	subi  r18,     0x40
	sbci  r19,     0xFF    ; Add 192
	st    X+,      r18
	st    X+,      r19
	ld    r20,     Z+      ; Dragon Y low
	ld    r21,     Z+      ; Dragon Y high
	subi  r20,     0x40
	sbci  r21,     0xFF    ; Add 192
	st    X+,      r20
	st    X+,      r21

	; Iterate over the actors, and remove those which are not active.

	ldi   XL,      lo8(v_store)
	ldi   XH,      hi8(v_store)
	ldi   r25,     12
	mov   r0,      r25
	clr   r1               ; Make sure zero, for freeing
	movw  ZL,      r20     ; Dragon Y will be here
updl0:
	ld    r24,     X
	sbrs  r24,     7       ; Occupied?
	rjmp  updl0s           ; Unoccupied: skip calculations

	adiw  XL,      2
	call  levelcor_decode_asm
	sub   r22,     r18
	sbc   r23,     r19     ; Subtract right edge: should be between -384 - -1 for visibility.
	subi  r22,     0x80
	sbci  r23,     0xFE    ; Add 384 to check if within visibility box
	brcs  updcl            ; Not in visibility box
	sub   r20,     ZL
	sbc   r21,     ZH      ; Subtract bottom edge: should be between -384 - -1 for visibility.
	subi  r20,     0x80
	sbci  r21,     0xFE    ; Add 384 to check if within visibility box
	brcs  updcl            ; Not in visibility box

	adiw  XL,      4       ; Remained active, go on
	dec   r0
	brne  updl0
	ret

updcl:
	sbiw  XL,      5
	st    X,       r1      ; Zero flags: Free slot
updl0s:
	adiw  XL,      9       ; Was a free slot, go on
	dec   r0
	brne  updl0
	ret
