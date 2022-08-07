#include "kairos.h"

static const char *TAG = "KAIROS";

void app_main(void)
{
    KAIROS_LOGI(TAG, "System is starting.");
    system_init();
    xTaskCreatePinnedToCore(task_test_display, TAG, configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL, APP_CPU_NUM);

    while(1);
}
