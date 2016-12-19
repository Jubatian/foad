/*
 *  Dragon - Simple 16 bit full sequence PRNG
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
** A simple 16 bit full sequence PRNG, using the "5 byte PRNG" concept.
**
** The base concept is as follows:
**
** lsl reg
** adc reg, NUM1
** eor reg, NUM2
**
** This on a 6502 takes 5 bytes, and iterates "reg" in a pseudorandom manner
** over all 256 possible values with the right NUM1 and NUM2 combination.
**
** This PRNG here is a 16 bit adaptation with the following constants:
**
** NUM1 = 0x7F25
** NUM2 = 0x00DB
**
** This is the best full sequence where NUM2 is below 256.
**
** No seeding is provided, variation may be ensured by a dummy fetch in every
** frame (so the unpredictable timing of the user's actions will ensure
** variation).
*/


#ifndef RANDOM_H
#define RANDOM_H



#include "types.h"


/*
** Gets next number of sequence. For asm use, r22 and r23 are clobbered.
*/
uint16 random_get(void);


#endif
