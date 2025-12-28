#include "FreeRTOS.h"
#include "task.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"

#include "system_state.h"
#include "project_config.h"
#include "aht10.h"
#include "bh1750.h"

void task_env_sensors(void *pv)
{
    (void) pv;

    env_data_t env = {0};

    i2c_init(I2C_ENV_PORT, I2C_ENV_BAUDRATE);
    gpio_set_function(I2C_ENV_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_ENV_SCL_PIN, GPIO_FUNC_I2C);

    gpio_pull_up(I2C_ENV_SDA_PIN);
    gpio_pull_up(I2C_ENV_SCL_PIN);
    aht10_init();
    bh1750_init();

    for (;;)
    {
        bool ok1 = aht10_read(&env.temperature, &env.humidity);
        bool ok2 = bh1750_read(&env.lux);

        env.valid = ok1 && ok2;

        system_state_set_env(&env);

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void task_env_sensors_start(void)
{
    xTaskCreate(
        task_env_sensors,
        "env",
        1024,
        NULL,
        2,
        NULL
    );
}
