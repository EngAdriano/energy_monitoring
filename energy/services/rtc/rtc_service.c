#include "rtc_service.h"
#include <stdio.h>

/* Driver real do seu projeto */
#include "rtc_ds3231.h"

/* =====================================================
 * Inicialização
 * ===================================================== */
void rtc_service_init(void)
{
    rtc_ds3231_init();
}

/* =====================================================
 * Atualiza data/hora (Web → RTC → system_state)
 * ===================================================== */
bool rtc_service_set_datetime(const app_datetime_t *dt)
{
    if (!dt || !dt->valid)
        return false;

    if (!rtc_ds3231_set_time(dt))
        return false;

    system_state_set_time(dt);
    return true;
}

/* =====================================================
 * Sincroniza RTC → system_state (boot / periodic)
 * ===================================================== */
bool rtc_service_sync_from_rtc(void)
{
    app_datetime_t dt;

    if (!rtc_ds3231_get_time(&dt))
        return false;

    system_state_set_time(&dt);
    return true;
}
