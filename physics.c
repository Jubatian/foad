/*
 *  Dragon - Low level physics
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



#include "physics.h"
#include "passable.h"
#include "level.h"
#include "m74cfg.h"



/* Collision flag */
#define PHYSICS_F_COLL  0x80U
/* Ramp flag for ramp following (prevent jumps) */
#define PHYSICS_F_RAMP  0x40U

/* Horizontal collision */
#define PHYSICS_COLL_H  0x01U
/* Vertical collision */
#define PHYSICS_COLL_V  0x02U


/* Gravity divider counter */
static auint physics_gdiv = 0U;





/*
** Advances a frame.
**
** This is used for counting the frames for generating gravity, so call once
** in each logic frame.
*/
void physics_frame(void)
{
 physics_gdiv ++;
 if (physics_gdiv >= PHYSICS_G_DIV){ physics_gdiv = 0U; }
}



/*
** Returns frames until a gravity frame (in which in-air object y velocities
** increase towards ground). This may be used to get deterministic results for
** actions involving gravity.
*/
auint physics_gravrem(void)
{
 auint ret = PHYSICS_G_DIV - physics_gdiv;
 if (ret == PHYSICS_G_DIV){ ret = 0U; }
 return ret;
}



/*
** Checks if the given location is settled on the ground.
** Returns the appropriate flags for it. If the return is nonzero, then the
** sprite is supported on the ground.
**
** This is the check which is done for any downwards movement, using three
** points:
**
** +-------+
** |       |
** X       X
** |       |
** +---X---+
**     :
**     X (Hooking constraint: If this is impassable, assume proper stand)
**
** The bottom constraint is fixed so sprites changing their Y size won't mess
** up here.
*/
static auint physics_getground(uint16 tx, uint16 ty, auint bx, auint by)
{
 auint mid = passable(tx, ty + 1U);
 auint lft;
 auint rgt;

 if (bx == 0U){
  lft = mid;
  rgt = mid;
 }else{
  lft = passable(tx - bx, ty - (by >> 1) + 1U);
  rgt = passable(tx + bx, ty - (by >> 1) + 1U);
 }

 if (mid != 0U){
  if (lft != 0U){
   if (rgt != 0U){ return 0U; }
   else{
    if (passable(tx, ty + 12U + 1U) != 0U){
     return PHYSICS_F_GRND | PHYSICS_F_HK_R;
    }else{
     return PHYSICS_F_GRND;
    }
   }
  }else{
   if (rgt == 0U){ return PHYSICS_F_GRND; }
   else{
    if (passable(tx, ty + 12U + 1U) != 0U){
     return PHYSICS_F_GRND | PHYSICS_F_HK_L;
    }else{
     return PHYSICS_F_GRND;
    }
   }
  }
 }else{
  return PHYSICS_F_GRND;
 }
}



