# Retro_CALC

The ultimate goal is to build a powerful RPN calculator, programmable in BASIC-like language

## The FIRST mockup version:

1. Arduino MEGA 2560
1. [Arduino MEGA SRAM shield](https://hackaday.io/project/21561-arduino-mega-2560-32kb-ram-shield) (my PCB is modified for 56k RAM instead of 32 in the original version)
1. Arduino SD card reader; this version is for Uno, but runs on Mega with pins 10-13 using Soft SPI
1. Piezo speaker on line 9
1. Hardware keyboard on lines A0-A4
1. LCD screen 128x64 pixels on SPI

The code development for this version is currently suspended and is likely to be deprecated.

## The SECOND mockup version:

1. ESP32 Dev Module
1. Micro SD card reader on VSPI 5, 18, 19, 23
1. Hardware keyboard on lines 25-27 and 32, 33; pin 34 is used for power switch
1. LCD screen 128x64 pixels on HSPI 12-15 
1. Arduino Micro Pro 3.3V, 8 MHz as a "keyboard/mouse injector"

The code is partially ported and tested on the ESP-32 platform. Production PCBs from China are delayed due to Novel Coronavirus outbreak; the Keyboard PCB (KBD-2090) has been received, assembled and tested.

# Build Instructions

## To build for Arduino Mega 2560
1. Tested on Arduino IDE 1.18.10.
1. Install 3 libraries: [U8g2](https://github.com/ptravnik/Retro_CALC/tree/master/U8g2) for the LCD, [SD2](https://github.com/ptravnik/Retro_CALC/tree/master/SD2), and [HWKbd](https://github.com/ptravnik/Retro_CALC/tree/master/HWKbd) for the keyboard. Alternatively just copy the same into the Arduino libraries folder (will not install the examples).
1. Copy the source, [RetroRPN_Arduino](https://github.com/ptravnik/Retro_CALC/tree/master/RetroRPN_Arduino), to any convenient folder and open with Arduino IDE.
1. Select the board as **Arduino Mega or Mega 2560**, processor **ATMega 2560**, compile and upload as usual.

## To build for ESP32 Dev Board
1. Tested on Arduino IDE 1.18.10. and 1.18.12 (VSCode IDE also can be used if Arduino IDE is installed).
1. Install 2 libraries: [U8g2](https://github.com/ptravnik/Retro_CALC/tree/master/U8g2) for the LCD and [HWKbd](https://github.com/ptravnik/Retro_CALC/tree/master/HWKbd) for the keyboard. Alternatively just copy the same into the Arduino libraries folder (will not install the examples).
1. Copy the source, [RetroRPN_ESP32](https://github.com/ptravnik/Retro_CALC/tree/master/RetroRPN_ESP32), to any convenient folder and open with Arduino IDE.
1. Validate which version of hardware keyboard is connected: the production layout uses different h file as the buttons\' position is different from the early mock-up.
1. Select the board as **ESP32 Dev Module**, compile and upload as usual. Dev module requires you to hold **BOOT** down for security reasons; on my module (v 1) this pin is not exposed, so must be done with the button on the module itself. Your board may have this on GPIO 00 pin - wiring to an external button is recommended.

## To build for the Keyboard Injector:
1. Tested only with cheap Chinese clones of Sparkfun Pro Micro on Arduino IDE 1.18.10 and 1.18.12.
1. Carefully follow the [install instructions from Sparkfun](https://learn.sparkfun.com/tutorials/pro-micro--fio-v3-hookup-guide/all)
1. Download the [code from github](https://github.com/ptravnik/Retro_CALC/tree/master/Keyboard_Injector_Micro)
1. Select the board as **Sparkfun Pro Micro** and the processor as **ATMega 32U4 3.3V, 8MHz** or **ATMega 5.0V 16MHz** version (depending on the one you have); **CAUTION** "Leonardo" may work for 5V / 16 MHz boards, but will surely brick a 3.3V version!
1. Select the board's port
1. Upload to board (shorting Reset to Ground may be required after the interface says "Uploading...")
