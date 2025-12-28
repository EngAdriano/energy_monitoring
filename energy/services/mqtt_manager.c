#include <stdio.h>
#include <string.h>

/* lwIP */
#include "lwip/apps/mqtt.h"
#include "lwip/dns.h"
#include "lwip/ip_addr.h"
#include "lwip/err.h"

/* FreeRTOS */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Projeto */
#include "system_state.h"
#include "mqtt_manager.h"

/* =========================================================
 * Configuração MQTT
 * ========================================================= */
#define MQTT_BROKER        "broker.hivemq.com"
#define MQTT_BROKER_PORT   1883
#define MQTT_CLIENT_ID     "pico_freertos_client"
#define MQTT_KEEPALIVE     60
#define MQTT_DNS_RETRY_MS  5000

/* =========================================================
 * Estado interno do módulo
 * ========================================================= */
static mqtt_client_t *mqtt_client = NULL;
static ip_addr_t broker_ip;
static QueueHandle_t mqttQueue = NULL;
static TickType_t last_dns_try = 0;

static volatile bool mqtt_connected     = false;
static volatile bool mqtt_publish_busy  = false;

/* =========================================================
 * Prototipação interna
 * ========================================================= */
static void mqtt_dns_callback(const char *name,
                              const ip_addr_t *ipaddr,
                              void *callback_arg);

static void mqtt_connection_cb(mqtt_client_t *client,
                               void *arg,
                               mqtt_connection_status_t status);

static void mqtt_pub_request_cb(void *arg, err_t err);

/* =========================================================
 * Inicialização do módulo MQTT
 * ========================================================= */
void mqtt_manager_init(void)
{
    mqtt_client = mqtt_client_new();
    if (!mqtt_client) {
        printf("[MQTT] ERRO: mqtt_client_new() retornou NULL\n");
        return;
    }

    mqttQueue = xQueueCreate(4, sizeof(mqtt_message_t));
    if (!mqttQueue) {
        printf("[MQTT] ERRO: Falha ao criar fila MQTT\n");
        return;
    }

    mqtt_connected    = false;
    mqtt_publish_busy = false;
    last_dns_try      = 0;

    printf("[MQTT] Manager inicializado\n");
}

/* =========================================================
 * API pública: status da conexão
 * ========================================================= */
bool mqtt_is_connected(void)
{
    return mqtt_connected;
}

/* =========================================================
 * Publicação assíncrona (thread-safe)
 * ========================================================= */
void mqtt_publish_async(const char *topic, const char *payload)
{
    if (!mqttQueue || !topic || !payload)
        return;

    mqtt_message_t msg = {0};

    strncpy(msg.topic, topic, sizeof(msg.topic) - 1);
    strncpy(msg.payload, payload, sizeof(msg.payload) - 1);

    xQueueSend(mqttQueue, &msg, 0);
}

/* =========================================================
 * Callback: publicação concluída
 * ========================================================= */
static void mqtt_pub_request_cb(void *arg, err_t err)
{
    (void)arg;
    (void)err;

    mqtt_publish_busy = false;
}

/* =========================================================
 * Callback: conexão MQTT
 * ========================================================= */
static void mqtt_connection_cb(mqtt_client_t *client,
                               void *arg,
                               mqtt_connection_status_t status)
{
    (void)client;
    (void)arg;

    mqtt_state_t mqtt = {0};

    if (status == MQTT_CONNECT_ACCEPTED) {
        mqtt_connected = true;
        mqtt.connected = true;
        printf("[MQTT] Conectado ao broker\n");
    } else {
        mqtt_connected = false;
        mqtt.connected = false;
        printf("[MQTT] Falha na conexão MQTT (%d)\n", status);
    }

    system_state_set_mqtt(&mqtt);
}

/* =========================================================
 * Callback: DNS resolvido
 * ========================================================= */
static void mqtt_dns_callback(const char *name,
                              const ip_addr_t *ipaddr,
                              void *callback_arg)
{
    (void)name;
    (void)callback_arg;

    if (!ipaddr || !mqtt_client) {
        printf("[MQTT] DNS falhou ou client NULL\n");
        return;
    }

    broker_ip = *ipaddr;

    struct mqtt_connect_client_info_t ci = {
        .client_id  = MQTT_CLIENT_ID,
        .keep_alive = MQTT_KEEPALIVE
    };

    printf("[MQTT] Conectando ao broker...\n");

    mqtt_client_connect(
        mqtt_client,
        &broker_ip,
        MQTT_BROKER_PORT,
        mqtt_connection_cb,
        NULL,
        &ci
    );
}

/* =========================================================
 * Task: gerenciamento da conexão MQTT
 * ========================================================= */
void vTaskMQTTConnection(void *pv)
{
    (void) pv;

    wifi_state_t wifi;

    printf("[MQTT] Task MQTTConnection iniciou\n");

    for (;;)
    {
        system_state_get_wifi(&wifi);

        if (!wifi.connected || !mqtt_client)
        {
            mqtt_connected = false;
            vTaskDelay(pdMS_TO_TICKS(2000));
            continue;
        }

        if (!mqtt_connected &&
            (xTaskGetTickCount() - last_dns_try) > pdMS_TO_TICKS(MQTT_DNS_RETRY_MS))
        {
            last_dns_try = xTaskGetTickCount();

            err_t err = dns_gethostbyname(
                MQTT_BROKER,
                &broker_ip,
                mqtt_dns_callback,
                NULL
            );

            if (err == ERR_OK)
            {
                mqtt_dns_callback(MQTT_BROKER, &broker_ip, NULL);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

/* =========================================================
 * Task: publicação MQTT
 * ========================================================= */
void vTaskMQTTPublisher(void *pv)
{
    (void) pv;

    mqtt_message_t msg;
    err_t err;

    for (;;)
    {
        if (!mqtt_connected)
        {
            vTaskDelay(pdMS_TO_TICKS(500));
            continue;
        }

        if (xQueueReceive(mqttQueue, &msg, portMAX_DELAY))
        {
            if (mqtt_publish_busy)
            {
                vTaskDelay(pdMS_TO_TICKS(100));
                continue;
            }

            mqtt_publish_busy = true;

            err = mqtt_publish(
                mqtt_client,
                msg.topic,
                msg.payload,
                strlen(msg.payload),
                0,
                0,
                mqtt_pub_request_cb,
                NULL
            );

            if (err != ERR_OK)
            {
                mqtt_publish_busy = false;
            }

            vTaskDelay(pdMS_TO_TICKS(300));
        }
    }
}
