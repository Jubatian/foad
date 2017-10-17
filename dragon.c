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



#include "dragon.h"
#include "levelscr.h"
#include "sprite.h"
#include "spritelv.h"
#include "actordim.h"
#include "fireball.h"
#include "sound.h"
#include "m74cfg.h"
#include "global.h"
#include <avr/pgmspace.h>
#include <uzebox.h>



/* Scrolling map center */
#define MAP_CX       96U
#define MAP_CY       92U

/* Individual dragon actions. The input commands translate to these. */
/* Standing, breathing animation. */
#define DI_STAND     0x00U
/* Sitting, breathing animation. */
#define DI_SIT       0x01U
/* Laying, breathing animation. */
#define DI_LAY       0x02U
/* Walking, animation cycle. */
#define DI_WALK      0x03U
/* Turn around, animation cycle. */
#define DI_TURN      0x04U
/* Running, animation cycle. */
#define DI_RUN       0x05U
/* Jump preparation, animation sequence. */
#define DI_JUMPP     0x06U
/* Air, frames depend on velocity components. */
#define DI_AIR       0x07U

/* Dragon command flags */
/* LEFT orientation. If set, left, if clear, right */
#define DF_LEFT      0x01U
/* DOWN request active. */
#define DF_DOWN      0x02U
/* UP request active. */
#define DF_UP        0x04U
/* Transition block present (dragon can't proceed onto other sequence) */
#define DF_TBLK      0x08U
/* Map follow present */
#define DF_FOLLOW    0x10U
/* Hooks on an edge */
#define DF_HOOK      0x20U
/* Jump command blocked (need to release and press input) */
#define DF_JBLOCK    0x40U

/* Dragon hit timeout (how many red & nonsilent logic frames) */
#define DT_HIT       4U



/* Dragon sprite props (exposed as read-only global) */
physics_spr_t dragon_spr;

/* Dragon status (exposed as read-only global) */
dragon_stat_t dragon_stat;

/* Dragon's effective action */
static auint dragon_act;

/* Dragon's command flags. */
static auint dragon_flg;

/* Current frame of an animation cycle */
static auint dragon_frame;

/* Counter for next frame */
static auint dragon_frctr;

/* Timeout for sit down / lay */
static auint dragon_idlet;

/* Timeout after fire spit */
static auint dragon_firet;

/* Timeout after dragon was hit (dragon flashes red) */
static auint dragon_hitt;

/* Last head relative positions, used for fireball launch */
static asint dragon_hx;
static asint dragon_hy;

/* Acceleration timer */
static auint dragon_acct;

/* Jump power */
static auint dragon_jppw;




