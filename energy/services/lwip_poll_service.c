#include "pico/cyw43_arch.h"
#include "FreeRTOS.h"
#include "task.h"

void vTaskLWIPPoll(void *pv)
{
    (void) pv;

    while (1)
    {
        cyw43_arch_poll();
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}
