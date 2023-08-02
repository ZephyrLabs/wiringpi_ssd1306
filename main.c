/**
 * @file main.c
 * @author Sravan Senthilnathan
 * @brief sample usage of custom SSD1306 display driver in wiringPi
 * @version 0.1
 * @date 2023-08-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "ssd1306.h"
#include "bitmap.h"

const char* sample_text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, "
                          "sed do eiusmod tempor incididunt ut labore et dolore magna"
                          "aliqua. Ut enim ad minim veniam, quis nostrud exercitation" 
                          "ullamco laboris nisi ut aliquip ex ea commodo consequat.";
int main()
{
	int fd;

	if(-1 == wiringPiSetup()){
		printf("set up error");
		exit(1);
	}

	fd = wiringPiI2CSetup(SSD1306_I2C_ADDRESS);

    if (fd < 0) {
        fprintf(stderr, "Error initializing I2C\n");
        return 1;
    }

    // typial initialization process
	ssd1306_init(fd);

    // cycle brightness
    fillDisplay();
    display();
    
    for(int i = 0; i < 255; i++) { setBrightness(i); delay(10); }
    for(int i = 255; i > -1; i--) { setBrightness(i); delay(10); }

    setBrightness(0x7f);
    clearDisplay();

    // draw some lines
    printf("Draw Some lines\n");
    clearFrameBuffer();
    for(int i = 0; i < 64; i+=4) {drawLine(0, 0, 127, i);}
    for(int i = 128; i > 0; i-=4) {drawLine(0, 0, i, 64);}
    display();
    delay(2000);

    // draw a bitmap
    clearFrameBuffer();
    drawBitmap(0, 0, bitmap_data, 128, 64);
    display();
    delay(2000);

    // write some text
    clearFrameBuffer();
    drawString(0, 0, sample_text);
    display();
    delay(2000);

    // clear everything
    clearFrameBuffer();
    display();

	exit(0);
}
