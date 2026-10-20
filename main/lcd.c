#include "lcd.h"
#include "string.h"

static const char *TAG = "lcd";

void lcd_setup(){

    gpio_config_t gpio_cfg = {
        .pin_bit_mask = 1ULL << LCD_BL,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
    };
    spi_bus_config_t bus_cfg = {
        .mosi_io_num = MOSI_PIN,
        .sclk_io_num = SCLK_PIN,        
        .miso_io_num = NO_USE,
        .quadwp_io_num = NO_USE,
        .quadhd_io_num = NO_USE,
    };
    esp_lcd_panel_io_spi_config_t panel_io_cfg = {
        .cs_gpio_num = LCD_CS, 
        .dc_gpio_num = LCD_DC,
        .spi_mode = 0,
        .pclk_hz = 20 * 1000 * 1000,
        .trans_queue_depth = 1,
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
    };
    esp_lcd_panel_dev_config_t dev_panel_cfg ={
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
        .data_endian = LCD_RGB_DATA_ENDIAN_BIG,
        .bits_per_pixel = 16,
        .reset_gpio_num = LCD_RST,
    };

    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_handle_t panel_handle = NULL;

    const lvgl_port_cfg_t lvgl_cfg = ESP_LVGL_PORT_INIT_CONFIG();
    // Initialising SPI
    ESP_ERROR_CHECK(spi_bus_initialize(LCD_SPI, &bus_cfg, SPI_DMA_CH_AUTO));
    ESP_ERROR_CHECK(gpio_config(&gpio_cfg));

    // st7789 controller setup
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi(LCD_SPI, &panel_io_cfg, &io_handle));
    ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(io_handle, &dev_panel_cfg, &panel_handle));
    
    // LCD panel setup
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_set_gap(panel_handle, 34, 0));
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, 1));
    ESP_ERROR_CHECK(gpio_set_level(LCD_BL, 1));
    //ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_handle, true));
    ESP_ERROR_CHECK(lvgl_port_init(&lvgl_cfg));

    lvgl_port_display_cfg_t disp_cfg = {
        .io_handle = io_handle,    
        .panel_handle = panel_handle,
        .buffer_size = 5504,       
        .double_buffer = 1,     
        .hres = 172,
        .vres = 320,
        .monochrome = 0,
        .flags.buff_dma = 1,
        .flags.swap_bytes = 1,
    };
    lv_display_t *disp = lvgl_port_add_disp(&disp_cfg);
    if (disp == NULL){
        ESP_LOGE(TAG,  "couldn't initialise display");
    };

}

void lcd_draw_screen(rectangle_t* rect_array, size_t length, esp_lcd_panel_handle_t panel_handle, uint16_t colour){
    for(int i=0; i<length; i++){
        size_t pixel_count =  (rect_array[i].w * rect_array[i].h);
        uint16_t *buffer = heap_caps_malloc(pixel_count * sizeof(uint16_t), MALLOC_CAP_DMA); 
        for(int j=0; j<pixel_count; j++){
            buffer[j] = colour;
        };
        ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(
            panel_handle, rect_array[i].x,
            rect_array[i].y,
            rect_array[i].x + rect_array[i].w,
            rect_array[i].y + rect_array[i].h,
            buffer)
        );
    }
}

void lcd_clear(esp_lcd_panel_handle_t panel_handle){
    uint16_t strip_pixels = LCD_WIDTH * STRIP_H;
    size_t pixel_bg = strip_pixels * 2;
    uint16_t *bg = heap_caps_malloc(pixel_bg * sizeof(uint16_t), MALLOC_CAP_DMA); 
    for(int i=0; i<pixel_bg; i++){
        bg[i] = 0x0000;
    }
    for(int i=0; i<16; i++){
        int y = STRIP_H * i;
        ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(panel_handle, 0, y, LCD_WIDTH, y + STRIP_H, bg));
    }
}


void lcd_draw_image(esp_lcd_panel_handle_t panel_handle, const uint8_t *img, size_t len){
    const size_t expected = LCD_WIDTH * LCD_HEIGHT * 2;
    if (len != expected){
        ESP_LOGI(TAG, "image is %u bytes, expected %u", (unsigned)len, (unsigned)expected);
        return;
    }

    const size_t row_bytes = LCD_WIDTH * 2;
    const size_t strip_bytes = row_bytes * STRIP_H;
    const int strips = LCD_HEIGHT / STRIP_H ;

    static uint8_t *buf[2];
    if(buf[0] == NULL){
        buf[0] = heap_caps_malloc(strip_bytes, MALLOC_CAP_DMA);
        buf[1] = heap_caps_malloc(strip_bytes, MALLOC_CAP_DMA);
        if (buf[0] == NULL || buf[1] == NULL) {
            ESP_LOGE(TAG, "no DMA memory for two %u-byte strips", (unsigned)strip_bytes);
            heap_caps_free(buf[0]);
            heap_caps_free(buf[1]);
            buf[0] = buf[1] = NULL;
            return;
        }
    }

    for(int i=0; i<strips; i++){
        uint8_t *strip = buf[i%2];
        int y = i * STRIP_H;
        memcpy(strip, img + i * strip_bytes, strip_bytes);
        ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(panel_handle, 0, y, LCD_WIDTH, y + STRIP_H, strip));
    }

}




