/*
 *  Mode 74 tests
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


#include <uzebox.h>
#include "types.h"


#ifndef RES_H
#define RES_H


extern const uint8  res_maplist[]  __attribute__ ((section (".res")));
extern const uint8  res_sequence[]  __attribute__ ((section (".res")));
extern const struct PatchStruct res_patches[] __attribute__ ((section (".res")));
extern const uint8  res_music[]  __attribute__ ((section (".res")));
extern const uint16 res_music_list[]  __attribute__ ((section (".res")));


#endif
