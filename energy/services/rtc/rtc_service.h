#ifndef RTC_SERVICE_H
#define RTC_SERVICE_H

#include <stdbool.h>
#include "system_state.h"

void rtc_service_init(void);

/* Atualiza RTC físico + system_state */
bool rtc_service_set_datetime(const app_datetime_t *dt);

/* Lê RTC físico e atualiza system_state */
bool rtc_service_sync_from_rtc(void);

#endif
