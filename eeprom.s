/*
 *  Dragon - EEPROM score storage
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
** Default EEPROM data:
**
** Alec-Uze    1200
** D3thAdd3r    950
** Jubatian     500
**
** (Maximal possible score in the demo is above 1400 points)
*/
eeprom_def:
	.byte  0, 11 + 26,  4 + 26,  2 + 26, 62,      20,      25 + 26,  4 + 26, 63,      63, 1200 & 0xFF, 1200 >> 8
	.byte  3,  3 + 52, 19 + 26,  7 + 26,  0,       3 + 26,  3 + 26,  3 + 52, 17 + 26, 63,  950 & 0xFF,  950 >> 8
	.byte  9, 20 + 26,  1 + 26,  0 + 26, 19 + 26,  8 + 26,  0 + 26, 13 + 26, 63,      63,  500 & 0xFF,  500 >> 8



/*
** Loads EEPROM score data to target address. If it doesn't exist, default
** scores are created. 36 bytes of RAM is required for it at addr, tadd is
** used for temporary storage, 32 bytes (can not overlap with addr).
**
** r25:r24: Address of 36 byte score data
** r23:r22: Temporary workspace (32 bytes)
*/
.global eeprom_load
eeprom_load:

	push  r22
	push  r23
	push  r24
	push  r25
	ldi   r25,     0xF0    ; EEPROM block ID: 0xF0AD
	ldi   r24,     0xAD
	call  EepromReadBlock
	cpi   r24,     0x00
	pop   r25
	pop   r24
	pop   r23
	pop   r22
	breq  loads

	; EEPROM loading failed: Apply default EEPROM data

	ldi   ZL,      lo8(eeprom_def)
	ldi   ZH,      hi8(eeprom_def)
	ldi   r22,     36
	movw  XL,      r24
	lpm   r23,     Z+
	st    X+,      r23
	dec   r22
	brne  .-8
	ret

loads:

	; EEPROM loading succeed: Extract data

	movw  XL,      r22     ; Source: Temp storage
	movw  ZL,      r24     ; Destination: Score data
	adiw  XL,      2       ; Skip ID
	ldi   r21,     3
loadl:
	call  text_x_ram_xz
	ld    r22,     X+
	st    Z+,      r22
	ld    r22,     X+
	st    Z+,      r22
	dec   r21
	brne  loadl
	ret



/*
** Saves EEPROM score data from source address (36 bytes at addr), tadd is
** used for temporary storage, 32 bytes (can not overlap with addr). Note
** that it disables Mode 74 for the save (to prevent resets), re-enabling it
** upon return. The state of the frame is not reliable this case (a reset
** might happen any time), so only an M74_Halt() should follow this.
**
** r25:r24: Address of 36 byte score data
** r23:r22: Temporary workspace (32 bytes)
*/
.global eeprom_save
eeprom_save:

	; Turn off Mode 74 (to prevent resets during slow EEPROM write)

	lds   r21,     m74_config
	push  r21
	clr   r21
	sts   m74_config, r21

	; Compress data

	movw  XL,      r24     ; Source: Score data
	movw  ZL,      r22     ; Destination: Temp storage

	ldi   r25,     0xF0    ; EEPROM block ID: 0xF0AD
	ldi   r24,     0xAD
	st    Z+,      r24
	st    Z+,      r25     ; Little Endian

	ldi   r21,     3
savel:
	call  text_c_ram_xz
	ld    r22,     X+
	st    Z+,      r22
	ld    r22,     X+
	st    Z+,      r22
	dec   r21
	brne  savel

	; Store

	sbiw  ZL,      32
	movw  r24,     ZL      ; Temp storage: EEPROM block to save
	call  EepromWriteBlock

	; Re-enable Mode 74 in whatever mode it was last left

	pop   r21
	sts   m74_config, r21
	ret
