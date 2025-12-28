#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

#include "system_state.h"
#include "mqtt_manager.h"
#include "mqtt_payload_service.h"

/* =========================================================
 * Configuração
 * ========================================================= */
#define MQTT_TOPIC_STATUS       "embarca/energy/status"
#define MQTT_PAYLOAD_PERIOD_MS  5000

void vTaskMQTTPayload(void *pv)
{
    (void) pv;

    char payload[512];

    env_data_t    env;
    energy_data_t energy;

    for (;;)
    {
        /* Aguarda MQTT conectado */
        if (!mqtt_is_connected())
        {
            vTaskDelay(pdMS_TO_TICKS(1000));
            continue;
        }

        /* Lê estado global (thread-safe) */
        system_state_get_env(&env);
        system_state_get_energy(&energy);

        /* Opcional: só publica se dados forem válidos */
        if (!env.valid || !energy.valid)
        {
            vTaskDelay(pdMS_TO_TICKS(1000));
            continue;
        }

        /* Monta payload JSON */
        snprintf(payload, sizeof(payload),
            "{"
              "\"env\":{"
                "\"temperature\":%.2f,"
                "\"humidity\":%.2f,"
                "\"lux\":%.1f"
              "},"
              "\"energy\":{"
                "\"voltage\":%.1f,"
                "\"current\":%.3f,"
                "\"power\":%.1f,"
                "\"energy\":%.3f,"
                "\"frequency\":%.1f,"
                "\"pf\":%.2f"
              "}"
            "}",
            env.temperature,
            env.humidity,
            env.lux,
            energy.voltage,
            energy.current,
            energy.power,
            energy.energy,
            energy.frequency,
            energy.pf
        );

        /* Publica via MQTT */
        mqtt_publish_async(MQTT_TOPIC_STATUS, payload);

        printf("[MQTT PAYLOAD] %s\n", payload);

        vTaskDelay(pdMS_TO_TICKS(MQTT_PAYLOAD_PERIOD_MS));
    }
}
