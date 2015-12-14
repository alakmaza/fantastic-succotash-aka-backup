/*
 *  Copyright 2006 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  HECC header file
 *
 */

#ifndef HECC_
#define HECC_

#include "evmdm6437.h"

#define STANDARD_ID     0
#define EXTENDED_ID     1

#define DATA_LENGTH_0   0
#define DATA_LENGTH_1   1
#define DATA_LENGTH_2   2
#define DATA_LENGTH_3   3
#define DATA_LENGTH_4   4
#define DATA_LENGTH_5   5
#define DATA_LENGTH_6   6
#define DATA_LENGTH_7   7
#define DATA_LENGTH_8   8

#define PRIORITY_0      0
#define PRIORITY_1      1

typedef struct {
    volatile Uint32 ME;
    volatile Uint32 MD;
    volatile Uint32 TRS;
    volatile Uint32 TRR;
    volatile Uint32 TA;
    volatile Uint32 AA;
    volatile Uint32 RMP;
    volatile Uint32 RML;
    volatile Uint32 RFP;
    volatile Uint32 GAM;
    volatile Uint32 MC;
    volatile Uint32 BTC;
    volatile Uint32 ES;
    volatile Uint32 TEC;
    volatile Uint32 REC;
    volatile Uint32 GIF0;
    volatile Uint32 GIM;
    volatile Uint32 GIF1;
    volatile Uint32 MIM;
    volatile Uint32 MIL;
    volatile Uint32 OPC;
    volatile Uint32 TIOC;
    volatile Uint32 RIOC;
    volatile Uint32 LNT;
    volatile Uint32 TOC;
    volatile Uint32 TOS;
    volatile Uint32 TIOCE;
    volatile Uint32 RIOCE;
    volatile Uint32 ETC;
    volatile Uint32 RSVD0[995];
    struct MBX  {
    volatile Uint32 MID;
    volatile Uint32 MCF;
    volatile Uint32 MDL;
    volatile Uint32 MDH;
    } MBX[32];
    volatile Uint32 RSVD1[896];
    volatile Uint32 LAM[32];
    volatile Uint32 MOTS[32];
    volatile Uint32 MOTO[32];
} HECC_REGS;

typedef struct {
    HECC_REGS* regs;
} HECC_OBJ;

static HECC_OBJ HECC_MODULE = { ( HECC_REGS* )HECC_BASE };

typedef HECC_OBJ* HECC_Handle;

/* ------------------------------------------------------------------------ *
 *  Prototypes                                                              *
 * ------------------------------------------------------------------------ */
void  can_setup( );
Int16 can_start( );
Int16 can_tx_setup  ( Uint32 mailbox, Uint32 msg_id, Uint32 tx_length, Int32 tx_priority );
Int16 can_tx        ( Uint32 mailbox, Uint32 low, Uint32 high );
Int16 can_rx_setup  ( Uint32 mailbox, Uint32 msg_id );
Int16 can_rx        ( Uint32 mailbox, Uint32* low, Uint32* high );

#endif
