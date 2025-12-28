#ifndef RTC_DS3231_H
#define RTC_DS3231_H

#include <stdbool.h>
#include "system_state.h"

bool rtc_ds3231_init(void);
bool rtc_ds3231_get_time(app_datetime_t *dt);
bool rtc_ds3231_set_time(const app_datetime_t *dt);

#endif
