#include "bh1750.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"

#define BH1750_ADDR 0x23

bool bh1750_init(void)
{
    uint8_t cmd = 0x10; /* Continuous H-Resolution */
    return i2c_write_blocking(i2c1, BH1750_ADDR, &cmd, 1, false) == 1;
}

bool bh1750_read(float *lux)
{
    uint8_t buf[2];

    if (i2c_read_blocking(i2c1, BH1750_ADDR, buf, 2, false) != 2)
        return false;

    uint16_t raw = (buf[0] << 8) | buf[1];
    *lux = raw / 1.2f;

    return true;
}
