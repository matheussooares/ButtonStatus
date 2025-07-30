#include "button.h"  // Inclusão do cabeçalho com definições dos pinos e funções relacionadas aos botões

// Função que inicializa os pinos dos botões como entradas com resistor pull-up interno
bool button_init(void) {
    gpio_init(BUTTON1_PIN);              // Inicializa o pino do botão 1
    gpio_set_dir(BUTTON1_PIN, GPIO_IN);  // Define como entrada
    gpio_pull_up(BUTTON1_PIN);           // Habilita resistor pull-up interno

    gpio_init(BUTTON2_PIN);              // Inicializa o pino do botão 2
    gpio_set_dir(BUTTON2_PIN, GPIO_IN);  // Define como entrada
    gpio_pull_up(BUTTON2_PIN);           // Habilita resistor pull-up interno

    // Mensagem de confirmação no terminal
    printf("Botões configurados com pull-up nos pinos %d e %d\n", BUTTON1_PIN, BUTTON2_PIN);
    return true;  // Indica sucesso na inicialização
}

// Função que monitora alterações nos estados dos botões e atualiza as mensagens fornecidas
bool button_monitor(char *button1_message, char *button2_message) {
    // Variáveis estáticas para armazenar o último estado conhecido de cada botão
    static bool button1_last_state = false;
    static bool button2_last_state = false;

    // Lê o estado atual dos pinos (inversão lógica porque pressionado = nível lógico 0)
    bool button1_state = !gpio_get(BUTTON1_PIN);
    bool button2_state = !gpio_get(BUTTON2_PIN);

    // Verifica mudança de estado do botão 1
    if (button1_state != button1_last_state) {
        button1_last_state = button1_state;  // Atualiza o estado
        if (button1_state) {
            snprintf(button1_message, 50, "Pressionado!");  // Atualiza a mensagem
            printf("Botão 1 %s\n", button1_message);
        } else {
            snprintf(button1_message, 50, "Solto!");
            printf("Botão 1 %s\n", button1_message);
        }
    }

    // Verifica mudança de estado do botão 2
    if (button2_state != button2_last_state) {
        button2_last_state = button2_state;
        if (button2_state) {
            snprintf(button2_message, 50, "Pressionado!");
            printf("Botão 2 %s\n", button2_message);
        } else {
            snprintf(button2_message, 50, "Solto!");
            printf("Botão 2 %s\n", button2_message);
        }
    }
}
