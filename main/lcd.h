#ifndef LCD_H
#define LCD_H

#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/spi_master.h"
#include "esp_lcd_panel_io.h"
#include "esp_check.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_heap_caps.h"
#include <stdint.h>
#include "esp_partition.h"

// PINS for ESP32 for WAVESHARE 1.47" LCD 
#define LCD_BL 22
#define MOSI_PIN 6
#define SCLK_PIN 7
#define LCD_CS 14	
#define LCD_DC 15	
#define LCD_RST 21	
#define NO_USE -1
#define LCD_SPI SPI2_HOST

#define LCD_HEIGHT 320
#define LCD_WIDTH 172
//#define SQUARE_S 10
#define STRIP_H 20

#define FRAME_COUNT 28
#define FRAME_SIZE LCD_WIDTH * LCD_HEIGHT * 2 
#define FRAME_DELAY_MS 10

typedef struct {
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;
} rectangle_t;

esp_lcd_panel_handle_t lcd_setup(void);
void lcd_draw_screen(rectangle_t* rect_array, size_t length, esp_lcd_panel_handle_t panel_handle, uint16_t colour);
void lcd_clear(esp_lcd_panel_handle_t panel_handle);
void lcd_draw_image(esp_lcd_panel_handle_t panel_handle, const uint8_t *img, size_t len);
//extern const uint8_t dex_listening_start[] asm("_binary_dex_listening_bin_start");
//extern const uint8_t dex_listening_end[]   asm("_binary_dex_listening_bin_end");
//
// Draw rectangle in given x, y location 
// with w, h width and height respectively

#endif
