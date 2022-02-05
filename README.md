# qr-wifi

All testing was done on a Windows 11 machine.


For hardware I used:
- an ESP32-DevKitC board, 
- a waveshare 2.9inch e-Paper Display Module https://www.amazon.com/gp/product/B07P6MJPTD.
- jumper wires


Pinouts:
- screen - ESP32 (see labels on bottom of board)
- VCC - 3V3
- GND - GND
- DIN - P14
- CLK - P13
- CS - P15
- DC (cmd) - P27
- RST - P26
- BUSY - P25


On my machine, I copy the [libraries](.\libraries) folder to [C:\Program Files (x86)\Arduino\libraries](C:\Program%20Files%20(x86)\Arduino\libraries)


After copying the library folder when I launch the Arduino IDE, I can find the BoundingBox example by navigating to File >> Examples >> Examples from any board >> escreen >> BoundingBox.