/*
** Logic frame processing
**
** Should be called in a logic frame to control the dragon. The 'cmd'
** parameter is for passing the controls ('DRAGON_xxx' defines).
*/
void dragon_logic(auint cmd)
{
 asint vtg = 0;
 auint acc = 0U;
 auint act = DI_STAND;
 auint air = 0U;
 auint vab;
 asint tvc;
 auint t;


 dragon_acct = (dragon_acct + 1U) & 0x03U;


 /* Fire breath: always possible if the dragon has energy for it */

 if (dragon_firet != 0U){
  dragon_firet --;
 }else{
  if ( ((cmd & DRAGON_FIRE) != 0U) &&  /* Commanded? */
       (dragon_stat.fi >= 24U) ){      /* Has energy? */
   dragon_stat.fi -= 24U;              /* Energy cost */
   dragon_firet = (FIREBALL_AGE / (FIREBALL_N - 4U));
   vtg = dragon_spr.xvel;
   tvc = dragon_spr.yvel;
   if ((dragon_flg & DF_LEFT) != 0U){ vtg -= 5; }
   else                             { vtg += 5; }
   if ((cmd & DRAGON_UP)      != 0U){ tvc -= 4; }
   if ((cmd & DRAGON_DOWN)    != 0U){ tvc += 2; }
   fireball_new(dragon_spr.xpos + dragon_hx,
                dragon_spr.ypos + dragon_hy,
                vtg,
                tvc);
   sound_effect(SOUND_FIRE, 0x40U);
  }
 }


 /* Up and down commands are always active and effective */

 if ((cmd & DRAGON_UP)   != 0U){ dragon_flg |=  DF_UP; }
 else                          { dragon_flg &= ~DF_UP; }
 if ((cmd & DRAGON_DOWN) != 0U){ dragon_flg |=  DF_DOWN; }
 else                          { dragon_flg &= ~DF_DOWN; }

 /* Jump command needs to be released to start another jump */

 if ((cmd & DRAGON_JUMP) == 0U){ dragon_flg &= ~DF_JBLOCK; }

 /* If a jump command is present, then increase power for up to 20 logic
 ** frames (this is also the max. jump capability when added 7 then divided by
 ** 4) */

 if       ((cmd & DRAGON_JUMP) != 0U){
  if (dragon_jppw < 8U){ dragon_jppw ++; }
 }else if (dragon_act != DI_JUMPP){
  dragon_jppw = 0U;
 }


 /* Get current absolute velocity */

 if (dragon_spr.xvel < 0){ vab = (auint)(-dragon_spr.xvel); }
 else                    { vab = (auint)( dragon_spr.xvel); }


 /* Determine next action and target velocities */

 if ((dragon_spr.flg & PHYSICS_F_GRND) != 0U){ /* On ground */

  /* Horizontal movements */

  if ( ((cmd & DRAGON_MOVE) == 0U) &&
       ((cmd & DRAGON_RUN) != 0U) ){
   cmd &= ~DRAGON_RUN; /* Remove running if not moving */
  }

  if ( ((cmd & DRAGON_JUMP) != 0U) &&
       ((dragon_flg & DF_JBLOCK) == 0U) &&
       (dragon_act == DI_RUN) ){ /* Jumping: keep running if was running */
   cmd |= DRAGON_RUN;
   cmd |= DRAGON_MOVE;
  }

  if       ( ( ((cmd & DRAGON_LEFT) != 0U) &&
               ((dragon_flg & DF_LEFT) == 0U) ) ||
             ( ((cmd & DRAGON_RIGHT) != 0U) &&
               ((dragon_flg & DF_LEFT) != 0U) ) ){ /* Orientation differs */
   vtg = 0;          /* Velocity target is zero (need to turn around) */
   acc = 2U;         /* Tries to decelerate swiftly */
   act = DI_TURN;
  }else if ( ((cmd & DRAGON_JUMP) != 0U) &&
             ((dragon_flg & DF_JBLOCK) == 0U) ){ /* Start jumping */
   if ( (vab != 0) ||
        (physics_gravrem() == 2U) ){    /* When not running, wait for a gravity frame (considering when the jump will start) */
    act = DI_JUMPP;                     /* (to get a deterministic result these cases) */
   }
  }else if ((cmd & DRAGON_RUN ) != 0U){
   vtg = 4;
   if (dragon_stat.en < 16U){ vtg --; } /* Slower with low energy */
   acc = 1U;
   act = DI_RUN;
  }else if ((cmd & DRAGON_MOVE) != 0U){
   vtg = 1;
   acc = 1U;
   act = DI_WALK;
  }else{
   vtg = 0;
   acc = 1U;
   if       (vab >  2U){
    act = DI_RUN;
   }else if ((dragon_flg & DF_DOWN) != 0U){
    act = DI_LAY;
   }else if (vab != 0U){
    act = DI_WALK;
   }else if ( (dragon_act != DI_SIT) &&
              (dragon_act != DI_LAY) ){
    act = DI_STAND;
   }else{
    act = dragon_act;
   }
  }

  if ((dragon_flg & DF_LEFT) != 0U){ vtg = -vtg; }

  /* Velocity can only increment if already running or walking */

  if ( (dragon_act != DI_RUN) &&
       (dragon_act != DI_WALK) ){
   if ( (dragon_spr.xvel >= 0) &&
        (dragon_spr.xvel <  vtg) ){ vtg = dragon_spr.xvel; }
   if ( (dragon_spr.xvel <= 0) &&
        (dragon_spr.xvel >  vtg) ){ vtg = dragon_spr.xvel; }
  }

  /* Idle transitions to sitting / laying down */

  if (dragon_idlet != 0U){
   dragon_idlet --;
  }else{
   if      (dragon_act == DI_STAND){ act = DI_SIT; }
   else if (dragon_act == DI_SIT)  { act = DI_LAY; }
  }

  /* Sequences (target alterations when the dragon can only start it going
  ** through a sequence of other actions) */

  if ( (act == DI_TURN) ||
       (act == DI_WALK) ){
   if ( (dragon_act == DI_SIT) ||
        (dragon_act == DI_LAY) ){ act = DI_STAND; }
  }
  if (act == DI_STAND){
   if ( (dragon_act == DI_LAY) ){ act = DI_SIT; }
  }

 }else{              /* In air */

  act = DI_AIR;

  if ( ( ((cmd & DRAGON_LEFT) != 0U) &&
         ((dragon_flg & DF_LEFT) == 0U) ) ||
       ( ((cmd & DRAGON_RIGHT) != 0U) &&
         ((dragon_flg & DF_LEFT) != 0U) ) ){ /* Orientation differs */
   vtg = -1;         /* Velocity target is opposing (need to turn around) */
   acc = 1U;
  }else if ( ((cmd & DRAGON_MOVE) != 0U) ){
   vtg = 4;
   acc = 1U;
  }else{
   vtg = 0;
   acc = 0U;
  }

  if ((dragon_flg & DF_LEFT) != 0U){ vtg = -vtg; }

 }


 /* Apply sequence change if possible. Also consume energy if a costly
 ** transition is performed. (Maybe...) */

 if ( ((dragon_flg & DF_TBLK) == 0U) &&
      (dragon_act != act) ){
  if ( (act == DI_JUMPP) &&
       (dragon_act == DI_RUN) ){ /* Running to jumping transition */
   dragon_frame = 1U;
  }else{
   dragon_frame = 0U;
  }
  dragon_frctr = 0U;
  dragon_flg  |= DF_TBLK;
 }else{
  act = dragon_act;
 }


 /* Process sequences (dragon_act will be updated after it, so the
 ** act != dragon_act condition can check for entry). */

 t = dragon_frctr;
 tvc = 0;            /* Temporary velocity change (if an animation demands) */

 switch (act){

  case DI_TURN:

   if       (dragon_frctr == 0x00U){
    dragon_frame = 0U;
   }else if (dragon_frctr == 0x40U){
    dragon_frame = 1U;
    tvc = -2;
    if ((dragon_flg & DF_LEFT) != 0U){ tvc = -tvc; }
   }else if (dragon_frctr == 0x80U){
    dragon_frame = 2U;
    tvc = -3;
    if ((dragon_flg & DF_LEFT) != 0U){ tvc = -tvc; }
   }else if (dragon_frctr == 0xC0U){
    dragon_frame = 3U;
    tvc = -2;
    if ((dragon_flg & DF_LEFT) != 0U){ tvc = -tvc; }
    dragon_flg  ^= DF_LEFT; /* Dragon turns around here by graphics */
   }
   dragon_frctr += 0x10U;
   if (dragon_frctr == 0U){ dragon_flg  &= ~DF_TBLK; }
   break;

  case DI_STAND:
  case DI_SIT:
  case DI_LAY:

   if (dragon_act != act){ /* Transition happened, so start idle timer */
    dragon_idlet = 0xFFU;
   }
   dragon_frctr += 30U; /* Temp, will depend on energy */
   if (t > dragon_frctr){
    dragon_frame = (dragon_frame + 1U) & 1U;
    dragon_flg &= ~DF_TBLK;
   }
   break;

  case DI_WALK:

   dragon_frctr += 0x40U * vab;
   if (t > dragon_frctr){
    dragon_frame = (dragon_frame + 1U) & 3U;
   }
   dragon_flg &= ~DF_TBLK;
   break;

  case DI_RUN:

   if (dragon_stat.en != 0U){ dragon_stat.en --; } /* Energy: Can't recharge */
   dragon_frctr += 0x20U * vab;
   if (t > dragon_frctr){
    dragon_frame ++;
    if (dragon_frame >= 6U){ dragon_frame = 0U; }
   }
   dragon_flg &= ~DF_TBLK;
   break;

  case DI_JUMPP:

   if (dragon_stat.en != 0U){ dragon_stat.en --; } /* Energy: Can't recharge */
   dragon_frctr += 0x40U;
   if (t > dragon_frctr){
    if (dragon_frame < 3U){
     dragon_frame ++;
     if (dragon_frame == 3U){ /* Transition to air with starting velocity (max speed run gives higher jumps) */
      dragon_spr.yvel = -(asint)((dragon_jppw + 3U + ((vab + 3U) >> 2)) >> 1);
      if (dragon_stat.en < 16U){
       if (dragon_spr.yvel < -1){ dragon_spr.yvel ++; } /* Low energy, less jumping */
      }
      if (dragon_stat.en >= 32U){
       dragon_stat.en -= 32U; /* Jumping energy cost */
      }else{
       dragon_stat.en  =  0U;
      }
      if (vab < 2U){          /* Jumping always have a base horizontal velocity */
       if ((dragon_flg & DF_LEFT) != 0U){ dragon_spr.xvel = -2; }
       else                             { dragon_spr.xvel =  2; }
      }
      air          = 1U;
      dragon_flg  &= ~DF_TBLK;
      dragon_flg  |=  DF_JBLOCK;
     }
    }
   }
   break;

  case DI_AIR:

   if (dragon_stat.en != 0U){ dragon_stat.en --; } /* Energy: Can't recharge */
   if ( (dragon_spr.xvel == 0) &&
        ( ( ((dragon_flg & DF_LEFT) != 0U) &&
            (vtg > 0) ) ||
          ( ((dragon_flg & DF_LEFT) == 0U) &&
            (vtg < 0) ) ) ){
    dragon_flg ^= DF_LEFT; /* Turn around */
   }
   dragon_flg &= ~DF_TBLK;
   break;

  default:
   dragon_flg &= ~DF_TBLK;
   break;
 }

 dragon_act = act;

 /* Apply velocity changes. Physics is applied here as well. */

 if ((acc != 0U) && (dragon_acct == 0U)){
  if      (dragon_spr.xvel < vtg){ dragon_spr.xvel++; }
  else if (dragon_spr.xvel > vtg){ dragon_spr.xvel--; }
 }
 if ( ((dragon_spr.flg & PHYSICS_F_HK_R) != 0U) ||
      ((dragon_spr.flg & PHYSICS_F_HK_L) != 0U) ){ /* Hooking */
  if ((dragon_flg & DF_LEFT) != 0U){ dragon_spr.xvel = -2; }
  else                             { dragon_spr.xvel =  2; }
 }
 if (air != 0U){
  dragon_spr.flg = PHYSICS_F_PWR_X;
 }else{
  dragon_spr.flg = PHYSICS_F_GRND | PHYSICS_F_PWR_X;
 }
 dragon_spr.xvel += tvc;
 actordim_get(act | 0x80U, &dragon_spr);
 if ((dragon_flg & DF_HOOK) != 0U){
  dragon_spr.ybs = 16U; /* Hooking height is same as shortest movement frame's */
 }
 physics_proc(&dragon_spr);
 dragon_spr.xvel -= tvc;
 if ( (air != 0U) &&
      (dragon_spr.yvel >= 0) ){ /* Unsuccessful jump */
  dragon_act = DI_STAND;        /* Cancel it */
 }

 /* Apply hooks on edge if any */

 dragon_flg &= ~DF_HOOK;
 if ( (dragon_act != DI_JUMPP) &&
      (dragon_act != DI_TURN) &&
      (dragon_act != DI_AIR) &&
      (dragon_spr.xvel == 0) ){
  if ((dragon_flg & DF_LEFT) != 0U){
   if ((dragon_spr.flg & PHYSICS_F_HK_L) != 0U){ dragon_flg |= DF_HOOK; }
  }else{
   if ((dragon_spr.flg & PHYSICS_F_HK_R) != 0U){ dragon_flg |= DF_HOOK; }
  }
 }

 /* Recharges */

 t = ((dragon_stat.cap & 0x0CU) << 4) | 0x3FU;
 if (dragon_stat.en < t){
  dragon_stat.en ++;
 }
 if (dragon_stat.en < t){ /* Extra charging when upgraded (significant!) */
  if ( (               ((global_framectr & 0x1FU) == 0U)) ||
       ((t >= 0x80U) & ((global_framectr & 0x0FU) == 0U)) ||
       ((t >= 0xC0U) & ((global_framectr & 0x07U) == 0U)) ){
   dragon_stat.en ++;
  }
 }
 if (dragon_stat.fi < (((dragon_stat.cap & 0x30U) << 2) | 0x3FU)){
  dragon_stat.fi ++;
 }

}



