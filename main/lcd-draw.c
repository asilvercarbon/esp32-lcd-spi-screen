#include "lcd.h"
#include "esp_timer.h"

static const char *TAG = "Test LCD" ;

void app_main(void)
{
    esp_lcd_panel_handle_t panel = lcd_setup();
    //lcd_clear(panel);
    const esp_partition_t * partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, "frames");
    if (partition == NULL){
        perror("Couldn't read partition table");
    }
    printf("%lu", partition->size);

    const void *ptr;
    esp_partition_mmap_handle_t handle;
    ESP_ERROR_CHECK(esp_partition_mmap(partition, 0, FRAME_COUNT * FRAME_SIZE, ESP_PARTITION_MMAP_DATA, &ptr, &handle));

    const uint8_t* frames = (const uint8_t *)ptr;
    while(1){
        for(int idx=0; idx<27; idx++){
            int64_t t0 = esp_timer_get_time();
            //lcd_draw_image(panel, example1_start, example1_end - example1_start);
            lcd_draw_image(panel, frames + (idx * FRAME_SIZE), FRAME_SIZE);
            ESP_LOGI(TAG, "image drawn in %lld us", esp_timer_get_time() - t0);
            vTaskDelay(pdMS_TO_TICKS(FRAME_DELAY_MS));
            
        }
    }

    // print this rectangle
};
