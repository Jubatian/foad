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

#include <avr/io.h>
#include <defines.h>

.global steptable
.global ProcessPatches
.global TriggerFx
.global TriggerNote
.global SetTrackVolume
.global GetTrackVolume
.global SetMasterVolume
.global GetMasterVolume
.global ResetAudio



;
; Global assembly delay macro for 0 to 1535 cycles
; Parameters: reg=Registerto use in inner loop (will be destroyed)
;             clocks=CPU clocks to wait
;
.macro WAIT reg, clocks
.if     (\clocks) >= 768
	ldi   \reg,    0
	dec   \reg
	brne  .-4
.endif
.if     ((\clocks) % 768) >= 6
	ldi   \reg,    ((\clocks) % 768) / 3
	dec   \reg
	brne  .-4
.if     ((\clocks) % 3) == 2
	rjmp  .
.elseif ((\clocks) % 3) == 1
	nop
.endif
.elseif ((\clocks) % 768) == 5
	rjmp  .
	rjmp  .
	nop
.elseif ((\clocks) % 768) == 4
	rjmp  .
	rjmp  .
.elseif ((\clocks) % 768) == 3
	rjmp  .
	nop
.elseif ((\clocks) % 768) == 2
	rjmp  .
.elseif ((\clocks) % 768) == 1
	nop
.endif
.endm



; Size of track structure (TrackStruct)
#define TRACKSTRUCT_SIZE 13

; Size of patch list structure (PatchStruct)
#define PATCHSTRUCT_SIZE 2



#include "soundMixerInline.s"



.section .bss


fx_ptime: .space 1           ; Effect 0 playing time (last channel)
          .space 1           ; Effect 1 playing time (last-1 channel)
fx_patch: .space 1           ; Effect 0 patch (last channel)
          .space 1           ; Effect 1 patch (last-1 channel)
                             ; (Note: Order of these 4 bytes must be kept)


.section .text



;*****************************
; Resets audio channels to no output
; (C-Callable)
;*****************************
.section .text.ResetAudio
ResetAudio:

	clr   r1

	ldi   ZL,      lo8(mixer)
	ldi   ZH,      hi8(mixer)
	ldi   XL,      lo8(mixer + (CHANNELS * MIXER_SIZE))
	ldi   XH,      hi8(mixer + (CHANNELS * MIXER_SIZE))

	rcall ra_rut

	ldi   ZL,      lo8(tracks)
	ldi   ZH,      hi8(tracks)
	ldi   XL,      lo8(tracks + (CHANNELS * TRACKSTRUCT_SIZE))
	ldi   XH,      hi8(tracks + (CHANNELS * TRACKSTRUCT_SIZE))

ra_rut:

	st    Z+,      r1
	cp    ZL,      XL
	cpc   ZH,      XH
	brne  .-8
	ret



;**********************
; Sets mixer note. Note that the mixer runs in interrupt (inline mixer), so
; hazards may arise.
; (C-callable)
;     r24: Channel
;     r22: MIDI note, 69=A5(440) for waves channels
;          (Noise params for a noise channel, no such here)
; Clobbered: Z, X, r0, r1 (zero)
;***********************
.section .text.SetMixerNote
SetMixerNote:

	; Get step for note

	mov   ZL,      r22
	lsl   ZL
	clr   ZH
	subi  ZL,      lo8(-(steptable))
	sbci  ZH,      hi8(-(steptable))

	lpm   XL,      Z+
	lpm   XH,      Z+

	; Update mixer with it

	ldi   ZL,      MIXER_SIZE
	mul   ZL,      r24
	movw  ZL,      r0
	subi  ZL,      lo8(-(mixer))
	sbci  ZH,      hi8(-(mixer))

	std   Z + MIXER_STEP_LO, XL
	std   Z + MIXER_STEP_HI, XH

	clr   r1
	ret



;**********************
; Sets sound waveform for channel. Note that the mixer runs in interrupt
; (inline mixer), so hazards may arise.
; (C-callable)
;     r24 Channel
;     r22 Waves channels: patch (0x00-0xFD)
;         (Noise channel: 0xFE=7 bit lfsr, 0xFF=15 bit lfsr, no such here)
; Clobbered: Z, XL, r0, r1 (zero)
;***********************
.section .text.SetMixerWave
SetMixerWave:

	ldi   ZL,      MIXER_SIZE
	mul   ZL,      r24
	movw  ZL,      r0
	subi  ZL,      lo8(-(mixer))
	sbci  ZH,      hi8(-(mixer))

	ldi   XL,      hi8(waves)
	add   XL,      r22
	std   Z + MIXER_SPOS_HI, XL ; Store wave No

	clr   r1
	ret



