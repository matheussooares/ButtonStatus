#include "wifiConnection.h"  // Cabeçalho com definições das funções de Wi-Fi e inclusão da biblioteca cyw43

// Função que inicializa o chip Wi-Fi da Raspberry Pi Pico W
bool wifi_init(void) {
    // Inicializa o chip Wi-Fi, retorna 0 se sucesso, !=0 se falha
    if (cyw43_arch_init()) {
        printf("Falha ao inicializar Wi-Fi\n");  // Mensagem de erro
        return false;                            // Retorna falso indicando falha na inicialização
    } else {
        printf("Wi-Fi inicializado com sucesso\n");  // Mensagem de sucesso
        return true;                                  // Retorna verdadeiro indicando sucesso
    }
}

// Função para conectar a uma rede Wi-Fi utilizando SSID e senha
bool wifi_connect(const char* ssid, const char* password) {
    if (!wifi_init()) {  // Inicializa o Wi-Fi antes de conectar
        return false;    // Se falhar na inicialização, encerra a função retornando falso
    }

    // Ativa o modo estação (STA), que permite conectar-se a redes Wi-Fi
    cyw43_arch_enable_sta_mode();

    printf("Conectando ao Wi-Fi...\n");

    // Tenta conectar ao ponto de acesso Wi-Fi com timeout de 30 segundos
    // CYW43_AUTH_WPA2_AES_PSK indica autenticação WPA2 com AES (padrão atual)
    if (cyw43_arch_wifi_connect_timeout_ms(ssid, password, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("Falha ao conectar ao Wi-Fi\n");  // Caso falhe na conexão
        return false;
    } else {
        printf("Conectado ao Wi-Fi\n");
        // Obtém o endereço IP atribuído em formato legível
        uint8_t *ip_address = (uint8_t*)&(cyw43_state.netif[0].ip_addr.addr);
        printf("IP address %d.%d.%d.%d\n", ip_address[0], ip_address[1], ip_address[2], ip_address[3]);
        return true;  // Indica conexão bem sucedida
    }
}
