/*
 *  Uzebox Kernel functions
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

#ifndef __UZEBOX_H_
#define __UZEBOX_H_

	#include <stdbool.h>
	#include "defines.h"
	#include "kernel.h"
	
	//Include functions & var specific to the video mode used
	#ifdef VMODE_C_PROTOTYPES
		#include VMODE_C_PROTOTYPES
	#endif

	/*
	 * Video Engine structures & functions
	 */
	extern void FadeIn(unsigned char speed,bool blocking);
	extern void FadeOut(unsigned char speed,bool blocking);
	extern void SetSpritesOptions(unsigned char params);
	extern void SetSpritesTileTable(const char *data);

	/**
	 * Toggles the processing of sprites.
	 *
	 * @param visible	When true, sprites are displayed. When set to false, sprites are turned off.
	 * 					Depending on the video mode, a varying number of cpu cycles normally used
	 * 					for processing are freed.
	 */
	extern void SetSpriteVisibility(bool visible);
	extern void MapSprite(unsigned char startSprite,const char *map);
	extern void MapSprite2(unsigned char startSprite,const char *map,u8 spriteFlags);
	extern void MoveSprite(unsigned char startSprite,unsigned char x,unsigned char y,unsigned char width,unsigned char height);
	extern void Scroll(char sx,char sy);

	/**
	 * Fills the entire VRAM with tile #0.
	 *
	 * Before calling this function, you must invoke SetTileTable() to specify the tile set to use.
	 */
	extern void ClearVram(void);
	extern void SetTile(char x,char y, unsigned int tileId);
	extern void SetFont(char x,char y, unsigned char tileId);
	extern void SetFontTilesIndex(unsigned char index);
	extern void SetFontTable(const char *data);
	extern void SetTileTable(const char *data);	
	extern void DrawMap(unsigned char x,unsigned char y,const VRAM_PTR_TYPE *map); 	
	extern void Print(int x,int y,const char *string);
	extern void PrintRam(int x,int y,unsigned char *string);
	extern void PrintBinaryByte(char x,char y,unsigned char byte);
	extern void PrintHexByte(char x,char y,unsigned char byte);
	extern void PrintHexInt(char x,char y,int byte);
	extern void PrintHexLong(char x,char y, uint32_t value);
	extern void PrintLong(int x,int y, unsigned long val);
	extern void PrintByte(int x,int y, unsigned char val,bool zeropad);
	extern void PrintChar(int x,int y,char c);
	extern void PrintInt(int x,int y, unsigned int,bool zeropad);

	extern void Fill(int x,int y,int width,int height,int tile);
	extern void FontFill(int x,int y,int width,int height,int tile);

	extern void WaitVsync(int count);
	extern void ClearVsyncFlag(void);
	extern   u8 GetVsyncFlag(void);

	extern void SetRenderingParameters(u8 firstScanlineToRender, u8 verticalTilesToRender);


	/*
	 * Sound Engine functions
	 */	
	extern void SetMasterVolume(unsigned char vol);		//global player volume
	extern   u8 GetMasterVolume(void);
	extern void TriggerNote(unsigned char channel,unsigned char patch,unsigned char note,unsigned char volume);
	extern void TriggerFx(unsigned char patch,unsigned char volume, bool retrig); //uses a simple voice stealing algorithm
	extern void StopSong(void);
	#if MUSIC_ENGINE == MIDI
		extern void StartSong(const char *midiSong);
	#else
		extern void StartSong(const char *song, u16 startPos, bool loop);
	#endif
	extern void ResumeSong(void);
	extern void InitMusicPlayer(const struct PatchStruct *patchPointersParam);
	extern void SetSongSpeed(u8 speed);
	extern   u8 GetSongSpeed(void);
	extern void SetTrackVolume(u8 channel, u8 vol);
	extern   u8 GetTrackVolume(u8 channel);
	extern void ProcessMusic(void);   // Must be used, no VSync tasks
	extern void ProcessPatches(void); // Can be used if no music player


	/*
	 * Controllers functions
	 */
	#define BUTTON_UP 0
	#define BUTTON_DOWN 1
	#define BUTTON_CLICK 2
	#define BUTTON_DBLCLICK 3

	extern unsigned int ReadJoypad(unsigned char joypadNo);
	extern unsigned int ReadJoypadExt(unsigned char joypadNo); //use with SNES mouse
	extern unsigned char DetectControllers();
	extern void ReadControllers(void); // Must be used, no VSync tasks


	/*
	 * EEPROM functions
	 */
	extern void WriteEeprom(unsigned int addr,unsigned char value);
	extern unsigned char ReadEeprom(unsigned int addr);
	extern char EepromWriteBlock(struct EepromBlockStruct *block);
	extern char EepromReadBlock(unsigned int blockId,struct EepromBlockStruct *block);
	extern bool isEepromFormatted(void);
	extern void FormatEeprom(void);
	extern void FormatEeprom2(u16 *ids, u8 count);


	/*
	 * Misc functions
	 */
	extern void WaitClocks(u16 clocks);
	extern void WaitUs(unsigned int microseconds);
	extern void SoftReset(void);
	extern bool IsRunningInEmulator(void);

	extern void SetLedOn(void);
	extern void SetLedOff(void);
	extern void ToggleLed(void);

	extern u16  GetRandomSeed(void);

	//Debug
	extern void debug_clear(void);
	extern void debug_char(char c);
	extern void debug_hex(char c);
	extern void debug_str_p(const char* data);
	extern void debug_str_r(char* data,u8 size,bool hex);
	extern void debug_byte(unsigned char val);
	extern void debug_int(u16 i);
	extern void debug_long_hex(u32 i);
	extern void debug_long(unsigned long val);
	extern void debug_crlf(void);

	/*
	 * Deprecated functions
	 */
	 extern void DrawMap2(unsigned char x,unsigned char y,const char *map) __attribute__ ((deprecated)); // Use generalized DrawMap() function


#endif
