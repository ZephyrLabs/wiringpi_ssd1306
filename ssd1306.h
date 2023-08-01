/**
 * @file ssd1306.h
 * @author Sravan Senthilnathan
 * @brief Header for SSD1306 OLED module driver (128x64 pixels)
 * @version 0.1
 * @date 2023-08-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#define SSD1306_I2C_ADDRESS 0x3C

/* methods for sending data and commands */

/**
 * @brief Write to SSD1306 command register
 * 
 * @param command 
 */
void send_command(int command);

/**
 * @brief Write to the SSD1306 data register
 * 
 * @param data 
 */
void send_data(int data);

/**
 * @brief Initialize the SSD1306 display over i2C
 * 
 * @param fd i2C handler from wiringPiI2CSetup(<Address>)
 */
void ssd1306_init(int fd);

/**
 * @brief Fill the screen with pixels (note: this bypasses the framebuffer)
 * 
 */
void fillDisplay();

/**
 * @brief Clear all the pixels on screen (note: this bypasses the framebuffer)
 * 
 */
void clearDisplay();

/**
 * @brief Fill the framebuffer and turn on all the pixels
 * 
 */
void fillFrameBuffer();

/**
 * @brief Clear the framebuffer and flush all the data
 * 
 */
void clearFrameBuffer();

/**
 * @brief Set the Brightness of the pixels
 * 
 * @param brightness 8 bit brightness from 0 to 255 (0x00 to 0xFF)
 *                   note: the display is set to 127 (0x7F) brightness by default 
 */
void setBrightness(int brightness);

/**
 * @brief Set the pixel at the given display coordinates *in the framebuffer* (note: ignores out of bound values)
 * 
 * @param x x coordinate of the pixel
 * @param y y coordinate of the pixel
 * @param color 
 */
void setPixel(int x, int y, int color);

/**
 * @brief Draws a line of pixels between the two coordinate points *in the framebuffer*
 * 
 * @param x0 
 * @param y0 
 * @param x1 
 * @param y1 
 */
void drawLine(int x0, int y0, int x1, int y1);

/**
 * @brief Draw a 5x8 size character at the coordinate point *in the framebuffer*
 * 
 * @param x 
 * @param y 
 * @param character 
 */
void drawCharacter(int x, int y, char character);

/**
 * @brief Draws multiple characters in sequence at the coordinate points *in the framebuffer* (note: the framebuffer wraps around)
 * 
 * @param x 
 * @param y 
 * @param string 
 */
void drawString(int x, int y, const char* string);

/**
 * @brief Draws a bitmap based on given a char array of pixel values *in the framebuffer*
 * 
 * @param x 
 * @param y 
 * @param bitmap 
 * @param width 
 * @param height 
 */
void drawBitmap(int x, int y, const char *bitmap, int width, int height);

/**
 * @brief callback to refresh the framebuffer to the display
 * 
 */
void display();
