#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

#include "core/system_state.h"
#include "tasks/task_energy.h"

#define ENERGY_TASK_STACK   1024
#define ENERGY_TASK_PRIO    2
#define ENERGY_TASK_PERIOD pdMS_TO_TICKS(3000)

static void task_energy(void *pv)
{
    (void) pv;

    while (1) {
        /* Simulação inicial */
        energy_data_t energy = {
            .voltage   = 220.0f,
            .current   = 1.35f,
            .power     = 297.0f,
            .energy    = 12.4f,
            .frequency = 60.0f,
            .pf        = 0.95f,
            .valid     = true
        };

        system_state_set_energy(&energy);

        printf("[ENERGY] V=%.1f I=%.2f P=%.1f E=%.2f\n",
               energy.voltage,
               energy.current,
               energy.power,
               energy.energy);

        vTaskDelay(ENERGY_TASK_PERIOD);
    }
}

void task_energy_start(void)
{
    xTaskCreate(
        task_energy,
        "energy",
        ENERGY_TASK_STACK,
        NULL,
        ENERGY_TASK_PRIO,
        NULL
    );
}
