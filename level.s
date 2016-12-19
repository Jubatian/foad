/*
 *  Dragon - Level data interfacing
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



#include "leveldef.h"
#include <avr/io.h>
; (Needed only for register names for X, Y and Z)



/*
** Map width (in L1 blocks: 16 tiles wide).
*/
.global level_w

/*
** Map height (in L1 blocks: 8 tiles tall).
*/
.global level_h



.section .bss

.equ	level_w,       MEM_LOC_6  ; Map pitch (in L1 blocks)
.equ	level_h,       MEM_LOC_7  ; Map height (in L1 blocks)

.equ	v_mapi_lo,     MEM_LOC_8  ; Map L1 index table low
.equ	v_mapi_hi,     MEM_LOC_9  ; Map L1 index table high

v_l0chx:
	.space 1               ; L0 cache, last X position
v_l0chy:
	.space 1               ; L0 cache, last Y position
v_l0ch:
	.space 1               ; L0 cache, remembered block

.section .text



/*
** Invalidates L0 cache (internal)
*/
level_l0ch_inv:

	ldi   r25,     0xFF
	sts   v_l0chx, r25
	sts   v_l0chy, r25
	ret



/*
** Clears replacements.
*/
.global level_repclear
level_repclear:

	clr   XL
	ldi   XH,      hi8(LEVEL_RL_OFF)
repcl:
	st    X+,      r1      ; 'r1' is zero by C interface
	cpi   XL,      0
	brne  repcl

	rjmp  level_l0ch_inv



/*
** Adds L0 block to replacements.
**
** Marks the given L0 block as replaced. Marks by x:y locations (low 2 bits
** unused as they index within the L0. If it is already marked, nothing
** happens.
**
** r25:r24: x
** r23:r22: y
*/
.global level_repadd
level_repadd:

	lsr   r25
	ror   r24
	lsr   r25
	ror   r24              ; r24: L0 X (8 bits)
	lsr   r23
	ror   r22
	lsr   r23
	ror   r22              ; r22: L0 Y (8 bits)
	lds   r0,      level_w
	lsr   r0               ; Pitch in bytes for the replacement map
	mul   r22,     r0
	clr   r1
	movw  ZL,      r0      ; ZH:ZL: Y based replacement map offset
	ldi   r25,     1
	lsr   r24
	brcs  .+2
	lsl   r25
	lsr   r24
	brcs  .+4
	lsl   r25
	lsl   r25
	lsr   r24
	brcs  .+2
	swap  r25              ; r25: Mask of bit in replacement map
	add   ZL,      r24
	ldi   ZH,      hi8(LEVEL_RL_OFF)
	ld    r0,      Z
	or    r0,      r25
	st    Z,       r0      ; Replacement marked

	rjmp  level_l0ch_inv



/*
** Removes L0 block from replacements.
**
** Removes replaced mark from an L0. If it isn't marked, nothing happens.
**
** r25:r24: x
** r23:r22: y
*/
.global level_reprem
level_reprem:

	lsr   r25
	ror   r24
	lsr   r25
	ror   r24              ; r24: L0 X (8 bits)
	lsr   r23
	ror   r22
	lsr   r23
	ror   r22              ; r22: L0 Y (8 bits)
	lds   r0,      level_w
	lsr   r0               ; Pitch in bytes for the replacement map
	mul   r22,     r0
	clr   r1
	movw  ZL,      r0      ; ZH:ZL: Y based replacement map offset
	ldi   r25,     1
	lsr   r24
	brcs  .+2
	lsl   r25
	lsr   r24
	brcs  .+4
	lsl   r25
	lsl   r25
	lsr   r24
	brcs  .+2
	swap  r25              ; r25: Mask of bit in replacement map
	com   r25              ; Invert to make an AND mask
	add   ZL,      r24
	ldi   ZH,      hi8(LEVEL_RL_OFF)
	ld    r0,      Z
	and   r0,      r25
	st    Z,       r0      ; Replacement marked

	rjmp  level_l0ch_inv



/*
** Set map by its L1 index table and width.
**
** The L1 index table (map) is in ROM starting at arbitrary location. Only map
** width is used here to serve as pitch. Width and height is to be interpreted
** in L1 blocks (so 16 x 8 tile blocks).
**
** The replacement map is cleared upon calling this.
**
** r25:r24: map (L1 index table)
**     r22: wdt (width or pitch of map in L1 blocks)
**     r20: hgt (height of map in L1 blocks)
*/
.global level_set
level_set:
	sts   v_mapi_lo, r24
	sts   v_mapi_hi, r25
	sts   level_w, r22
	sts   level_h, r20
	rjmp  level_repclear



