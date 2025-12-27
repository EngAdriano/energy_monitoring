#ifndef SYSTEM_STATE_H
#define SYSTEM_STATE_H

#include <stdbool.h>
#include "FreeRTOS.h"
#include "semphr.h"

/* =====================
 * TIPO DE DATA/HORA DA APLICAÇÃO
 * ===================== */
typedef struct {
    int year;
    int month;
    int day;
    int hour;
    int min;
    int sec;
    bool valid;
} app_datetime_t;

/* =====================
 * DADOS AMBIENTAIS
 * ===================== */
typedef struct {
    float temperature;
    float humidity;
    float lux;
    bool  valid;
} env_data_t;

/* =====================
 * ENERGIA
 * ===================== */
typedef struct {
    float voltage;
    float current;
    float power;
    float energy;
    float frequency;
    float pf;
    bool  valid;
} energy_data_t;

/* =====================
 * STATUS
 * ===================== */
typedef struct {
    bool connected;
    int  rssi;
} wifi_state_t;

typedef struct {
    bool connected;
} mqtt_state_t;

/* =====================
 * ESTADO GLOBAL
 * ===================== */
typedef struct {
    env_data_t        env;
    energy_data_t     energy;
    wifi_state_t      wifi;
    mqtt_state_t      mqtt;
    app_datetime_t    time;
} system_state_t;

/* =====================
 * API
 * ===================== */
void system_state_init(void);

void system_state_set_env(const env_data_t *env);
void system_state_set_energy(const energy_data_t *energy);
void system_state_set_wifi(const wifi_state_t *wifi);
void system_state_set_mqtt(const mqtt_state_t *mqtt);
void system_state_set_time(const app_datetime_t *time);

void system_state_get_env(env_data_t *env);
void system_state_get_energy(energy_data_t *energy);
void system_state_get_wifi(wifi_state_t *wifi);
void system_state_get_mqtt(mqtt_state_t *mqtt);
void system_state_get_time(app_datetime_t *time);

#endif /* SYSTEM_STATE_H */
