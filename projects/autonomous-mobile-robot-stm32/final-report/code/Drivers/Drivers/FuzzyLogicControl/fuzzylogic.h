#ifndef FUZZYLOGIC_H_
#define FUZZYLOGIC_H_

#include <stdint.h>

typedef struct { float a,b,c,d; } TrapMF;
typedef struct { float a,b,c;   } TriMF;

typedef struct
{
    TrapMF vel_rapido;
    TriMF  vel_medio;
    TrapMF vel_lento;

    TrapMF giro_der;
    TriMF  giro_cen;
    TrapMF giro_izq;

    TrapMF pwmR_frena;
    TriMF  pwmR_mant;
    TrapMF pwmR_acelera;

    TrapMF pwmL_frena;
    TriMF  pwmL_mant;
    TrapMF pwmL_acelera;

} FuzzySystem;

void     fuzzy_init(FuzzySystem* f);
uint16_t fuzzy_motor_derecho  (FuzzySystem* f, float e_vel, float e_giro);
uint16_t fuzzy_motor_izquierdo(FuzzySystem* f, float e_vel, float e_giro);

#endif
