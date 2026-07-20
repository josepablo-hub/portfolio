/*
 * fuzzylogic.c
 * Implementa el FIS "fuzzylogicpreciso" de MATLAB
 */

#include "fuzzylogic.h"
#include <math.h>

static float trapmf_eval(float x, TrapMF m)
{
    if (x <= m.a || x >= m.d) return 0.0f;
    if (x >= m.b && x <= m.c) return 1.0f;

    if (x > m.a && x < m.b)
    {
        if (m.b == m.a) return 1.0f;
        return (x - m.a) / (m.b - m.a);
    }

    if (x > m.c && x < m.d)
    {
        if (m.d == m.c) return 1.0f;
        return (m.d - x) / (m.d - m.c);
    }

    return 0.0f;
}

static float trimf_eval(float x, TriMF m)
{
    if (x <= m.a || x >= m.c) return 0.0f;
    if (x == m.b) return 1.0f;

    if (x > m.a && x < m.b)
        return (x - m.a) / (m.b - m.a);

    if (x > m.b && x < m.c)
        return (m.c - x) / (m.c - m.b);

    return 0.0f;
}

void fuzzy_init(FuzzySystem* f)
{
    // ENTRADA e_vel (igual)
    f->vel_rapido = (TrapMF){ -0.3f, -0.3f, -0.09f,  0.0f };
    f->vel_medio  = (TriMF ){ -0.09f,  0.0f,  0.09f  };
    f->vel_lento  = (TrapMF){  0.0f,   0.09f,  0.3f, 0.3f };

    // ENTRADA e_giro – aún más preciso cerca de 0°
    f->giro_der = (TrapMF){ -90.0f, -90.0f,  -3.0f,  0.0f };
    f->giro_cen = (TriMF ){  -7.0f,   0.0f,   7.0f   };
    f->giro_izq = (TrapMF){   0.0f,   3.0f,  90.0f, 90.0f };

    // SALIDA PWM Motor Derecho (35–120)
    f->pwmR_frena   = (TrapMF){ 35.0f, 35.0f, 70.0f, 75.0f };
    f->pwmR_mant    = (TriMF ){ 60.0f, 75.0f, 90.0f        };
    f->pwmR_acelera = (TrapMF){ 75.0f, 80.0f,120.0f,120.0f };

    // SALIDA PWM Motor Izquierdo (35–120)
    f->pwmL_frena   = (TrapMF){ 35.0f, 35.0f, 70.0f, 75.0f };
    f->pwmL_mant    = (TriMF ){ 60.0f, 75.0f, 90.0f        };
    f->pwmL_acelera = (TrapMF){ 75.0f, 80.0f,120.0f,120.0f };
}



static uint16_t defuzz3(
    float weights[3],
    TrapMF traps[3],
    TriMF  tris[3],
    int    isTrap[3],
    float xmin,
    float xmax,
    float step )
{
    float num = 0.0f;
    float den = 0.0f;

    for (float x = xmin; x <= xmax; x += step)
    {
        float mu_total = 0.0f;

        for (int i = 0; i < 3; i++)
        {
            if (weights[i] <= 0.0f) continue;

            float mu = isTrap[i] ? trapmf_eval(x, traps[i])
                                 : trimf_eval(x, tris[i]);

            float act = fminf(mu, weights[i]);
            if (act > mu_total)
                mu_total = act;
        }

        num += mu_total * x;
        den += mu_total;
    }

    if (den == 0.0f)
        return (uint16_t)((xmin + xmax) * 0.5f + 0.5f);

    return (uint16_t)(num / den + 0.5f);
}

// ============================= Motor Derecho =============================

uint16_t fuzzy_motor_derecho(FuzzySystem* f, float e_vel, float e_giro)
{
    float vel[3] = {
        trapmf_eval(e_vel, f->vel_rapido),
        trimf_eval (e_vel, f->vel_medio),
        trapmf_eval(e_vel, f->vel_lento)
    };

    float giro[3] = {
        trapmf_eval(e_giro, f->giro_der),
        trimf_eval (e_giro, f->giro_cen),
        trapmf_eval(e_giro, f->giro_izq)
    };

    float w[3] = {0,0,0};

    const int R[9][3] = {
        {1,1,2},
        {2,1,3},
        {3,1,3},
        {1,2,1},
        {2,2,2},
        {3,2,3},
        {1,3,1},
        {2,3,1},
        {3,3,2}
    };

    for (int i=0;i<9;i++)
    {
        int iv=R[i][0]-1, ig=R[i][1]-1, out=R[i][2]-1;
        float s=fminf(vel[iv],giro[ig]);
        if (s>w[out]) w[out]=s;
    }

    int isTrap[3] = {1,0,1};

    TrapMF traps[3] = {
        f->pwmR_frena,
        (TrapMF){0,0,0,0},
        f->pwmR_acelera
    };

    TriMF tris[3] = {
        (TriMF){0,0,0},
        f->pwmR_mant,
        (TriMF){0,0,0}
    };

    return defuzz3(w,traps,tris,isTrap,35.0f,120.0f,0.5f);
}

// ============================= Motor Izquierdo =============================

uint16_t fuzzy_motor_izquierdo(FuzzySystem* f, float e_vel, float e_giro)
{
    float vel[3] = {
        trapmf_eval(e_vel, f->vel_rapido),
        trimf_eval (e_vel, f->vel_medio),
        trapmf_eval(e_vel, f->vel_lento)
    };

    float giro[3] = {
        trapmf_eval(e_giro, f->giro_der),
        trimf_eval (e_giro, f->giro_cen),
        trapmf_eval(e_giro, f->giro_izq)
    };

    float w[3] = {0,0,0};

    const int R[9][3] = {
        {1,1,1},
        {2,1,1},
        {3,1,2},
        {1,2,1},
        {2,2,2},
        {3,2,3},
        {1,3,2},
        {2,3,3},
        {3,3,3}
    };

    for (int i=0;i<9;i++)
    {
        int iv=R[i][0]-1, ig=R[i][1]-1, out=R[i][2]-1;
        float s=fminf(vel[iv],giro[ig]);
        if (s>w[out]) w[out]=s;
    }

    int isTrap[3]={1,0,1};

    TrapMF traps[3] = {
        f->pwmL_frena,
        (TrapMF){0,0,0,0},
        f->pwmL_acelera
    };

    TriMF tris[3] = {
        (TriMF){0,0,0},
        f->pwmL_mant,
        (TriMF){0,0,0}
    };

    return defuzz3(w,traps,tris,isTrap,35.0f,120.0f,0.5f);
}
