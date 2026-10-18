#include "lcd.h"
#include "esp_timer.h"

static const char *TAG = "Test LCD" ;

void app_main(void)
{
    // So this is our face coordinates:
    rectangle_t closed_mouth[] = {
        {.x=(172*(0.25)),             .y=(320*(0.25)),                    .w=20, .h=20},
        {.x=(172*(0.75)),             .y=(320*(0.25)),                     .w=20, .h=20},
        {.x=((172*(0.25)) - 20),      .y=((320 * (0.75)) - 20),           .w=20, .h=20},
        {.x=((172*(0.75)) + 20),      .y=((320 * (0.75)) - 20),           .w=20, .h=20},
        {.x=(172*(0.25)),             .y=(320 * (0.75)),                  .w=106, .h=20},
    };

    rectangle_t open_mouth[] = {
        {.x=(172*(0.25)),             .y=(320*(0.25)),                    .w=20, .h=20},
        {.x=(172*(0.75)),             .y=(320*(0.25)),                     .w=20, .h=20},
        {.x=((172*(0.25)) - 20),      .y=((320 * (0.75)) - 20),           .w=20, .h=20},
        {.x=((172*(0.75)) + 20),      .y=((320 * (0.75)) - 20),           .w=20, .h=20},
        {.x=(172*(0.25)),             .y=(320 * (0.75)),                  .w=106, .h=20},
        {.x=(172*(0.25)),             .y=(320 * (0.75) - 40),                  .w=106, .h=20},
    };

    esp_lcd_panel_handle_t panel = lcd_setup();
    lcd_clear(panel);
    uint16_t open_colour = 0xbe83;
    uint16_t closed_colour = 0xbe83;
    /*
    while (1){
        lcd_clear(panel);
        lcd_draw_screen(closed_mouth, 5, panel, open_colour);
        vTaskDelay(500/portTICK_PERIOD_MS);
        //lcd_clear(panel);
        lcd_draw_screen(open_mouth, 6, panel, closed_colour);
        vTaskDelay(500/portTICK_PERIOD_MS);
    }
    */


    while(1){
        int64_t t0 = esp_timer_get_time();
        lcd_draw_image(panel, example1_start, example1_end - example1_start);
        ESP_LOGI(TAG, "image drawn in %lld us", esp_timer_get_time() - t0);
        vTaskDelay(portMAX_DELAY);
    }

    // print this rectangle
};
