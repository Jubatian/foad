/*
 *  Dragon - Enemy projectiles
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



#include "eproj.h"
#include "levelcor.h"
#include "spritelv.h"
#include "dragon.h"
#include "random.h"
#include <uzebox.h>



/* Projectile descriptor */
typedef struct{
 auint typ;
 asint xvel;
 asint yvel;
 uint8 lcor[3];
}eproj_t;


/* Projectile storage */
static eproj_t eproj_arr[EPROJ_N];

/* X velocity decay timer (air resistance) */
static auint eproj_dtim;


/* Arrow specific data mask */
#define EPROJ_ARROW_MASK 0x01U




/*
** Initializes the storage with all projectiles cleared (free).
*/
void  eproj_init(void)
{
 auint i;

 for (i = 0U; i < EPROJ_N; i++){
  eproj_arr[i].typ = 0U;
 }
}



/*
** Creates a new projectile if possible. If not possible, nothing happens.
*/
void  eproj_new(uint16 x, uint16 y, asint xvel, asint yvel, auint typ)
{
 auint i;

 for (i = 0U; i < EPROJ_N; i++){
  if (eproj_arr[i].typ == 0U){ break; }
 }

 if (i != EPROJ_N){
  eproj_arr[i].typ  = typ;
  eproj_arr[i].xvel = xvel;
  eproj_arr[i].yvel = yvel;
  levelcor_encode(&(eproj_arr[i].lcor[0]), x, y);
 }

}



/*
** Ages (if applicable to the type) or removes a projectile. Aging is
** performed by the passed number. 0xFF removes any projectile.
*/
void  eproj_age(auint id, auint age)
{
 eproj_arr[id].typ = 0U;
}



/*
** Retrieve a projectile structure for reading, possibly used for collisions,
** interactions between sprites. Returns nonzero if the projectile doesn't
** exist.
*/
auint eproj_get(auint id, physics_spr_t* dest)
{
 if (eproj_arr[id].typ == 0U){ return 1U; }

 dest->xvel = eproj_arr[id].xvel;
 dest->yvel = eproj_arr[id].yvel;
 levelcor_decode(&(eproj_arr[id].lcor[0]), &(dest->xpos), &(dest->ypos));
 return 0U;
}



/*
** Processes projectiles and their collisions with the dragon
*/
void  eproj_process(void)
{
 auint i;
 auint typ;
 auint hit;
 physics_spr_t ph;
 auint dragon_w = dragon_spr.xbs;
 auint dragon_h = dragon_spr.ybs;

 eproj_dtim ++;

 for (i = 0U; i < EPROJ_N; i++){

  typ = eproj_arr[i].typ;

  if (typ != 0U){

   ph.xbs  = 0U;
   ph.ybs  = 0U;
   ph.xvel = eproj_arr[i].xvel;
   ph.yvel = eproj_arr[i].yvel;
   ph.flg  = 0U;

   levelcor_decode(&(eproj_arr[i].lcor[0]), &(ph.xpos), &(ph.ypos));
   physics_proc(&ph);
   levelcor_encode(&(eproj_arr[i].lcor[0]), ph.xpos, ph.ypos);

   /* Test if the projectile hits the dragon */

   if ( ((ph.xpos + dragon_w) >= (dragon_spr.xpos)) &&
        ((ph.xpos) <= (dragon_spr.xpos + dragon_w)) &&
        ((ph.ypos + dragon_h) >= (dragon_spr.ypos)) &&
        ((ph.ypos) <= (dragon_spr.ypos           )) ){
    hit = 1U;
   }else{
    hit = 0U;
   }

   /* Process projectiles */

   if ((typ & (~EPROJ_ARROW_MASK)) == EPROJ_ARROW){

    /* Effects on the dragon */

    if (hit != 0U){

     dragon_mod(DRAGON_STA_HP | DRAGON_P_SUB, (random_get() & 1U) + 2U);
     eproj_arr[i].typ = 0U; /* Arrow consumed */

    }else{

     /* Movement logic beyond physics */

     if ((ph.flg & PHYSICS_F_GRND) != 0U){ /* On ground the arrow disappears */
      eproj_arr[i].typ = 0U;
     }else{                                /* In air they have a minimal resistance */
      if ((eproj_dtim & 0xFU) == 0U){
       if      (ph.xvel > 0){ ph.xvel--; }
       else if (ph.xvel < 0){ ph.xvel++; }
      }
     }

    }

   }

   eproj_arr[i].xvel = ph.xvel;
   eproj_arr[i].yvel = ph.yvel;

  }

 }
}



/*
** Renders projectiles
*/
void  eproj_render(void)
{
 auint  i;
 auint  fra;
 auint  flg;
 auint  typ;
 physics_spr_t ph;

 for (i = 0U; i < EPROJ_N; i++){

  typ = eproj_arr[i].typ;
  if (typ != 0U){

   ph.xvel = eproj_arr[i].xvel;
   ph.yvel = eproj_arr[i].yvel;
   levelcor_decode(&(eproj_arr[i].lcor[0]), &(ph.xpos), &(ph.ypos));

   flg = M74_SPR_I2 | M74_SPR_MASK;
   if (ph.xvel < 0){
    ph.xvel = -ph.xvel;
    flg |= M74_SPR_FLIPX;
   }

   if ((typ & (~EPROJ_ARROW_MASK)) == EPROJ_ARROW){ /* Arrow type */

    flg ^= M74_SPR_FLIPX;   /* Arrows ended up inverted by graphics */

    if       (ph.yvel < 0){ /* Going up */
     if       ((-ph.yvel) <= ((ph.xvel) >> 1)){
      fra = 0x83U;
     }else if ((-ph.yvel)  < ((ph.xvel) << 1)){
      fra = 0x82U;
     }else if (ph.xvel != 0){
      fra = 0x81U;
     }else{
      fra = 0x80U;
     }
    }else if (ph.yvel > 0){ /* Going down */
     if       (( ph.yvel) <= ((ph.xvel) >> 1)){
      fra = 0x85U;
     }else if (( ph.yvel)  < ((ph.xvel) << 1)){
      fra = 0x86U;
     }else if (ph.xvel != 0){
      fra = 0x87U;
     }else{
      fra = 0x88U;
     }
    }else{                  /* Horizontal */
     fra = 0x84U;
    }

   }else{ /* Unknown type */

    fra = 0x80U;

   }

   spritelv_blit(&ph, fra, flg, REC_FLAT_0);

  }

 }
}
