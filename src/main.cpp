#include <Arduino.h>
#include <WiFi.h>
#include <SSLClient.h>
#include <PubSubClient.h>
#include "AWS_Root_CA.h" // This file is created using AmazonRootCA1.pem from https://www.amazontrust.com/repository/AmazonRootCA1.pem

const char* ssid     = "TRITONASETUP";
const char* password = "20120501!!";


#define THING_NAME "FStest"
#define MQTT_PACKET_SIZE  1024

void MQTTPublish(const char *topic, char *payload);
void updateThing();

const char my_cert[] = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDWTCCAkGgAwIBAgIUXAP/TBNa1kSzw08qGYsaoK9RMAMwDQYJKoZIhvcNAQEL\n" \
"BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g\n" \
"SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIxMDYwMzA3NDU1\n" \
"M1oXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0\n" \
"ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBANLGLpCB/fx5SFzP2QHE\n" \
"/DnTK01D35Ah1ASo8/FB8S7mZ/tDusGfvPXED8T4fbJyzXfDJXyy7icoNZb2ukb3\n" \
"WbWJx9pPcFZ7jJR+pyLbx6Cd1KIwsDBDQb3/LfbfuO18EbqsID7GVKorVfYXZ2Mq\n" \
"VxEXUWTKXNIMkYjxbxpRk9LM4Zs2GBdhskHNLM7443gLaTf1g2shOCiZXHb0+12q\n" \
"lNKHCdM1igrjdwJeaYiH14XnXXHuCV34SQ1p99tCMnI4cOPSloJLXWi/C0UmqseZ\n" \
"loE91AQMyxDWgGfsrdRqlFruNlqFVwa/TeBFP1bXu3kCsSluvoqNvaYihtmCLT7i\n" \
"cHkCAwEAAaNgMF4wHwYDVR0jBBgwFoAUKMdWH/rmAamMpDR2uebDugRXVrgwHQYD\n" \
"VR0OBBYEFOdPBrIGka50qnVFiLa4yUQiZ63QMAwGA1UdEwEB/wQCMAAwDgYDVR0P\n" \
"AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQCS3N6WfIymCSkAkBnLKSlbTHUV\n" \
"UuqBc7kDrNBkhwSPA6rlxSikynR6luOnO7SWN1BKaDxN+FooVLkufqp5ywff284C\n" \
"K6mtUtvKA1k1OmAoePA5UHKr+eT/eusqGoKwSguXmFefNpUS/H6RATBu1Kl0fxhG\n" \
"YCxRzRHPsTp1Cy0S+913zxk/mAKwTueJEGeiekB3zOLQdptAx+jn6/H9YotzW9Vx\n" \
"s4npKHcuHjW/LlLxTrDerVEcwqVgvEmgaLfX5sXeota7+uvy+8Crm5+LQfFrV6zx\n" \
"+hedivK7TvFqMtWvT5kQ3wYvWuHaumBlQ6ffUR33E85Tb0WasXhO9siEKgGD\n" \
"-----END CERTIFICATE-----\n";

const char my_key[] = \
"-----BEGIN RSA PRIVATE KEY-----\n" \
"MIIEpAIBAAKCAQEA0sYukIH9/HlIXM/ZAcT8OdMrTUPfkCHUBKjz8UHxLuZn+0O6\n" \
"wZ+89cQPxPh9snLNd8MlfLLuJyg1lva6RvdZtYnH2k9wVnuMlH6nItvHoJ3UojCw\n" \
"MENBvf8t9t+47XwRuqwgPsZUqitV9hdnYypXERdRZMpc0gyRiPFvGlGT0szhmzYY\n" \
"F2GyQc0szvjjeAtpN/WDayE4KJlcdvT7XaqU0ocJ0zWKCuN3Al5piIfXheddce4J\n" \
"XfhJDWn320Iycjhw49KWgktdaL8LRSaqx5mWgT3UBAzLENaAZ+yt1GqUWu42WoVX\n" \
"Br9N4EU/Vte7eQKxKW6+io29piKG2YItPuJweQIDAQABAoIBAE3/QKQBNrgDOZ/+\n" \
"2lMpTh0n49JH16dCnW7MBtm0iUfNw5Tbz3RvZEGeOCevvPQYP6id8nVe0Xp0CsC+\n" \
"tNgmGHBxeB91XiDiv3cEF11y70Ckp1cNpWV2vhFNKPk+ePSJARaPM6+HPTVrq/fR\n" \
"WVT9ChMtCdW+fPLN6909WtvmbEmJE4y+xHyuVDrSj+KeWrbDz7+4FIyHrAZ4/CwC\n" \
"oiyzS6a3fNdGHiC28noEgnud6v1gmS2jzMrWbSLVUe1nitvaxIDp8fT+ccRd5Kjw\n" \
"JiYsd20sNGkGfa40Fc5IqZpAf4LySg8yTreUBSAtvjh2AIgd1hMdtdTcpAicvwZL\n" \
"FGd9BTECgYEA9kjMCBRrc5+/fZvDRkrF3BO8sKYljLDw1X0VcCS+C0SbGQSISa5U\n" \
"rQWHNznEqmj1RSLmxWObvq3z3+5XVfUv5vJclQggnR1uNnyKBl5M8+kYoOKTaEa9\n" \
"Kth+YcUyVBAJJLM6LKC0IRX2oiZd/GFxULGc/wGaL4yZgrEFaLxmwO8CgYEA2xbF\n" \
"QH1hOpwSDAlie7liXQMJRutcm9J4UpEAyvHDHM7exYqfrtSAvLgnamD/p5xA21CJ\n" \
"wI0YhN7diFZgMnYiP8KcBMgmNECCmgVnqV6BOn8cW7yScB/rmzQpAoSUEXkGoDnY\n" \
"zf57sVPBhUSHgDL88FAW5mU/IBhCokCd7TpklRcCgYEAny7IKUTBerc69r3RhkJY\n" \
"Bnn/UJHi3T+UOfih/ZzR9MiAErh6Ol1iP36+ObSt55L1TJ1VWSmbhbA7W3ZanqVe\n" \
"M8BclLrvEX3vOWpfb+RNiq0TjRFFQ3XNWL8bWKgzcl/2+PCa3jN0mZnvkjiIMNLu\n" \
"gcB6/9uSQ791jSJLo7l6FKMCgYEApTalpv0lrfn1jgghsSVabMOWYlmoa4Uq4TCh\n" \
"xWjM9tN1oMfppQBw2F+SsaDWCYJGUlyIlKCSBdi+fEqOnl8Bcs2BtAVu8eZX1mgP\n" \
"BOTUVNV4ACHS2CTqGg8ageCSfB6lBXy55pVql42OmL3V5LIOlfX/Xtyo3r4Muag6\n" \
"r6NiKB8CgYACVzGVdaJZcacAeOe7px0g9iLI/PiUwpxlgtD8M6vzOcXGo2ZCyZqO\n" \
"16NPL1QtXsSUxwydNb+aIpkKv74ApXWDtM5+td4Y8sXq9ZglCBdv+p6rgrb9fmiP\n" \
"3rRq2J2ePSTzkSixE7tD5x8GKfdZFaWiYLcYIbMA2TwDct6Mwith2g==\n" \
"-----END RSA PRIVATE KEY-----\n";


