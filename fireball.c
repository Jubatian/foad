/*
 *  Dragon - Fireballs (dragon's breath)
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



#include "fireball.h"
#include "levelcor.h"
#include "spritelv.h"
#include "levelscr.h"
#include "gstat.h"
#include "sound.h"
#include <uzebox.h>



/* Fireball descriptor */
typedef struct{
 auint btim;
 asint xvel;
 asint yvel;
 uint8 lcor[3];
}fireball_t;


/* Fireball storage. This is global (no "static") only so the companion asm
** module (firebals.s) can use it. It is not exposed in the header. */
fireball_t fireball_arr[FIREBALL_N];

/* X velocity decay timer (air resistance) */
static auint fireball_dtim;



/*
** Initializes the storage with all fireballs cleared (free).
*/
void  fireball_init(void)
{
 auint i;

 for (i = 0U; i < FIREBALL_N; i++){
  fireball_arr[i].btim = 0U;
 }
}



/*
** Creates a new fireball if possible. If not possible, nothing happens.
*/
void  fireball_new(uint16 x, uint16 y, asint xvel, asint yvel)
{
 auint i;

 for (i = 0U; i < FIREBALL_N; i++){
  if (fireball_arr[i].btim == 0U){ break; }
 }

 if (i != FIREBALL_N){
  fireball_arr[i].btim = FIREBALL_AGE;
  fireball_arr[i].xvel = xvel;
  fireball_arr[i].yvel = yvel;
  levelcor_encode(&(fireball_arr[i].lcor[0]), x, y);
 }
}



/*
** Ages a given fireball, accelerating its burnout. A fireball may be
** destroyed (removed) by bumping its burnout by 0xFF.
*/
void  fireball_age(auint id, auint age)
{
 if (fireball_arr[id].btim < age){
  fireball_arr[id].btim  = 0U;
 }else{
  fireball_arr[id].btim -= age;
 }
}



/*
** Retrieve a fireball structure for reading, possibly used for collisions,
** interactions between sprites. Returns nonzero if the fireball doesn't
** exist.
*/
auint fireball_get(auint id, physics_spr_t* dest)
{
 if (fireball_arr[id].btim == 0U){ return 1U; }

 dest->xvel = fireball_arr[id].xvel;
 dest->yvel = fireball_arr[id].yvel;
 levelcor_decode(&(fireball_arr[id].lcor[0]), &(dest->xpos), &(dest->ypos));
 return 0U;
}



/*
** Processes fireballs (only the physics, not collisions)
*/
void  fireball_process(void)
{
 auint  i;
 physics_spr_t ph;
 uint16 tx;
 uint16 ty;
 auint  t0;
 auint  t1;

 fireball_dtim ++;

 for (i = 0U; i < FIREBALL_N; i++){

  if (fireball_arr[i].btim != 0U){
   fireball_arr[i].btim--;

   ph.xbs  = 0U;
   ph.ybs  = 0U;
   ph.xvel = fireball_arr[i].xvel;
   ph.yvel = fireball_arr[i].yvel;
   ph.flg  = 0U;

   levelcor_decode(&(fireball_arr[i].lcor[0]), &(ph.xpos), &(ph.ypos));
   physics_proc(&ph);
   levelcor_encode(&(fireball_arr[i].lcor[0]), ph.xpos, ph.ypos);

   if      (ph.xacc < 0){ ph.xvel = -((-ph.xacc) >> 2); }
   else if (ph.xacc > 0){ ph.xvel =  (( ph.xacc) >> 2); }
   if      (ph.yacc < 0){ ph.yvel = -((-ph.yacc) >> 2); }
   else if (ph.yacc > 0){ ph.yvel =  (( ph.yacc) >> 2); }

   if ( ((fireball_dtim & 0xFU) == 0U) ||      /* Air resistance */
        ( ((ph.flg & PHYSICS_F_GRND) != 0U) && /* Ground resistance */
          ((fireball_dtim & 0x3U) == 0U) ) ){
    if      (ph.xvel > 0){ ph.xvel--; }
    else if (ph.xvel < 0){ ph.xvel++; }
   }

   fireball_arr[i].xvel = ph.xvel;
   fireball_arr[i].yvel = ph.yvel;

   tx = ph.xpos >> 3;
   ty = ph.ypos >> 3;
   t0 = level_gettile(tx, ty     );
   t1 = level_gettile(tx, ty + 1U);
   if ( (t0 == 0x50U) || (t0 == 0x51U) ||
        (t1 == 0x50U) || (t1 == 0x51U) ){
    level_repadd(tx, ty); /* Light up torch */
    levelscr_reset();
    gstat_score_add(10U); /* A little fun element: a minor score increment for making light */
    sound_effect(SOUND_FIRE, 0x40U);
   }

  }

 }
}



/*
** Renders fireballs
*/
void  fireball_render(void)
{
 auint  i;
 auint  btim;
 auint  fra;
 auint  flg;
 physics_spr_t ph;

 for (i = 0U; i < FIREBALL_N; i++){

  btim = fireball_arr[i].btim;
  if (btim != 0U){

   ph.xvel = fireball_arr[i].xvel;
   ph.yvel = fireball_arr[i].yvel;
   levelcor_decode(&(fireball_arr[i].lcor[0]), &(ph.xpos), &(ph.ypos));

   flg = M74_SPR_I1 | M74_SPR_MASK;
   if (ph.xvel < 0){
    flg |= M74_SPR_FLIPX;
    ph.xvel = -ph.xvel;
   }

   if       ( (ph.yvel < 0) ||              /* Going up */
              (ph.xvel > (ph.yvel + 2)) ){  /* Large horiz. component */
    fra = 0xF6U;
   }else if (ph.xvel > (ph.yvel + 1)){
    fra = 0xF8U;
    if (btim > 30U){ fra |= 0x02U; }
   }else if (ph.xvel > (ph.yvel >> 1)){
    fra = 0xFCU;
    if (btim > 30U){ fra |= 0x02U; }
   }else{                                   /* Fire burns upright */
    fra = 0xF4U;
    if (btim > 30U){ fra = 0xF0U | ((btim & 0x04U) >> 1); }
   }
   fra |= (btim & 0x02U) >> 1;

   spritelv_blit(&ph, fra, flg, REC_STRAIGHT);

  }

 }
}



/*
** Generates a single burning fire, may be used for objects on fire. Age is
** the fire's age which should decrement once every render frame until burning
** out.
*/
void  fireball_render_fire(uint16 x, uint16 y, auint age)
{
 auint fra = 0xF4U;
 if (age >= 0x10U){ fra = 0xF0U | ((age & 0x04U) >> 1); }
 fra |= (age & 0x02U) >> 1;
 spritelv_blit_xy(x, y, fra, M74_SPR_I1 | M74_SPR_MASK, REC_STRAIGHT);
}
