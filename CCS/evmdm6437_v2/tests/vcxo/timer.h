/*
 *  Copyright 2006 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  TIMER Header file
 *
 */

#ifndef TIMER_
#define TIMER_

#include "evmdm6437.h"

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  TIMER interface                                                         *
 *                                                                          *
 * ------------------------------------------------------------------------ */
typedef struct {
    Uint32 PID12;
    volatile Uint32 EMUMGT_CLKSPD;
    Uint32 rsvd08;
    Uint32 rsvd0C;
    volatile Uint32 TIM12;
    volatile Uint32 TIM34;
    volatile Uint32 PRD12;
    volatile Uint32 PRD34;
    volatile Uint32 TCR;
    volatile Uint32 TGCR;
    volatile Uint32 WDTCR;
} TIMER_REGS;

typedef struct {
    TIMER_REGS* regs;
} TIMER_OBJ;

static TIMER_OBJ TIMER_MODULE_0 = { ( TIMER_REGS* )TIMER0_BASE };
static TIMER_OBJ TIMER_MODULE_1 = { ( TIMER_REGS* )TIMER1_BASE };
static TIMER_OBJ TIMER_MODULE_2 = { ( TIMER_REGS* )TIMER2_BASE };

typedef TIMER_OBJ* TIMER_Handle;

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  Prototype                                                               *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 _timer_init( Uint32 timer_id );
Uint32 _timer_read( Uint32 timer_id, Uint32 usec );

#endif
