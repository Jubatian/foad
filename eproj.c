/*
 *  Dragon - Enemy projectiles
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



#include "eproj.h"
#include "levelcor.h"
#include "levelscr.h"
#include "spritelv.h"
#include "dragon.h"
#include "random.h"
#include "passable.h"
#include "acsupp.h"
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


/* Data mask */
#define EPROJ_DMASK (0xFFU ^ EPROJ_TMASK)




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
 asint txv;
 asint tyv;
 auint vda;
 auint tmp;

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

   hit = acsupp_iscordindragon(ph.xpos, ph.ypos);

   /* Calculate relative velocity difference, used for determining damage.
   ** This is calculated into vda for use by projectiles needing it. The txv
   ** and tyv variables may also be used (relative velocities). */

   txv = dragon_spr.xvel - ph.xvel;
   tyv = dragon_spr.yvel - ph.yvel;
   if (txv < 0){ vda = (auint)(-txv); }
   else        { vda = (auint)( txv); }
   if (tyv < 0){
    if (vda < (auint)(-tyv)){ vda = (auint)(-tyv); }
   }else{
    if (vda < (auint)( tyv)){ vda = (auint)( tyv); }
   }

   /* Process projectiles */

   switch (typ & EPROJ_TMASK){

    case EPROJ_ARROW:

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

     break;

    case EPROJ_SROCK:
    case EPROJ_LROCK:

     /* Effects on the dragon */

     if ( (hit != 0U) && ((typ & 0x30U) == 0U) && (vda > 1U) ){

      tmp = random_get();
      if ((typ & EPROJ_TMASK) == EPROJ_LROCK){ tmp = (tmp & 7U) + (vda << 1); }
      else                                   { tmp = (tmp & 3U) + (vda >> 1); }
      dragon_mod(DRAGON_STA_HP | DRAGON_P_SUB, tmp);
      eproj_arr[i].typ |= 0x30U; /* Start damage timeout */
      if (txv < 0){ ph.xvel--; } /* Slow down or affect rock */
      if (txv > 0){ ph.xvel++; }
      if (tyv < 0){ ph.yvel--; }
      if (tyv > 0){ ph.yvel++; }

     }

     /* Movement logic beyond physics */

     if ( ((eproj_dtim & 0xFU) == 0U) ||         /* Air resistance */
          ( ((ph.flg & PHYSICS_F_GRND) != 0U) && /* Ground resistance */
            ((eproj_dtim & 0x3U) == 0U) ) ){
      if      (ph.xvel > 0){ ph.xvel--; }
      else if (ph.xvel < 0){ ph.xvel++; }
     }

     if (ph.xacc >= 0){ ph.xvel += (asint)((auint)( ph.xacc) >> 1); } /* Bounces */
     else{              ph.xvel -= (asint)((auint)(-ph.xacc) >> 1); }
     if (ph.yacc >= 0){ ph.yvel += (asint)((auint)( ph.yacc) >> 1); }
     else{              ph.yvel -= (asint)((auint)(-ph.yacc) >> 1); }

     if ( (ph.xacc != 0) || (ph.yacc != 0) ){ /* Bouncing effects (shaking, sounds) */
      tmp = 1U;
      if ((typ & EPROJ_TMASK) == EPROJ_LROCK){ tmp = 2U; }
      if ( (ph.xacc >=  3) ||
           (ph.xacc <= -3) ||
           (ph.yacc >=  3) ||
           (ph.yacc <= -3) ){
       tmp <<= 1;
      }
      levelscr_shake(tmp);
     }

     if ( ( ((eproj_dtim & 0x3U) == 0U) || (ph.yacc < 0) ) &&
          ((ph.flg & PHYSICS_F_GRND) != 0U) ){   /* Rolling or bouncing on ramps */
      if       (passable(ph.xpos + 2U, ph.ypos + 1U)){
       ph.xvel++;
      }else if (passable(ph.xpos - 2U, ph.ypos + 1U)){
       ph.xvel--;
      }
     }

     /* Set up low 3 type bits for rolling animation */

     tmp = eproj_arr[i].typ & 0x07U;
     if       (ph.xvel <   0){
      if (ph.xvel > -2){ if ((eproj_dtim & 0x7U) == 0U){ tmp--; } }
      else             { if ((eproj_dtim & 0x3U) == 0U){ tmp--; } }
     }else if (ph.xvel >   0){
      if (ph.xvel <  2){ if ((eproj_dtim & 0x7U) == 0U){ tmp++; } }
      else             { if ((eproj_dtim & 0x3U) == 0U){ tmp++; } }
     }else{}
     eproj_arr[i].typ = (eproj_arr[i].typ & 0xF8U) | (tmp & 0x07U);

     /* Timer: After hit */

     if ((typ & 0x30U) != 0U){ eproj_arr[i].typ -= 0x10U; }

     /* Destroy rock when stopped */

     if ( (ph.xvel == 0) &&
          (ph.yvel == 0) &&
          ((ph.flg & PHYSICS_F_GRND) != 0U) &&
          ((eproj_dtim & 0x3U) == 0U) &&
          ((random_get() & 7U) == 0U) ){
      eproj_arr[i].typ = 0U; /* Rock dies */
     }

     break;

    default:

     break;

   }

   eproj_arr[i].xvel = ph.xvel;
   eproj_arr[i].yvel = ph.yvel;

  }

  /* Test if projectile got too far, then destroy */

  if (!acsupp_iscordneardragon(ph.xpos, ph.ypos, 0xC0C0U)){
   eproj_arr[i].typ = 0U; /* Projectile dies (too far) */
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
 auint  rec;
 physics_spr_t ph;

 for (i = 0U; i < EPROJ_N; i++){

  rec = REC_FLAT_0;
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

   switch (typ & EPROJ_TMASK){

    case EPROJ_ARROW:

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

     break;

    case EPROJ_SROCK:

     fra = 0xE0U + (typ & 0x07U);
     rec = REC_ROCK;
     break;

    default:  /* Unknown type */

     fra = 0x80U;
     break;

   }

   spritelv_blit(&ph, fra, flg, rec);

  }

 }
}
