/*
 *  Dragon - Dragon character
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
** Handles the dragon character.
**
** This involves the logic of the dragon sprite (frames) and the actions as
** far as they relate the dragon itself. The dragon's condition is also
** handled here, related to the actions.
*/



#ifndef DRAGON_H
#define DRAGON_H


#include "physics.h"


/*
** Dragon orientation for command. If neither is given, the previous is
** preserved.
*/
#define DRAGON_RIGHT 0x80U
#define DRAGON_LEFT  0x40U

/*
** Dragon movement request. If active, the dragon moves depending on
** orientation and action. If in air, he tries to control fall direction.
*/
#define DRAGON_MOVE  0x20U

/*
** Jump preparation. When this action is active, the dragon prepares for
** jumping, which happens when it releases. The power of the jump depends on
** the preparation time (and energy). If any MOVE was applied, it will be
** stuck until the release.
*/
#define DRAGON_JUMP  0x10U

/*
** Running. Requests the dragon to run when moving.
*/
#define DRAGON_RUN   0x08U

/*
** Vertical requests. Down requests the dragon to lay down, to look or aim
** downwards. Up the opposite. These are also used for adjusting field of
** vision vertically.
*/
#define DRAGON_UP    0x04U
#define DRAGON_DOWN  0x02U

/*
** Fire. Requests preparing a fire spit.
*/
#define DRAGON_FIRE  0x01U

/*
** Dragon parameter identifications, for modifying with dragon_mod()
*/
#define DRAGON_STA_HP  0x01U
#define DRAGON_STA_EN  0x02U
#define DRAGON_STA_FI  0x04U
#define DRAGON_CAP_HP  0x08U
#define DRAGON_CAP_EN  0x10U
#define DRAGON_CAP_FI  0x20U

/*
** Parameter modification direction. If neither ADD or SUB is applied, then it
** replaces.
*/
#define DRAGON_P_ADD   0x40U
#define DRAGON_P_SUB   0x80U


/* Dragon status structure. Note that position is provided in a physics sprite
** structure. */
typedef struct{
 auint hp;           /* Health */
 auint en;           /* Energy */
 auint fi;           /* Fire breath reserve */
 auint cap;          /* Capacities:
                        bit 0-1: Health
                        bit 2-3: Energy
                        bit 4-5: Fire
                        0: 0x3F
                        1: 0x7F
                        2: 0xBF
                        3: 0xFF */
}dragon_stat_t;



/*
** Logic frame processing
**
** Should be called in a logic frame to control the dragon. The 'cmd'
** parameter is for passing the controls ('DRAGON_xxx' defines).
*/
void dragon_logic(auint cmd);


/*
** Render frame processing
**
** Should be called in a render frame to produce the dragon sprite on the
** map.
*/
void dragon_render(void);


/*
** Dragon sprite. Read only!
*/
extern physics_spr_t dragon_spr;


/*
** Dragon status. Read only!
*/
extern dragon_stat_t dragon_stat;


/*
** Returns whether the dragon is silent. This is so when he is idling or
** walking. Can be used by enemies to check whether they may hear him.
*/
auint dragon_issilent(void);


/*
** Enables (nonzero) / disables (zero) map following
**
** If enabled, the map scrolls to follow the dragon, otherwise it doesn't.
*/
void dragon_follow(auint ena);


/*
** Modify a dragon parameter
**
** This can be used for example to apply damage on the dragon (note that
** physics itself applies damage on its own). Capacities range from 0 to 3.
** In 'sel' the direction and the parameter to modify has to be supplied.
** Multiple parameters may be adjusted at once.
*/
void dragon_mod(auint sel, auint val);


/*
** Relocate the dragon
**
** This may be used when starting a new map, to set the initial position. It
** also resets whatever action was ongoing. Location is in pixels. If map
** following is enabled, the map is also relocated to the dragon.
*/
void dragon_setloc(uint16 x, uint16 y, auint fol);


#endif