;**********************
; Sets volume. Note that the mixer runs in interrupt (inline mixer), so
; hazards may arise.
; (C-callable)
;     r24 Channel
;     r22 Volume (0x00-0xff)
; Clobbered: Z, r0, r1 (zero)
;***********************
.section .text.SetMixerVolume
SetMixerVolume:

	ldi   ZL,      MIXER_SIZE
	mul   ZL,      r24
	movw  ZL,      r0
	subi  ZL,      lo8(-(mixer))
	sbci  ZH,      hi8(-(mixer))

	std   Z + MIXER_VOL, r22 ; Store volume

	clr   r1
	ret



;**********************
; Cycles mixer volumes for starting a new tick. Attempts to do it in a
; hazardless manner (inline mixer in interrupt).
; (C-callable)
; Clobbered: Z, r24, r25
;***********************
.section .text.MixerTick
MixerTick:

	ldi   ZL,      lo8(mixer)
	ldi   ZH,      hi8(mixer)
	ldi   r25,     CHANNELS

	ldd   r24,     Z + MIXER_VOL
	std   Z + MIXER_VOLP, r24
	adiw  ZL,      MIXER_SIZE
	dec   r25
	brne  .-10             ; Old volume = Current "new" volume

	sts   mixer_vmix, r25  ; Ratio of new volume: zero

	ret



;*****************************
; Process Patches
; (C-Callable)
;*****************************
.section .text.ProcessPatches
ProcessPatches:

	push  r16
	push  r17

	; Cycle mixer volumes

	call  MixerTick

	; Patch processing loop (for all channels)

	ldi   r16,     lo8(tracks)
	ldi   r17,     hi8(tracks)
	movw  XL,      r16     ; First channel's track pointer
	clr   r24              ; Channel number (as taken by SetMixerXxx functions)

ppl:

	; Process patch commands

	ld    r18,     X+      ; track->flags
	sbrc  r18,     6       ; TRACK_FLAGS_HOLD_ENV
	rjmp  ppl_cmde         ; Hold envelope set

	ld    r21,     X       ; track->remTicks
	subi  r21,     1
	st    X+,      r21     ; Reduce, if reaching or going below zero, command
	breq  .+2
	brcc  ppl_cmde         ; No command to process

ppl_cmdl:

	ld    ZL,      X+
	ld    ZH,      X+      ; track->patPtr
	cpi   ZH,      0
	brne  .+4
	cpi   ZL,      0
	breq  ppl_cmde         ; 0x0000: Not a patch, nothing to play

	lpm   r20,     Z+      ; Patch command
	cpi   r20,     15      ; 0 - 14: Valid commands, otherwise end of patch
	brcc  ppl_pate         ; 15 or above: End of patch

	lpm   r22,     Z+      ; Command parameter (also arg. for SetMixerXxx)
	lpm   r25,     Z+      ; Next delta
	sbiw  XL,      3
	st    X+,      r25     ; track->remTicks
	st    X+,      ZL      ; track->patPtr
	st    X+,      ZH
	movw  XL,      r16     ; Reset track pointer
	clr   r21
	subi  r20,     lo8(-(pm(ppl_patjmp)))
	sbci  r21,     hi8(-(pm(ppl_patjmp)))
	movw  ZL,      r20
	icall
	cpi   r25,     0
	brne  ppl_cmde         ; Nonzero delta: Processing completed
	movw  XL,      r16     ; Reset track pointer
	adiw  XL,      2       ; To track->patPtr
	rjmp  ppl_cmdl

ppl_pate:

	ldi   ZH,      0       ; End of patch
	sbiw  XL,      2
	st    X+,      ZH
	st    X+,      ZH
	movw  XL,      r16
	andi  r18,     0x5F    ; Clear bit 5 & 7: TRACK_FLAGS_PLAYING & TRACK_FLAGS_PRIORITY
	st    X,       r18     ; (Essentially a note cut patch command)

