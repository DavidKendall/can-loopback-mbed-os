# A loopback program for Flexcan on FRDM-K64F

## Introduction

This repository contains the source.


## Build Instructions


1. Ensure that you have installed mbed-cli
   (https://os.mbed.com/docs/v5.9/tools/arm-mbed-cli.html) and the GCC_ARM
   tools
   (https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads). 

1. Clone the repository and set the current working directory to the new repository.

     ```sh
     $ git clone https://github.com/davidkendall/can-mbed-os
     $ cd can-mbed-os
     ```

1. Download the required libraries.

   ```sh
   $ mbed deploy
   ```

1. Connect a terminal program (e.g. minicom, 9600, 8N1, no flow control) to the
   virtual serial port associated with your K64F board (e.g. /dev/ttyACM0),
   after ensuring that your K64F board is plugged into your computer and
   mounted.

1. Compile the program, load to flash and run.

     ```sh
     $ mbed compile -t GCC_ARM -m K64F -f
     ```
   Notice that the `-f` (`--flash`) option causes the program to be loaded and
   executed. You should observe a sequence of lines in the terminal window,
   reporting the value of a counter that is transmitted and received by the
   CAN.

