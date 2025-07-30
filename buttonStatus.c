#include <stdio.h>
#include "pico/stdlib.h"           // Biblioteca padrão da Raspberry Pi Pico
#include "wifiConnection.h"        // Biblioteca personalizada para conexão Wi-Fi (implementação externa)
#include "button.h"                // Biblioteca personalizada para leitura dos botões (implementação externa)

#include "lwip/tcp.h"              // Biblioteca da pilha TCP/IP (Lightweight IP)

// Definições das credenciais da rede Wi-Fi
#define SSID ""
#define PASSWORD ""

// Variáveis globais que armazenam mensagens sobre o estado dos botões
char button1_message[50] = "Nenhum evento no botão 1";
char button2_message[50] = "Nenhum evento no botão 2";

// Buffer que armazenará a resposta HTTP enviada ao cliente
char http_response[2048];

// Função que monta dinamicamente a resposta HTML com base no estado dos botões
void create_http_response() {
    snprintf(http_response, sizeof(http_response),
        "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n"
        "<!DOCTYPE html>"
        "<html lang=\"pt-br\">"
        "<head>"
        "  <meta charset=\"UTF-8\">"
        "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
        "  <title>Controle dos Botões</title>"
        "  <style>"
        "    body { font-family: Arial, sans-serif; background-color: #f0f0f0; padding: 20px; }"
        "    .container { background: white; padding: 20px; border-radius: 8px; max-width: 400px; margin: auto; box-shadow: 0 0 10px rgba(0,0,0,0.1); }"
        "    h1 { color: #333; }"
        "    p, h2 { color: #555; }"
        "    a.button { display: inline-block; padding: 10px 15px; background-color: #007BFF; color: white; text-decoration: none; border-radius: 5px; }"
        "    a.button:hover { background-color: #0056b3; }"
        "  </style>"
        "</head>"
        "<body>"
        "  <div class=\"container\">"
        "    <h1>Controle dos Botões</h1>"
        "    <p><a href=\"/update\" class=\"button\">Atualizar Estado</a></p>"
        "    <h2>Estado dos Botões:</h2>"
        "    <p>Botão 1: %s</p>"
        "    <p>Botão 2: %s</p>"
        "  </div>"
        "</body>"
        "</html>\r\n",
        button1_message, button2_message);
}

// Callback chamado ao receber uma requisição HTTP
static err_t http_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (p == NULL) {
        // Se o pacote estiver nulo, o cliente encerrou a conexão
        tcp_close(tpcb);
        return ERR_OK;
    }

    // Interpreta a requisição recebida (neste caso, não há parsing avançado)
    char *request = (char *)p->payload;

    // Gera a resposta HTTP com os estados atualizados dos botões
    create_http_response();

    // Envia a resposta HTTP para o cliente
    tcp_write(tpcb, http_response, strlen(http_response), TCP_WRITE_FLAG_COPY);

    // Libera o buffer de entrada
    pbuf_free(p);

    return ERR_OK;
}

// Callback que é executado quando uma nova conexão é aceita
static err_t connection_callback(void *arg, struct tcp_pcb *newpcb, err_t err) {
    // Define o callback para tratar as requisições recebidas
    tcp_recv(newpcb, http_callback);
    return ERR_OK;
}

// Inicializa o servidor HTTP na porta 80
static void start_http_server(void) {
    struct tcp_pcb *pcb = tcp_new();  // Cria um novo controle de protocolo TCP
    if (!pcb) {
        printf("Erro ao criar PCB\n");
        return;
    }

    // Associa o PCB à porta 80 (HTTP padrão)
    if (tcp_bind(pcb, IP_ADDR_ANY, 80) != ERR_OK) {
        printf("Erro ao ligar o servidor na porta 80\n");
        return;
    }

    pcb = tcp_listen(pcb);  // Configura o PCB para escutar conexões TCP
    tcp_accept(pcb, connection_callback);  // Define o callback de aceitação

    printf("Servidor HTTP rodando na porta 80...\n");
}

// Função principal
int main() {
    stdio_init_all();         // Inicializa entrada e saída padrão (ex: printf via USB)
    sleep_ms(5000);           // Espera inicial para garantir estabilidade do sistema

    // Tenta conectar à rede Wi-Fi
    if (!wifi_connect(SSID, PASSWORD)) {
        printf("Falha ao conectar ao Wi-Fi\n");
        return -1;
    }

    button_init();            // Inicializa os pinos dos botões

    start_http_server();      // Inicia o servidor HTTP

    while (true) {
        cyw43_arch_poll();                         // Mantém a pilha Wi-Fi ativa
        button_monitor(button1_message, button2_message);  // Atualiza o estado dos botões
        sleep_ms(1000);                            // Aguarda 1 segundo entre leituras
    }

    cyw43_arch_deinit();      // Finaliza o uso da pilha Wi-Fi (não alcançado nesse loop infinito)
    return 0;
}