ppl_cmde:

	; Process volumes & Tremolo

	clr   r22              ; Will store final volume
	sbrs  r18,     5       ; TRACK_FLAGS_PLAYING
	rjmp  ppl_vzero        ; Not playing: Volume is zero

	movw  XL,      r16
	adiw  XL,      5
	ld    r22,     X+      ; track->trackVol
	ld    r1,      X+      ; track->noteVol
	ld    r20,     X+      ; track->envelopeVol

	inc   r20
	breq  .+2
	mul   r1,      r20
	sbrc  r18,     7       ; TRACK_FLAGS_PRIORITY
	ldi   r22,     0xFF    ; Effect: Ignore track volume
	inc   r22
	breq  .+2
	mul   r1,      r22
	lds   r22,     masterVolume
	inc   r22
	breq  .+2
	mul   r1,      r22
	mov   r22,     r1

	ld    r21,     X+      ; track->envelopeStep
	ld    ZH,      X+      ; track->tremoloWave
	ld    ZL,      X+      ; track->tremoloPos
	ld    r1,      X+      ; track->tremoloLevel
	and   r1,      r1      ; Test zero
	breq  ppl_notm

	subi  ZH,      hi8(-(waves))
	lpm   r23,     Z
	subi  r23,     0x80    ; Convert it to unsigned to use it as volume
	inc   r23
	breq  .+2
	mul   r1,      r23
	com   r1
	inc   r22
	breq  .+2
	mul   r1,      r22
	mov   r22,     r1

	ld    r23,     X+      ; track->tremoloRate
	add   ZL,      r23
	sbiw  XL,      3
	st    X,       ZL      ; track->tremoloPos

ppl_notm:
ppl_vzero:

	call  SetMixerVolume   ; (Clears r1 as well)

	; Process envelope (will have effect in the next frame as volume is
	; written out above). r20: volume + 1; r21: step from above
	; Also increment playing time, used for effects management

	sbrs  r18,     5       ; TRACK_FLAGS_PLAYING
	rjmp  ppl_eend         ; Not playing: No envelope

	movw  XL,      r16
	adiw  XL,      7       ; track->envelopeVol
	dec   r20
	add   r20,     r21
	brcs  .+4              ; No negative to zero saturation
	sbrc  r21,     7
	ldi   r20,     0x00    ; Step was negative & No carry: Saturate
	brcc  .+4              ; No too large to 0xFF saturation
	sbrs  r21,     7
	ldi   r20,     0xFF    ; Step was positive & Carry: Saturate
	st    X,       r20     ; track->envelopeVol

ppl_eend:

	; Everything completed, go on to next track

	movw  XL,      r16
	adiw  XL,      TRACKSTRUCT_SIZE
	movw  r16,     XL
	inc   r24
	cpi   r24,     CHANNELS
	breq  .+2
	rjmp  ppl

	; Update playing time of effects

	ldi   ZL,      lo8(fx_ptime)
	ldi   ZH,      hi8(fx_ptime)
	ldi   r24,     2       ; 2 channels for effects
ppl_fxl:
	ld    r25,     Z
	inc   r25
	brne  .+2
	dec   r25              ; Saturate to 0xFF
	st    Z+,      r25
	dec   r24
	brne  ppl_fxl

	; All done

	pop   r17
	pop   r16
	ret

ppl_patjmp:
	rjmp  patcmd_00
	rjmp  patcmd_01
	rjmp  patcmd_02
	rjmp  patcmd_03
	rjmp  patcmd_04
	rjmp  patcmd_05
	rjmp  patcmd_06
	rjmp  patcmd_07
	rjmp  patcmd_08
	rjmp  patcmd_09
	rjmp  patcmd_0A
	rjmp  patcmd_0B
	rjmp  patcmd_0C
	rjmp  patcmd_0D
	rjmp  patcmd_0E

patcmd_00:
	; PC_ENV_SPEED: Set envelope speed per frame (signed; 0: no envelope)
	adiw  XL,      8
	st    X,       r22     ; track->envelopeStep = param
	ret

patcmd_01:
	; PC_NOISE_PARAMS: Set noise channel params (no noise channel, so empty)
	ret

patcmd_02:
	; PC_WAVE: Set wave
	jmp   SetMixerWave

patcmd_03:
	; PC_NOTE_UP: Note up * param
	adiw  XL,      4
	ld    r0,      X       ; track->note
	add   r22,     r0
	st    X,       r22     ; track->note
	jmp   SetMixerNote

patcmd_04:
	; PC_NOTE_DOWN: Note down * param
	adiw  XL,      4
	ld    r0,      X       ; track->note
	sub   r0,      r22
	mov   r22,     r0
	st    X,       r22     ; track->note
	jmp   SetMixerNote

patcmd_05:
	; PC_NOTE_CUT: End of note / fx
	andi  r18,     0x5F    ; Clear bit 5 & 7: TRACK_FLAGS_PLAYING & TRACK_FLAGS_PRIORITY
	st    X,       r18
	ret

patcmd_06:
	; PC_NOTE_HOLD: Note hold
	ori   r18,     0x40    ; Set bit 5: TRACK_FLAGS_HOLD_ENV
	st    X,       r18
	ret

