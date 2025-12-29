#ifndef WEBSERVER_TASK_H
#define WEBSERVER_TASK_H

#include <stdbool.h>

/* Cria a task do WebServer (chamar após Wi-Fi conectado) */
void webserver_start(void);

/* Task principal */
void vTaskWebServer(void *pv);

#endif /* WEBSERVER_TASK_H */
