#include <stdio.h>

#include "pico/stdlib.h"

/* Core */
#include "system_state.h"

/* Services */
#include "wifi_manager.h"
#include "mqtt_manager.h"

/* App */
#include "app_tasks.h"

void app_init(void)
{
    /* Inicialização do estado global */
    system_state_init();

    /* Wi-Fi */
    wifi_manager_init();

    /* MQTT (após Wi-Fi) */
    mqtt_manager_init();

    /* Criação das tasks */
    app_tasks_start();
}
