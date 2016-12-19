/*
 *  Uzebox Kernel
 *  Copyright (C) 2008-2009 Alec Bourque
 *  Optimized and trimmed to the game by Sandor Zsuga (Jubatian), 2016
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
 *
 *  Uzebox is a reserved trade mark
*/

/*
 * Audio mixer that mixes 4 channels into one sample during each HSYNC.
 *
 */

#include <avr/io.h>
#include <defines.h>

.global InitSound
.global update_sound_buffer
.global update_sound_buffer_2
.global update_sound_buffer_fast
.global process_music
.global waves
.global mix_pos
.global mix_buf
.global mix_bank
.global tr4_barrel_lo
.global tr4_barrel_hi
.global tr4_params
.global update_sound
.global counter


/* Public variables */

.global mixer
.global mixer_vmix



#define MIXER_VOL       0
#define MIXER_STEP_LO   1
#define MIXER_STEP_HI   2
#define MIXER_SPOS_FR   3
#define MIXER_SPOS_LO   4
#define MIXER_SPOS_HI   5
#define MIXER_VOLP      6

#define MIXER_SIZE      7


.section .bss



mixer_vmix:    .space 1        ; Mixer new volume mix ratio

mixer:

tr1_vol:       .space 1
tr1_step_lo:   .space 1
tr1_step_hi:   .space 1
tr1_pos_frac:  .space 1
tr1_pos_lo:    .space 1
tr1_pos_hi:    .space 1
tr1_volp:      .space 1

tr2_vol:       .space 1
tr2_step_lo:   .space 1
tr2_step_hi:   .space 1
tr2_pos_frac:  .space 1
tr2_pos_lo:    .space 1
tr2_pos_hi:    .space 1
tr2_volp:      .space 1

tr3_vol:       .space 1
tr3_step_lo:   .space 1
tr3_step_hi:   .space 1
tr3_pos_frac:  .space 1
tr3_pos_lo:    .space 1
tr3_pos_hi:    .space 1
tr3_volp:      .space 1

tr4_vol:       .space 1
tr4_step_lo:   .space 1
tr4_step_hi:   .space 1
tr4_pos_frac:  .space 1
tr4_pos_lo:    .space 1
tr4_pos_hi:    .space 1
tr4_volp:      .space 1

tr5_vol:       .space 1
tr5_step_lo:   .space 1
tr5_step_hi:   .space 1
tr5_pos_frac:  .space 1
tr5_pos_lo:    .space 1
tr5_pos_hi:    .space 1
tr5_volp:      .space 1



.section .text



;****************************
; Inline sound mixing
; In: ZL = video phase (1 = Pre-eq / Post-eq, 2 = Hsync, 0 = No sync)
;
; If ZL is zero, it is not necessary to maintain cycle sync.
;
; Destroys: Z, r0, r1
; Takes AUDIO_OUT_HSYNC_CYCLES
;****************************

