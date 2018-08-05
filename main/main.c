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
#include "ssd1306.h"

void disp_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p);

static void lv_tick_task(void);

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
	ssd1306_init(0, 4, 0);
	// ssd1306_draw_rectangle(0, 10, 30, 20, 20, 1);

	lv_disp_drv_t disp;
	lv_disp_drv_init(&disp);
	disp.disp_flush = disp_flush;
	lv_disp_drv_register(&disp);

	esp_register_freertos_tick_hook(lv_tick_task);

	// demo_create();
	// button_example();
	text_example();
	// led_example();

	while(1) {
		vTaskDelay(1);
		lv_task_handler();
	}
}

void disp_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p)
{
	int32_t act_x1 = x1 < 0 ? 0 : x1;
    int32_t act_y1 = y1 < 0 ? 0 : y1;
    int32_t act_x2 = x2 > LV_HOR_RES - 1 ? LV_HOR_RES - 1 : x2;
    int32_t act_y2 = y2 > LV_VER_RES - 1 ? LV_VER_RES - 1 : y2;

    int32_t x;
    int32_t y;
    unsigned int p;

    for (y = act_y1; y <= act_y2; y++)
    {
        for (x = act_x1; x <= act_x2; x++)
        {
            p = 0;
            p = y >> 3; /* :8 */
            p = p << 7; /* *128 */
            p += x;
            if (lv_color_to1(*color_p) != 0)
            {
				char out_string[30];	
				// sprintf(out_string, "x: %u - y: %u - COLOR: %u\n", x, y, lv_color_to1(*color_p));
				sprintf(out_string, "x: %u - y: %u - COLOR: %u\n", x, y, SSD1306_COLOR_BLACK);
				printf(out_string);
				// continue;
				// ssd1306_draw_pixel(0, x, y, SSD1306_COLOR_BLACK);
				// ssd1306_refresh(0, true);
            }
            else
            {
				char out_string[30];	
				sprintf(out_string, "x: %u - y: %u - COLOR: %u\n", x, y, SSD1306_COLOR_WHITE);
				// sprintf(out_string, "x: %u - y: %u - COLOR: %u\n", x, y, lv_color_to1(*color_p));
				printf(out_string);
				ssd1306_draw_pixel(0, x, y, SSD1306_COLOR_WHITE);
				ssd1306_refresh(0, true);
            }
            color_p++;
        }
		lv_flush_ready();
    }
	lv_flush_ready();
}

static void lv_tick_task(void)
{
	lv_tick_inc(portTICK_RATE_MS);
}