/*
** Render frame processing
**
** Should be called in a render frame to produce the dragon sprite on the
** map.
*/
void dragon_render(void)
{
 auint  t;
 asint  mx = -32; /* Map offsets from center. Defaults. */
 asint  my = -24;
 auint  act = dragon_act;
 auint  fra = dragon_frame << 1;
 auint  rec;

 if ((dragon_flg & DF_UP)   != 0U){ my = -56; }
 if ((dragon_flg & DF_DOWN) != 0U){ my =  32; }

 /* Graphics alterations */

 if ( (act == DI_WALK) ||
      (act == DI_RUN) ){
  if (dragon_spr.xvel == 0){
   act = DI_STAND;
   fra = 0U;
  }
 }

 /* Select frame */

 switch (act){

  case DI_STAND:
   fra += 0x08U;
   break;

  case DI_SIT:
   fra += 0x0CU;
   break;

  case DI_LAY:
   fra += 0x10U;
   break;

  case DI_WALK:
   fra += 0x00U;
   break;

  case DI_TURN:
   fra += 0x14U;
   mx  = 0;
   break;

  case DI_RUN:
   fra += 0x1CU;
   break;

  case DI_JUMPP:
   fra += 0x2AU;
   break;

  case DI_AIR:
   if (dragon_spr.xvel < 0){ t = (auint)(-dragon_spr.xvel); }
   else{                     t = (auint)( dragon_spr.xvel); }
   if       (-dragon_spr.yvel >= 3){
    fra = 0x30U;
   }else if (-dragon_spr.yvel >= 0){
    fra = (auint)(0x36 + (dragon_spr.yvel * 2));
   }else if (-dragon_spr.yvel >  -(asint)((((t + 3U) >> 2)) * 3U)){
    fra = 0x38U;
   }else if (-dragon_spr.yvel >  -(asint)((((t + 3U) >> 1)) * 3U)){
    fra = 0x3AU;
    if (my <  0){ my  =  0; }
   }else{
    fra = 0x3CU;
    if (my < 24){ my  = 24; }
   }
   break;

  default:
   fra = 0U;
   break;

 }

 /* Hook overrides (if dragon is hanging on an edge) */

 if ((dragon_flg & DF_HOOK) != 0U){
  fra = 0x3EU;
 }

 /* Looking up: select appropriate frame */

 if ((dragon_flg & DF_UP) != 0U){ fra |= 1U; }

 /* Store head relative locations for fire spits */

 dragon_hx =  (asint)(pgm_read_byte((uint16)(RES_DHEAD_OFF) + (fra << 1) + 0U));
 dragon_hy = -(asint)(pgm_read_byte((uint16)(RES_DHEAD_OFF) + (fra << 1) + 1U));

 /* Process map location & blit */

 t = M74_SPR_I3 | M74_SPR_MASK;
 if ((dragon_flg & DF_LEFT) == 0U){
  t |= M74_SPR_FLIPX;
  mx = -mx;
  if (dragon_spr.xvel >  0){ mx =   64; }
  if (dragon_spr.xvel >  1){ mx =   80; }
  if (dragon_spr.xvel >  2){ mx =  100; }
  if (dragon_spr.xvel >  3){ mx =  120; }
 }else{
  dragon_hx = -dragon_hx;
  if (dragon_spr.xvel <  0){ mx =  -64; }
  if (dragon_spr.xvel < -1){ mx =  -80; }
  if (dragon_spr.xvel < -2){ mx = -100; }
  if (dragon_spr.xvel < -3){ mx = -120; }
 }

 if (dragon_hitt != 0U){
  rec = REC_DRAGON_HIT;
  dragon_hitt --;
 }else{
  rec = REC_DRAGON;
 }
 spritelv_blit(&dragon_spr, fra, t, rec);

 if ((dragon_flg & DF_FOLLOW) != 0U){
  levelscr_settarget( dragon_spr.xpos - MAP_CX + (uint16)(mx),
                      dragon_spr.ypos - MAP_CY + (uint16)(my) );
 }
}



