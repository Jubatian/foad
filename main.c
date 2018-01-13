/*
 *  Dragon - Main program file
 *  Copyright (C) 2015 Sandor Zsuga (Jubatian)
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

#include <avr/pgmspace.h>
#include <uzebox.h>
#include "seq.h"
#include "res.h"
#include "music.h"
#include "global.h"



int main(){

 /* Relocate stack (Top of memory will be used from now) */

 SPL = (M74_MAIN_STACK - 1U) & 0xFFU;
 SPH = (M74_MAIN_STACK - 1U) >> 8;

 /* Init audio player */

 SetMasterVolume(0xFFU);
 music_init(res_patches, res_music);

 /* Reset JAMMA credit / coin counter */

 global_jammac = 0U;

 /* Enter game sequence */

 seq_new(0U);

}