SSLClientParameters mTLS = SSLClientParameters::fromPEM(my_cert, sizeof my_cert, my_key, sizeof my_key);

const char* mqttServer = "a2wzjwo14dtw6n-ats.iot.ap-northeast-2.amazonaws.com";
//const char* mqttServer = "arn:aws:iot:ap-northeast-2:901349641272:thing/TritonaPublic";
const char publishShadowUpdate[] = "$aws/things/"THING_NAME"/shadow/name/FSShadow/update";
const char publishShadowGet[] = "$aws/things/"THING_NAME"/shadow/name/FSShadow/get";
const char publishShadowGet1[] = "$aws/things/"THING_NAME"/shadow/name/FSShadow/get/accepted";
const char publishShadowGet2[] = "$aws/things/"THING_NAME"/shadow/name/FSShadow/get/rejected";
char publishPayload[MQTT_PACKET_SIZE];
char publishPayload1[MQTT_PACKET_SIZE];

char *subscribeTopic[6] =
{
  "$aws/things/"THING_NAME"/shadow/name/FSShadow/update/accepted",
  "$aws/things/"THING_NAME"/shadow/name/FSShadow/update/rejected",
  "$aws/things/"THING_NAME"/shadow/name/FSShadow/update/delta",
  "$aws/things/"THING_NAME"/shadow/name/FSShadow/update/documents",
  "$aws/things/"THING_NAME"/shadow/name/FSShadow/get/accepted",
  "$aws/things/"THING_NAME"/shadow/name/FSShadow/get/rejected"
};

void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) 
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}


//EthernetClient ethClient;
WiFiClient WiFiclient;
SSLClient ClientSSL(WiFiclient, TAs, (size_t)TAs_NUM, A5);
//SSLClient ClientSSL(WiFiclient, TAs, (size_t)TAs_NUM, A5, 1, SSLClient :: SSL_INFO);  //디버그 버전 변경
PubSubClient mqtt(mqttServer, 8883, callback, ClientSSL);

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02
};

void reconnect() 
{
  while (!mqtt.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    if (mqtt.connect("arduinoClient")) 
    {
      Serial.println("connected");
      for (int i = 0; i < 6; i++) 
      {
//        Serial.println(subscribeTopic[i]);
        mqtt.subscribe(subscribeTopic[i]);
      }
        Serial.println("Started updateThing ");
        updateThing();
        Serial.println("Done updateThing ");

    } 
    else 
    {
      Serial.print("failed, rc=");
      Serial.print(mqtt.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }

}

void setup()
{
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);


  ClientSSL.setMutualAuthParams(mTLS);

  mqtt.setBufferSize(MQTT_PACKET_SIZE);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

int value = 0;

void loop()
{
  delay(5000);
  value++;

  if (!mqtt.connected()) 
  {
    reconnect();
  }
  mqtt.loop();
  updateThing();
    Serial.print("testing.\n");
}

void updateThing()
{
  strcpy(publishPayload, "{\"state\": {\"reported\": {\"powerState\":\"ON\"}}}");
  MQTTPublish(publishShadowUpdate, publishPayload);

  //strcpy(publishPayload, "{\"state\": {\"reported\": {\"temp\": \"value\"}}}");
  sprintf(publishPayload, "%s%d%s", "{\"state\": {\"reported\": {\"temp\": \"", value, "\"}}}");
  MQTTPublish(publishShadowUpdate, publishPayload);

  strcpy(publishPayload1, "{\"state\": {\"reported\": {\"led\": \"off\"}}}");
  MQTTPublish(publishShadowUpdate, publishPayload1);

  MQTTPublish(publishShadowGet, nullptr);
  MQTTPublish(publishShadowGet1, nullptr);
  MQTTPublish(publishShadowGet2, nullptr);

  //MQTTPublish(publishShadowUpdate, "{  \"reported\": { \"temp\": \"50\", \"led\": \"on\", \"powerState\": \"ON\" } } ");
}

void MQTTPublish(const char *topic, char *payload)
{
  mqtt.publish(topic, payload);
  Serial.print("Published [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.println(payload);
}
