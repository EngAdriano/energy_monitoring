#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Tipo de mensagem MQTT */
typedef struct {
    char topic[64];
    char payload[512];
} mqtt_message_t;

/* API pública */
void mqtt_manager_init(void);
void mqtt_publish_async(const char *topic, const char *payload);

void vTaskMQTTConnection(void *pv);
void vTaskMQTTPublisher(void *pv);

bool mqtt_is_connected(void);

#ifdef __cplusplus
}
#endif

#endif /* MQTT_MANAGER_H */
