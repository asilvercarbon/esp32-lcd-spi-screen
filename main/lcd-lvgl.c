#include "lcd.h"
#include "esp_timer.h"

static const char *TAG = "Test LCD" ;

void app_main(void)
{
    lcd_setup();
    const esp_partition_t * partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, "frames");
    if (partition == NULL){
        perror("Couldn't read partition table");
    }
    printf("%lu", partition->size);

    const void *ptr;
    esp_partition_mmap_handle_t handle;
    ESP_ERROR_CHECK(esp_partition_mmap(partition, 0, FRAME_COUNT * FRAME_SIZE, ESP_PARTITION_MMAP_DATA, &ptr, &handle));

    const uint8_t* frames = (const uint8_t *)ptr;
    lvgl_port_lock(0);                              // wait forever; returns bool
    lv_obj_t *label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "DEX listening...");
    lv_label_set_long_mode(label, LV_LABEL_LONG_MODE_WRAP);
    lv_obj_center(label);
    
    lvgl_port_unlock();
};
