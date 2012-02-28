Arduino Project
=============

The goal for this arduino project is to create a device
for controlling the status of LCD screen.

The display is controlled using SMS messages or via a Twitter tweet.

Notes
-----------

### Limited RAM size

The Arduino board has muche less RAM available for programs than a normal
personal computer. By specification the Arduino Duemilanove has 1 KB
(ATmega168) or 2 KB (ATmega328) SRAM memory.

Use avr-size command to check used RAM size.

    >avr-size ArduinoProject.cpp.elf
       text    data     bss     dec     hex filename
      20032     954     409   21395    5393 ArduinoProject.cpp.elf

Sum of DATA and BSS is 954+409=1363. This needs to be smaller than
RAM available on Arduino board.

1. See [AvailableMemory][availmem] for querying used memory.
2. About RAM http://www.designer2k2.at/home/arduino/49-arduino-ram-overflow.html

### Serial buffer length

In "SoftwareSerial.h", you need to change the default buffer size
from 64 to 128 to enable reading complete SMS messages.

    #define _SS_MAX_RX_BUFF 64 // RX buffer size
	

Reference
-----------

AT command reference
 * http://garden.seeedstudio.com/images/a/a8/SIM900_AT_Command_Manual_V1.03.pdf
SIMCOM SIM900 HD V1.05 reference
 * http://garden.seeedstudio.com/images/e/e3/SIM900_HD_V1.05.pdf

[availmem]: http://www.arduino.cc/playground/Code/AvailableMemory
