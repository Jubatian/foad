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
	Changes
	---------------------------------------------
	See http://code.google.com/p/uzebox/source/browse/trunk/README.txt
*/

#include <avr/io.h>
#include "defines.h"



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



;Public methods
.global TIMER1_COMPA_vect
.global TIMER1_COMPB_vect
.global GetVsyncFlag
.global ClearVsyncFlag
.global ReadJoypad
.global ReadJoypadExt
.global WriteEeprom
.global ReadEeprom
.global WaitUs
.global UartInitRxBuffer
.global IsRunningInEmulator

;Public variables
.global sync_pulse
.global sync_flags
.global joypad1_status_lo
.global joypad2_status_lo
.global first_render_line_tmp
.global render_lines_count_tmp


;Includes the video mode selected by
;the -DVIDEO_MODE compile switch
#include VMODE_ASM_SOURCE

.section .bss
	.align 1

	sync_pulse:    .space 1  ; Scanline counter
	sync_flags:    .space 1  ; b0: vsync flag, set at 60Hz when video frame rendered
	                         ; b1: current field (0=odd, 1=even); unused

	first_render_line:        .space 1
	render_lines_count:       .space 1

	first_render_line_tmp:    .space 1
	render_lines_count_tmp:   .space 1

	; Last read results of joypads

	joypad1_status_lo:        .space 2
	joypad1_status_lo_t:      .space 2
#if P2_DISABLE == 0
	joypad2_status_lo:        .space 2
	joypad2_status_lo_t:      .space 2
#endif

#if TRUE_RANDOM_GEN == 1
	random_value:             .space 2
#endif

.section .text



	; Some general concepts:
	;
	; Sync pulse falls are timed by OCR1A, during normal pulses Timer 1
	; counting from 0 to 1819.
	; Sync pulse rises normally happen in the mixer, otherwise they are
	; timed using OCR1B (within some constraints short mixers should be
	; possible).
	; The sync state machine is realized using GPIOR0 which is accessible
	; for individual bits. Its layout is as follows:
	; bit 0: 68 cycles wide low pulse (normally used by the inline mixer)
	; bit 1: 136 cycles wide low pulse (normally used by the inline mixer)
	; bit 2: A VSync double rate pulse which requires no mixer operation
	; bit 3: Marks if within first segment of Vsync
	; bit 4: Vsync odd (no mixer) pulses remaining: 1, Clear: Transition
	; bit 5: Vsync odd (no mixer) pulses remaining: 2
	; bit 6: Vsync odd (no mixer) pulses remaining: 3
	; bit 7: First pulse marker (to request timer reprogramming)



;
; OCR1B Interrupt entry
;
TIMER1_COMPB_vect:

	; (3 cy IT entry latency)
	; (3 cy JMP)

	out   _SFR_IO_ADDR(GPIOR1), ZL
	lds   ZL,      _SFR_MEM_ADDR(TCNT1L)

	; Timer: (0x08 - 0x0D) + COMPB (5cy jitter)
	; COMPB (effective) is 70 / 138 / 776

	sbic  _SFR_IO_ADDR(GPIOR0), 0
	rjmp  rise_68
	sbis  _SFR_IO_ADDR(GPIOR0), 1
	rjmp  rise_774

rise_136:

	; 136 cycles: Low 3 bits of Timer:
	; 0x08 => 0x2 (010)
	; 0x09 => 0x3 (011)
	; 0x0A => 0x4 (100)
	; 0x0B => 0x5 (101)
	; 0x0C => 0x6 (110)
	; 0x0D => 0x7 (111)

	sbrs  ZL,      2
	rjmp  rise_6cy         ; 0x09 ( 9) or 0x08 (10)
	sbrc  ZL,      1
	rjmp  rise_b0          ; 0x0D ( 5) or 0x0C ( 6)
	nop
	rjmp  rise_b0          ; 0x0B ( 7) or 0x0A ( 8)

