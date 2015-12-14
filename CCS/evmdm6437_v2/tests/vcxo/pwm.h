/*
 *  Copyright 2006 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  PWM Header file
 *
 */

#ifndef PWM_
#define PWM_

#include "evmdm6437.h"

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  PWM interface                                                           *
 *                                                                          *
 * ------------------------------------------------------------------------ */
typedef struct {
    volatile Uint32 PID;
    volatile Uint32 PCR;
    volatile Uint32 CFG;
    volatile Uint32 START;
    volatile Uint32 RPT;
    volatile Uint32 PER;
    volatile Uint32 PH1D;
} PWM_REGS;

typedef struct {
    PWM_REGS* regs;
} PWM_OBJ;

static PWM_OBJ PWM_MODULE_0 = { ( PWM_REGS* )PWM0_BASE };
static PWM_OBJ PWM_MODULE_1 = { ( PWM_REGS* )PWM1_BASE };
static PWM_OBJ PWM_MODULE_2 = { ( PWM_REGS* )PWM2_BASE };

typedef PWM_OBJ* PWM_Handle;

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  Prototype                                                               *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 _setup_pwm( Uint32 pwm_id, Uint32 active, Uint32 inactive );

#endif
