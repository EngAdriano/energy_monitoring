#include "system_state.h"
#include <string.h>

/* Estado interno */
static system_state_t g_state;
static SemaphoreHandle_t g_mutex;

/* =====================
 * INICIALIZAÇÃO
 * ===================== */
void system_state_init(void)
{
    memset(&g_state, 0, sizeof(g_state));
    g_mutex = xSemaphoreCreateMutex();
}

/* =====================
 * FUNÇÕES INTERNAS
 * ===================== */
static void lock(void)
{
    xSemaphoreTake(g_mutex, portMAX_DELAY);
}

static void unlock(void)
{
    xSemaphoreGive(g_mutex);
}

/* =====================
 * SETTERS
 * ===================== */
void system_state_set_env(const env_data_t *env)
{
    lock();
    g_state.env = *env;
    unlock();
}

void system_state_set_energy(const energy_data_t *energy)
{
    lock();
    g_state.energy = *energy;
    unlock();
}

void system_state_set_wifi(const wifi_state_t *wifi)
{
    lock();
    g_state.wifi = *wifi;
    unlock();
}

void system_state_set_mqtt(const mqtt_state_t *mqtt)
{
    lock();
    g_state.mqtt = *mqtt;
    unlock();
}

void system_state_set_time(const app_datetime_t *time)
{
    lock();
    g_state.time = *time;
    unlock();
}

/* =====================
 * GETTERS
 * ===================== */
void system_state_get_env(env_data_t *env)
{
    lock();
    *env = g_state.env;
    unlock();
}

void system_state_get_energy(energy_data_t *energy)
{
    lock();
    *energy = g_state.energy;
    unlock();
}

void system_state_get_wifi(wifi_state_t *wifi)
{
    lock();
    *wifi = g_state.wifi;
    unlock();
}

void system_state_get_mqtt(mqtt_state_t *mqtt)
{
    lock();
    *mqtt = g_state.mqtt;
    unlock();
}

void system_state_get_time(app_datetime_t *time)
{
    lock();
    *time = g_state.time;
    unlock();
}