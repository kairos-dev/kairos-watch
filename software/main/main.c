// #include "esp_common.h"
#include <driver/uart.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <driver/i2c.h>
#include "esp_freertos_hooks.h"

// #include <timers.h>
#include <string.h>
#include "../lv_conf.h"
#include "../lvgl/lvgl.h"
#include "SSD1306.h"
#include "battery.h"

#define I2C_CONNECTION true
#define TICK_HANDLER   false
#define OVERCLOCK      false

#define TICK_INC_MS (10)
#define RST_PIN (LV_DRIVER_NOPIN)
#define SECOND (1000 / portTICK_PERIOD_MS)

#define PROTOCOL SSD1306_PROTO_I2C
#define ADDR     SSD1306_I2C_ADDR_0
#define I2C_BUS  0
#define SCL_PIN  4
#define SDA_PIN  0

i2c_dev_t i2c_dev =
{
    .bus = I2C_NUM_0,
    .addr = ADDR
};

/* Declare device descriptor */
static ssd1306_t dev;

static lv_style_t style;
static lv_obj_t * label;

static void ssd1306_task(void *pvParameters);

void font_timer(void);

void lvgl_timer(void);

void user_init(void);

void i2c0_init(void){
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = SDA_PIN,
        .scl_io_num = SCL_PIN,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 400000,
    };
    i2c_param_config(I2C_NUM_0, &conf);
    i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);
}

lv_obj_t* test_home()
{
    lv_obj_t *label_date, *label_icons, *label_hour;
    static lv_style_t style_icons, style_hour, style_date;
    
    lv_obj_t * scr = lv_obj_create(NULL, NULL);
    lv_scr_load(scr);
    // Create the icons style
    lv_style_copy(&style_icons, &lv_style_plain);
    style_icons.text.font = &lv_font_dejavu_20;
    style_icons.text.letter_space = 2*10;
    style_icons.text.line_space = 1*10;

    // Create the hour style
    lv_style_copy(&style_hour, &lv_style_plain);
    style_hour.text.font = &lv_font_dejavu_40;
    style_hour.text.letter_space = 2*3;
    style_hour.text.line_space = 1*3;

    // Create the date style
    lv_style_copy(&style_date, &lv_style_plain);
    style_date.text.font = &lv_font_dejavu_10;
    style_date.text.letter_space = 2;
    style_date.text.line_space = 1;

    // Create the icons label
    label_icons = lv_label_create(lv_scr_act(), NULL);
    lv_obj_set_style(label_icons, &style_icons);
    lv_label_set_text(label_icons, " "SYMBOL_BELL SYMBOL_BLUETOOTH SYMBOL_BATTERY_3);
    lv_obj_align(label_icons, scr, LV_ALIGN_IN_TOP_MID, 0, 0);

    // Create the date label
    label_date = lv_label_create(lv_scr_act(), NULL);
    lv_obj_set_style(label_date, &style_date);
    lv_label_set_text(label_date, "Fri 17 Aug");
    lv_obj_align(label_date, scr, LV_ALIGN_CENTER, 0, 0);

    // Create the hour label
    label_hour = lv_label_create(lv_scr_act(), NULL);
    lv_obj_set_style(label_hour, &style_hour);
    lv_label_set_text(label_hour, "19:09");
    lv_obj_align(label_hour, scr, LV_ALIGN_CENTER, 0, 19);

    return scr;
}

lv_obj_t* test_menu()
{
    /*Crate the list*/
    static lv_style_t style_btn_rel, style_btn_pr;
    lv_obj_t *label, *list;

    lv_obj_t *scr = lv_obj_create(NULL, NULL);
    lv_scr_load(scr);
    label = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(label, "Menu");

    list = lv_list_create(lv_scr_act(), NULL);
    lv_obj_set_size(list, 128, LV_VER_RES);
    lv_obj_align(list, scr, LV_ALIGN_CENTER, 0, 20);

    /*Add list elements*/
    char voltage[10], raw_value[10], bat_voltage[10];

    sprintf(voltage, "%dmV", get_battery_raw_voltage());
    sprintf(bat_voltage, "%dmV", get_battery_mv());
    // lv_list_add(list, SYMBOL_FILE, "New", NULL);
    // lv_list_add(list, SYMBOL_DIRECTORY, "Open", NULL);
    lv_list_add(list, NULL, voltage, NULL);
    lv_list_add(list, NULL, bat_voltage, NULL);
    lv_list_add(list, SYMBOL_CLOSE, "Delete", NULL);
    lv_list_add(list, SYMBOL_EDIT, "Edit", NULL);
    lv_list_add(list, SYMBOL_SAVE, "Save", NULL);

    lv_obj_align(label, scr, LV_ALIGN_OUT_TOP_MID, 0, 20);

    /*********************
     * Create new styles
     ********************/
    /*Create styles for the buttons*/
    lv_style_copy(&style_btn_rel, &lv_style_btn_rel);
    style_btn_rel.body.padding.ver = 2;
    style_btn_rel.body.padding.inner = 1;
    style_btn_rel.body.main_color = LV_COLOR_MAKE(0x30, 0x30, 0x30);
    style_btn_rel.body.grad_color = LV_COLOR_BLACK;
    style_btn_rel.body.border.color = LV_COLOR_SILVER;
    // style_btn_rel.body.border.width = 1;
    style_btn_rel.body.border.opa = LV_OPA_50;
    style_btn_rel.body.radius = 0;
    style_btn_rel.text.font = &lv_font_dejavu_20;

    lv_style_copy(&style_btn_pr, &style_btn_rel);
    style_btn_pr.body.padding.ver = 2;
    style_btn_pr.body.padding.inner = 1;
    style_btn_pr.body.main_color = LV_COLOR_MAKE(0x55, 0x96, 0xd8);
    style_btn_pr.body.grad_color = LV_COLOR_MAKE(0x37, 0x62, 0x90);
    style_btn_pr.text.color = LV_COLOR_MAKE(0xbb, 0xd5, 0xf1);
    style_btn_pr.text.font = &lv_font_dejavu_20;
    
    lv_list_set_sb_mode(list, LV_SB_MODE_DRAG);
    lv_list_set_style(list, LV_LIST_STYLE_BG, &lv_style_transp_tight);
    lv_list_set_style(list, LV_LIST_STYLE_SCRL, &lv_style_transp_tight);
    lv_list_set_style(list, LV_LIST_STYLE_BTN_REL, &style_btn_rel); /*Set the new button styles*/
    lv_list_set_style(list, LV_LIST_STYLE_BTN_PR, &style_btn_pr);

    return scr;
}

