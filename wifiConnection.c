#include "wifiConnection.h"

bool wifi_init(void){

    // Initialise the Wi-Fi chip
    if (cyw43_arch_init()) {
        printf("Falha ao inicializar Wi-Fi\n");
        return false;
    }else {
        printf("Wi-Fi inicializado com sucesso\n");
        return true;
    }
}

bool wifi_connect(const char* ssid, const char* password) {
   
    if (!wifi_init()) {
        return false;
    }
    // Enable wifi station
    cyw43_arch_enable_sta_mode();

    printf("Conectando ao Wi-Fi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms(ssid, password, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("Falha ao conectar ao Wi-Fi\n");
        return false;
    } else {
        printf("Conectado ao Wi-Fi\n");
        // Read the ip address in a human readable way
        uint8_t *ip_address = (uint8_t*)&(cyw43_state.netif[0].ip_addr.addr);
        printf("IP address %d.%d.%d.%d\n", ip_address[0], ip_address[1], ip_address[2], ip_address[3]);
        return true;
    }
}
