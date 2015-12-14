DM6437 Tests
------------

> aic33_mcasp     : Uses MCASP module to interface AIC33, output a 1Khz tone on HP_OUT & LINE_OUT, next loops the input signal on LINE_IN to HP_OUT & LINE_OUT, followed by looping the input signal on MIC_IN to HP_OUT & LINE_OUT.

> aic33_mcbsp     : Uses MCBSP module to interface AIC33, output a 1Khz tone on HP_OUT & LINE_OUT, next loops the input signal on LINE_IN to HP_OUT & LINE_OUT, followed by looping the input signal on MIC_IN to HP_OUT & LINE_OUT.

> boot_mode       : Displays the current BOOTMODE settings set by switch banks SW1 & SW2.

> can_rx & can_tx : Complementary tests to be run on 2 DM6437 EVMS.  CAN_RX must be running before CAN_TX.  The 2 DM6437 EVMS are connected through a special cable that links the ( P7 ) CAN_H & CAN_L of each to the other.

> ddr             : Tests the DDR on DM6437 EVMs.  The tests will check the data & address lines are properly connected and that the memory is error free.

> dip_switch      : Displays the current DIP switch state as to the corresponding LEDs.  Also checks the position of SW7 & JP1.

> eeprom          : Tests the I2C EEPROM ( U25 )

> emac            : Test the onboard Ethernet ( U30 ) using a special Ethernet loopback connector

> led             : Displays the LEDs with a running 4s pattern.

> nandflash       : Tests the NAND Flash memory

> norflash        : Tests the NOR Flash memory

> pll1705         : Tests the onboard audio and video clocking module.

> sram            : Tests the SRAM memory

> srom            : Tests the SPIROM memory, requires an external SPI EEPROM ( U65 )

> uart            : Tests the UART module which requires an external loopback connected to P8.

> vcxo            : Tests the Voltage controlled oscillator by varying in the input voltage and recording the clock frequency.

> video_colorbars : Video colorbar test output only, outputs between SVideo and Composite video by using SW7 as the selector.

> video_loopback  : Video loopback test input to output, the choice between SVideo and Composite is chosen by the SW7.

> vlynq_loopback  : Tests the VLYNQ module by using a special VLYNQ connector to loopback the signals.

> vlynq_master & vlynq_slave : Complementary tests to be run on 2 DM6437 EVMs.  VLYNQ_SLACE must be running before VLYNQ_MASTER.  The 2 DM6437 are connected between 2 external mini-pci cards in slot J20.
