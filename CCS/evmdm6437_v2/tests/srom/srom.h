/*
 *  Copyright 2006 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  SPI ROM header file
 *
 */

#include "evmdm6437.h"

/* SROM Definitions */
#define SROM_SIZE               0x008000
#define SROM_BASE               0x000000
#define SROM_PAGESIZE           64
#define SROM_PAGEMASK           0xFFFFFFC0

/* SROM Commands */
#define SROM_CMD_WREN           0x06
#define SROM_CMD_WRDI           0x04
#define SROM_CMD_RDSR           0x05
#define SROM_CMD_WRSR           0x01
#define SROM_CMD_READ           0x03
#define SROM_CMD_WRITE          0x02

void srom_read( Uint32 src, Uint32 dst, Uint32 length );
void srom_write( Uint32 src, Uint32 dst, Uint32 length );
