#include "FreeRTOS.h"
#include "task.h"

#include "system_state.h"
#include "rtc_ds3231.h"

static void time_task(void *pv)
{
    (void) pv;

    app_datetime_t now = {0};

    rtc_ds3231_init();

    for (;;)
    {
        if (rtc_ds3231_get_time(&now)) {
            system_state_set_time(&now);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void time_manager_start(void)
{
    xTaskCreate(
        time_task,
        "time",
        1024,
        NULL,
        2,
        NULL
    );
}
