
; 4bpp tile palette (color 0 is transparent)

tilepal:
	.byte 0xF4, 0xE3, 0x91, 0x00, 0x09, 0x11, 0x19, 0x23, 0x15, 0x0B, 0x52, 0x5B, 0xA4, 0xAD, 0x76, 0xFF

; 4bpp tile data (20 tiles; 640 bytes)

tiledata:
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x04, 0x40, 0x00
	.byte 0x00, 0x0D, 0x40, 0x00
	.byte 0x00, 0x0D, 0x44, 0x00
	.byte 0x00, 0x00, 0xD4, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x44, 0x00, 0x00
	.byte 0x00, 0xD4, 0x00, 0x00
	.byte 0x00, 0xDD, 0x40, 0x00
	.byte 0x00, 0x0D, 0x40, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x44, 0x00, 0x00
	.byte 0x00, 0xD4, 0x00, 0x00
	.byte 0x00, 0xDD, 0x40, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x44, 0x00, 0x00
	.byte 0x00, 0xD4, 0x00, 0x00
	.byte 0x00, 0xDD, 0x40, 0x00
	.byte 0x00, 0x0E, 0xE0, 0x00
	.byte 0x00, 0x0E, 0xE0, 0x00
	.byte 0x00, 0x0E, 0xE0, 0x00
	.byte 0x00, 0x08, 0xE0, 0x00
	.byte 0x00, 0x0D, 0x80, 0x00
	.byte 0x00, 0x08, 0x80, 0x00
	.byte 0x00, 0x08, 0x80, 0x00
	.byte 0x00, 0x0B, 0xB0, 0x00
	.byte 0x00, 0x0E, 0xE0, 0x00
	.byte 0x00, 0x0E, 0xE0, 0x00
	.byte 0x00, 0x0E, 0xE0, 0x00
	.byte 0x00, 0xDE, 0x80, 0x00
	.byte 0x00, 0x08, 0x88, 0x00
	.byte 0x00, 0x08, 0x8B, 0x00
	.byte 0x00, 0x08, 0x0B, 0x00
	.byte 0x00, 0xBB, 0x00, 0x00
	.byte 0x00, 0x0E, 0x40, 0x00
	.byte 0x00, 0x0E, 0xE0, 0x00
	.byte 0x00, 0x0E, 0xEE, 0x00
	.byte 0x00, 0x0E, 0xE0, 0x00
	.byte 0x00, 0x0D, 0x80, 0x00
	.byte 0x00, 0x08, 0x88, 0x00
	.byte 0x00, 0x0B, 0x88, 0x00
	.byte 0x00, 0x00, 0xBB, 0x00
	.byte 0x00, 0x0E, 0x40, 0x00
	.byte 0x00, 0x0E, 0xE0, 0x00
	.byte 0x00, 0x0E, 0xE0, 0x00
	.byte 0x00, 0x0E, 0x80, 0x00
	.byte 0x00, 0xD8, 0x88, 0x00
	.byte 0x00, 0x88, 0x88, 0x00
	.byte 0x00, 0x88, 0x88, 0xB0
	.byte 0x0B, 0x00, 0x00, 0xB0
	.byte 0x00, 0xBB, 0xBA, 0x00
	.byte 0x0A, 0xBB, 0x9A, 0xA0
	.byte 0x0A, 0xA4, 0x44, 0x30
	.byte 0x0B, 0xBA, 0xA9, 0xA0
	.byte 0x0B, 0xB9, 0xA9, 0xA0
	.byte 0x0A, 0xA4, 0x44, 0x30
	.byte 0x0A, 0xBB, 0x9A, 0xA0
	.byte 0x00, 0xAA, 0xAA, 0x40
	.byte 0x00, 0x88, 0x8B, 0x00
	.byte 0x08, 0x88, 0xB9, 0x90
	.byte 0x0A, 0xA4, 0x44, 0x30
	.byte 0x08, 0xB9, 0xA9, 0xA0
	.byte 0x08, 0xB9, 0xA9, 0xA0
	.byte 0x0A, 0xA4, 0x44, 0x30
	.byte 0x08, 0x8B, 0x99, 0x90
	.byte 0x00, 0x99, 0x9A, 0x40
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x0D, 0xD0, 0x00
	.byte 0x00, 0x0B, 0x40, 0x00
	.byte 0x00, 0x0B, 0xB0, 0x00
	.byte 0x00, 0x00, 0xB0, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0xDD, 0x00, 0x00
	.byte 0x00, 0xB4, 0x00, 0x00
	.byte 0x00, 0xBB, 0x00, 0x00
	.byte 0x00, 0x0B, 0xD0, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0xDD, 0x00, 0x00
	.byte 0x00, 0xB4, 0x00, 0x00
	.byte 0x00, 0xBB, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0xDD, 0x00, 0x00
	.byte 0x00, 0xB4, 0x00, 0x00
	.byte 0x00, 0xBB, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0xD0, 0x00
	.byte 0x00, 0x00, 0xB0, 0x00
	.byte 0x00, 0x00, 0xB0, 0x00
	.byte 0x00, 0x00, 0xB0, 0x00
	.byte 0x00, 0x04, 0xD0, 0x00
	.byte 0x00, 0x0D, 0x40, 0x00
	.byte 0x00, 0x04, 0xD0, 0x00
	.byte 0x00, 0x0D, 0x40, 0x00
	.byte 0x00, 0x0B, 0xD0, 0x00
	.byte 0x00, 0x04, 0x40, 0x00
	.byte 0x00, 0x0D, 0xD0, 0x00
	.byte 0x00, 0x0B, 0xB0, 0x00
	.byte 0x00, 0x0D, 0xD0, 0x00
	.byte 0x00, 0x04, 0x40, 0x00
	.byte 0x00, 0x4D, 0xD0, 0x00
	.byte 0x00, 0xB4, 0xD0, 0x00
	.byte 0x00, 0x0D, 0x4D, 0x00
	.byte 0x00, 0x04, 0xDB, 0x00
	.byte 0x00, 0x0D, 0x0B, 0x00
	.byte 0x00, 0xBB, 0x00, 0x00
	.byte 0x00, 0x0D, 0x40, 0x00
	.byte 0x00, 0x04, 0xD4, 0x00
	.byte 0x00, 0x0D, 0x4D, 0x00
	.byte 0x00, 0x04, 0xD0, 0x00
	.byte 0x00, 0x0B, 0x40, 0x00
	.byte 0x00, 0x00, 0xD4, 0x00
	.byte 0x00, 0x0B, 0xDD, 0x00
	.byte 0x00, 0x00, 0xBB, 0x00
	.byte 0x00, 0x0D, 0xD0, 0x00
	.byte 0x00, 0x04, 0xD0, 0x00
	.byte 0x00, 0x0D, 0x40, 0x00
	.byte 0x00, 0x4D, 0xD0, 0x00
	.byte 0x00, 0xB4, 0x4D, 0x00
	.byte 0x00, 0x4D, 0xD4, 0x00
	.byte 0x00, 0xD0, 0x0D, 0xB0
	.byte 0x0B, 0x00, 0x00, 0xB0
	.byte 0x00, 0x0B, 0xDB, 0xD0
	.byte 0x00, 0xDB, 0x4B, 0xD0
	.byte 0x00, 0x00, 0xD0, 0x00
	.byte 0x00, 0x00, 0x40, 0x00
	.byte 0x00, 0x00, 0xD0, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x0D, 0xDD, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

