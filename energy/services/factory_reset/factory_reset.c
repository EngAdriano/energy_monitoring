#include "factory_reset.h"

#include <stdio.h>

/* Pico */
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/watchdog.h"

/* FreeRTOS */
#include "FreeRTOS.h"
#include "task.h"

/* Auth */
#include "services/auth/auth_storage.h"

/* =====================================================
 * Configurações
 * ===================================================== */
#define FACTORY_RESET_GPIO        6
#define FACTORY_RESET_TIME_MS     5000

/* =====================================================
 * Inicialização do botão
 * ===================================================== */
void factory_reset_init(void)
{
    gpio_init(FACTORY_RESET_GPIO);
    gpio_set_dir(FACTORY_RESET_GPIO, GPIO_IN);
    gpio_pull_up(FACTORY_RESET_GPIO);
}

/* =====================================================
 * Task Factory Reset
 * ===================================================== */
static void vTaskFactoryReset(void *pv)
{
    (void) pv;

    for (;;)
    {
        if (gpio_get(FACTORY_RESET_GPIO) == 0)
        {
            TickType_t start = xTaskGetTickCount();

            while (gpio_get(FACTORY_RESET_GPIO) == 0)
            {
                if ((xTaskGetTickCount() - start) >
                    pdMS_TO_TICKS(FACTORY_RESET_TIME_MS))
                {
                    //printf("[FACTORY] Reset de credenciais\n");

                    auth_factory_reset();

                    vTaskDelay(pdMS_TO_TICKS(500));
                    watchdog_reboot(0, 0, 0);
                }
                vTaskDelay(pdMS_TO_TICKS(50));
            }
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

/* =====================================================
 * Start
 * ===================================================== */
void factory_reset_task_start(void)
{
    xTaskCreate(
        vTaskFactoryReset,
        "factory_reset",
        512,
        NULL,
        1,
        NULL
    );
}
