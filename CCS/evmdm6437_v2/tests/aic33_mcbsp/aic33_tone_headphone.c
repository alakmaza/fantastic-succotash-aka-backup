/*
 *  Copyright 2006 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  AIC33 Tone
 *
 */

#include "evmdm6437_aic33.h"

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  AIC33 Setup                                                             *
 *                                                                          *
 *      AIC33.MCLK = PLL1705.SCK02                                          *
 *      FS = ( AIC33.MCLK * K ) / ( 2048 * P )                              *
 *                                                                          *
 *      For a FS=[48 kHz] & MCLK=[22.5792 MHz]                              *
 *          : 48kHz = ( 22.5792 MHz * K ) / ( 2048 * P )                    *
 *          : P = 2, K[J.D] = 8.7075                                        *
 *                                                                          *
 * ------------------------------------------------------------------------ */
static AIC33_Config aic33config = {
       0, // 0  Page Select                     <- [Page=0]
READONLY, // 1  Software Reset                  <- [ResetOnly]
       0, // 2  Codec Sample Rate Select        <- [ADC=FS][DAC=FS]

    // For: [FS=48 kHz][MCLK=22.5792 MHz]
    0x92, // 3  PLL A                           <- [PLL=ON][P=2][Q=2]
    0x20, // 4  PLL B                           <- [J=8]
    0x6E, // 5  PLL C                           <- [D=7075]
    0x23, // 6  PLL D                           <- [D=7075]
    0x0A, // 7  Codec Datapath Setup            <- [FS=48 kHz][LeftDAC=LEFT][RightDAC=RIGHT]

    0xC0, // 8  Audio Serial Data A             <- [BCLK=Master][WCLK=Master][3DEffects=OFF][DigitalMIC=OFF]
       0, // 9  Audio Serial Data B             <- [Mode=I2S][Length=16-bit][TransferMode=Continous]
       0, // 10 Audio Serial Data C             <- [DataOffset=0]
READONLY, // 11 Audio Codec Overflow Flag       <- [ReadOnly]
       0, // 12 Audio Codec Digital Filter      <- [OFF]

       0, // 13 Headset Detection A             <- [OFF]
       0, // 14 Headset Detection B             <- []

       0, // 15  Left ADC PGA Gain              <- [Mute=OFF]
       0, // 16 Right ADC PGA Gain              <- [Mute=OFF]
    0xFF, // 17 MIC3L/R to  Left ADC            <- [OFF]
    0xFF, // 18 MIC3L/R to Right ADC            <- [OFF]
    0x04, // 19  LINE1L to  Left ADC            <- [SingleEnd][Gain=0dB][Power=ON][SoftStep=OncePerFS]
    0x78, // 20  LINE2L to  Left ADC            <- [OFF]
    0x78, // 21  LINE1R to  Left ADC            <- [OFF]
    0x04, // 22  LINE1R to Right ADC            <- [SingleEnd][Gain=0dB][Power=ON][SoftStep=OncePerFS]
    0x78, // 23  LINE2R to Right ADC            <- [OFF]
    0x78, // 24  LINE1L to Right ADC            <- [OFF]
       0, // 25 MICBIAS                         <- [Power=OFF]

       0, // 26  Left AGC A                     <- [LeftAGC=OFF]
       0, // 27  Left AGC B                     <- [OFF]
       0, // 28  Left AGC C                     <- [OFF]
       0, // 29 Right AGC A                     <- [RightAGC=OFF]
       0, // 30 Right AGC B                     <- [OFF]
       0, // 31 Right AGC C                     <- [OFF]
READONLY, // 32  Left AGC Gain                  <- [ReadOnly]
READONLY, // 33 Right AGC Gain                  <- [ReadOnly]
       0, // 34  Left AGC Noise Debounce        <- []
       0, // 35 Right AGC Noise Debounce        <- []
READONLY, // 36 ADC Flag                        <- [ReadOnly]

    0xE0, // 37 DAC Power & Output Dvr          <- [LeftDAC=ON][RightDAC=ON][HPLCOM=SingleEnd]
    0x10, // 38 High Power Output Dvr           <- [HPRCOM=SingleEnd][ShortCircuit=OFF]
RESERVED, // 39                                 <- [Reserved]
       0, // 40 High Power Output Stage         <- []
       0, // 41 DAC Output Switching            <- []
       0, // 42 Output Driver Pop Reduction     <- []

       0, // 43  Left DAC Digital Volume        <- [Mute=OFF][Gain=0dB]
       0, // 44 Right DAC Digital Volume        <- [Mute=OFF][Gain=0dB]

       0, // 45 Line2L to HPLOUT Volume         <- []
       0, // 46 PGA_L  to HPLOUT Volume         <- []
    0x80, // 47 DAC_L1 to HPLOUT Volume         <- [Routed]
       0, // 48 LINE2R to HPLOUT Volume         <- []
       0, // 49 PGA_R  to HPLOUT Volume         <- []
       0, // 50 DAC_R1 to HPLOUT Volume         <- []
    0x09, // 51           HPLOUT Output         <- [Mute=OFF][Power=ON]

       0, // 52 LINE2L to HPLCOM Volume         <- []
       0, // 53 PGA_L  to HPLCOM Volume         <- []
       0, // 54 DAC_L1 to HPLCOM Volume         <- []
       0, // 55 LINE2R to HPLCOM Volume         <- []
       0, // 56 PGA_R  to HPLCOM Volume         <- []
       0, // 57 DAC_R1 to HPLCOM Volume         <- []
       0, // 58           HPLCOM Output         <- []

       0, // 59 LINE2L to HPROUT Volume         <- []
       0, // 60 PGA_L  to HPROUT Volume         <- []
       0, // 61 DAC_L1 to HPROUT Volume         <- []
       0, // 62 LINE2R to HPROUT Volume         <- []
       0, // 63 PGA_R  to HPROUT Volume         <- []
    0x80, // 64 DAC_R1 to HPROUT Volume         <- [Routed]
    0x09, // 65           HPROUT Output         <- [Mute=OFF][Power=ON]

       0, // 66 LINE2L to HPRCOM Volume         <- []
       0, // 67 PGA_L  to HPRCOM Volume         <- []
       0, // 68 DAC_L1 to HPRCOM Volume         <- []
       0, // 69 LINE2R to HPRCOM Volume         <- []
       0, // 70 PGA_R  to HPRCOM Volume         <- []
       0, // 71 DAC_R1 to HPRCOM Volume         <- []
       0, // 72           HPRCOM Output         <- []

       0, // 73 LINE2L to MONO_LOP/M Volume     <- []
       0, // 74 PGA_L  to MONO_LOP/M Volume     <- []
       0, // 75 DAC_L1 to MONO_LOP/M Volume     <- []
       0, // 76 LINE2R to MONO_LOP/M Volume     <- []
       0, // 77 PGA_R  to MONO_LOP/M Volume     <- []
       0, // 78 DAC_R1 to MONO_LOP/M Volume     <- []
       0, // 79           MONO_LOP/M Output     <- []

       0, // 80 LINE2L to LEFT_LOP/M Volume     <- []
       0, // 81 PGA_L  to LEFT_LOP/M Volume     <- []
       0, // 82 DAC_L1 to LEFT_LOP/M Volume     <- []
       0, // 83 LINE2R to LEFT_LOP/M Volume     <- []
       0, // 84 PGA_R  to LEFT_LOP/M Volume     <- []
       0, // 85 DAC_R1 to LEFT_LOP/M Volume     <- []
       0, // 86           LEFT_LOP/M Output     <- []

       0, // 87 LINE2L to RIGHT_LOP/M Volume    <- []
       0, // 88 PGA_L  to RIGHT_LOP/M Volume    <- []
       0, // 89 DAC_L1 to RIGHT_LOP/M Volume    <- []
       0, // 90 LINE2R to RIGHT_LOP/M Volume    <- []
       0, // 91 PGA_R  to RIGHT_LOP/M Volume    <- []
       0, // 92 DAC_R1 to RIGHT_LOP/M Volume    <- []
       0, // 93           RIGHT_LOP/M Output    <- []

READONLY, // 94 Module Power Status             <- [ReadOnly]
READONLY, // 95 Short Circuit Detection         <- [ReadOnly]

RESERVED, // 96                                 <- [Reserved]
RESERVED, // 97                                 <- [Reserved]

       0, // 98 GPIO1                           <- [GPIO1=OFF]
       0, // 99 GPIO2                           <- [GPIO2=OFF]
       0, // 100 Additional GPIO Control A      <- []
       0  // 101 Additional GPIO Control B      <- []
};

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  AIC33 Tone                                                              *
 *      Output a 1 kHz tone through the HEADPHONE/LINEOUT jacks             *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 aic33_tone_headphone( )
{
    /* Pre-generated sine wave data, 16-bit signed samples */
    Int16 sinetable[48] = {
        0x0000, 0x10b4, 0x2120, 0x30fb, 0x3fff, 0x4dea, 0x5a81, 0x658b,
        0x6ed8, 0x763f, 0x7ba1, 0x7ee5, 0x7ffd, 0x7ee5, 0x7ba1, 0x76ef,
        0x6ed8, 0x658b, 0x5a81, 0x4dea, 0x3fff, 0x30fb, 0x2120, 0x10b4,
        0x0000, 0xef4c, 0xdee0, 0xcf06, 0xc002, 0xb216, 0xa57f, 0x9a75,
        0x9128, 0x89c1, 0x845f, 0x811b, 0x8002, 0x811b, 0x845f, 0x89c1,
        0x9128, 0x9a76, 0xa57f, 0xb216, 0xc002, 0xcf06, 0xdee0, 0xef4c
    };

    Int16 msec, sec;
    Int16 sample;
    AIC33_CodecHandle aic33handle;

    /* Grab AIC33 handle */
    aic33handle = EVMDM6437_AIC33_openCodec( AIC33_MCBSP_ID, &aic33config );

    /* Play Tone */
    for ( sec = 0 ; sec < 5 ; sec++ )
    {
        for ( msec = 0 ; msec < 1000 ; msec++ )
        {
            for ( sample = 0 ; sample < 48 ; sample++ )
            {
                /* Send a sample to the left channel */
                while ( ! EVMDM6437_AIC33_write16( aic33handle, sinetable[sample] ) );

                /* Send a sample to the right channel */
                while ( ! EVMDM6437_AIC33_write16( aic33handle, sinetable[sample] ) );
            }
        }
    }

    EVMDM6437_AIC33_closeCodec( aic33handle );
    return 0;
}