patcmd_07:
	; PC_ENV_VOL: Set envelope volume
	adiw  XL,      7
	st    X,       r22     ; track->envelopeVol
	ret

patcmd_08:
	; PC_PITCH: Set note / pitch
	andi  r18,     0xF7    ; Clear bit 3: TRACK_FLAGS_SLIDING
	st    X,       r18
	adiw  XL,      4
	st    X,       r22     ; track->note
	jmp   SetMixerNote

patcmd_09:
	; PC_TREMOLO_LEVEL: Set tremolo level
	adiw  XL,      11
	st    X,       r22     ; track->tremoloLevel
	ret

patcmd_0A:
	; PC_TREMOLO_RATE: Set tremolo rate
	adiw  XL,      12
	st    X,       r22     ; track->tremoloRate
	ret

patcmd_0B:
	; PC_SLIDE: Pitch slide, param = (+/-) half steps to slide to
	; Currently no support
	; (Possibly I might add it with DDA across notes, to get it somewhat
	; logarithmic)
	ret

patcmd_0C:
	; PC_SLIDE_SPEED: Pitch slide speed
	; Currently no support
	ret

patcmd_0D:
	; PC_TREMOLO_POS: Set tremolo position
	adiw  XL,      10
	st    X,       r22     ; track->tremoloPos
	ret

patcmd_0E:
	; PC_TREMOLO_WAVE: Set tremolo waveform
	adiw  XL,      9
	st    X,       r22     ; track->tremoloWave
	ret



;*****************************
; Common Note / Effect trigger
; (C-Callable, but not exposed)
;     r24: Channel
;     r22: Patch
;     r20: Note (ignored for effects)
;     r18: Volume
;*****************************
.section .text.TriggerCommon
TriggerCommon:

	ldi   XL,      TRACKSTRUCT_SIZE
	mul   XL,      r24
	movw  XL,      r0
	subi  XL,      lo8(-(tracks))
	sbci  XH,      hi8(-(tracks))

	ld    r19,     X       ; track->flags
	ori   r19,     0x20    ; add TRACK_FLAGS_PLAYING
	andi  r19,     0xB7    ; rem TRACK_FLAGS_HOLD_ENV & TRACK_FLAGS_SLIDING
	st    X+,      r19

	lds   ZL,      patchPointers + 0
	lds   ZH,      patchPointers + 1
	ldi   r25,     PATCHSTRUCT_SIZE
	mul   r22,     r25
	add   ZL,      r0
	adc   ZH,      r1
	lpm   r25,     Z+
	lpm   ZH,      Z       ; Patch data pointer
	mov   ZL,      r25     ; In Z
	ldi   r23,     0
	cpi   ZH,      0
	brne  .+4
	cpi   ZL,      0
	breq  .+2
	lpm   r23,     Z+      ; Load first delta if available
	st    X+,      r23     ; track->remTicks
	st    X+,      ZL
	st    X+,      ZH      ; track->patPtr
	movw  ZL,      XL

	clr   r1
	std   Z + 2,   r18     ; track->noteVol
	std   Z + 4,   r1      ; track->envelopeStep
	std   Z + 5,   r1      ; track->tremoloWave
	std   Z + 7,   r1      ; track->tremoloLevel

	mov   r22,     r20
	sbrc  r19,     7       ; TRACK_FLAGS_PRIORITY
	ret                    ; Effect: ignore note
	std   Z + 0,   r20     ; track->note
	jmp   SetMixerNote



;*****************************
; Effect trigger
; (C-Callable)
;     r24: Patch
;     r22: Volume
;     r20: Retrigger flag
;*****************************
.section .text.TriggerFx
TriggerFx:

	; Effect will normally play on the last channel, if it is not
	; available, then the channel before. Other channels are never taken
	; (they are reserved for music).

	mov   r18,     r22     ; r18: Volume
	mov   r22,     r24     ; r22: Patch
	ldi   r24,     CHANNELS - 1 ; r24: Channel
	clr   r1
	ldi   XL,      lo8(tracks + (TRACKSTRUCT_SIZE * (CHANNELS - 1)))
	ldi   XH,      hi8(tracks + (TRACKSTRUCT_SIZE * (CHANNELS - 1)))
	ldi   ZL,      lo8(fx_ptime)
	ldi   ZH,      hi8(fx_ptime)

	cpi   r20,     0
	breq  fx_nr            ; Don't retrigger

	; Retrigger

	ldd   r25,     Z + 2   ; fx_patch[0] (Last ch.)
	cp    r22,     r25
	breq  fx_sel           ; Found patch on last ch. channel selected
	dec   r24
	ldd   r25,     Z + 3   ; fx_patch[1] (Ls-1 ch.)
	cp    r22,     r25
	breq  fx_sel           ; Found patch on ls-1 ch. channel selected
	inc   r24

