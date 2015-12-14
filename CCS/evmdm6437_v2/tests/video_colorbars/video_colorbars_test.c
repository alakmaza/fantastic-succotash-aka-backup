/*
 *  Copyright 2006 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  Video Colorbars Test
 *
 */

#include "evmdm6437_dip.h"
#include "stdio.h"

#define NTSC            JP1_JUMPER_NTSC
#define PAL             JP1_JUMPER_PAL

#define COLORBARS       1
#define LOOPBACK        0

#define SVIDEO_OUT      SW7_SWITCH_RIGHT
#define COMPOSITE_OUT   SW7_SWITCH_LEFT

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  vpbe_init( )                                                            *
 *                                                                          *
 *  NTSC:                                                                   *
 *      Width:  720                                                         *
 *      Height: 480                                                         *
 *                                                                          *
 *                                                                          *
 * ------------------------------------------------------------------------ */
void vpbe_init( Uint32 colorbar_loopback_mode, Uint32 ntsc_pal_mode, Uint32 output_mode )
{
    Uint32 video_buffer = DDR_BASE + ( DDR_SIZE / 2 );
    Uint32 basep_x;
    Uint32 basep_y;
    Uint32 width;
    Uint32 height;

    if ( ntsc_pal_mode == NTSC )
    {
        basep_x = 0x7A; // 122
        basep_y = 0x12; // 18
        width   = 720;
        height  = 480;
    }
    else
    {
        basep_x = 0x84; // 132
        basep_y = 0x16; // 22
        width   = 720;
        height  = 480;
    }

    /*
     * Setup VPBE
     */
    VPSS_CLK_CTRL       = 0x00000018;   // Enable DAC and VENC clock, both at 27 MHz
    VPBE_PCR            = 0;            // No clock div, clock enable

    /*
     * Setup OSD
     */
    VPBE_OSD_MODE       = 0x000000fc;   // Blackground color blue using clut in ROM0
    VPBE_OSD_OSDWIN0MD  = 0;            // Disable both osd windows and cursor window
    VPBE_OSD_OSDWIN1MD  = 0;
    VPBE_OSD_RECTCUR    = 0;

    VPBE_OSD_VIDWIN0OFST = width >> 4;
    VPBE_OSD_VIDWIN0ADR = video_buffer;
    VPBE_OSD_BASEPX     = basep_x;
    VPBE_OSD_BASEPY     = basep_y;
    VPBE_OSD_VIDWIN0XP  = 0;
    VPBE_OSD_VIDWIN0YP  = 0;
    VPBE_OSD_VIDWIN0XL  = width;
    VPBE_OSD_VIDWIN0YL  = height >> 1;
    VPBE_OSD_MISCCTL    = 0;

    VPBE_OSD_VIDWINMD   = 0x00000003;   // Disable vwindow 1 and enable vwindow 0
                                        // Frame mode with no up-scaling

    /*
     *  Setup VENC
     */
    if ( ntsc_pal_mode == NTSC )
        VPBE_VENC_VMOD  = 0x00000003;   // Standard NTSC interlaced output
    else
        VPBE_VENC_VMOD  = 0x00000043;   // Standard PAL interlaced output

    VPBE_VENC_VDPRO     = colorbar_loopback_mode << 8;
    VPBE_VENC_DACTST    = 0;

    /*
     *  Choose Output mode
     */
    if ( output_mode == COMPOSITE_OUT )
        VPBE_VENC_DACSEL = 0x00000000;
    else if ( output_mode == SVIDEO_OUT )
        VPBE_VENC_DACSEL = 0x00004210;
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  Video color bar test                                                    *
 *                                                                          *
 *                                                                          *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 video_colorbars_test( )
{
    Int16 ntsc_pal_mode;
    Int16 output_mode;

    EVMDM6437_DIP_init( );

    /*
     *  Just loop forever accepting any new changes w/ the mode & output
     */
    do
    {
        /* Check Video Settings */
        ntsc_pal_mode = EVMDM6437_DIP_get( JP1_JUMPER );    // NTSC/PAL
        output_mode   = EVMDM6437_DIP_get( SW7_SWITCH );    // SVideo/Composite

        if ( ntsc_pal_mode == NTSC )
        {
            if ( output_mode == COMPOSITE_OUT )
                printf( "    Video Colorbar test: [NTSC][COMPOSITE]\n" );
            else if ( output_mode == SVIDEO_OUT )
                printf( "    Video Colorbar test: [NTSC][S-VIDEO]\n" );
            else
                return -1;
        }
        else if ( ntsc_pal_mode == PAL )
        {
            if ( output_mode == COMPOSITE_OUT )
                printf( "    Video Colorbar test:  [PAL][COMPOSITE]\n" );
            else if ( output_mode == SVIDEO_OUT )
                printf( "    Video Colorbar test:  [PAL][S-VIDEO]\n" );
            else
                return -1;
        }
        else
            return -2;

        /* Setup Back-End */
        vpbe_init( COLORBARS, ntsc_pal_mode, output_mode );

        /* Wait for the USER to press something useful */
        while ( 1 )
        {
            EVMDM6437_I2C_GPIO_waitForIntr( -1 );

            if ( EVMDM6437_DIP_get( JP1_JUMPER ) != ntsc_pal_mode )
                break;
            if ( EVMDM6437_DIP_get( SW7_SWITCH ) != output_mode )
                break;
        }
    } while ( 1 );

    return 0;
}
