/* SPI Master example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "../lvgl/lvgl.h"
#include "../lvgl/lv_misc/lv_color.h"
#include "../lv_examples/lv_apps/demo/demo.h"
#include "../lvgl/lv_misc/lv_fonts/lv_symbol_def.h"
#include "esp_freertos_hooks.h"
#include "esp_log.h"


// #include "../drv/disp_spi.h"
// #include "../drv/ili9341.h"
// #include "ssd1306.h"
#include "SSD1306.h"

//void disp_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p);

static void lv_tick_task(void);
void test_home()
{
    lv_obj_t *label_date, *label_icons, *label_hour;
    static lv_style_t style_txt;
    lv_style_copy(&style_txt, &lv_style_plain);
    style_txt.text.font = &lv_font_dejavu_10;
    style_txt.text.letter_space = 2;
    style_txt.text.line_space = 1;
    // style_txt.text.color = LV_COLOR_HEX(0x606060);

    label_date = lv_label_create(lv_scr_act(), NULL);
    lv_obj_set_style(label_date, &style_txt);
    lv_label_set_text(label_date, "Fri 17 Aug");
    lv_obj_align(label_date, NULL, LV_ALIGN_IN_TOP_LEFT, 5, 5);

    label_icons = lv_label_create(lv_scr_act(), NULL);
    lv_obj_set_style(label_icons, &style_txt);
    lv_label_set_text(label_icons, " "SYMBOL_BELL SYMBOL_WIFI SYMBOL_BLUETOOTH SYMBOL_MUTE SYMBOL_BATTERY_3);
    lv_obj_align(label_icons, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 5);

    static lv_style_t style_hour;
    lv_style_copy(&style_hour, &lv_style_plain);
    style_hour.text.font = &lv_font_dejavu_40;
    style_hour.text.letter_space = 2;
    style_hour.text.line_space = 1;
    // style_hour.text.color = LV_COLOR_HEX(0x606060);

    label_hour = lv_label_create(lv_scr_act(), NULL);
    lv_obj_set_style(label_hour, &style_hour);
    lv_label_set_text(label_hour, "19:09");
    lv_obj_align(label_hour, NULL, LV_ALIGN_CENTER, 0, 10);
}

void test_menu()
{
    /************************
     * Create a default list
     ************************/

    /*Crate the list*/
    lv_obj_t * list1 = lv_list_create(lv_scr_act(), NULL);
    lv_obj_set_size(list1, 100, LV_VER_RES);
    lv_obj_align(list1, NULL, LV_ALIGN_CENTER, 0, 20);

    /*Add list elements*/
    lv_list_add(list1, SYMBOL_FILE, "New", list_release_action);
    lv_list_add(list1, SYMBOL_DIRECTORY, "Open", list_release_action);
    lv_list_add(list1, SYMBOL_CLOSE, "Delete", list_release_action);
    lv_list_add(list1, SYMBOL_EDIT, "Edit", list_release_action);
    lv_list_add(list1, SYMBOL_SAVE, "Save", list_release_action);

    /*Create a label above the list*/
    static lv_style_t label_style;
    // label_style.text.font = &lv_font_dejavu_20;
    lv_obj_t * label;
    label = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(label, "Menu");
    // lv_label_set_style(label, &label_style);
    lv_obj_align(label, list1, LV_ALIGN_OUT_TOP_MID, 0, 0);

    /*********************
     * Create new styles
     ********************/
    /*Create a scroll bar style*/
    static lv_style_t style_sb;
    lv_style_copy(&style_sb, &lv_style_plain);
    style_sb.body.main_color = LV_COLOR_BLACK;
    style_sb.body.grad_color = LV_COLOR_BLACK;
    style_sb.body.border.color = LV_COLOR_WHITE;
    style_sb.body.border.width = 1;
    style_sb.body.border.opa = LV_OPA_70;
    style_sb.body.radius = LV_RADIUS_CIRCLE;
    style_sb.body.opa = LV_OPA_60;

    /*Create styles for the buttons*/
    static lv_style_t style_btn_rel;
    static lv_style_t style_btn_pr;
    lv_style_copy(&style_btn_rel, &lv_style_btn_rel);
    style_btn_rel.body.padding.ver = 0;
    style_btn_rel.body.padding.hor = 0;
    style_btn_rel.body.padding.inner = 0;
    style_btn_rel.body.main_color = LV_COLOR_MAKE(0x30, 0x30, 0x30);
    style_btn_rel.body.grad_color = LV_COLOR_BLACK;
    style_btn_rel.body.border.color = LV_COLOR_SILVER;
    style_btn_rel.body.border.width = 1;
    style_btn_rel.body.border.opa = LV_OPA_50;
    style_btn_rel.body.radius = 0;
    style_btn_rel.text.font = &lv_font_dejavu_10;

    lv_style_copy(&style_btn_pr, &style_btn_rel);
    style_btn_pr.body.padding.ver = 0;
    style_btn_pr.body.padding.hor = 0;
    style_btn_pr.body.padding.inner = 0;
    style_btn_pr.body.main_color = LV_COLOR_MAKE(0x55, 0x96, 0xd8);
    style_btn_pr.body.grad_color = LV_COLOR_MAKE(0x37, 0x62, 0x90);
    style_btn_pr.text.color = LV_COLOR_MAKE(0xbb, 0xd5, 0xf1);
    style_btn_pr.text.font = &lv_font_dejavu_10;
    
    lv_list_set_sb_mode(list1, LV_SB_MODE_DRAG);
    lv_list_set_style(list1, LV_LIST_STYLE_BG, &lv_style_transp_tight);
    lv_list_set_style(list1, LV_LIST_STYLE_SCRL, &lv_style_transp_tight);
    lv_list_set_style(list1, LV_LIST_STYLE_BTN_REL, &style_btn_rel); /*Set the new button styles*/
    lv_list_set_style(list1, LV_LIST_STYLE_BTN_PR, &style_btn_pr);
}

