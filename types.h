/*
 *  Base types and ranges
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



#ifndef TYPES_H
#define TYPES_H


/* 8 bit unsigned integer */
typedef unsigned char uint8;

/* Maximum value for 8 bit unsigned integer */
#define UINT8_H  0xFFU

/* 16 bit unsigned integer */
typedef unsigned int uint16;

/* Maximum value for 16 bit unsigned integer */
#define UINT16_H 0xFFFFU

/* 32 bit unsigned integer */
typedef unsigned long int uint32;

/* Maximum value for 32 bit unsigned integer */
#define UINT32_H 0xFFFFFFFFUL

/* Architecture optimal unsigned integer, at least 8 bits */
typedef uint8 auint;

/* Architecture optimal signed integer, at least 8 bits */
typedef signed char asint;

/* Maximum value for architecture optimal unsigned integer */
#define AUINT_H  UINT8_H


#endif
