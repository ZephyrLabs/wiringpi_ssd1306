/**
 * @file ssd1306.c
 * @author Sravan Senthilnathan
 * @brief Source for SSD1306 OLED module driver (128x64 pixels)
 * @version 0.1
 * @date 2023-08-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

/* include headers */
#include "ssd1306.h"
#include "font.h"
#include "string.h"

/* define command and data registers */
#define SSD1306_COMMAND_MODE 0x00
#define SSD1306_DATA_MODE 0x40

/* i2c handle for the display */
int i2c_fd;

/* framebuffer memory for the display */
char fb[1024] = {0};

void send_command(int command) {
    // write to the command registers
    wiringPiI2CWriteReg8(i2c_fd, SSD1306_COMMAND_MODE, command);
}

void send_data(int data) {
    // write to the data registers
    wiringPiI2CWriteReg8(i2c_fd, SSD1306_DATA_MODE, data);
}

unsigned const int init_sequence[] = {
        0xAE,           // Display OFF
        0x20, 0b00,     // Set Memory Addressing Mode to Horizontal addressing mode
        0x81, 0x10,     // Set Contrast Control to 0x8F
        0xA1,           // Set Segment Re-map
        0xA8, 0x3F,     // Set Multiplex Ratio to 0x3F
        0xC8,           // Set COM Output Scan Direction
        0xD3, 0x00,     // Set Display Offset to 0x00
        0xD5, 0x80,     // Set Display Clock Divide Ratio/OSC Frequency to 0x80
        0xD9, 0x22,     // Set Pre-charge Period to 0x22
        0xDA, 0x12,     // Set COM Pins Hardware Configuration to Alternative
        0xDB, 0x20,     // Set VCOMH Deselect Level to 0x20
        0xA4,           // Entire Display ON (resume)
        0xA6,           // Set Normal Display (default)
        0xAF,           // Display ON
};

void ssd1306_init(int fd) {
	delayMicroseconds(100);
    i2c_fd = fd;

    /* setup the display with the appropriate commands */
	for(int i = 0; i < 22; i++){
  		send_command(init_sequence[i]); 
	}
}

void fillDisplay() {
    // turn on all the display pixels (bypasses the framebuffer)
    send_command(0xA5);
}

void clearDisplay() {
    // turn off all the display pixels (bypasses the framebuffer)
  	send_command(0xA4);
}

void setBrightness(int brightness) {
    send_command(0x81);
    send_command(brightness);
}

void setPixel(int x, int y, int color) {
    if (x < 0 || x >= 128 || y < 0 || y >= 64) {
        return; // Coordinates out of bounds
    }

    int page = y/8;
    int bit = y % 8;

    if (color) fb[128 * page + x] |= (1 << bit);
    else fb[128 * page + x] &= ~(1 << bit); 
}

void drawLine(int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1)?1:-1;
    int sy = (y0 < y1)?1:-1;
    int err = dx - dy;
    int e2;

    while (1) {
        // Set the pixel at (x0, y0) in the framebuffer
        fb[(y0 / 8) * 128 + x0] |= (1 << (y0 % 8));

        if (x0 == x1 && y0 == y1) break;

        e2 = 2 * err;

        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }

        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void drawCharacter(int x, int y, char character) {
    if (character < 32 || character > 127) {
        // haracter is not in the printable ASCII range, so we won't draw anything.
        return;
    }

    // Each character is 5 columns wide, and each column is represented by an 8-bit value.
    // We'll loop through each column and draw it into the framebuffer.
    for (int col = 0; col < 5; col++) {
        char column_data = font[character - 32][col];

        // Loop through each bit in the column_data and draw the pixel if the bit is set (1).
        for (int bit = 0; bit < 8; bit++) {
            if (column_data & (1 << bit)) {
                // Calculate the correct page and column for the pixel in the framebuffer.
                int page = (y + bit) / 8;
                int column = x + col;

                // Set the pixel in the framebuffer at the calculated position.
                fb[page * 128 + column] |= (1 << ((y + bit) % 8));
            }
        }
    }
}

void drawString(int x, int y, const char* string){
    int i = 0;
    while(string[i] != '\0'){
        drawCharacter(x + i * 6, y, string[i]);
        i++;
    }
}

void drawBitmap(int x, int y, const char* bitmap, int width, int height) {
    int i, j;
    // find out how many bytes a row of pixels occupies
    int byte_width = (width + 7) / 8;

    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            int pixel_index = i * byte_width + j / 8;
            int pixel_bit = j % 8;

            if (bitmap[pixel_index] & (1 << (7 - pixel_bit))) {
                int pixel_x = x + j;
                int pixel_y = y + i;
                int page = pixel_y / 8;
                int column = pixel_x;

                if (page >= 0 && page < 8 && column >= 0 && column < 128) {
                    fb[page * 128 + column] |= (1 << (pixel_y % 8));
                }
            }
        }
    }
}

void fillFrameBuffer() {
    // fill all the bits in the framebuffer
    memset(fb, 0xff, 1024);
}

void clearFrameBuffer() {
    // flush all the bits of the framebuffer
    memset(fb, 0, 1024);
}

void display() {
    int i;

    // Set column address range
    send_command(0x21); // Set column address
    send_command(0);    // Start address
    send_command(127);  // End address

    // Set page address range
    send_command(0x22); // Set page address
    send_command(0);    // Start address
    send_command(7);    // End address (for 64 pixels height, 8 pages of 8 pixels each)

    // Send framebuffer data
    for (i = 0; i < 1024; i++) {
        send_data(fb[i]);
    }
}

