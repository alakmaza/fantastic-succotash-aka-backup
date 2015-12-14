/*
 *  Copyright 2006 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

#include "evmdm6437.h"
#include "evmdm6437_gpio.h"

typedef struct _EMAC_Desc {
    struct _EMAC_Desc *pNext;   // Pointer to next descriptor
    Uint8* pBuffer;             // Pointer to data buffer
    Uint32 BufOffLen;           // Buffer Offset(MSW) and Length(LSW)
    Uint32 PktFlgLen;           // Packet Flags(MSW) and Length(LSW)
} EMAC_Desc;
