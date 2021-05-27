/*
 WiFiEsp test: BasicTest
 
 Performs basic connectivity test and checks.
*/

#include "WiFiEsp.h"
#include "WifiEspMqtt.h"
#include "SoftwareSerial.h"
SoftwareSerial esp8266_serial(5, 6); // RX, TX
uint32_t _startMillis = 0;

char ssid[] = "emakefun";        // your network SSID (name)
char passwd[] = "501416wf";        // your network password

char mqtt_host[] = "192.168.2.56";
uint16_t mqtt_port = 1883;
char mqtt_client_id[] = "esp8266_client";
char mqtt_name[] = "test_name";
char mqtt_pass[] = "test_pass";

WiFiEspMqtt esp8266;

void setup()
{
  Serial.begin(115200);
  esp8266_serial.begin(9600);
  Serial.println("MqttSendReveive Test");
  WiFi.init(&esp8266_serial);
  assertEquals("Firmware version", WiFi.firmwareVersion(), "3.0.2");
  assertEquals("Status is (WL_DISCONNECTED)", WiFi.status(), WL_DISCONNECTED);

  esp8266.mqtt_connect_cfg(120, 1, "lws_topic", "lws_kill", 0);
  esp8266.mqtt_usercfg(mqtt_client_id, mqtt_name, mqtt_pass);
  esp8266.mqtt_connect(mqtt_host, mqtt_port, 0); 

  if (WiFi.begin(ssid, passwd) == WL_CONNECTED)
  {
     Serial.println("wifi connected");
     esp8266.mqtt_sub("/emakefun/user/get", 0); //订阅topic_1主题，qos为0
  }
  esp8266.mqtt_public("/emakefun/user/push", "on", 0);
  delay(30000);
}

void loop()
{
    if (esp8266.mqtt_receive())
    {

     Serial.print("topic:");
     Serial.println(esp8266.mqtt_topic);
     Serial.print("message:");
     Serial.println(esp8266.mqtt_message);

    }
}


////////////////////////////////////////////////////////////////////////////////////

void assertNotEquals(const char* test, int actual, int expected)
{
  if(actual!=expected)
    pass(test);
  else
    fail(test, actual, expected);
}

void assertEquals(const char* test, int actual, int expected)
{
  if(actual==expected)
    pass(test);
  else
    fail(test, actual, expected);
}

void assertEquals(const char* test, char* actual, char* expected)
{
  if(strcmp(actual, expected) == 0)
    pass(test);
  else
    fail(test, actual, expected);
}


void pass(const char* test)
{
  Serial.print(F("********** "));
  Serial.print(test);
  Serial.println(" > PASSED");
  Serial.println();
}

void fail(const char* test, char* actual, char* expected)
{
  Serial.print(F("********** "));
  Serial.print(test);
  Serial.print(" > FAILED");
  Serial.print(" (actual=\"");
  Serial.print(actual);
  Serial.print("\", expected=\"");
  Serial.print(expected);
  Serial.println("\")");
  Serial.println();
  delay(10000);
}

void fail(const char* test, int actual, int expected)
{
  Serial.print(F("********** "));
  Serial.print(test);
  Serial.print(" > FAILED");
  Serial.print(" (actual=");
  Serial.print(actual);
  Serial.print(", expected=");
  Serial.print(expected);
  Serial.println(")");
  Serial.println();
  delay(10000);
}
