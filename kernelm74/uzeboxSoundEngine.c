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

#include <stdbool.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include "uzebox.h"


#define DEFAULT_WAVE           0
#define DEFAULT_PATCH          0
#define DEFAULT_TRACK_VOL      0xff


extern void ResetAudio(void);

extern u8  waves[];
extern u16 steptable[];

struct TrackStruct tracks[CHANNELS];
u8     masterVolume;

const struct PatchStruct *patchPointers;



void InitMusicPlayer(const struct PatchStruct *patchPointersParam){

	patchPointers=patchPointersParam;

	masterVolume=DEFAULT_MASTER_VOL;

	ResetAudio();

	// initialize default channels patches

	for(u8 t = 0U; t < CHANNELS; t ++){
		tracks[t].trackVol = DEFAULT_TRACK_VOL;
	}

}



void ProcessMusic(void){

	ProcessPatches();

}
