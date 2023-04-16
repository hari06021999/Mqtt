#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#define LED D0            // Led in NodeMCU at pin GPIO16 (D0).
// WiFi
const char *ssid = "FelicitousSolutions"; // Enter your WiFi name
const char *password = "Felicitous@aad12345";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "broker.hivemq.com";
const char *topic = "starttopic";

const int mqtt_port = 1883;
int solar=A0;
int ir_status=0;
int buff[20];
//int diff[20];
int diff=0;
int a=0,b[20];
int i=0;
int c;
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  // Set software serial baud to 115200;
  Serial.begin(115200);
   pinMode(LED, OUTPUT);
    pinMode(solar,INPUT);
      digitalWrite(LED,LOW);
   pinMode(2,OUTPUT);  
    

  // connecting to a WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
     // Serial.println("Connecting to WiFi..");
  }
 // Serial.println("Connected to the WiFi network");
  digitalWrite(2,HIGH);
    delay(500);
  digitalWrite(2,LOW);
    delay(500);
  //connecting to a mqtt broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) {
      String client_id = "esp8266-client-";
      client_id += String(WiFi.macAddress());
    //  Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
      if (client.connect(client_id.c_str())) {
          Serial.println("Public  mqtt broker connected");
          digitalWrite(2,HIGH);
            delay(500);
          digitalWrite(2,LOW);
            delay(500);

      } else {
        //  Serial.print("failed with state ");
          Serial.print(client.state());
          delay(2000);
      }
  }
  // publish and subscribe
  client.subscribe("starttopic");
}

void callback(char *topic, byte *payload, unsigned int length) {
  int i;
 Serial.print("Received [");
  Serial.print(topic);
  Serial.print("]: ");
  for (i = 0; i < length; i++)
  {
         Serial.print((char)payload[i]);
  }
  payload = 0;
  // Serial.println();
  
}

void loop() {
   solar_fun();
  client.loop();
 
   if(Serial.available())
  {
    char rec = Serial.read();
       publishMessage(rec);


  }
}
void publishMessage(char rec)
{
  StaticJsonDocument<200> doc;
  doc["Status"] =rec;
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client
 
  client.publish("resulttopic", jsonBuffer);
  digitalWrite(BUILTIN_LED,HIGH);
  delay(500);
  digitalWrite(BUILTIN_LED,LOW);
    delay(500);

}

void solar_fun()
{
  // digitalWrite(LED,LOW);
     ir_status=analogRead(solar);
      buff[i]=ir_status;
      // Serial.print("Value: ");
        Serial.println(buff[i]);
     if(i>0)
     {
        c= buff[i]-buff[i-1];
        // Serial.print("Value: ");
        // Serial.println(c);
         delay(50);
        if(c>13&&c<23)
        {
             digitalWrite(LED,HIGH);
              Serial.println("Shot success");

             delay(5000);
            //  digitalWrite(LED,LOW);
                    }
        else
        {
          digitalWrite(LED,LOW);
        }
        
             

        
     }
     if(i>19)
        i=0;
    i++;
}
  
