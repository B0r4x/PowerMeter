#include "EmonLib.h"
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "fusim6";
const char* password = "fusiglobaltechno";
const char* mqtt_server = "139.59.219.194";
#define MQTTuser  "fusi" 
#define MQTTpasw  "fusitech2021"
const char* pubTopic = "powermeter/value";


long lastMsg = 0;
char msg[80];
int value = 0;
