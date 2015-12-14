/*
 *  Copyright 2006 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  VLYNQ API
 *
 */

#ifndef VLYNQ_
#define VLYNQ_

#include "evmdm6437.h"
#include "stdio.h"

#define VLYNQ_USE_4_PINS        4
#define VLYNQ_USE_3_PINS        3
#define VLYNQ_USE_2_PINS        2
#define VLYNQ_USE_1_PINS        1
#define VLYNQ_USE_NO_PINS       0

#define VLYNQ_USE_SCRUN         1
#define VLYNQ_DONT_USE_SCRUN    0

#define VLYNQ_LINK_FOUND        0
#define VLYNQ_NO_LINK_FOUND     -1

/* ---------------------------------------------------------------- *
 *                                                                  *
 *  API                                                             *
 *                                                                  *
 * ---------------------------------------------------------------- */

Int16 vlynq_init ( Int16 pins_used, Int16 scrun_used );
Int16 vlynq_quit ( );
Int16 vlynq_reset( );

Int16 vlynq_wait_for_link( Int32 timeout );
Int16 vlynq_rx_pins_used( );
Int16 vlynq_tx_pins_used( );

#endif
