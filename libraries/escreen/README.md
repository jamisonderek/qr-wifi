# escreen

This is a library for controlling the 2.9inch e-paper v2 device.
I ordered the device at https://www.amazon.com/gp/product/B07P6MJPTD 


This code is based on the specification for the 2.9inch e-paper v2, which can be found on the 
waveshare web site at https://www.waveshare.com/w/upload/7/79/2.9inch-e-paper-v2-specification.pdf

All of the steps are based on the flowchart in section 4.2 Reference Program Code.  Details were also figured out by reading the section 5 Command table. Minor changes to the flowchart are as follows:
-  Slight delay after step 4 & step 5; otherwise screen update didn't happen.
-  Step 6 - we use 0x01 which is "(Y-,X+)" not "(Y+,X-)" as described in chart.
-  Step 8 - we don't do the SPI(0x1A, value)
-  Step 10 - we sent 296*22 = 6512 bytes not 5000.
-  Step 11 - added SPI(0x20) that is missing from the diagram (it just says "Master Activation:")

You can use ANY pins you want, we do GPIO bit-bang, so doesn't need to be SPI specific pins.  The default values were based on the ESP32 wiring pinout that is defined in https://www.waveshare.com/wiki/2.9inch_e-Paper_Module 

Typical usage should be: 
```
EScreen screen(14, 13, 15, 27, 26, 25);
```
