/*
 *  Dragon - Palette render
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
** Renders palettes including the background rasters.
**
** Note that it needs levelscr to determine current Y location, used for
** aligning the background. So the map has to be positioned (scrolled) before
** the palette render for it is called.
**
** It creates the palettes (status row and main game field), and the
** background pattern. Various fadings may be applied.
*/



#ifndef PAL_H
#define PAL_H


#include "types.h"


/* Sky types */

#define PAL_SKY_DAY    0U

/* Top palette types (Black & White / Yellow & Red) */

#define PAL_TOP_BW     0U
#define PAL_TOP_YR     1U



/*
** Renders the sky. The parameter is the sky type to render.
*/
void pal_sky_render(auint sky);


/*
** Renders the sky with fading. The "sky" parameter is the sky type to render.
** "tg" is the target color to fade towards, "rat" is the ratio; 0: no fading,
** 255: maximal fading.
*/
void pal_sky_render_fade(auint sky, auint tg, auint rat);


/*
** Renders the top palette.
*/
void pal_top_render(auint typ);


/*
** Renders the top palette with fading. "tg" is the target color to fade
** towards, "rat" is the ratio; 0: no fading, 255: maximal fading.
*/
void pal_top_render_fade(auint typ, auint tg, auint rat);


/*
** Renders the playfield palette.
*/
void pal_pf_render(void);


/*
** Renders the playfield palette with fading. "tg" is the target color to fade
** towards, "rat" is the ratio; 0: no fading, 255: maximal fading.
*/
void pal_pf_render_fade(auint tg, auint rat);


#endif
