/*
 *  Mode 74 configuration
 *  Copyright (C) 2017 Sandor Zsuga (Jubatian)
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


#ifndef M74CFG_H
#define M74CFG_H


/* Where the tile & similar aligned data is placed (not Mode 74 specific) */
/* Note the 0xFFFF - 0xFFFF part: it is to avoid undefined behavior related
** to signed overflow as without an 'U' suffix these are signed. It will turn
** into a long though, which will produce other warnings, but nothing which
** would break things. The 'U' suffix can not be used due to the definitions
** being used in assembler code (where they are not supported). */
#define RES_SECT           (0xFFFF - 0xFFFF + 0x7B00)

/* Text material */
#define TXT_SIZE           2057
#define TXT_TITLE_POS      0
#define TXT_AUTH_POS       25
#define TXT_OVER_POS       78
#define TXT_GAMEEND_POS    1936
#define TXT_GAMEEND_END    2350

/* Resource locations (not Mode 74 specific) */
#define RES_SCR_STORY_OFF  (RES_SECT + 0x0020)
#define RES_TID_STORY_OFF  (RES_SECT + 0x0040)
#define RES_SCR_GAME_OFF   (RES_SECT + 0x0080)
#define RES_TID_GAME_OFF   (RES_SECT + 0x00A0)
#define RES_PAL_00_OFF     (RES_SECT + 0x00E0)
#define RES_TILES_00_OFF   (RES_SECT + 0x0A00)
#define RES_CHARSET_00_OFF (RES_SECT + 0x0200)
#define RES_L0_OFF         (RES_SECT + 0x1B00)
#define RES_L1_OFF         (RES_SECT + 0x2700)
#define RES_L0REPD_OFF     (RES_SECT + 0x3000)
#define RES_SPR_TILES_OFF  (RES_SECT + 0x3100)
#define RES_SPR_OFF        (RES_SECT + 0x4B00)
#define RES_TILEFLG_OFF    (RES_SECT + 0x2F00)
#define RES_SPRIDCONV_OFF  (RES_SECT + 0x4900)
#define RES_SPRIDSIZE_OFF  (RES_SECT + 0x0180)
#define RES_DHEAD_OFF      (RES_SECT + 0x5600)
#define RES_WAVES_OFF      (RES_SECT + 0x5200)
#define RES_TEXT_OFF       (RES_SECT + 0x5680)
#define RES_MAP_OFF        (RES_TEXT_OFF + TXT_SIZE)

/* Others (not Mode 74 specific) */
#define LOC_TDESC_OFF      0x0B80
#define LOC_STATVRAM_OFF   0x0E10
#define LOC_INTXTVRAM_OFF  (M74_VRAM_OFF + 224)
#define LOC_TOPPAL_OFF     0x0BB0
#define LOC_MAINPAL_OFF    0x0BA0
#define LOC_L0REP_OFF      0x0300
#define LOC_RAMTILES_OFF   0x0400

/* Color remappings */
#define REC_STRAIGHT       0x00
#define REC_DRAGON         0x10
#define REC_FLAT_0         0x20
#define REC_FLAT_1         0x30
#define REC_DRAGON_HIT     0x40
#define REC_FIRE           0x50
#define REC_ROCK           0x60

/* Mode 74 specifics */
#define M74_SPR_ENABLE     1
#define M74_MSK_ENABLE     1
#define M74_M2_ENABLE      1
#define M74_ROWS_PTRE      0
#define M74_VRAM_W         25
#define M74_VRAM_H         24
#define M74_ROMTD_OFF      (RES_SECT + 0x0000)
#define M74_RECTB_OFF      (RES_SECT + 0x0100)
#define M74_ROMMASKIDX_OFF (RES_SECT + 0x5180 - (RES_TILES_00_OFF / 32))
#define M74_ROMMASK_OFF    (RES_SECT + 0x1A00)
#define M74_RTLIST_OFF     0x1051
#define M74_ROWS_OFF       0x1000
#define M74_RAMMASKIDX_OFF 0x0FC0
#define M74_XSHIFT_OFF     0x0FA0
#define M74_MAIN_STACK     0x0FA0
#define M74_VIDEO_STACK    0x0EF0
#define M74_PAL_OFF        LOC_TOPPAL_OFF
#define M74_COL0_OFF       0x0E00
#define M74_VRAM_OFF       0x0BB8
#define M74_RESET_ENABLE   1
#define M74_REC_SLOW       1
#define M74_LOGO_WORK      (LOC_RAMTILES_OFF + 0x0400)
#define M74_LOGO_RAMTILES  LOC_RAMTILES_OFF

/* Screen layout elements. The screen:
** - 2 separator lines (just padding color)
** - 8 lines for Status row
** - 1 separator line with palette change afterwards
** - 185 lines for game field (23 rows + 1 line) */
#define RSL_STATUS         (M74_ROWS_OFF + 2)
#define RSL_SEPPAL         (M74_ROWS_OFF + 5)
#define RSL_GAME           (M74_ROWS_OFF + 8)

/* Holes (free areas) in the memory map. See MEMORY_MAP. */
#define MEM_LOC_0          0x10F9
#define MEM_LOC_1          0x10FA
#define MEM_LOC_2          0x10FB
#define MEM_LOC_3          0x10FC
#define MEM_LOC_4          0x10FD
#define MEM_LOC_5          0x10FE
#define MEM_LOC_6          0x10FF
#define MEM_LOC_7          0x0BB3
#define MEM_LOC_8          0x0BB4
#define MEM_LOC_9          0x0BB5
#define MEM_LOC_A          0x0BB6
#define MEM_LOC_B          0x0BB7

#endif
