#include "lcd.h"
#include "ui.h"

void app_main(void)
{
    lcd_setup();
    ui_init();
    while(1){
        ui_set_mode(UI_WAITING);
        vTaskDelay(pdMS_TO_TICKS(2000));
        ui_set_mode(UI_LISTENING);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
};