/*
** Processes a sprite.
**
** Pass the sprite structure to process physics for the frame on the sprite.
** - Writes xpos and ypos according to the new position.
** - Writes xvel and yvel according to the constraints.
** - Writes xacc and yacc according to any acceleration (due to impact).
** - Writes flg according to the environment.
**
** In flg it so returns whether the sprite is standing on ground or is
** falling, and whether it succesfully hangs on a climbable tile.
*/
void physics_proc(physics_spr_t* spr)
{
 uint16 tx  = spr->xpos; /* Sprite location */
 uint16 ty  = spr->ypos;
 auint  bx  = spr->xbs;  /* Sprite bounding box */
 auint  by  = spr->ybs;
 auint  byc = by;
 asint  vx  = spr->xvel; /* Work velocities for stepping pixels */
 asint  vy  = spr->yvel;
 asint  vxn = 0;         /* New effective velocities (which affect motion) */
 asint  vyn = 0;
 asint  vxp = 0;         /* Power adjustments for writing back (powered motion) */
 asint  vyp = 0;
 auint  flg = (spr->flg) & (~(PHYSICS_F_COLL | PHYSICS_F_RAMP | PHYSICS_F_HK_L | PHYSICS_F_HK_R));
 auint  xrd = 0U;        /* Whether X velocity was reduced due to ascending */
 auint  cdr = 0U;        /* When collision happens, the direction (H / V) */
 auint  t;

 /* For 0 sized elements, enforce a minimal 2 pixels height for certain
 ** comparisons (to allow these to roll up on a ramp) */

 if (byc < 2U){ byc = 2U; }

 /* Check for standing on ground. */

 if (physics_getground(tx, ty, bx, by) == 0U){
  flg &= ~PHYSICS_F_GRND; /* Not on ground, so can not stick to ground */
 }else{
  flg |=  PHYSICS_F_GRND; /* On ground */
 }

 /* Check for climbing. It is so if the sprite location is on a climbable tile
 ** with a climb request present. Currently not implemented. */

 flg &= ~PHYSICS_F_CLIMB;

 /* If not on ground, the Y velocity increases if the gravity requests so */

 if ( ((flg & PHYSICS_F_GRND) == 0U) &&
      ((flg & PHYSICS_F_CLIMB) == 0U) &&
      ((flg & PHYSICS_F_PWR_Y) == 0U) ){
  if (physics_gdiv == 0U){
   if (vy < 0x20){ vy ++; }
  }
 }

 /* X direction movement check. The checked points are as follows:
 **
 ** +-------X              X-------+
 ** |       |              |       |
 ** |       X ===>    <=== X       |
 ** |       |              |       |
 ** +---X---+              +---X---+
 **
 ** Y direction movement check. The checked points are as follows:
 **
 ** +-------+                  A
 ** |       |                  |
 ** X       X              X-------X
 ** |       |              |       |
 ** +---X---+              |       |
 **     |                  |       |
 **     V                  +-------+
 **
 ** It loops until either X or Y velocities are present. First it does an X
 ** step then an Y step, so ground follow can be implemented from the X step
 ** part. Collisions with map are detected (normal ground follow isn't a
 ** collision). When moving on X if a ramp is hit, then the X velocity is
 ** reduced (to adapt the slope).
 */

 while ((vx != 0) || (vy != 0)){

  if (vy > 0){
   if (physics_getground(tx, ty, bx, by) == 0U){
    flg &= ~PHYSICS_F_GRND;
    ty ++;
    vy --;
    if ((flg & PHYSICS_F_RAMP) == 0U){
     vyn ++;
    }else{
     flg |= PHYSICS_F_GRND;
    }
   }else{
    vy = 0;
    flg |= PHYSICS_F_COLL;
    cdr |= PHYSICS_COLL_V;
    flg |= PHYSICS_F_GRND; /* Fell on ground */
   }
  }

  if (vy < 0){
   flg &= ~PHYSICS_F_GRND;
   if ( (              (passable(tx - bx, ty - byc - 1U) != 0U)) &&
        ((bx == 0U) || (passable(tx + bx, ty - byc - 1U) != 0U)) ){
    ty --;
    vy ++;
    if ((flg & PHYSICS_F_RAMP) == 0U){
     vyn --;
    }else{
     flg |= PHYSICS_F_GRND;
    }
   }else{
    vy = 0;
    flg |= PHYSICS_F_COLL;
    cdr |= PHYSICS_COLL_V;
   }
  }

  flg &= ~PHYSICS_F_RAMP;

  if (vx > 0){
   t = 0U;
   if ( (              (passable(tx + bx + 1U, ty -  byc      ) != 0U)) &&
        ((by == 0U) || (passable(tx + bx + 1U, ty - (byc >> 1)) != 0U)) ){
    t = 1U; /* Didn't hit wall */
   }
   if (t != 0U){
    if ((flg & PHYSICS_F_GRND) != 0U){ /* Ground follow */
     if       (passable(tx + 1U, ty - 1U) == 0U){
      t = 0U;                          /* Hits wall */
     }else if (passable(tx + 1U, ty + 1U) != 0U){
      if (passable(tx + 1U, ty + 2U) != 0U){ /* Fall off on an edge */
       flg &= ~PHYSICS_F_GRND;         /* Not on ground any more */
      }else{                           /* Ramp down */
       flg |= PHYSICS_F_RAMP;
       vy ++;                          /* No X velocity reduction */
      }
     }else if (passable(tx + 1U, ty) == 0U){
      vy --;                           /* Ramp up */
      flg |= PHYSICS_F_RAMP;
      if (xrd == 0U){                  /* Once per frame, X velocity reduction due to ascending */
       vx --;
       if ((flg & PHYSICS_F_PWR_X) != 0U){ vxp ++; }
       xrd = 1U;
      }
     }else{                            /* Normal level ground */
     }
    }else{                             /* In air */
     if (passable(tx + 1U, ty) == 0U){ t = 0U; } /* Hit wall */
    }
   }
   if (t != 0U){ /* If didn't hit wall, then velocities were valid. */
    tx ++;
    if (vx != 0){
     vx  --;
     vxn ++;
    }
   }else{        /* Hit wall, X velocity over. */
    vx = 0;
    flg |= PHYSICS_F_COLL;
    cdr |= PHYSICS_COLL_H;
   }
  }

  if (vx < 0){
   t = 0U;
   if ( (              (passable(tx - bx - 1U, ty -  byc      ) != 0U)) &&
        ((by == 0U) || (passable(tx - bx - 1U, ty - (byc >> 1)) != 0U)) ){
    t = 1U; /* Didn't hit wall */
   }
   if (t != 0U){
    if ((flg & PHYSICS_F_GRND) != 0U){ /* Ground follow */
     if       (passable(tx - 1U, ty - 1U) == 0U){
      t = 0U;                          /* Hits wall */
     }else if (passable(tx - 1U, ty + 1U) != 0U){
      if (passable(tx - 1U, ty + 2U) != 0U){ /* Fall off on an edge */
       flg &= ~PHYSICS_F_GRND;         /* Not on ground any more */
      }else{                           /* Ramp down */
       flg |= PHYSICS_F_RAMP;
       vy ++;                          /* No X velocity reduction */
      }
     }else if (passable(tx - 1U, ty) == 0U){
      vy --;                           /* Ramp up */
      flg |= PHYSICS_F_RAMP;
      if (xrd == 0U){                  /* Once per frame, X velocity reduction due to ascending */
       vx ++;
       if ((flg & PHYSICS_F_PWR_X) != 0U){ vxp --; }
       xrd = 1U;
      }
     }else{                            /* Normal level ground */
     }
    }else{                             /* In air */
     if (passable(tx - 1U, ty) == 0U){ t = 0U; } /* Hit wall */
    }
   }
   if (t != 0U){ /* If didn't hit wall, then velocities were valid. */
    tx --;
    if (vx != 0){
     vx  ++;
     vxn --;
    }
   }else{        /* Hit wall, X velocity over. */
    vx = 0;
    flg |= PHYSICS_F_COLL;
    cdr |= PHYSICS_COLL_H;
   }
  }

 }

 flg &= ~PHYSICS_F_RAMP;

 /* Add flags if standing or hooked on the ground */

 flg &= ~PHYSICS_F_GRND;
 flg |= physics_getground(tx, ty, bx, by);

 /* Apply results */

 spr->xacc = 0;
 spr->yacc = 0;
 if ((flg & PHYSICS_F_COLL) != 0U){
  if (cdr & PHYSICS_COLL_H){ spr->xacc = -(spr->xvel); }
  if (cdr & PHYSICS_COLL_V){ spr->yacc = -(spr->yvel); }
 }
 spr->xvel = vxn + vxp;
 spr->yvel = vyn + vyp;
 spr->xpos = tx;
 spr->ypos = ty;
 spr->flg  = flg;
}
