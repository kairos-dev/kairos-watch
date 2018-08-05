#include <driver/gpio.h>
#include <driver/spi_master.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include <string.h>
#include <linked_list.h>

void app_main()
{
    while(1)
    {
        printf("This is a test!\n");
    }
}