rise_774:

	; 774 cycles: Low 3 bits of Timer:
	; 0x08 => 0x0 (000)
	; 0x09 => 0x1 (001)
	; 0x0A => 0x2 (010)
	; 0x0B => 0x3 (011)
	; 0x0C => 0x4 (100)
	; 0x0D => 0x5 (101)

	nop
	sbrc  ZL,      2
	rjmp  rise_b0          ; 0x0D ( 5) or 0x0C ( 6)
	sbrc  ZL,      1
	rjmp  rise_b0          ; 0x0B ( 7) or 0x0A ( 8)
	nop
	rjmp  rise_b0          ; 0x09 ( 9) or 0x08 (10)

rise_68:

	; 68 cycles: Low 3 bits of Timer:
	; 0x08 => 0x6 (110)
	; 0x09 => 0x7 (111)
	; 0x0A => 0x0 (000)
	; 0x0B => 0x1 (001)
	; 0x0C => 0x2 (010)
	; 0x0D => 0x3 (011)

	nop
	sbrc  ZL,      2
	rjmp  rise_6cy         ; 0x09 ( 9) or 0x08 (10)
	sbrc  ZL,      1
	rjmp  rise_b0          ; 0x0D ( 5) or 0x0C ( 6)
	nop
	rjmp  rise_b0          ; 0x0B ( 7) or 0x0A ( 8)
rise_6cy:
	rjmp  .
	rjmp  .-6

