#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

#include "core/system_state.h"
#include "tasks/task_env_sensors.h"

#define ENV_TASK_STACK   1024
#define ENV_TASK_PRIO    2
#define ENV_TASK_PERIOD pdMS_TO_TICKS(2000)

static void task_env_sensors(void *pv)
{
    (void) pv;

    while (1) {
        /* Simulação inicial */
        env_data_t env = {
            .temperature = 25.0f,
            .humidity    = 60.0f,
            .lux         = 120.0f,
            .valid       = true
        };

        system_state_set_env(&env);

        printf("[ENV] T=%.1f H=%.1f L=%.1f\n",
               env.temperature,
               env.humidity,
               env.lux);

        vTaskDelay(ENV_TASK_PERIOD);
    }
}

void task_env_sensors_start(void)
{
    xTaskCreate(
        task_env_sensors,
        "env",
        ENV_TASK_STACK,
        NULL,
        ENV_TASK_PRIO,
        NULL
    );
}
