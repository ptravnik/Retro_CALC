# Retro_CALC

The ultimate goal is to build a powerful RPN calculator, programmable in BASIC-like language

The current mockup version is based upon:

(1) Arduino MEGA 2560

(2) Arduino MEGA SRAM shield (my PCB is modified for 56k RAM instead of 32 in the original version)
https://hackaday.io/project/21561-arduino-mega-2560-32kb-ram-shield

(3) Arduino SD card reader; this version is for Uno, but runs on Mega with pins 10-13 using Soft SPI

(4) Piezo speaker on line 9

(5) Hardware keyboard on lines A0-A4

(6) LCD screen 128x64 pixels on SPI 

The code will be ported and tested on ESP-32 platform after the production PCBs arrive from China.

The build is performed in Arduino IDEas following:

(1) Copy 3 libraries: U8g2 (for the LCD), HWKbd (for the Keyboard), and SD2 (for the SD reader) to the Arduino libraries folder; notably the former two are current versions, but the latter is an older version modified for the soft SPI (specific to the hardware mockup).

(2) Copy the source folder, RetroRPN_Arduino, to any place on the harddrive and load the project into the Arduino IDE

(3) Selact the board as Mega 2560, compile and upload as usual.
