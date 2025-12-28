#include "aht10.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"

#define AHT10_ADDR 0x38

static bool aht10_send_cmd(uint8_t cmd, uint8_t p1, uint8_t p2)
{
    uint8_t buf[3] = { cmd, p1, p2 };
    return i2c_write_blocking(i2c1, AHT10_ADDR, buf, 3, false) == 3;
}

bool aht10_init(void)
{
    sleep_ms(40);
    return aht10_send_cmd(0xE1, 0x08, 0x00);
}

bool aht10_read(float *temperature, float *humidity)
{
    uint8_t buf[6];

    if (!aht10_send_cmd(0xAC, 0x33, 0x00))
        return false;

    sleep_ms(80);

    if (i2c_read_blocking(i2c1, AHT10_ADDR, buf, 6, false) != 6)
        return false;

    uint32_t raw_h =
        ((uint32_t)(buf[1]) << 12) |
        ((uint32_t)(buf[2]) << 4) |
        (buf[3] >> 4);

    uint32_t raw_t =
        ((uint32_t)(buf[3] & 0x0F) << 16) |
        ((uint32_t)(buf[4]) << 8) |
        buf[5];

    *humidity = ((float)raw_h / 1048576.0f) * 100.0f;
    *temperature = ((float)raw_t / 1048576.0f) * 200.0f - 50.0f;

    return true;
}