/*
** Returns whether the dragon is silent. This is so when he is idling or
** walking. Can be used by enemies to check whether they may hear him.
*/
auint dragon_issilent(void)
{
 if (dragon_firet != 0U){ return 0U; } /* Firing */
 if (dragon_hitt  != 0U){ return 0U; } /* Dragon is hit - attract */
 if ( (dragon_act == DI_SIT) ||
      (dragon_act == DI_LAY) ||
      (dragon_act == DI_STAND) ||
      (dragon_act == DI_WALK) ||
      (dragon_act == DI_TURN) ){ return 1U; } /* These actions are silent */
 return 0U;
}



/*
** Enables (nonzero) / disables (zero) map following
**
** If enabled, the map scrolls to follow the dragon, otherwise it doesn't.
*/
void dragon_follow(auint ena)
{
 if (ena != 0U){
  dragon_flg |=  DF_FOLLOW;
 }else{
  dragon_flg &= ~DF_FOLLOW;
 }
}



/*
** Internal function for evaulating a dragon parameter command
*/
static auint dragon_mod_ecmd(auint sel, auint val, auint cur, auint cap)
{
 if       ((sel & DRAGON_P_SUB) != 0U){
  if (       cur  > val){ cur -= val; }
  else                  { cur  = 0U;  }
 }else if ((sel & DRAGON_P_ADD) != 0U){
  if ((cap - cur) > val){ cur += val; }
  else                  { cur  = cap; }
 }else{
  if ( cap        > val){ cur  = val; }
  else                  { cur  = cap; }
 }
 return cur;
}