void app_main()
{
    user_init();
}

static void ssd1306_task(void *pvParameters)
{
    printf("%s: Started user interface task\n", __FUNCTION__);
    vTaskDelay(SECOND);
    ssd1306_set_whole_display_lighting(&dev, false);

    // test_menu();
    // test_home();
    // lv_obj_t *scr2 = test_menu();
    battery_monitor_init();
    uint16_t timer = 0;
    uint8_t change_screen = 0;
    test_menu();

    while (1) {
        /*draw system call */

        if((timer++) > 500)
        {
            // change_screen^=1;
            timer = 0;
            // printf("> Battery Level: %d\n> Battery Voltage: %dmV\n", _get_battery_value(), get_battery_raw_voltage());
            printf("> Status Battery:\n");
            printf("\t> %d mV\n", get_battery_mv());
            printf("\t> ADC Raw Value %d\n", _get_battery_value());
            printf("\t> Raw Voltage: %d mV\n", get_battery_raw_voltage());

            // if(change_screen == 1)
            // {
            //     test_menu();
            // }
            // else 
            // {
            //     test_home();
            // }
            
        }
        
        if(ssd1306_need_redraw())
        {
            ssd1306_load_frame_buffer(&dev);
        }
        lv_task_handler();

        vTaskDelay(1);
    }
}

void font_timer(void)
{
    static uint8_t index = 0;
    if (index >= 2)
        index=0;
    else
        index++;

    switch (index) {
    case 0:
        style.text.font = &lv_font_dejavu_10;
        break;
    case 1:
        style.text.font = &lv_font_dejavu_20;
        break;
    case 2:
        style.text.font = &lv_font_dejavu_30;
        break;
    default:
        break;
    }
    lv_label_set_style(label, &style);
    printf("Selected builtin font %d\n", index);
}

void lvgl_timer(void)
{
    lv_tick_inc(portTICK_RATE_MS);
}

void user_init(void)
{
    /*Gui initialization*/
    lv_init();

    /*Screen initialization*/
    i2c0_init();

    dev.i2c_dev = (lv_i2c_handle_t)i2c_dev;
    dev.protocol = PROTOCOL ; //SSD1306_PROTO_SPI3;
    dev.screen   = SSD1306_SCREEN; //SSD1306_SCREEN
    dev.width    = LV_HOR_RES;
    dev.height   = LV_VER_RES;
    // dev.rst_pin  = RST_PIN; //No RST PIN USED
    // ssd1306_command(&dev, 0xDA);

    while (ssd1306_init(&dev) != 0) {
        printf("%s: failed to init SSD1306 lcd\n", __func__);
        vTaskDelay(SECOND);
    }
    
    ssd1306_set_whole_display_lighting(&dev, true);

    // ssd1306_set_scan_direction_fwd(&dev,true);
    ssd1306_set_scan_direction_fwd(&dev,false);
    ssd1306_set_inversion(&dev, false);
    // Flip display 180º
    ssd1306_set_segment_remapping_enabled(&dev, true);

    vTaskDelay(SECOND);

    /*lvgl screen registration */
    /*Set up the functions to access to your display*/
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.disp_flush = ssd1306_flush; /*Used in buffered mode (LV_VDB_SIZE != 0  in lv_conf.h)*/
    disp_drv.disp_fill = ssd1306_fill;//ex_disp_fill;   /*Used in unbuffered mode (LV_VDB_SIZE == 0  in lv_conf.h)*/
    disp_drv.disp_map = ssd1306_map;//ex_disp_map;    /*Used in unbuffered mode (LV_VDB_SIZE == 0  in lv_conf.h)*/
    lv_disp_drv_register(&disp_drv);
    
    /* Create user interface task */
    xTaskCreate(&ssd1306_task, "ssd1306_task", 4096, NULL, 2, NULL);

#if (!TICK_HANDLER)
    esp_register_freertos_tick_hook(lvgl_timer);
#endif
}
