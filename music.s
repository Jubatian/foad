/*
 *  Dragon - Music processing
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


; Call nesting depth
#define MUSIC_D  4

; Number of music tracks
#define MUSIC_TN 3

; Track size
#define MUSIC_TS (5 + (MUSIC_D * 3))



.section .bss


music_tracks:
	.space MUSIC_TS * MUSIC_TN ; Track states. See "music.h" header.

music_dptr:
	.space 2                   ; Music data pointer (command streams)


.section .text



/*
** Small internal function to reset tracks
**
** r23:r22: Reset stream pointer
*/
music_rt:

	ldi   r18,     0       ; Channel counter
	ldi   ZL,      lo8(music_tracks)
	ldi   ZH,      hi8(music_tracks)
	clr   r1
	ldi   r19,     0xFF
	ldi   r20,     0x01

in_l:

	std   Z + 0,   r19     ; Nothing to play
	std   Z + 1,   r20     ; Remaining ticks: should start immediately
	std   Z + 2,   r1
	std   Z + 3,   r1
	std   Z + 4,   r1
	std   Z + 5,   r22
	std   Z + 6,   r23
	std   Z + 7,   r1
	adiw  ZL,      MUSIC_TS
	inc   r18
	cpi   r18,     MUSIC_TN
	brne  in_l

	ret



/*
** Inits sound & music over a given patch structure & command stream set.
** Addresses within the command stream set are relative to this passed
** pointer.
**
** r25:r24: patches
** r23:r22: command stream
*/
.global music_init
music_init:

	sts   music_dptr + 0, r22
	sts   music_dptr + 1, r23

	rcall music_rt

	jmp   InitMusicPlayer  ; Init kernel component



/*
** Starts playing music by setting Track 0's command set start pointer.
** This also resets all tracks for volume 0xFF (with no ramping) and all note
** shifts to zero.
**
** r25:r24: start address
*/
.global music_start
music_start:

	lds   r22,     music_dptr + 0
	lds   r23,     music_dptr + 1
	add   r22,     r24
	adc   r23,     r25

	rcall music_rt

	ldi   ZL,      lo8(music_tracks)
	ldi   ZH,      hi8(music_tracks)
	clr   r24
	ldi   r22,     0xFF
	std   Z + 0,   r24     ; Initial call level: 0

	jmp   SetTrackVolume   ; r24: Channel 0; r22: Volume 0xFF



/*
** Start music by res_music_list index
**
**     r24: start id
*/
.global music_start_id
music_start_id:

	mov   ZL,      r24
	lsl   ZL
	clr   ZH
	subi  ZL,      lo8(-(res_music_list))
	sbci  ZH,      hi8(-(res_music_list))
	lpm   r24,     Z+
	lpm   r25,     Z+
	rjmp  music_start



/*
** Processes music, then patches (kernel's ProcessPatches()).
*/
.global music_process
music_process:

	push  r13
	push  r14
	push  r15
	push  r16
	push  r17
	push  YL
	push  YH

	ldi   r16,     0       ; Channel counter
	ldi   YL,      lo8(music_tracks)
	ldi   YH,      hi8(music_tracks)

mp_l:

	ldd   r17,     Y + 0   ; Call depth
	cpi   r17,     MUSIC_D
	brcc  mp_cend          ; No music to play

	ldd   r25,     Y + 1   ; Remaining ticks
	dec   r25
	std   Y + 1,   r25
	brne  mp_cend          ; Nonzero after decrement: Wait for command

	; Just reached zero: Process commands

mp_cl:

	ldi   r25,     3
	mul   r17,     r25
	add   YL,      r0
	adc   YH,      r1      ; r1 remains zero
	ldd   ZL,      Y + 5
	ldd   ZH,      Y + 6   ; Stream pointer
	sub   YL,      r0
	sbc   YH,      r1

	lpm   r13,     Z+      ; Command & Delay
	ldi   r25,     0xF
	movw  r14,     ZL
	mov   ZL,      r13
	swap  ZL
	and   ZL,      r25     ; Command
	and   r13,     r25     ; Delay after command
	clr   ZH
	subi  ZL,      lo8(-(pm(mp_jtb)))
	sbci  ZH,      hi8(-(pm(mp_jtb)))
	icall

	cpi   r17,     0xFF
	breq  mp_nrw
	ldi   r25,     3
	mul   r17,     r25
	add   YL,      r0
	adc   YH,      r1      ; r1 remains zero
	std   Y + 5,   ZL
	std   Y + 6,   ZH      ; Stream pointer
	sub   YL,      r0
	sbc   YH,      r1
mp_nrw:
	ldd   r17,     Y + 0
	cpi   r17,     0xFF
	breq  mp_cend          ; No more commands

	cp    r13,     r1
	breq  mp_cl            ; Delay after command is zero: Next command
	lsl   r13
	lsl   r13
	std   Y + 1,   r13

mp_cend:

	; Ramp track volume

	mov   r24,     r16
	call  GetTrackVolume
	ldd   r22,     Y + 3   ; Track volume ramping
	add   r24,     r22
	brcs  .+4              ; No negative to zero saturation
	sbrc  r22,     7
	ldi   r24,     0x00    ; Step was negative & No carry: Saturate
	brcc  .+4              ; No too large to 0xFF saturation
	sbrs  r22,     7
	ldi   r24,     0xFF    ; Step was positive & Carry: Saturate
	mov   r22,     r24
	mov   r24,     r16
	call  SetTrackVolume

	adiw  YL,      MUSIC_TS
	inc   r16
	cpi   r16,     MUSIC_TN
	breq  .+2
	rjmp  mp_l

	pop   YH
	pop   YL
	pop   r17
	pop   r16
	pop   r15
	pop   r14
	pop   r13

	jmp   ProcessPatches   ; Go on to patch processing (in kernel)