/*
** Modify a dragon parameter
**
** This can be used for example to apply damage on the dragon (note that
** physics itself applies damage on its own). Capacities range from 0 to 3.
** In 'sel' the direction and the parameter to modify has to be supplied.
** Multiple parameters may be adjusted at once.
*/
void dragon_mod(auint sel, auint val)
{
 auint tv;
 auint tc;

 if ((sel & DRAGON_CAP_HP) != 0U){
  tv = (dragon_stat.cap     ) & 0x03U;
  tv = dragon_mod_ecmd(sel, val, tv, 3U);
  dragon_stat.cap = (dragon_stat.cap & (~0x03U)) | (tv     );
  tv = (tv << 6) | 0x3FU;
  if (dragon_stat.hp > tv){ dragon_stat.hp = tv; }
 }

 if ((sel & DRAGON_CAP_EN) != 0U){
  tv = (dragon_stat.cap >> 2) & 0x03U;
  tv = dragon_mod_ecmd(sel, val, tv, 3U);
  dragon_stat.cap = (dragon_stat.cap & (~0x0CU)) | (tv << 2);
  tv = (tv << 6) | 0x3FU;
  if (dragon_stat.en > tv){ dragon_stat.en = tv; }
 }

 if ((sel & DRAGON_CAP_FI) != 0U){
  tv = (dragon_stat.cap >> 4) & 0x03U;
  tv = dragon_mod_ecmd(sel, val, tv, 3U);
  dragon_stat.cap = (dragon_stat.cap & (~0x30U)) | (tv << 4);
  tv = (tv << 6) | 0x3FU;
  if (dragon_stat.fi > tv){ dragon_stat.fi = tv; }
 }

 if ((sel & DRAGON_STA_HP) != 0U){
  tv = dragon_stat.hp;
  tc = ((dragon_stat.cap & 0x03U) << 6) | 0x3FU;
  tv = dragon_mod_ecmd(sel, val, tv, tc);
  if (tv < dragon_stat.hp){
   dragon_hitt = DT_HIT;
  }
  dragon_stat.hp = tv;
 }

 if ((sel & DRAGON_STA_EN) != 0U){
  tv = dragon_stat.en;
  tc = ((dragon_stat.cap & 0x0CU) << 4) | 0x3FU;
  tv = dragon_mod_ecmd(sel, val, tv, tc);
  dragon_stat.en = tv;
 }

 if ((sel & DRAGON_STA_FI) != 0U){
  tv = dragon_stat.fi;
  tc = ((dragon_stat.cap & 0x30U) << 2) | 0x3FU;
  tv = dragon_mod_ecmd(sel, val, tv, tc);
  dragon_stat.fi = tv;
 }
}



/*
** Relocate the dragon
**
** This may be used when starting a new map, to set the initial position. It
** also resets whatever action was ongoing. Location is in pixels. If map
** following is enabled, the map is also relocated to the dragon.
*/
void dragon_setloc(uint16 x, uint16 y, auint fol)
{
 dragon_spr.xpos = x;
 dragon_spr.ypos = y;
 dragon_spr.xvel = 0U;
 dragon_spr.yvel = 0U;
 dragon_spr.flg  = 0U;
 dragon_idlet    = 0xFFU;
 dragon_acct     = 0x00U;
 dragon_act      = DI_STAND;
 dragon_flg      = 0U;
 dragon_hitt     = 0U;
 if (fol != 0U){
  dragon_flg    |= DF_FOLLOW;
  levelscr_setloc(x - MAP_CX, y - MAP_CY);
 }
}

