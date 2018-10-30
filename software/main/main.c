#include <driver/uart.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <driver/i2c.h>
#include "esp_freertos_hooks.h"

#include <string.h>
#include "lv_conf.h"
#include "lvgl.h"
#include <SSD1306.h>
#include <display.h>
#include "battery.h"
#include "navigation.h"
#include "keypad.h"

#include "launcher/home.h"
#include "launcher/menu.h"

#define SECOND (1000 / portTICK_PERIOD_MS)

/* Declare device descriptor */
// static lv_style_t style;
// static lv_obj_t * label;

static void ssd1306_task(void *pvParameters);

// void font_timer(void);

void lvgl_timer(void);

void user_init(void);

void app_main()
{
    user_init();
}

static void ssd1306_task(void *pvParameters)
{
    printf("%s: Started user interface task\n", __FUNCTION__);
    vTaskDelay(SECOND);
    
    battery_monitor_init();
    // input_init();
    // test_menu();

    while (1)
    {
        /*draw system call */
        // printf("> Status Battery:\n");
        // printf("\t> %d mV\n", get_battery_mv());
        // printf("\t> ADC Raw Value %d\n", _get_battery_value());
        // printf("\t> Raw Voltage: %d mV\n", get_battery_raw_voltage());

        // uint8_t keys = 0;

        // // input_read(&keys);

        // printf("> Status Keypad:\n");
        // printf("\t> LEFT KEY: %d", !(keys&0x4));
        // printf("\t- CENTER KEY: %d", !(keys&0x2));
        // printf("\t- RIGHT KEY: %d\n", !(keys&0x1));

        test_menu();
        
        display_update();

        lv_task_handler();

        vTaskDelay(SECOND);
    }
}

// void font_timer(void)
// {
//     static uint8_t index = 0;
//     if (index >= 2)
//         index=0;
//     else
//         index++;

//     switch (index) {
//     case 0:
//         style.text.font = &lv_font_dejavu_10;
//         break;
//     case 1:
//         style.text.font = &lv_font_dejavu_20;
//         break;
//     case 2:
//         style.text.font = &lv_font_dejavu_30;
//         break;
//     default:
//         break;
//     }
//     lv_label_set_style(label, &style);
//     printf("Selected builtin font %d\n", index);
// }

void lvgl_timer(void)
{
    lv_tick_inc(portTICK_RATE_MS);
}

void user_init(void)
{
    /*Gui initialization*/
    lv_init();
    // vTaskDelay(SECOND);
    display_init();
    /*lvgl screen registration */
    /*Set up the functions to access to your display*/
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.disp_flush = ssd1306_flush; /*Used in buffered mode (LV_VDB_SIZE != 0  in lv_conf.h)*/
    disp_drv.disp_fill = ssd1306_fill;//ex_disp_fill;   /*Used in unbuffered mode (LV_VDB_SIZE == 0  in lv_conf.h)*/
    disp_drv.disp_map = ssd1306_map;//ex_disp_map;    /*Used in unbuffered mode (LV_VDB_SIZE == 0  in lv_conf.h)*/
    lv_disp_drv_register(&disp_drv);
    
    keypad_init();
    /* Create user interface task */
    xTaskCreate(&ssd1306_task, "ssd1306_task", 4096, NULL, 2, NULL);
    xTaskCreate(&navigation_task, "navigation_task", 4096, NULL, 2, NULL);
    esp_register_freertos_tick_hook(lvgl_timer);

}
