#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

#include "system_state.h"
#include "pzem004t.h"

static void task_energy(void *pv)
{
    (void) pv;

    energy_data_t energy = {0};
    pzem_data_t pzem;

    pzem_init();

    for (;;)
    {
        bool ok = pzem_read(&pzem);

        if (ok) {
            energy.voltage   = pzem.voltage;
            energy.current   = pzem.current;
            energy.power     = pzem.power;
            energy.energy    = pzem.energy;
            energy.frequency = pzem.frequency;
            energy.pf        = pzem.pf;
            energy.valid     = true;
        } else {
            energy.valid = false;
        }

        system_state_set_energy(&energy);

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void task_energy_start(void)
{
    xTaskCreate(
        task_energy,
        "energy",
        1024,
        NULL,
        2,
        NULL
    );
}