/*
** Retrieve tile from map.
**
** Fetches the tile at a given tile location on the map, to be used for
** filling and scrolling the VRAM. Returns the tile's index.
**
** For assembly routine calls XH:XL is not clobbered.
**
** r25:r24: x
** r23:r22: y
**
** Returns in r24 (r25 zero).
*/
.global level_gettile
level_gettile:

	; Calculate offset within L0

	mov   r18,     r24
	andi  r18,     0x03
	sbrc  r22,     0
	ori   r18,     0x04
	sbrc  r22,     1
	ori   r18,     0x08

	; Fetch L0 block

	rcall level_getl0

	; Load tile

	swap  r24
	mov   ZH,      r24
	andi  ZH,      0x0F
	andi  r24,     0xF0
	mov   ZL,      r18
	add   ZL,      r24
	subi  ZH,      hi8(-LEVEL_L0_OFF)
	lpm   r24,     Z

	; Return

	ret



/*
** Retrieve L0 from map.
**
** Fetches the L0 block at the given tile location on the map. The low 2 bits
** of the location are not used (they specify offset within the L0). When an
** out of map coordinate is passed, return is generated according to the
** related defines in leveldef.h.
**
** For assembly routine calls XH:XL is not clobbered.
**
** r25:r24: x
** r23:r22: y
**
** Returns in r24 (r25 zero).
*/
.global level_getl0
level_getl0:

	; Get L0 addresses (both are 8 bits)

	lsr   r25
	ror   r24
	lsr   r25
	ror   r24              ; r24: L0 X (8 bits)
	lsr   r23
	ror   r22
	lsr   r23
	ror   r22              ; r22: L0 Y (8 bits)
	clr   r25              ; r25: Zero from now (for return)

	; Check cache, return from it if valid (cache notes: this enhances
	; greatly requests near to each other which is common when drawing the
	; map and also when requesting tiles for physics processing)

	lds   r19,     v_l0chx
	cp    r19,     r24
	brne  gl0_nch
	lds   r23,     v_l0chy
	cp    r23,     r22
	brne  gl0_nch
	lds   r24,     v_l0ch  ; Cache valid, load from it & return
	ret
gl0_nch:
	sts   v_l0chx, r24
	sts   v_l0chy, r22

	; Get L1 addresses (both are 8 bits)

	mov   r19,     r24     ; r19: L0 X (8 bits)
	mov   r23,     r22
	lsr   r24
	lsr   r24              ; r24: L1 X
	lsr   r23              ; r23: L1 Y

	; Get L1 block from the L1 index table

	lds   ZL,      v_mapi_lo
	lds   ZH,      v_mapi_hi
	lds   r21,     level_w ; r21: Will hold the pitch
	cp    r24,     r21
	brcc  gl0_oxm          ; Off map by X
	lds   r0,      level_h
	cp    r23,     r0
	brcc  gl0_oym          ; Off map by Y
	mul   r23,     r21
	add   ZL,      r0
	adc   ZH,      r1
	add   ZL,      r24
	adc   ZH,      r25
	lpm   r0,      Z       ; r0: L1 block

	; Calculate L0 index within the L1

	mov   r20,     r19
	andi  r20,     0x03
	sbrc  r22,     0
	ori   r20,     0x04    ; r20: L0 index within L1

	; Get L0 block

	ldi   ZL,      8
	mul   r0,      ZL
	movw  ZL,      r0
	add   ZL,      r20     ; Add L0 index
	subi  ZH,      hi8(-LEVEL_L1_OFF)
	lpm   r24,     Z       ; r20: L0 block

	; Check for replacement

	lsr   r21              ; Pitch in bytes for rep. map
	mul   r21,     r22
	clr   r1
	movw  ZL,      r0      ; ZH:ZL: Rep. map Y based offset
	ldi   r20,     1
	lsr   r19
	brcs  .+2
	lsl   r20
	lsr   r19
	brcs  .+4
	lsl   r20
	lsl   r20
	lsr   r19
	brcs  .+2
	swap  r20              ; r20: Mask of bit in replacement map
	add   ZL,      r19
	ldi   ZH,      hi8(LEVEL_RL_OFF)
	ld    r0,      Z
	and   r0,      r20     ; If nonzero, tile is replaced
	breq  .+6
	mov   ZL,      r24
	ldi   ZH,      hi8(LEVEL_RLD_OFF)
	lpm   r24,     Z       ; Replace

	; Return (r25:r24 OK as return value)

gl0_ret:
	sts   v_l0ch,  r24
	ret

gl0_oxm:

	ldi   r24,     LEVEL_RET_XI
	rjmp  gl0_ret

gl0_oym:

	ldi   r24,     LEVEL_RET_YI
	rjmp  gl0_ret
