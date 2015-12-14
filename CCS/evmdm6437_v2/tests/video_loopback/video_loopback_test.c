/*
 *  Copyright 2006 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  Video Loopback Test
 *
 */

#include "stdio.h"
#include "evmdm6437_dip.h"
#include "tvp5146.h"

#define NTSC            1
#define PAL             0

#define COLORBARS       1
#define LOOPBACK        0

#define SVIDEO_OUT      1
#define COMPOSITE_OUT   0

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  vpfe_init( ntsc/pal )                                                   *
 *                                                                          *
 * ------------------------------------------------------------------------ */
static void vpfe_init( Uint32 ntsc_pal_mode )
{
    Uint32 video_buffer = DDR_BASE + ( DDR_SIZE / 2 );
    Uint32 width;
    Uint32 height;

    if ( ntsc_pal_mode == NTSC )
    {
        width   = 720;
        height  = 480;
    }
    else
    {
        width   = 720;
        height  = 480;
    }


    VPFE_CCDC_SYN_MODE  = 0x00032F84;   // interlaced, with VD pority as negative
    VPFE_CCDC_HD_VD_WID = 0;
    VPFE_CCDC_PIX_LINES = 0x02CF020D;

    /*
     *  sph = 1, nph = 1440, according to page 32-33 of the CCDC spec
     *  for BT.656 mode, this setting captures only the 720x480 of the
     *  active NTSV video window
     */
    VPFE_CCDC_HORZ_INFO = width << 1;   // Horizontal lines
    VPFE_CCDC_HSIZE_OFF = width << 1;   // Horizontal line offset
    VPFE_CCDC_VERT_START = 0;           // Vertical start line
    VPFE_CCDC_VERT_LINES = height >> 1; // Vertical lines
    VPFE_CCDC_CULLING   = 0xFFFF00FF;   // Disable cullng

    /*
     *  Interleave the two fields
     */
    VPFE_CCDC_SDOFST    = 0x00000249;
    VPFE_CCDC_SDR_ADDR  = video_buffer;
    VPFE_CCDC_CLAMP     = 0;
    VPFE_CCDC_DCSUB     = 0;
    VPFE_CCDC_COLPTN    = 0xEE44EE44;
    VPFE_CCDC_BLKCMP    = 0;
    VPFE_CCDC_FPC_ADDR  = 0x86800000;
    VPFE_CCDC_FPC       = 0;
    VPFE_CCDC_VDINT     = 0;
    VPFE_CCDC_ALAW      = 0;
    VPFE_CCDC_REC656IF  = 0x00000003;

    /*
     *  Input format is Cb:Y:Cr:Y, w/ Y in odd-pixel position
     */
    VPFE_CCDC_CCDCFG    = 0x00000800;
    VPFE_CCDC_FMTCFG    = 0;
    VPFE_CCDC_FMT_HORZ  = 0x000002D0;
    VPFE_CCDC_FMT_VERT  = 0x0000020E;
    VPFE_CCDC_FMT_ADDR0 = 0;
    VPFE_CCDC_FMT_ADDR1 = 0;
    VPFE_CCDC_FMT_ADDR2 = 0;
    VPFE_CCDC_FMT_ADDR3 = 0;
    VPFE_CCDC_FMT_ADDR4 = 0;
    VPFE_CCDC_FMT_ADDR5 = 0;
    VPFE_CCDC_FMT_ADDR6 = 0;
    VPFE_CCDC_FMT_ADDR7 = 0;
    VPFE_CCDC_PRGEVEN_0 = 0;
    VPFE_CCDC_PRGEVEN_1 = 0;
    VPFE_CCDC_PRGODD_0  = 0;
    VPFE_CCDC_PRGODD_1  = 0;
    VPFE_CCDC_VP_OUT    = 0x041A2D00;
    VPFE_CCDC_PCR       = 0x00000001;   // Enable CCDC
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  vpbe_init( colorbars/loopback, ntsc/pal, svideo/composite )             *
 *                                                                          *
 * ------------------------------------------------------------------------ */
static void vpbe_init( Uint32 colorbar_loopback_mode, Uint32 ntsc_pal_mode, Uint32 output_mode )
{
    Uint32 video_buffer = DDR_BASE + ( DDR_SIZE / 2 );
    Uint32 basep_x;
    Uint32 basep_y;
    Uint32 width;
    Uint32 height;

    if ( ntsc_pal_mode == NTSC )
    {
        basep_x = 122;
        basep_y = 18;
        width   = 720;
        height  = 480;
    }
    else
    {
        basep_x = 132;
        basep_y = 22;
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
    VPBE_VENC_DACSEL    = 0x00004210;

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
 *  video_loopback_test( )                                                  *
 *                                                                          *
 *                                                                          *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 video_loopback_test( )
{
    Int16 ntsc_pal_mode;
    Int16 output_mode;

    EVMDM6437_DIP_init( );

    do
    {
        /* Check Video Settings */
        ntsc_pal_mode = EVMDM6437_DIP_get( JP1_JUMPER );    // NTSC/PAL
        output_mode   = EVMDM6437_DIP_get( SW7_SWITCH );    // SVideo/Composite

        if ( ntsc_pal_mode == NTSC )
        {
            if ( output_mode == COMPOSITE_OUT )
                printf( "    Video Loopback test: [NTSC][COMPOSITE]\n" );
            else if ( output_mode == SVIDEO_OUT )
                printf( "    Video Loopback test: [NTSC][S-VIDEO]\n" );
            else
                return -1;
        }
        else if ( ntsc_pal_mode == PAL )
        {
            if ( output_mode == COMPOSITE_OUT )
                printf( "    Video Loopback test:  [PAL][COMPOSITE]\n" );
            else if ( output_mode == SVIDEO_OUT )
                printf( "    Video Loopback test:  [PAL][S-VIDEO]\n" );
            else
                return -1;
        }
        else
            return -2;

        /* Setup Front-End */
        tvp5146_init( ntsc_pal_mode, output_mode );
        vpfe_init( ntsc_pal_mode );

        /* Setup Back-End */
        vpbe_init( LOOPBACK, ntsc_pal_mode, output_mode );

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