rise_b0:

	sbrs  ZL,      0
	rjmp  .

	; An lds of TCNT1L here would result 0x1A

	sbi   _SFR_IO_ADDR(SYNC_PORT), SYNC_PIN

	; Process VSync transitions if any.
	; When this increases the pulse width (68 => 774 or 68 => 136), it
	; will generate a second "ghost" interrupt with no effect (the sync
	; pulse is already high, and the transition comparison below will fail
	; as bit 4 of GPIOR0 became set). This slightly messes up the Uzem
	; emulator as it doesn't check if the sbi actually caused a rising
	; edge (it doesn't break it, though). "Fixing" the "problem" here
	; wastes more clocks than leaving it this way.

	in    ZL,      _SFR_IO_ADDR(GPIOR1)
	sbic  _SFR_IO_ADDR(GPIOR0), 4
	reti                   ; No transition request

	sbis  _SFR_IO_ADDR(GPIOR0), 0
	rjmp  rise_tr_from774
	sbis  _SFR_IO_ADDR(GPIOR0), 3
	rjmp  rise_tr_from68r

	; Transition from first segment 68 cy wide LOW pulses
	; Prepare to generate the following:
	; 774 cycles Mixer LOW pulse, (910 - 774) cycles HIGH.
	; 774 cycles no mixer, (910 - 774) cycles HIGH.
	; 774 cycles Mixer LOW pulse, (910 - 774) cycles HIGH.
	; 774 cycles no mixer, (910 - 774) cycles HIGH.
	; 774 cycles Mixer LOW pulse, (910 - 774) cycles HIGH.
	; 774 cycles no mixer, (910 - 774) cycles HIGH.
	; The first Mixer LOW pulse will have to reconfigure the timer to
	; generate 910 cycles wide pulses with 774 cycles wide LOW.

	ldi   ZL,      0x70    ; 3 x 2 pulses, 774 cy LOW, start with Mixer
	out   _SFR_IO_ADDR(GPIOR0), ZL
	ldi   ZL,      hi8(773 + 2)
	sts   _SFR_MEM_ADDR(OCR1BH), ZL
	ldi   ZL,      lo8(773 + 2)
	rjmp  rise_tr_end

rise_tr_from774:

	; Transition from 774 cy wide LOW pulses
	; Prepare to generate the following:
	; 68 cycles Mixer LOW pulse, (910 - 68) cycles HIGH.
	; 68 cycles no mixer, (910 - 68) cycles HIGH.
	; 68 cycles Mixer LOW pulse, (910 - 68) cycles HIGH.
	; 68 cycles no mixer, (910 - 68) cycles HIGH.
	; 68 cycles Mixer LOW pulse, (910 - 68) cycles HIGH.
	; 68 cycles no mixer, (910 - 68) cycles HIGH.
	; Within last segment (distinguish since first segment is identical).
	; The next transition will happen in the no mixer segment.
	; The first Mixer LOW pulse will have to reconfigure the timer to
	; generate 910 cycles wide pulses with 68 cycles wide LOW.

	ldi   ZL,      0x71    ; 3 x 2 pulses, 68 cy LOW, start with Mixer
	out   _SFR_IO_ADDR(GPIOR0), ZL
	ldi   ZL,      hi8(67 + 2)
	sts   _SFR_MEM_ADDR(OCR1BH), ZL
	ldi   ZL,      lo8(67 + 2)
	rjmp  rise_tr_end

rise_tr_from68r:

	; Transition from last segment 68 cy wide LOW pulses to normal display
	; pulses.

	ldi   ZL,      0xF2    ; 136 cycles wide sync
	out   _SFR_IO_ADDR(GPIOR0), ZL
	ldi   ZL,      hi8(135 + 2)
	sts   _SFR_MEM_ADDR(OCR1BH), ZL
	ldi   ZL,      lo8(135 + 2)
rise_tr_end:
	sts   _SFR_MEM_ADDR(OCR1BL), ZL
	in    ZL,      _SFR_IO_ADDR(GPIOR1)
	reti



;
; OCR1A Interrupt entry
;
TIMER1_COMPA_vect:

	; (3 cy IT entry latency)
	; (3 cy JMP)

	out   _SFR_IO_ADDR(GPIOR1), ZL
	lds   ZL,      _SFR_MEM_ADDR(TCNT1L) ; Timer: 0x08 - 0x0D (5cy jitter)

	sbrc  ZL,      2
	rjmp  .+8              ; 0x0D ( 5) or 0x0C ( 6)
	sbrc  ZL,      1
	rjmp  .+4              ; 0x0B ( 7) or 0x0A ( 8)
	nop
	rjmp  .                ; 0x09 ( 9) or 0x08 (10)
	sbrs  ZL,      0
	rjmp  .

	; An lds of TCNT1L here would result 0x14

	sbic  _SFR_IO_ADDR(GPIOR0), 2
	rjmp  sync_nomix       ; ( 3) VSync double rate pulses

	; A fall which requires mixer operation. Depending on required pulse
	; width (68, 136 or 774 cycles) the mixer has to be instructed. There
	; is a bug in the original kernel that the 68 cycle wide pulse is one
	; cycle longer in the mixer, which is compensated by calling it one
	; cycle sooner. This compensation is not present here (so the mixer
	; has to be fixed to use with this kernel). Since the mixer is
	; designed to perform the rise of SYNC, clear the OCR1B interrupt flag
	; here (to discard the rise interrupt request).

	in    ZL,      _SFR_IO_ADDR(SREG)
	out   _SFR_IO_ADDR(GPIOR2), ZL
	push  ZH               ; ( 6)
	push  r0               ; ( 8)
	cbi   _SFR_IO_ADDR(SYNC_PORT), SYNC_PIN
	push  r1
	in    ZL,      _SFR_IO_ADDR(GPIOR0) ; (Just for compatibility with old mixers)
	call  update_sound
	sbi   _SFR_IO_ADDR(TIFR1), (1 << OCF1B)

	sbis  _SFR_IO_ADDR(GPIOR0), 1
	rjmp  sync_vsync       ; Either 68 or 774 cycle pulses in VSync

	; Normal 136 cycle wide pulses: count them, call video mode where
	; appropriate, transition to vsync on the end running vsync tasks.

	lds   ZL,      sync_pulse
	dec   ZL
	sts   sync_pulse, ZL
	breq  sync_vsync_transfer

	lds   ZH,      first_render_line
	sub   ZL,      ZH
	breq  sync_vmode
	brcc  sync_ctrl
	rjmp  sync_line_com



sync_vmode:

	; Enter video mode

	WAIT  ZH,      239 - (AUDIO_OUT_HSYNC_CYCLES)

	; Push registers r2 -> r29

	ldi   ZL,      2
	clr   ZH
sync_vmode_push:
	ld    r0,      Z+
	push  r0
	cpi   ZL,      30
	brne  sync_vmode_push

	call  VMODE_FUNC

	; Pop registers and be done with it

	ldi   ZL,      30
	clr   ZH
sync_vmode_pop:
	dec   ZL
	pop   r0
	st    Z,       r0
	brne  sync_vmode_pop
	pop   ZH
	in    ZL,      _SFR_IO_ADDR(GPIOR2)
	out   _SFR_IO_ADDR(SREG), ZL
	in    ZL,      _SFR_IO_ADDR(GPIOR1)
	reti



sync_vsync_transfer:

	; Move saved regs to allow transitioning into nonblocking mode

	in    r1,      _SFR_IO_ADDR(GPIOR1)
	in    r0,      _SFR_IO_ADDR(GPIOR2)

	; At this point SYNC is HIGH. Prepare to generate the following:
	; 68 cycles Mixer LOW pulse, (910 - 68) cycles HIGH.
	; 68 cycles no mixer, (910 - 68) cycles HIGH.
	; 68 cycles Mixer LOW pulse, (910 - 68) cycles HIGH.
	; 68 cycles no mixer, (910 - 68) cycles HIGH.
	; 68 cycles Mixer LOW pulse, (910 - 68) cycles HIGH.
	; 68 cycles no mixer, (910 - 68) cycles HIGH.
	; Within first segment (distinguish since last segment is identical).
	; The next transition will happen in the no mixer segment (the
	; transition to normal lines also happen there).
	; The first Mixer LOW pulse will have to reconfigure the timer to
	; generate 910 cycles wide pulses with 68 cycles wide LOW.

	ldi   ZL,      SYNC_HSYNC_PULSES
	sts   sync_pulse, ZL   ; Prepare for next frame
	ldi   ZH,      0xF9    ; 3 x 2 pulses, 68 cy LOW, start with Mixer

	; Enable interrupts to allow the last 136 cycle wide LOW pulse to
	; complete by OCR1B with short mixers

	sei                    ; From now, non-interrupt

	; Keep render height registers updated

	lds   ZL,      first_render_line_tmp
	neg   ZL
	subi  ZL,      (-(SYNC_HSYNC_PULSES - 1))
	sts   first_render_line, ZL

	lds   ZL,      render_lines_count_tmp
	sts   render_lines_count, ZL

	; Add padding cycles to make sure the 136 cycle wide LOW pulse is
	; completed by OCR1B

#if (AUDIO_OUT_HSYNC_CYCLES < 110)
	WAIT  ZL,      (110 - AUDIO_OUT_HSYNC_CYCLES)
#endif

	out   _SFR_IO_ADDR(GPIOR0), ZH
	ldi   ZL,      hi8(67 + 2)
	sts   _SFR_MEM_ADDR(OCR1BH), ZL
	ldi   ZL,      lo8(67 + 2)
	sts   _SFR_MEM_ADDR(OCR1BL), ZL

	mov   ZL,      r1
	out   _SFR_IO_ADDR(SREG), r0
	pop   r1
	pop   r0
	pop   ZH
	reti



sync_ctrl:

	; Lines before display: Read the controllers
	; ZL contains lines until transfer to video (0), which are used the
	; following manner:
	; 18: Raise controller latch pin
	; 17: Release controller latch pin
	; 1-16: Read controller data bits
	; The clock is normally kept low. Rising edges advance the shifer in
	; the controller.

	cpi   ZL,      17
	brcs  sync_ctrl_rd
	breq  sync_ctrl_rel

	; Raise controller latch pin (maybe repeatedly for multiple scanlines,
	; it doesn't really matter); also this path contains the first
	; scanline after VSync, so also reprogram timer as needed.

	sbi   _SFR_IO_ADDR(JOYPAD_OUT_PORT), JOYPAD_LATCH_PIN

sync_line_com:

	; Was an ordinary line. Check for first line (to reconfigure timer
	; for 1820 cycles wide pulses with 136 cycle wide LOW), then return.

	pop   r1
	pop   r0
	pop   ZH
	in    ZL,      _SFR_IO_ADDR(GPIOR2)
	out   _SFR_IO_ADDR(SREG), ZL
	in    ZL,      _SFR_IO_ADDR(GPIOR1)
	sbis  _SFR_IO_ADDR(GPIOR0), 7
	reti                   ; No timer reprogramming requested

	ldi   ZL,      hi8(1819)
	sts   _SFR_MEM_ADDR(OCR1AH), ZL
	ldi   ZL,      lo8(1819)
	sts   _SFR_MEM_ADDR(OCR1AL), ZL
	cbi   _SFR_IO_ADDR(GPIOR0), 7
	in    ZL,      _SFR_IO_ADDR(GPIOR1)
	reti

sync_ctrl_rel:

	; Release controller latch pin.

	cbi   _SFR_IO_ADDR(JOYPAD_OUT_PORT), JOYPAD_LATCH_PIN
	rjmp  sync_line_com

sync_ctrl_rd:

	; Read controller data bits. Do this along with pushing the sbi of the
	; clock as far back as possible to get a nice wide pulse.

	cbi   _SFR_IO_ADDR(JOYPAD_OUT_PORT), JOYPAD_CLOCK_PIN
	lds   r0,      joypad1_status_lo_t + 0
	lds   r1,      joypad1_status_lo_t + 1
	clc
	sbis  _SFR_IO_ADDR(JOYPAD_IN_PORT), JOYPAD_DATA1_PIN
	sec
	ror   r1
	ror   r0
	sts   joypad1_status_lo_t + 0, r0
	sts   joypad1_status_lo_t + 1, r1
	cpi   ZL,      1
	brne  sync_ctrl_rd1c
	sts   joypad1_status_lo   + 0, r0
	sts   joypad1_status_lo   + 1, r1
sync_ctrl_rd1c:
#if (P2_DISABLE == 0)
	lds   r0,      joypad2_status_lo_t + 0
	lds   r1,      joypad2_status_lo_t + 1
	clc
	sbis  _SFR_IO_ADDR(JOYPAD_IN_PORT), JOYPAD_DATA2_PIN
	sec
	ror   r1
	ror   r0
	sts   joypad2_status_lo_t + 0, r0
	sts   joypad2_status_lo_t + 1, r1
	cpi   ZL,      1
	brne  sync_ctrl_rd2c
	sts   joypad2_status_lo   + 0, r0
	sts   joypad2_status_lo   + 1, r1
sync_ctrl_rd2c:
#endif
	pop   r1
	pop   r0
	pop   ZH
	in    ZL,      _SFR_IO_ADDR(GPIOR2)
	out   _SFR_IO_ADDR(SREG), ZL
	in    ZL,      _SFR_IO_ADDR(GPIOR1)
	sbi   _SFR_IO_ADDR(JOYPAD_OUT_PORT), JOYPAD_CLOCK_PIN
	reti



sync_vsync:

	sbi   _SFR_IO_ADDR(GPIOR0), 2
	pop   r1
	pop   r0
	pop   ZH
	in    ZL,      _SFR_IO_ADDR(GPIOR2)
	out   _SFR_IO_ADDR(SREG), ZL
	in    ZL,      _SFR_IO_ADDR(GPIOR1)
	sbis  _SFR_IO_ADDR(GPIOR0), 7
	reti                   ; No timer reprogramming requested
	ldi   ZL,      hi8(909)
	sts   _SFR_MEM_ADDR(OCR1AH), ZL
	ldi   ZL,      lo8(909)
	sts   _SFR_MEM_ADDR(OCR1AL), ZL
	in    ZL,      _SFR_IO_ADDR(GPIOR1)
	reti



sync_nomix:

	sbic  _SFR_IO_ADDR(GPIOR0), 6
	rjmp  sync_nomix_e2
	sbic  _SFR_IO_ADDR(GPIOR0), 5
	rjmp  sync_nomix_e1
	in    ZL,      _SFR_IO_ADDR(GPIOR1)
	cbi   _SFR_IO_ADDR(SYNC_PORT), SYNC_PIN
	cbi   _SFR_IO_ADDR(GPIOR0), 4
	reti

sync_nomix_e2:

	cbi   _SFR_IO_ADDR(GPIOR0), 6
	cbi   _SFR_IO_ADDR(SYNC_PORT), SYNC_PIN
	rjmp  sync_nomix_e2e

sync_nomix_e1:

	cbi   _SFR_IO_ADDR(SYNC_PORT), SYNC_PIN
	cbi   _SFR_IO_ADDR(GPIOR0), 5

sync_nomix_e2e:

	cbi   _SFR_IO_ADDR(GPIOR0), 2
	in    ZL,      _SFR_IO_ADDR(GPIOR1)
	reti



;
; Generate a H-Sync pulse
;
; For use by video modes when rendering a frame
;
; Clobbers: r0, r1, ZL, ZH, T flag
;
hsync_pulse:
	cbi   _SFR_IO_ADDR(SYNC_PORT), SYNC_PIN
	ldi   ZL,      2
	rjmp  .
	call  update_sound
	lds   ZL,      sync_pulse
	dec   ZL
	sts   sync_pulse, ZL
	ret



;************************************
; This flag is set on each VSYNC by
; the engine. This func is used to
; synchronize the programs on frame
; rate (60hz).
;
; C-callable
;************************************
.section .text.GetVsyncFlag
GetVsyncFlag:
	lds r24,sync_flags
	andi r24,SYNC_FLAG_VSYNC
	ret

;*****************************
; Clear the VSYNC flag.
; 
; C-callable
;*****************************
.section .text.ClearVsyncFlag
ClearVsyncFlag:
	lds r18,sync_flags
	andi r18,~SYNC_FLAG_VSYNC
	sts sync_flags,r18
	ret


;*****************************
; Return joypad 1 or 2 buttons status
; C-callable
; r24=joypad No (0 or 1)
; returns: (int) r25:r24
;*****************************
.section .text.ReadJoypad
ReadJoypad:

#if P2_DISABLE == 0
	tst   r24
	brne  rj_p2
#endif

	lds   r24,     joypad1_status_lo
	lds   r25,     joypad1_status_lo + 1
	ret

#if P2_DISABLE == 0
rj_p2:
	lds   r24,     joypad2_status_lo
	lds   r25,     joypad2_status_lo + 1
	ret
#endif


;****************************
; Wait for n microseconds
; r25:r24 - us to wait
; returns: void
;****************************
.section .text.WaitUs
WaitUs:	
	ldi r23,8
	dec 23
	brne .-4 ;~1 us
	nop
	sbiw r24,1
	brne WaitUs

	ret


;****************************
; Write byte to EEPROM
; extern void WriteEeprom(int addr,u8 value)
; r25:r24 - addr
; r22 - value to write
;****************************

.section .text.WriteEeprom
WriteEeprom:
   ; Wait for completion of previous write
   sbic _SFR_IO_ADDR(EECR),EEPE
   rjmp WriteEeprom
   ; Set up address (r25:r24) in address register
   out _SFR_IO_ADDR(EEARH), r25
   out _SFR_IO_ADDR(EEARL), r24
   ; Write data (r22) to Data Register
   out _SFR_IO_ADDR(EEDR),r22
   cli
   ; Write logical one to EEMPE
   sbi _SFR_IO_ADDR(EECR),EEMPE
   ; Start eeprom write by setting EEPE
   sbi _SFR_IO_ADDR(EECR),EEPE
   sei
   ret


;****************************
; Read byte from EEPROM
; extern unsigned char ReadEeprom(int addr)
; r25:r24 - addr
; r24 - value read
;****************************
.section .text.ReadEeprom
ReadEeprom:
   ; Wait for completion of previous write
   sbic _SFR_IO_ADDR(EECR),EEPE
   rjmp ReadEeprom
   ; Set up address (r25:r24) in address register
   out _SFR_IO_ADDR(EEARH), r25
   out _SFR_IO_ADDR(EEARL), r24
   ; Start eeprom read by writing EERE
   cli
   sbi _SFR_IO_ADDR(EECR),EERE
   ; Read data from Data Register
   in r24,_SFR_IO_ADDR(EEDR)
   sei
   ret


;****************************
; Return true (1) if the program run on uzem
; C callable
;****************************
.section .text.IsRunningInEmulator
IsRunningInEmulator:
	ldi r26,0xaa
	sts 0xff,r26
	lds r25,0xff
	ldi r24,1
	cpse r25,r26	
	clr r24
	ret


;****************************
; Wait for the specified amount of clocks * 4
; Note this is approximative. 
; C callable
; r25:r24 - clocks to wait
;****************************
.global WaitClocks
.section .text.WaitClocks
WaitClocks:

1:	
	sbiw r24,1
	brne 1b
		
	ret

;****************************
; Turns on the onboard LED on PD4
; C callable
;****************************
.global SetLedOn
.section .text.SetLedOn
SetLedOn:
	sbi _SFR_IO_ADDR(PORTD),PD4
	ret

;****************************
; Turns off the onboard LED on PD4
; C callable
;****************************
.global SetLedOff
.section .text.SetLedOff
SetLedOff:
	cbi _SFR_IO_ADDR(PORTD),PD4
	ret

;****************************
; Toggles the onboard LED on PD4
; C callable
;****************************
.global ToggleLed
.section .text.ToggleLed
ToggleLed:
	sbi _SFR_IO_ADDR(PIND),PD4
	ret

//for internal debug use
.global internal_spi_byte
.section .text.internal_spi_byte
internal_spi_byte:

	out _SFR_IO_ADDR(SPDR),r24
	ldi r25,5
	dec r25
	brne .-4 ;wait 15 cycles
	in r24,_SFR_IO_ADDR(SPSR) ;clear flag
	in r24,_SFR_IO_ADDR(SPDR) ;read next pixel
	ret



#if TRUE_RANDOM_GEN == 1
	;****************************
	; Generates a true 16-bit random number
	; based upon the watchdog timer RC oscillator
	; entropy. Should not be called by user code because it
	; messes timer1. This function is invoke upon 
	; initialization by wdt_init(void)
	;****************************

	.global wdt_randomize
	.section .text.wdt_randomize
	wdt_randomize:

		;set timer 1 full speed count to 0xffff
		ldi r24,0
		sts sync_pulse,r24

		sts _SFR_MEM_ADDR(TCCR1A),r24	
		ldi 24,(1<<CS10)
		sts _SFR_MEM_ADDR(TCCR1B),r24

		cli
		;enable watchdog at fastest speed and generate interrupts
		ldi r24,0
		sts _SFR_MEM_ADDR(MCUSR),r24	
		ldi r25,(1<<WDIE)+(1<<WDE)+(0<<WDP3)+(0<<WDP2)+(0<<WDP1)+(0<<WDP0)
		lds r24,_SFR_MEM_ADDR(WDTCSR)
		ori r24,(1<<WDCE)+(1<<WDE)
		sts _SFR_MEM_ADDR(WDTCSR),r24
		sts _SFR_MEM_ADDR(WDTCSR),r25
	
		sei

		;generate 8 random cycles
	wait:
		lds r24,sync_pulse ;using the yet unalocated "sync_pulse" as a temp variable
		cpi r24,8
		brlo wait

		ret

	;********************************
	; Returns the random seed generated
	; at startup
	; C-Callable
	; Returns: r24:r25(u16)
	;********************************

	.global GetRandomSeed
	.section .text.GetRandomSeed
	GetRandomSeed:
		lds r24,random_value
		lds r25,random_value+1
		ret

	.global WDT_vect
	.section .text.WDT_vect
	;*************************************
	; Watchdog timer interrupt
	;*************************************
	WDT_vect:
		;save flags & status register
		push r16
		push r17
	
		in r16,_SFR_IO_ADDR(SREG)
		push r16

		lds r16,sync_pulse
		inc r16
		sts sync_pulse,r16

		;XOR succesive timer1 LSB into a int
		sbrc r16,0
		rjmp 1f
		lds r17,random_value
		lds r16,_SFR_MEM_ADDR(TCNT1L)
		eor r17,r16
		sts random_value,r17
		rjmp 2f
	1:
		lds r17,random_value+1
		lds r16,_SFR_MEM_ADDR(TCNT1L)
		eor r17,r16
		sts random_value+1,r17
	2:

		ldi r16,(1<<WDIE)+(1<<WDE)
		sts _SFR_MEM_ADDR(WDTCSR),r16

		;restore flags
		pop r16
		out _SFR_IO_ADDR(SREG),r16
	
		pop r17
		pop r16
		reti
#endif
