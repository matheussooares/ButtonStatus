#ifndef BUTTON_H
#define BUTTON_H
    #define BUTTON1_PIN 5      // Pino do botão 1
    #define BUTTON2_PIN 6      // Pino do botão 2
    
    #include "pico/stdlib.h"
    #include <string.h>
    #include <stdio.h>

    bool button_init(void);
    bool button_monitor(char *button1_message, char *button2_message);
#endif