#ifndef __WIFI_H__
#define __WIFI_H__

#include "stdint.h"

void initialize_wifi_stack();


typedef struct {
    uint8_t ssid[33];
} wifi_scan_result;

// Scans for wifi networks, stores them in scan_results wifi_scan_results and
// returns the number of networks found.
uint16_t perform_wifi_network_scan();
wifi_scan_result* get_wifi_scan_results();

void connect_to_wifi_network(wifi_scan_result* network, char* password);

#endif  // __WIFI_H__
