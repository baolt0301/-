#ifndef __ESP_AT_H
#define __ESP_AT_H


#include <stdint.h>
#include <stdbool.h>


bool esp_at_init(void);
bool esp_at_reset(void);
bool esp_at_send_command(const char *cmd, const char **rsp, uint32_t timeout);

bool esp_at_wifi_init(void);
bool esp_at_wifi_connect(const char* ssid, const char* pwd);

bool esp_at_sntp_init(void);

bool esp_at_get_http(const char* url, const char** rsp, uint32_t *length, uint32_t timeout);
bool esp_at_get_time(uint32_t *timestamp);
bool esp_at_get_wifi_ip(char ip[16]);



#endif 