mp_jtb:
	rjmp  mp_c0
	rjmp  mp_c1
	rjmp  mp_c2
	rjmp  mp_c3
	rjmp  mp_c4
	rjmp  mp_c5
	rjmp  mp_c6
	rjmp  mp_c7
	rjmp  mp_c8
	rjmp  mp_c9
	rjmp  mp_ca
	rjmp  mp_cb
	rjmp  mp_cc
	rjmp  mp_cd
	rjmp  mp_ce
	rjmp  mp_cf

mp_c0:
	; 0x0: End of stream, return if possible
	dec   r17              ; Call depth, might become 0xFF which ends music
	std   Y + 0,   r17
	ldi   r17,     0xFF    ; To indicate end, avoiding rewriting pointers
mp_c0_e:
	movw  ZL,      r14
	ret

mp_c1:
	; 0x1: Reset track at given address
	movw  ZL,      r14
	lpm   r24,     Z+      ; Track to reset
	lpm   r22,     Z+      ; Address low
	lpm   r23,     Z+      ; Address high
	ldi   r25,     MUSIC_TS
	mul   r24,     r25
	movw  XL,      r0
	subi  XL,      lo8(-(music_tracks))
	sbci  XH,      hi8(-(music_tracks))
	clr   r1
	st    X+,      r1      ; Call depth: Empty
	inc   r1
	st    X+,      r1      ; Remaining ticks until next command: 1 (immediate)
	adiw  XL,      3       ; Skip patch, volume ramp & note shift
	lds   r20,     music_dptr + 0
	lds   r21,     music_dptr + 1
	add   r20,     r22
	adc   r21,     r23
	st    X+,      r20
	st    X+,      r21     ; Stream pointer
	clr   r1
	st    X+,      r1      ; Repeat count: zero
	cpse  r24,     r16     ; Resetting the track executing this?
	ret
	clr   r17              ; Empty depth
	std   Y + 0,   r17
	movw  ZL,      r20     ; Set stream pointer
	ret

mp_c2:
	; 0x2: Set patch
	movw  ZL,      r14
	lpm   r22,     Z+
	std   Y + 2,   r22     ; Patch
	ret

mp_c3:
	; 0x3: Set track volume
	movw  ZL,      r14
	lpm   r22,     Z+
	mov   r24,     r16
	movw  r14,     ZL
	call  SetTrackVolume
	rjmp  mp_c0_e

mp_c4:
	; 0x4: Note on
	movw  ZL,      r14
	lpm   r20,     Z+
	ldd   r22,     Y + 4   ; Note shift
	add   r20,     r22
	ldd   r22,     Y + 2   ; Patch
	ldi   r18,     0xFF
	movw  r14,     ZL
	rjmp  mp_c5_n

mp_c5:
	; 0x5: Note off
	ldi   r18,     0x00
mp_c5_n:
	mov   r24,     r16
	call  TriggerNote
	rjmp  mp_c0_e

mp_c6:
	; 0x6: Long delay
	swap  r13
	rjmp  mp_c0_e

mp_c7:
	; 0x7: Repeat
	movw  ZL,      r14
	lpm   r25,     Z+      ; Repeat count
	lpm   r24,     Z+      ; Jump back count
	add   YL,      r0
	adc   YH,      r1      ; Select music track again
	ldd   r23,     Y + 7   ; Repeat count
	subi  r23,     1
	brcc  .+2
	mov   r23,     r25     ; Decremented to 0xFF: Start new repeat
	std   Y + 7,   r23
	breq  .+4
	sub   ZL,      r24     ; Still nonzero: repeat (jump back)
	sbc   ZH,      r1
	sub   YL,      r0
	sbc   YH,      r1      ; Restore music track
	ret

mp_c8:
	; 0x8: Call other stream
	cpi   r17,     (MUSIC_D - 1)
	brcc  mp_c9            ; Turn into jump if too deep
	movw  ZL,      r14
	lpm   r14,     Z+
	lpm   r15,     Z+      ; Address to call
	add   YL,      r0
	adc   YH,      r1      ; Select music track again
	std   Y + 5,   ZL
	std   Y + 6,   ZH      ; Store last address (return address)
	std   Y + 10,  r1      ; Clear repeat count for new stack level (at 7 + 3)
	sub   YL,      r0
	sbc   YH,      r1      ; Restore music track
	inc   r17              ; Increment depth
	std   Y + 0,   r17
	rjmp  mp_c9_e

mp_c9:
	; 0x9: Jump to other stream
	movw  ZL,      r14
	lpm   r14,     Z+
	lpm   r15,     Z+      ; Address to jump onto
mp_c9_e:
	lds   ZL,      music_dptr + 0
	lds   ZH,      music_dptr + 1
	add   ZL,      r14
	adc   ZH,      r15
	ret

mp_ca:
	; 0xA: Ramp track volume
	movw  ZL,      r14
	lpm   r22,     Z+
	std   Y + 3,   r22     ; Track volume ramping
	ret

mp_cb:
	; 0xB: Set note shift
	movw  ZL,      r14
	lpm   r22,     Z+
	std   Y + 4,   r22     ; Note shift
	ret

mp_cc:
mp_cd:
mp_ce:
mp_cf:
	; Null commands
	rjmp  mp_c0_e

