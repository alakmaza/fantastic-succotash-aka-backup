/*
 *  Copyright 2006 by Spectrum Digital Incorporated.
 *  All rights reserved.  Property of Spectrum Digital Incorporated.
 *
 *  Not for distribution.
 */

/*
 *  Linker command file
 *
 */

-l rts64plus.lib

-stack          0x00002000      /* Stack Size */
-heap           0x00002000      /* Heap Size */

MEMORY
{
    MEMTEST:    o = 0x10800000  l = 0x00000200
    VECS:       o = 0x10804000  l = 0x00000200
    BOOT:       o = 0x10804200  l = 0x00000200
    IRAM:       o = 0x10804400  l = 0x0001BC00
    DDR2:       o = 0x80000000  l = 0x10000000
}

SECTIONS
{
    .bss        >   IRAM
    .cinit      >   IRAM
    .cio        >   IRAM
    .const      >   IRAM
    .data       >   IRAM
    .far        >   IRAM
    .stack      >   IRAM
    .switch     >   IRAM
    .sysmem     >   IRAM
    .text       >   IRAM
    .ddr2       >   DDR2
}
