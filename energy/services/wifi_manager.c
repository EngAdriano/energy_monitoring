#include <stdio.h>
#include <string.h>

#include "pico/cyw43_arch.h"
#include "lwip/netif.h"
#include "lwip/ip4_addr.h"

/* FreeRTOS */
#include "FreeRTOS.h"
#include "task.h"

/* Core */
#include "system_state.h"

/* Services */
#include "wifi_manager.h"
#include "services/webserver/webserver_task.h"

/* =====================================================
 * Configurações de rede
 * ===================================================== */
#define WIFI_SSID       "Lu e Deza"
#define WIFI_PASSWORD   "liukin1208"

#define WIFI_TASK_DELAY_MS   5000

/* =====================================================
 * Task Wi-Fi
 * ===================================================== */
static void wifi_task(void *pv)
{
    (void) pv;

    wifi_state_t wifi = {0};
    static bool web_started = false;

    for (;;)
    {
        /* Aguarda CYW43 pronto */
        if (!cyw43_is_initialized(&cyw43_state))
        {
            vTaskDelay(pdMS_TO_TICKS(1000));
            continue;
        }

        /* Estado do link */
        int status = cyw43_tcpip_link_status(
            &cyw43_state,
            CYW43_ITF_STA
        );

        wifi.connected = (status == CYW43_LINK_UP);
        wifi.rssi = cyw43_wifi_get_rssi(
            &cyw43_state,
            CYW43_ITF_STA
        );

        /* IP */
        if (wifi.connected)
        {
            struct netif *netif = netif_default;

            if (netif && netif_is_up(netif))
            {
                ip4addr_ntoa_r(
                    netif_ip4_addr(netif),
                    wifi.ip,
                    sizeof(wifi.ip)
                );
            }
            else
            {
                strcpy(wifi.ip, "0.0.0.0");
            }
        }
        else
        {
            strcpy(wifi.ip, "---.---.---.---");
        }

        /* Atualiza estado global */
        system_state_set_wifi(&wifi);

        /* Inicia WebServer uma única vez após conexão */
        if (wifi.connected && !web_started)
        {
            //printf("[WEB] Iniciando WebServer...\n");
            webserver_start();
            web_started = true;
        }

        /*/
        printf("[WIFI] %s | RSSI=%d | IP=%s\n",
               wifi.connected ? "CONNECTED" : "DISCONNECTED",
               wifi.rssi,
               wifi.ip);*/

        vTaskDelay(pdMS_TO_TICKS(WIFI_TASK_DELAY_MS));
    }
}

/* =====================================================
 * Inicialização do Wi-Fi
 * ===================================================== */
void wifi_manager_init(void)
{
    if (cyw43_arch_init())
    {
        printf("[WIFI] Falha ao inicializar CYW43\n");
        return;
    }

    cyw43_arch_enable_sta_mode();

    //printf("[WIFI] Conectando em %s...\n", WIFI_SSID);

    int rc = cyw43_arch_wifi_connect_timeout_ms(
        WIFI_SSID,
        WIFI_PASSWORD,
        CYW43_AUTH_WPA2_AES_PSK,
        30000
    );

    if (rc)
    {
        printf("[WIFI] Falha ao conectar (%d)\n", rc);
    }
    else
    {
        //printf("[WIFI] Conectado com sucesso\n");
    }

    xTaskCreate(
        wifi_task,
        "wifi",
        1024,
        NULL,
        2,
        NULL
    );
}
