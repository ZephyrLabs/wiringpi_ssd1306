# Wiring Pi example driver for SSD1306 OLED
A rough example driver using the Wiring Pi API to access 
the SSD1306 display over i2C.

* supports the 128x64 resolution version of the display
* default i2C address is `0x3c` (this can be changed in ssd1306.h)
* uses the default i2C adapter provided by the Wiring Pi library (depends on your platform ?)
* display refresh with a framebuffer callback
* some basic GFX methods (draw lines, strings, bitmap images, etc.)

## Tested on:
* Khadas VIM3
    * default configuration: /dev/i2c-3
    * **SCL:** pin 22 (I2C_M3)
    * **SDA:** pin 23 (I2C_M3)

* Khadas VIM4
    * default configuration: /dev/i2c-0
    * **SCL:** pin 25 (I2C_MA)
    * **SDA:** pin 26 (I2C_MA)

### Images

![collage](/images/collage.png)
