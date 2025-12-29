#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "st7735.h"

/* FreeRTOS */
#include "FreeRTOS.h"
#include "event_groups.h"

/* Core */
#include "system_state.h"

/* Services */
#include "wifi_manager.h"
#include "mqtt_manager.h"
#include "factory_reset.h"

/* App */
#include "app_tasks.h"

/* UI */
#include "ui_events.h"
#include "ui_energy.h"

/* =========================================================
 * Configuração do botão
 * ========================================================= */
#define UI_BUTTON_GPIO 5
#define DEBOUNCE_MS    200

/* =========================================================
 * Protótipos locais
 * ========================================================= */
static void ui_button_isr(uint gpio, uint32_t events);
void ui_button_init(void);

/* =========================================================
 * Inicialização da aplicação
 * ========================================================= */
void app_init(void)
{
    /* --------- BOOT VISUAL RÁPIDO --------- */
    ST7735_Init();
    ST7735_FillScreen(ST7735_BLACK);
    ST7735_SetRotation(1);   /* Paisagem: 160x128 */
    ST7735_DrawString(25, 50, "Inicializando...", Font_7x10, ST7735_WHITE, ST7735_BLACK);

    /* Estado global */
    system_state_init();

    /* --------- FACTORY RESET --------- */
    factory_reset_init();
    factory_reset_task_start();

    /* Eventos da UI */
    ui_event_group = xEventGroupCreate();

    /* Botão */
    ui_button_init();

    /* Wi-Fi */
    wifi_manager_init();

    /* MQTT */
    mqtt_manager_init();

    /* Tasks */
    app_tasks_start();
}

/* =========================================================
 * ISR do botão
 * ========================================================= */
static void ui_button_isr(uint gpio, uint32_t events)
{
    static TickType_t last_isr_tick = 0;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    TickType_t now = xTaskGetTickCountFromISR();

    if ((now - last_isr_tick) >= pdMS_TO_TICKS(DEBOUNCE_MS))
    {
        last_isr_tick = now;

        xEventGroupSetBitsFromISR(
            ui_event_group,
            UI_EVT_BUTTON_PRESS,
            &xHigherPriorityTaskWoken
        );
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/* =========================================================
 * Inicialização do botão
 * ========================================================= */
void ui_button_init(void)
{
    gpio_init(UI_BUTTON_GPIO);
    gpio_set_dir(UI_BUTTON_GPIO, GPIO_IN);
    gpio_pull_up(UI_BUTTON_GPIO);   /* pull-up interno */

    gpio_set_irq_enabled_with_callback(
        UI_BUTTON_GPIO,
        GPIO_IRQ_EDGE_FALL,
        true,
        &ui_button_isr
    );
}
