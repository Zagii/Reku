#include "Utils.h"
#include "Defy.h"
#include "CWifi.h"


void callback(char* topic, byte* payload, unsigned int length) 
{
  char* p = (char*)malloc(length);
  memcpy(p,payload,length);
  p[length]='\0';
  if(strstr(topic,"watchdog"))
  {
    DPRINT("Watchdog msg=");
    DPRINT(p);
    DPRINT(" teraz=");
   
    if(Utils::isNumber(p))
      WDmillis=strtoul (p, NULL, 0);
    DPRINTLN(WDmillis);
  }

  DPRINT("Debug: callback topic=");
  DPRINT(topic);
  DPRINT(" msg=");
  DPRINTLN(p);
  parsujIdodajDoKolejki(topic,p);
  free(p);
}

void CWifi::loop()
{
  if(!WiFiConnected())
  { 
    conStat=CONN_STAT_WIFI_CONNECTING;
    if (millis() - lastWIFIReconnectAttempt > 5000)
    {
      if(setup_wifi())
      {
        RSpisz(debugTopic,"WiFi=ok");
        conStat=CONN_STAT_WIFI_OK;
      }
      else
      {
        RSpisz(debugTopic,"WiFi=Err");
      
      }
      lastWIFIReconnectAttempt = millis();
    }
  }else
  {
    if (!client.connected()) 
    {
     conStat=CONN_STAT_WIFIMQTT_CONNECTING;
      if (millis() - lastMQTTReconnectAttempt > 5000)
      {
        lastMQTTReconnectAttempt = millis();
        if (reconnectMQTT())
        {
           RSpisz(debugTopic,"MQTT=ok");
           conStat=CONN_STAT_WIFIMQTT_OK;
          lastMQTTReconnectAttempt = 0;
         char t[100];
         sprintf(t,"Polaczono SSID=%s, IP=%d.%d.%d.%d",WiFi.SSID().c_str(),WiFi.localIP()[0],WiFi.localIP()[1],WiFi.localIP()[2],WiFi.localIP()[3]);
         RSpisz(debugTopic,t);
        }
        else
        {
             RSpisz(debugTopic,"MQTT=Err");
             DPRINT("Err MQTTstat= ");DPRINTLN(client.state());
             DPRINT("WIFI ip= ");DPRINTLN(WiFi.localIP());
        }
      }
    } else
    {
          client.loop();                
    }
  }
}

void CWifi::begin()
{
	
  wifiMulti.addAP("DOrangeFreeDom", "KZagaw01_ruter_key");
  wifiMulti.addAP("open.t-mobile.pl", "");
  wifiMulti.addAP("instalujWirusa", "blablabla123");
  
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}
bool CWifi::setup_wifi() 
{ 
  RSpisz(debugTopic,"Restart WiFi ");
  WiFi.mode(WIFI_STA);
  if(wifiMulti.run() == WL_CONNECTED)
  {
    IPAddress ip=WiFi.localIP();
    char b[100];
    sprintf(b,"WiFi connected: %s ,%d.%d.%d.%d\n", WiFi.SSID().c_str(), ip[0],ip[1],ip[2],ip[3]);
    RSpisz(debugTopic,b);
    
    return true;
  }else
  {
    RSpisz(debugTopic,"Wifi Connection Error."); 
    return false;
  }
}


//WiFiConnected
//true - connected
//false -not connected
bool CWifi::WiFiConnected() 
{
  return (WiFi.status() == WL_CONNECTED);
}


bool CWifi::reconnectMQTT()
{
  if (client.connect(nodeMCUid,mqtt_user,mqtt_pass)) 
  {
    char s[MAX_TOPIC_LENGHT];
    strcpy(s,inTopic);
    strcat(s,"\/#");  
    client.subscribe(s);
    const char *t="reconnectMQTT, subskrybcja do: ";
    char b[MAX_TOPIC_LENGHT+strlen(t)];
    sprintf(b,"%s%s",t,s);
    RSpisz(debugTopic,b);
   
  }
  return client.connected();
}


void CWifi::RSpisz(const char* topic,char* msg)
{
   DPRINT("Debug RSpisz, topic=");  DPRINT(topic); DPRINT(", msg=");  DPRINT(msg);DPRINT(", wynik=");
   DPRINTLN(client.publish(topic,msg));
}
