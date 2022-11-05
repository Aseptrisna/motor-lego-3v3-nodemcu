#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//WI-FI
const char *ssid = "KAPALSELAM"; 
const char *password = "12345678"; 

//RABBIT MQ
const char *mqtt_broker = "cloudrmqserver.pptik.id";
const char *topic = "kapalselam";
const char *mqtt_username = "/kapalselam:kapalselam";
const char *mqtt_password = "1245and4512";
const int mqtt_port = 1883;

int relay1 = D1 ;
int relay2 = D2 ;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  pinMode(relay1,OUTPUT);
  pinMode(relay2,OUTPUT);
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  
  // connecting to a WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi !");
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) {
      String client_id = "esp8266-client-";
      client_id += String(WiFi.macAddress());
      Serial.printf("The client %s connects to the rabbitmq \n", client_id.c_str());
      if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
          Serial.println("RabbitMQ connected !");
      } else {
          Serial.print("failed with state ");
          Serial.print(client.state());
          delay(2000);
      }
  }
//  client.publish(topic, "kapalselam");
  client.subscribe(topic);
}

void callback(char *topic, byte *payload, unsigned int length) {
  char message [7] ;
  Serial.print("Ada Pesan !! :");
  Serial.println(topic);
  Serial.println("Pesan :");
  for (int i = 0; i < length; i++) {
      Serial.print((char) payload[i]);
      message[i] = (char)payload[i];
  }
  String convertMsg = String(message) ;
  if (message[0] == '0') {
    digitalWrite(relay1, LOW);
    digitalWrite(relay2, HIGH);
  } else if (message[0] == '1') {
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, LOW);
  }else{
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
  }
 
}

void loop() {
  client.loop();
}

void sensor (){}
