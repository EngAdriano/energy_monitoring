#include <stdio.h>
#include "pico/stdlib.h"

#include "FreeRTOS.h"
#include "task.h"

/* App */
#include "app_init.h"

int main(void)
{
    stdio_init_all();

    printf("Iniciando Monitorament de energia..\n");

    app_init();

    vTaskStartScheduler();

    while (true) {}
}
