# This is the code ESP32 code

The code was developed using platformIO plugin for vscode

Rename ```src/private.h.example``` to ```src/private.h``` and fill in the information

0b111000000000000000000 this binary switches all LEDs on, first 3 bits ```111``` are the rows ```1``` on ```0``` off.

each following 3 bits is one LED each bit indicating a color in this order BGR. ```1``` off ```0``` on so ```101``` will switch green color on. 