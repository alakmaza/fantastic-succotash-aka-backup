call ..\..\..\DosRun.bat

cd aic33_mcasp
call build.bat
cd ..

cd aic33_mcbsp
call build.bat
cd ..

cd boot_mode_reader
call build.bat
cd ..

cd can_rx
call build.bat
cd ..

cd can_tx
call build.bat
cd ..

cd dip_switch
call build.bat
cd ..

cd ddr
call build.bat
cd ..

cd eeprom
call build.bat
cd ..

cd emac
call build.bat
cd ..

cd led
call build.bat
cd ..

cd nandflash
call build.bat
cd ..

cd norflash
call build.bat
cd ..

cd pll1705
call build.bat
cd ..

cd readmac
call build.bat
cd ..

cd setmac
call build.bat
cd ..

cd sram
call build.bat
cd ..

cd srom
call build.bat
cd ..

cd uart_loopback
call build.bat
cd ..

cd vcxo
call build.bat
cd ..

cd video_colorbars
call build.bat
cd ..

cd video_loopback
call build.bat
cd ..

cd vlynq_loopback
call build.bat
cd ..

cd vlynq_master
call build.bat
cd ..

cd vlynq_slave
call build.bat
cd ..
