#ifndef WIFICONNECTION_H
#define WIFICONNECTION_H
    #include "pico/cyw43_arch.h"
    bool wifi_init(void);
    bool wifi_connect(const char* ssid, const char* password);
#endif