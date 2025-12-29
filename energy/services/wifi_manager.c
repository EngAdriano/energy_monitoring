#include <stdio.h>
#include "pico/cyw43_arch.h"
#include "lwip/netif.h"
#include "lwip/ip4_addr.h"
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

#include "core/system_state.h"
#include "services/wifi_manager.h"

/* Ajuste conforme sua rede */
#define WIFI_SSID     "Lu e Deza"
#define WIFI_PASSWORD "liukin1208"

static void wifi_task(void *pv)
{
    (void) pv;

    wifi_state_t wifi = {0};

    while (1) {

        if (!cyw43_is_initialized(&cyw43_state)) {
            vTaskDelay(pdMS_TO_TICKS(1000));
            continue;
        }

        int status = cyw43_tcpip_link_status(&cyw43_state, CYW43_ITF_STA);

        wifi.connected = (status == CYW43_LINK_UP);
        wifi.rssi = cyw43_wifi_get_rssi(&cyw43_state, CYW43_ITF_STA);

        if (wifi.connected) {
            struct netif *netif = netif_default;

            if (netif && netif_is_up(netif)) {
                ip4addr_ntoa_r(
                    netif_ip4_addr(netif),
                    wifi.ip,
                    sizeof(wifi.ip)
                );
            } else {
                strcpy(wifi.ip, "0.0.0.0");
            }
        } else {
            strcpy(wifi.ip, "---.---.---.---");
        }

        system_state_set_wifi(&wifi);

        printf("[WIFI] %s RSSI=%d IP=%s\n",
               wifi.connected ? "CONNECTED" : "DISCONNECTED",
               wifi.rssi,
               wifi.ip);

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}


void wifi_manager_init(void)
{
    if (cyw43_arch_init()) {
        printf("WiFi init failed\n");
        return;
    }

    cyw43_arch_enable_sta_mode();

    printf("Connecting WiFi...\n");

    cyw43_arch_wifi_connect_timeout_ms(
        WIFI_SSID,
        WIFI_PASSWORD,
        CYW43_AUTH_WPA2_AES_PSK,
        30000
    );

    xTaskCreate(
        wifi_task,
        "wifi",
        1024,
        NULL,
        2,
        NULL
    );
}