void button_example()
{
	/*Create a screen*/
	lv_obj_t * scr = lv_obj_create(NULL, NULL);
	lv_scr_load(scr);       						    /*Load the screen*/

	/*Create 2 buttons*/
	lv_obj_t * btn1 = lv_btn_create(scr, NULL);         /*Create a button on the screen*/
	lv_btn_set_fit(btn1, true, true);                   /*Enable to automatically set the size according to the content*/
	lv_obj_set_pos(btn1, 0, 0);   
	lv_obj_align(btn1, NULL, LV_ALIGN_CENTER, 0, 0);           		    /*Set the position of the button*/

	/*Add labels to the buttons*/
	lv_obj_t * label1 = lv_label_create(btn1, NULL);	/*Create a label on the first button*/
	lv_label_set_text(label1, "Button 1");    
}

void text_example()
{
	/*Create label on the screen. By default it will inherit the style of the screen*/
	lv_obj_t * title = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(title, "Text Label");
	lv_obj_align(title, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);  /*Align to the top*/
	lv_obj_set_size(title, 10, 50);
}

void led_example()
{
	/*Create a style for the LED*/
	static lv_style_t style_led;
	lv_style_copy(&style_led, &lv_style_pretty_color);
	style_led.body.radius = LV_RADIUS_CIRCLE;
	style_led.body.main_color = LV_COLOR_MAKE(0xb5, 0x0f, 0x04);
	style_led.body.grad_color = LV_COLOR_MAKE(0x50, 0x07, 0x02);
	style_led.body.border.color = LV_COLOR_MAKE(0xfa, 0x0f, 0x00);
	style_led.body.border.width = 3;
	style_led.body.border.opa = LV_OPA_30;
	style_led.body.shadow.color = LV_COLOR_MAKE(0xb5, 0x0f, 0x04);
	style_led.body.shadow.width = 10;


	/*Create a LED and switch it ON*/
	lv_obj_t * led1  = lv_led_create(lv_scr_act(), NULL);
	lv_obj_set_style(led1, &style_led);
	lv_obj_align(led1, NULL, LV_ALIGN_IN_TOP_MID, 40, 0);
	lv_led_on(led1);

	/*Copy the previous LED and set a brightness*/
	lv_obj_t * led2  = lv_led_create(lv_scr_act(), led1);
	lv_obj_align(led2, led1, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
	lv_led_set_bright(led2, 190);

	/*Copy the previous LED and switch it OFF*/
	lv_obj_t * led3  = lv_led_create(lv_scr_act(), led1);
	lv_obj_align(led3, led2, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
	lv_led_off(led3);

	/*Create 3 labels next to the LEDs*/
	lv_obj_t * label = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(label, "LED On");
	lv_obj_align(label, led1, LV_ALIGN_OUT_LEFT_MID, -40, 0);

	label = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(label, "LED half");
	lv_obj_align(label, led2, LV_ALIGN_OUT_LEFT_MID, -40, 0);

	label = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(label, "LED Off");
	lv_obj_align(label, led3, LV_ALIGN_OUT_LEFT_MID, -40, 0);
}

void app_main()
{
	lv_init();
	ssd1306_t display = {
		.protocol = SSD1306_PROTO_I2C,
		.screen = SSD1306_SCREEN,
		// .lv_i2c_handle_t = SSD1306_I2C_ADDR_0,
		.width = 128,
		.width = 64
	};
	ssd1306_init(&display);
	// ssd1306_init(0, 4, 0);
	// ssd1306_draw_rectangle(0, 10, 30, 20, 20, 1);

	lv_disp_drv_t disp;
	lv_disp_drv_init(&disp);
	disp.disp_flush = ssd1306_flush;
	lv_disp_drv_register(&disp);

	esp_register_freertos_tick_hook(lv_tick_task);

	// demo_create();
	// button_example();
	// text_example();
	// test_menu();
	test_home();
	// led_example();

	while(1) {
		vTaskDelay(1);
		lv_task_handler();
	}
}

// void disp_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p)
// {
// 	int32_t act_x1 = x1 < 0 ? 0 : x1;
//     int32_t act_y1 = y1 < 0 ? 0 : y1;
//     int32_t act_x2 = x2 > LV_HOR_RES - 1 ? LV_HOR_RES - 1 : x2;
//     int32_t act_y2 = y2 > LV_VER_RES - 1 ? LV_VER_RES - 1 : y2;

//     int32_t x;
//     int32_t y;
//     unsigned int p;

//     for (y = act_y1; y <= act_y2; y++)
//     {
//         for (x = act_x1; x <= act_x2; x++)
//         {
//             p = 0;
//             p = y >> 3; /* :8 */
//             p = p << 7; /* *128 */
//             p += x;
//             if (lv_color_to1(*color_p) != 0)
//             {
// 				char out_string[30];	
// 				// sprintf(out_string, "x: %u - y: %u - COLOR: %u\n", x, y, lv_color_to1(*color_p));
// 				sprintf(out_string, "x: %u - y: %u - COLOR: %u\n", x, y, SSD1306_COLOR_BLACK);
// 				printf(out_string);
// 				// continue;
// 				// ssd1306_draw_pixel(0, x, y, SSD1306_COLOR_BLACK);
// 				// ssd1306_refresh(0, true);
//             }
//             else
//             {
// 				char out_string[30];	
// 				sprintf(out_string, "x: %u - y: %u - COLOR: %u\n", x, y, SSD1306_COLOR_WHITE);
// 				// sprintf(out_string, "x: %u - y: %u - COLOR: %u\n", x, y, lv_color_to1(*color_p));
// 				printf(out_string);
// 				ssd1306_draw_pixel(0, x, y, SSD1306_COLOR_WHITE);
// 				ssd1306_refresh(0, true);
//             }
//             color_p++;
//         }
// 		lv_flush_ready();
//     }
// 	lv_flush_ready();
// }

static void lv_tick_task(void)
{
	lv_tick_inc(portTICK_RATE_MS);
}
