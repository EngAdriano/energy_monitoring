#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "FreeRTOS.h"
#include "task.h"
#include "system_state.h"
#include "task_env_sensors.h"
#include "task_energy.h"
#include "wifi_manager.h"
#include "mqtt_manager.h"
#include "lwip_poll_service.h"
#include "mqtt_payload_service.h"


int main()
{
    stdio_init_all();

    system_state_init();

    /* Sensores podem iniciar antes */
    task_env_sensors_start();
    task_energy_start();

    /* Wi-Fi PRIMEIRO */
    wifi_manager_init();

    /* MQTT SÓ DEPOIS do Wi-Fi */
    mqtt_manager_init();

    /* lwIP poll (obrigatória) */
    xTaskCreate(vTaskLWIPPoll,       "lwip",     512,  NULL, 3, NULL);

    /* Tasks MQTT */
    xTaskCreate(vTaskMQTTConnection, "mqtt_con", 1024, NULL, 2, NULL);
    xTaskCreate(vTaskMQTTPublisher,  "mqtt_pub", 1024, NULL, 2, NULL);
    xTaskCreate(vTaskMQTTPayload,    "mqtt_pay", 1024, NULL, 2, NULL);

    vTaskStartScheduler();

    while (true) {}
}
