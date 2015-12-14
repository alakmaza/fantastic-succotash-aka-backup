/*
 *  Copyright 2006 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  AIC33 McBSP Test
 *
 */

#include "stdio.h"
#include "evmdm6437_aic33.h"

extern Int16 aic33_tone_headphone( );
extern Int16 aic33_tone_lineout( );
extern Int16 aic33_loop_linein( );
extern Int16 aic33_loop_micin( );

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  aic33_test( )                                                           *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 aic33_test( )
{
    printf( " -> 1 KHz Tone on Headphone [P12].\n" );
    if ( aic33_tone_headphone( ) )
        return 1;

    printf( " -> 1 Khz Tone on Lineout [P13].\n" );
    if ( aic33_tone_lineout( ) )
        return 2;

    printf( "<-> Audio Loopback from Linein [P10] --> to HP/Lineout [P12/P13]\n" );
    if ( aic33_loop_linein( ) )
        return 3;

    printf( "<-> Audio Loopback from Micin [P11] --> to HP/Lineout [P12/P13]\n" );
    if ( aic33_loop_micin( ) )
        return 4;

    return 0;
}
