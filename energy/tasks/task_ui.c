#include <stdio.h>

#include "st7735.h"

#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

#include "task_ui.h"

/* Core */
#include "system_state.h"

/* UI */
#include "ui_energy.h"
#include "ui_environment.h"

/* Eventos UI */
#include "ui_events.h"

/* =========================================================
 * Configuração
 * ========================================================= */
#define UI_REFRESH_MS   500

/* =========================================================
 * Task UI
 * ========================================================= */
static void vTaskUI(void *pv)
{
    (void) pv;

    system_state_t state;
    TickType_t splash_start = xTaskGetTickCount();

    /* ---------- Splash ---------- */
    system_state_set_ui_screen(UI_SCREEN_SPLASH);

    UI_Energy_ShowSplash();
    UI_Env_Init();     /* prepara display */
    //UI_Energy_Init();  /* desenha layout base */

    for (;;)
    {
        /* Atualiza dados */
        system_state_get_env(&state.env);
        system_state_get_energy(&state.energy);
        system_state_get_wifi(&state.wifi);
        system_state_get_time(&state.time);

        ui_screen_t current = system_state_get_ui_screen();

        /* ---------- Splash ---------- */
        if (current == UI_SCREEN_SPLASH)
        {
            if ((xTaskGetTickCount() - splash_start) > pdMS_TO_TICKS(2500))
            {
                system_state_set_ui_screen(UI_SCREEN_ENERGY);
                UI_Energy_Init();
            }

            vTaskDelay(pdMS_TO_TICKS(UI_REFRESH_MS));
            continue;
        }

        /* ---------- Atualiza tela ---------- */
        if (current == UI_SCREEN_ENERGY)
        {
            UI_Energy_Update(&state);
        }
        else if (current == UI_SCREEN_ENV)
        {
            UI_Env_Update(&state);
        }

        /* ---------- Aguarda botão ou refresh ---------- */
        EventBits_t ev = xEventGroupWaitBits(
            ui_event_group,
            UI_EVT_BUTTON_PRESS,
            pdTRUE,     /* limpa o bit */
            pdFALSE,
            pdMS_TO_TICKS(UI_REFRESH_MS)
        );

        if (ev & UI_EVT_BUTTON_PRESS)
        {
            system_state_toggle_ui_screen();

            /* redesenha layout ao trocar */
            if (system_state_get_ui_screen() == UI_SCREEN_ENERGY)
                UI_Energy_Init();
            else
                UI_Env_Init();
        }
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
