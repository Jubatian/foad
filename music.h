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


/*
** Processes a tick of music and calls ProcessPatches() to advance audio.
**
** This replaces the kernel's ProcessMusic() for providing a custom format.
**
** Format:
**
** Each track has a command stream, similar to the format of patches.
** Commands are formatted as follows:
**
** Byte 0, bit 4-7: Command specification (0 - 15)
** Byte 0, bit 0-3: Delay in 60Hz ticks after command (0 - 15)
** (Optional extra bytes)
**
** Commands:
**
** 0x0: End of stream. Returns if there is somewhere to return to.
**      No extra bytes.
**
** 0x1: Reset track at given address.
**      Byte 1: Track to reset
**      Byte 2: Address low
**      Byte 3: Address high
**      This resets the given track at call depth zero, not affecting anything
**      else (like pitch or volume). Should be used on track 0 to sequence
**      other tracks.
**
** 0x2: Set patch (instrument).
**      Byte 1: Patch to use.
**
** 0x3: Set track volume (Note volume is always 0xFF).
**      Byte 1: Track volume.
**
** 0x4: Note on.
**      Byte 1: Note frequency.
**
** 0x5: Note off.
**      No extra bytes.
**
** 0x6: Long delay (delay bits in byte 0 are shifted up by 4).
**      No extra bytes.
**
** 0x7: Repeat (can not be nested within stream).
**      Byte 1: Repeat count
**      Byte 2: Count of bytes to jump back from after this byte
**
** 0x8: Call other stream (there are 2 call levels).
**      Byte 1: Address low
**      Byte 2: Address high
**
** 0x9: Jump to other stream.
**      Byte 1: Address low
**      Byte 2: Address high
**
** 0xA: Ramp track volume (by default this is zero).
**      Byte 1: Signed ramp amount per tick.
**
** 0xB: Set note shift.
**      Byte 1: Signed note shift amount.
**
** Track state:
**
** Byte  0: Call depth; 0xFF: Nothing to play
** Byte  1: Remaining ticks until next command; 0: Sync wait
** Byte  2: Patch
** Byte  3: Track volume ramping
** Byte  4: Note shift
** Byte  5: 0; Stream pointer low
** Byte  6: 0; Stream pointer high
** Byte  7: 0; Repeat count
** Byte  8: 1; Stream pointer low
** Byte  9: 1; Stream pointer high
** Byte 10: 1; Repeat count
*/



#ifndef MUSIC_H
#define MUSIC_H


#include "types.h"
#include <uzebox.h>


/*
** Inits sound & music over a given patch structure & command stream set.
** Addresses within the command stream set are relative to this passed
** pointer.
*/
void music_init(const struct PatchStruct* patches, const void* cmdstr);


/*
** Starts playing music by setting Track 0's command set start pointer.
** This also resets all tracks for volume 0xFF (with no ramping) and all note
** shifts to zero.
*/
void music_start(uint16 saddr);


/*
** Start music by res_music_list index
*/
void music_start_id(uint8 idx);


/*
** Processes music, then patches (kernel's ProcessPatches()).
*/
void music_process();


#endif