fx_nr:

	; Try to select a channel not playing an effect

	ld    r25,     X       ; track->flags
	sbrs  r25,     7
	rjmp  fx_sel           ; Last ch. doesn't play effect, selected
	dec   r24
	sbiw  XL,      TRACKSTRUCT_SIZE
	ld    r25,     X       ; track->flags
	sbrs  r25,     7
	rjmp  fx_sel           ; Ls-1 ch. doesn't play effect, selected

	; Select oldest playing

	ldd   r23,     Z + 0   ; fx_ptime[0] (Last ch.)
	ldd   r25,     Z + 1   ; fx_ptime[1] (Ls-1 ch.)
	cp    r23,     r25
	brcs  fx_sel           ; Ls-1 ch. older or equal, selected
	inc   r24
	adiw  XL,      TRACKSTRUCT_SIZE

fx_sel:

	; Channel selected, apply effect (note will be ignored)

	ld    r23,     X       ; track->flags
	ori   r23,     0x80    ; TRACK_FLAGS_PRIORITY
	st    X,       r23
	cpi   r24,     CHANNELS - 1
	breq  .+2
	adiw  ZL,      1
	std   Z + 0,   r1      ; Reset playing time for the new effect
	std   Z + 2,   r22     ; Store last effect patch for retriggers
	jmp   TriggerCommon



;*****************************
; Note trigger
; (C-Callable)
;     r24: Channel
;     r22: Patch (Ignored for note off)
;     r20: Note (Ignored for note off)
;     r18: Volume (0: Note off)
;*****************************
.section .text.TriggerNote
TriggerNote:

	ldi   ZL,      TRACKSTRUCT_SIZE
	mul   ZL,      r24
	movw  ZL,      r0
	subi  ZL,      lo8(-(tracks))
	sbci  ZH,      hi8(-(tracks))

	ldd   r25,     Z + 0   ; track->flags
	sbrc  r25,     7       ; TRACK_FLAGS_PRIORITY
	ret                    ; An effect is playing, can't play music note

	cpi   r18,     0
	brne  nt_new

	; Note off

	sbrc  r25,     6       ; TRACK_FLAGS_HOLD_ENV
	rjmp  nt_nct
	ldd   r23,     Z + 8   ; track->envelopeStep
	cpi   r23,     0
	breq  nt_nct
	andi  r25,     0x5F    ; Clear bit 5 & 7: TRACK_FLAGS_PLAYING & TRACK_FLAGS_PRIORITY
nt_nct:
	andi  r25,     0xBF    ; Clear bit 6: TRACK_FLAGS_HOLD_ENV
	st    Z,       r25
	ret

nt_new:

	; Note on

	jmp   TriggerCommon



;**********************
; Sets track volume.
; (C-callable)
;     r24 Channel
;     r22 Volume (0x00-0xff)
; Clobbered: Z, r0, r1 (zero)
;***********************
.section .text.SetTrackVolume
SetTrackVolume:

	ldi   ZL,      TRACKSTRUCT_SIZE
	mul   ZL,      r24
	movw  ZL,      r0
	subi  ZL,      lo8(-(tracks))
	sbci  ZH,      hi8(-(tracks))

	std   Z + 5,   r22     ; track->trackVol

	clr   r1
	ret



;**********************
; Gets track volume.
; (C-callable)
;     r24 Channel
; Clobbered: Z, r0, r1 (zero)
;***********************
.section .text.GetTrackVolume
GetTrackVolume:

	ldi   ZL,      TRACKSTRUCT_SIZE
	mul   ZL,      r24
	movw  ZL,      r0
	subi  ZL,      lo8(-(tracks))
	sbci  ZH,      hi8(-(tracks))

	ldd   r24,     Z + 5   ; track->trackVol

	clr   r1
	ret



;**********************
; Sets master volume.
; (C-callable)
;     r24 Volume (0x00-0xff)
;***********************
.section .text.SetMasterVolume
SetMasterVolume:

	sts   masterVolume, r24
	ret



;**********************
; Gets master volume.
; (C-callable)
;***********************
.section .text.GetMasterVolume
GetMasterVolume:

	lds   r24,     masterVolume
	ret



steptable:
#include "data/steptable.inc"
