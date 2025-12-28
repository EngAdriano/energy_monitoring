#ifndef AHT10_H
#define AHT10_H

#include <stdbool.h>
#include <stdint.h>

bool aht10_init(void);
bool aht10_read(float *temperature, float *humidity);

#endif