update_sound:

	push r16
	push r17
	push r18

	; Advance volume mix ratio

	lds   r18,     mixer_vmix
	inc   r18
	brne  .+2
	dec   r18              ; Saturate at 0xFF
	sts   mixer_vmix, r18

	; Mix result is collected in r0:r1 (r0 is the high byte!)

	; Channel 1 (36 cy - 3 for initializing mix. buffer)

	lds   r17,     tr1_pos_frac
	lds   ZL,      tr1_pos_lo
	lds   ZH,      tr1_pos_hi
	lds   r16,     tr1_step_lo
	add   r17,     r16     ; Add step to fractional part of sample pos
	lds   r16,     tr1_step_hi
	adc   ZL,      r16     ; Add step to low byte of sample pos
	lpm   r16,     Z       ; Load sample
	sts   tr1_pos_lo, ZL
	sts   tr1_pos_frac, r17
	lds   r0,      tr1_vol
	mul   r0,      r18     ; New volume
	mov   r17,     r1
	neg   r18
	lds   r0,      tr1_volp
	mul   r0,      r18     ; Old volume
	add   r17,     r1
	mulsu r16,     r17     ; (sample * mixing_vol)
	sbc   r0,      r0      ; Sign extend

	; Channel 2 (36 cy + 2/3 sync generator)

	lds   r17,     tr2_pos_frac
	lds   ZL,      tr2_pos_lo
	lds   ZH,      tr2_pos_hi
	lds   r16,     tr2_step_lo
	add   r17,     r16     ; Add step to fractional part of sample pos
	lds   r16,     tr2_step_hi
	adc   ZL,      r16     ; Add step to low byte of sample pos
	;--- Video sync update ( 68 cy LOW pulse) ---
	sbic  _SFR_IO_ADDR(GPIOR0), 0
	sbi   _SFR_IO_ADDR(SYNC_PORT), SYNC_PIN
	;--------------------------------------------
	lpm   r16,     Z       ; Load sample
	sts   tr2_pos_lo, ZL
	sts   tr2_pos_frac, r17
	movw  ZL,      r0
	lds   r0,      tr2_volp
	mul   r0,      r18     ; Old volume
	mov   r17,     r1
	neg   r18
	lds   r0,      tr2_vol
	mul   r0,      r18     ; New volume
	add   r17,     r1
	mulsu r16,     r17     ; (sample * mixing_vol)
	sbc   r0,      r0      ; Sign extend
	add   r1,      ZH      ; Add ((sample * vol) >> 8) to mix buffer lsb
	adc   r0,      ZL      ; Ajust mix buffer msb

	; Channel 3 (36 cy)

	lds   r17,     tr3_pos_frac
	lds   ZL,      tr3_pos_lo
	lds   ZH,      tr3_pos_hi
	lds   r16,     tr3_step_lo
	add   r17,     r16     ; Add step to fractional part of sample pos
	lds   r16,     tr3_step_hi
	adc   ZL,      r16     ; Add step to low byte of sample pos
	lpm   r16,     Z       ; Load sample
	sts   tr3_pos_lo, ZL
	sts   tr3_pos_frac, r17
	movw  ZL,      r0
	lds   r0,      tr3_vol
	mul   r0,      r18     ; New volume
	mov   r17,     r1
	neg   r18
	lds   r0,      tr3_volp
	mul   r0,      r18     ; Old volume
	add   r17,     r1
	mulsu r16,     r17     ; (sample * mixing_vol)
	sbc   r0,      r0      ; Sign extend
	add   r1,      ZH      ; Add ((sample * vol) >> 8) to mix buffer lsb
	adc   r0,      ZL      ; Ajust mix buffer msb

	; Channel 4 (36 cy + 2/3 sync generator)

	lds   r17,     tr4_pos_frac
	lds   ZL,      tr4_pos_lo
	lds   ZH,      tr4_pos_hi
	;--- Video sync update (136 cy LOW pulse) ---
	sbic  _SFR_IO_ADDR(GPIOR0), 1
	sbi   _SFR_IO_ADDR(SYNC_PORT), SYNC_PIN
	;--------------------------------------------
	lds   r16,     tr4_step_lo
	add   r17,     r16     ; Add step to fractional part of sample pos
	lds   r16,     tr4_step_hi
	adc   ZL,      r16     ; Add step to low byte of sample pos
	lpm   r16,     Z       ; Load sample
	sts   tr4_pos_lo, ZL
	sts   tr4_pos_frac, r17
	movw  ZL,      r0
	lds   r0,      tr4_volp
	mul   r0,      r18     ; Old volume
	mov   r17,     r1
	neg   r18
	lds   r0,      tr4_vol
	mul   r0,      r18     ; New volume
	add   r17,     r1
	mulsu r16,     r17     ; (sample * mixing_vol)
	sbc   r0,      r0      ; Sign extend
	add   r1,      ZH      ; Add ((sample * vol) >> 8) to mix buffer lsb
	adc   r0,      ZL      ; Ajust mix buffer msb

	; Channel 5 (36 cy)

	lds   r17,     tr5_pos_frac
	lds   ZL,      tr5_pos_lo
	lds   ZH,      tr5_pos_hi
	lds   r16,     tr5_step_lo
	add   r17,     r16     ; Add step to fractional part of sample pos
	lds   r16,     tr5_step_hi
	adc   ZL,      r16     ; Add step to low byte of sample pos
	lpm   r16,     Z       ; Load sample
	sts   tr5_pos_lo, ZL
	sts   tr5_pos_frac, r17
	movw  ZL,      r0
	lds   r0,      tr5_vol
	mul   r0,      r18     ; New volume
	mov   r17,     r1
	neg   r18
	lds   r0,      tr5_volp
	mul   r0,      r18     ; Old volume
	add   r17,     r1
	mulsu r16,     r17     ; (sample * mixing_vol)
	sbc   r0,      r0      ; Sign extend
	add   ZH,      r1      ; Add ((sample * vol) >> 8) to mix buffer lsb
	adc   ZL,      r0      ; Ajust mix buffer msb

	; Restore no longer needed regs (6 cy)

	pop  r18
	pop  r17
	pop  r16

	; Final processing (9 cy)

	subi  ZH,      0x80
	sbci  ZL,      0xFF    ; Converts to unsigned
	brpl  .+6
	ldi   ZH,      0x00    ; Saturate from bottom to 0x00
	nop
	rjmp  .+6
	cpi   ZL,      0x00
	breq  .+2
	ldi   ZH,      0xFF    ; Saturate from top to 0xFF
	sts   _SFR_MEM_ADDR(OCR2A), ZH ; Output sound byte

	; Done

	ret
