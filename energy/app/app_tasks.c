#include "FreeRTOS.h"
#include "task.h"

/* Services */
#include "wifi_manager.h"
#include "mqtt_manager.h"
#include "time_manager.h"
#include "lwip_poll_service.h"
#include "mqtt_payload_service.h"

/* Tasks */
#include "task_env_sensors.h"
#include "task_energy.h"

void app_tasks_start(void)
{
    /* Tasks de aquisição */
    task_env_sensors_start();
    task_energy_start();
    time_manager_start();

    /* Task lwIP (infraestrutura) */
    xTaskCreate(
        vTaskLWIPPoll,
        "lwip",
        512,
        NULL,
        3,
        NULL
    );

    /* Tasks MQTT */
    xTaskCreate(
        vTaskMQTTConnection,
        "mqtt_conn",
        1024,
        NULL,
        2,
        NULL
    );

    xTaskCreate(
        vTaskMQTTPublisher,
        "mqtt_pub",
        1024,
        NULL,
        2,
        NULL
    );

    /* Task de payload MQTT */
    xTaskCreate(
        vTaskMQTTPayload,
        "mqtt_payload",
        1024,
        NULL,
        2,
        NULL
    );
}
