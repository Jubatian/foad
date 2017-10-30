/*
 *  Dragon - Maps (complete levels)
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



#include "map.h"
#include "levelscr.h"
#include "dragon.h"
#include "mapact.h"
#include "mapobjp.h"
#include "gstat.h"
#include <avr/pgmspace.h>



/* Object list location */
static uint16 map_objptr;



/*
** Initializes the passed level by the passed pointer to its structure.
*/
void  map_init(uint16 map)
{
 levelscr_setmap(
     (uint16)(pgm_read_byte(map +  4U)) |
     (uint16)(pgm_read_byte(map +  5U) << 8), /* Map offset */
     pgm_read_byte(map +  6U),                /* Map width */
     pgm_read_byte(map +  7U),                /* Map height */
     pgm_read_byte(map +  8U));               /* Parallax & Sky configuration */

 dragon_setloc(
     (uint16)(pgm_read_byte(map      )) |
     (uint16)(pgm_read_byte(map +  1U) << 8), /* Dragon X */
     (uint16)(pgm_read_byte(map +  2U)) |
     (uint16)(pgm_read_byte(map +  3U) << 8), /* Dragon Y */
     1U);

 dragon_mod(
     DRAGON_STA_HP | DRAGON_P_ADD,
     pgm_read_byte(map +  9U));               /* Initial dragon healing */

 gstat_time =
     pgm_read_byte(map + 10U);                /* Initial remaining time */

 map_objptr = map + 11U;

 mapobjp_init();
}



/*
** Polls map objects to generate active actors as the dragon moves around.
** Should be called periodically.
*/
void  map_pollobjs(void)
{
 uint16 cptr = map_objptr;
 uint16 csrc;
 auint  typ  = 0x00U;
 auint  id   = 0x00U;

 do{
  csrc  = (uint16)(pgm_read_byte(cptr     )) |
          (uint16)(pgm_read_byte(cptr + 1U) << 8);
  cptr += 2U;
  id    = mapact_pollsrc(id, typ | ((csrc >> 8) & 0x40U), (csrc | 0xC000U));
  typ  += 1U;
 }while ((csrc & 0x8000U) == 0U);
}
