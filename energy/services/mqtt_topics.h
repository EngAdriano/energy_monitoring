#ifndef MQTT_TOPICS_H
#define MQTT_TOPICS_H

/* =========================================================
 * Prefixo base do projeto
 * ========================================================= */
#define MQTT_TOPIC_BASE           "embarca"

/* =========================================================
 * Tópicos de status geral
 * ========================================================= */
#define MQTT_TOPIC_STATUS         MQTT_TOPIC_BASE "/energy/status"

/* =========================================================
 * (Futuros) Tópicos separados
 * ========================================================= */
/* Ambiente */
//#define MQTT_TOPIC_ENV            MQTT_TOPIC_BASE "/env"

/* Energia */
//#define MQTT_TOPIC_ENERGY         MQTT_TOPIC_BASE "/energy"

/* =========================================================
 * (Futuros) Comandos / controle
 * ========================================================= */
//#define MQTT_TOPIC_CMD            MQTT_TOPIC_BASE "/cmd"

#endif /* MQTT_TOPICS_H */
