#include "rtc_ds3231.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"

#define DS3231_ADDR 0x68

static uint8_t bcd2dec(uint8_t v)
{
    return (v >> 4) * 10 + (v & 0x0F);
}

static uint8_t dec2bcd(uint8_t v)
{
    return ((v / 10) << 4) | (v % 10);
}

bool rtc_ds3231_init(void)
{
    /* DS3231 não precisa de init especial */
    return true;
}

bool rtc_ds3231_get_time(app_datetime_t *dt)
{
    uint8_t reg = 0x00;
    uint8_t buf[7];

    if (i2c_write_blocking(i2c1, DS3231_ADDR, &reg, 1, true) != 1)
        return false;

    if (i2c_read_blocking(i2c1, DS3231_ADDR, buf, 7, false) != 7)
        return false;

    dt->sec   = bcd2dec(buf[0]);
    dt->min   = bcd2dec(buf[1]);
    dt->hour  = bcd2dec(buf[2] & 0x3F);
    dt->day   = bcd2dec(buf[4]);
    dt->month = bcd2dec(buf[5] & 0x1F);
    dt->year  = 2000 + bcd2dec(buf[6]);
    dt->valid = true;

    return true;
}

bool rtc_ds3231_set_time(const app_datetime_t *dt)
{
    uint8_t buf[8];

    buf[0] = 0x00;
    buf[1] = dec2bcd(dt->sec);
    buf[2] = dec2bcd(dt->min);
    buf[3] = dec2bcd(dt->hour);
    buf[4] = 0x01; /* day of week (opcional) */
    buf[5] = dec2bcd(dt->day);
    buf[6] = dec2bcd(dt->month);
    buf[7] = dec2bcd(dt->year - 2000);

    return i2c_write_blocking(i2c1, DS3231_ADDR, buf, 8, false) == 8;
}
