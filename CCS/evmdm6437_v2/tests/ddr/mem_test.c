/*
 *  Copyright 2006 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  Memory Test
 *
 */

#include "evmdm6437.h"

Uint32 memfill32( Uint32 start, Uint32 len, Uint32 val )
{
    Uint32 i;
    Uint32 end = start + len;
    Uint32 errorcount = 0;

    /* Write Pattern */
    for ( i = start; i < end; i += 4 )
    {
        *( volatile Uint32* )i = val;
    }

    /* Read Pattern */
    for ( i = start; i < end; i += 4 )
    {
        if ( *( volatile Uint32* )i != val )
        {
            errorcount++;
            break;
        }
    }

    return errorcount;
}

Uint32 memaddr32( Uint32 start, Uint32 len )
{
    Uint32 i;
    Uint32 end = start + len;
    Uint32 errorcount = 0;

    /* Write Pattern */
    for ( i = start; i < end; i += 4 )
    {
        *( volatile Uint32* )i = i;
    }

    /* Read Pattern */
    for ( i = start; i < end; i += 4 )
    {
        if ( *( volatile Uint32* )i != i )
        {
            errorcount++;
            break;
        }
    }

    return errorcount;
}

Uint32 meminvaddr32( Uint32 start, Uint32 len )
{
    Uint32 i;
    Uint32 end = start + len;
    Uint32 errorcount = 0;

    /* Write Pattern */
    for ( i = start; i < end; i += 4 )
    {
        *( volatile Uint32* )i = ~i;
    }

    /* Read Pattern */
    for ( i = start; i < end; i += 4 )
    {
        if ( *( volatile Uint32* )i != ~i )
        {
            errorcount++;
            break;
        }
    }

    return errorcount;
}
