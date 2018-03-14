#ifndef Defy_h
#define Defy_h
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <PubSubClient.h>
#include "Defy.h"
#include "Utils.h"

#define CONN_STAT_NO 0
#define CONN_STAT_WIFI_CONNECTING 1
#define CONN_STAT_WIFI_OK 2
#define CONN_STAT_WIFIMQTT_CONNECTING 3
#define CONN_STAT_WIFIMQTT_OK 4

ESP8266WiFiMulti wifiMulti;
WiFiClient espClient;
PubSubClient client(espClient);

const char* nodeMCUid="Reku";
const char* outTopic="Reku/OUT";
const char* inTopic="Reku/IN";
const char* debugTopic="DebugTopic/Reku";
const char* mqtt_server ="broker.hivemq.com"; //"m23.cloudmqtt.com";
const char* mqtt_user="";//"aigejtoh";
const char* mqtt_pass="";//"ZFlzjMm4T-XH";
const uint16_t mqtt_port=1883;


class CWifi
{
  
  unsigned long lastMQTTReconnectAttempt = 0;
  unsigned long lastWIFIReconnectAttempt = 0;
  unsigned long WDmillis=0;

  int conStat=CONN_STAT_NO;
	
  public:
  CWifi();
  void loop();
  void begin();
  bool setup_wifi();
  bool WiFiConnected();
  bool reconnectMQTT();
  void RSpisz(const char* topic,char* msg);
};

#endif
