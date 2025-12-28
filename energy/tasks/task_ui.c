#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "task_ui.h"

/* Core */
#include "system_state.h"

/* UI */
#include "ui_energy.h"
#include "ui_environment.h"

/* =========================================================
 * Configuração
 * ========================================================= */
#define UI_REFRESH_MS   500

typedef enum {
    UI_SCREEN_SPLASH = 0,
    UI_SCREEN_ENV,
    UI_SCREEN_ENERGY
} ui_screen_t;

/* =========================================================
 * Task UI
 * ========================================================= */
static void vTaskUI(void *pv)
{
    (void) pv;

    system_state_t state;
    ui_screen_t current = UI_SCREEN_SPLASH;
    TickType_t splash_start = xTaskGetTickCount();

    /* ---------- Splash ---------- */
    UI_Energy_ShowSplash();
    UI_Env_Init();     /* prepara display */
    UI_Energy_Init();  /* desenha layout */

    for (;;)
    {
        system_state_get_env(&state.env);
        system_state_get_energy(&state.energy);
        system_state_get_wifi(&state.wifi);
        system_state_get_time(&state.time);

        /* ---------- Controle de telas ---------- */
        if (current == UI_SCREEN_SPLASH)
        {
            if ((xTaskGetTickCount() - splash_start) > pdMS_TO_TICKS(2500))
            {
                current = UI_SCREEN_ENV;
                UI_Env_Init();
            }
        }
        else if (current == UI_SCREEN_ENV)
        {
            UI_Env_Update(&state);

            /* troca automática (exemplo) */
            if (state.energy.valid)
            {
                current = UI_SCREEN_ENERGY;
                UI_Energy_Init();
            }
        }
        else if (current == UI_SCREEN_ENERGY)
        {
            UI_Energy_Update(&state);
        }

        vTaskDelay(pdMS_TO_TICKS(UI_REFRESH_MS));
    }
}

/* =========================================================
 * Start
 * ========================================================= */
void task_ui_start(void)
{
    xTaskCreate(
        vTaskUI,
        "ui",
        1024,
        NULL,
        1,
        NULL
    );
}
