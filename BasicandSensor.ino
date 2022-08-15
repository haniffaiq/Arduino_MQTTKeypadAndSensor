#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define s0 4      //Module pins wiring
#define s1 5
#define s2 6
#define s3 7
#define out 8

// Stores frequency read by the photodiodes
int data=0; 
// WiFi
const char *ssid = "ZTE_2.4G_6a67KT"; // Enter your WiFi name
const char *password = "Siji2345";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "broker.hivemq.com";
const char *topic = "hanif/test";

const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);
void setup() {
   pinMode(s0,OUTPUT);    //pin modes
   pinMode(s1,OUTPUT);
   pinMode(s2,OUTPUT);
   pinMode(s3,OUTPUT);
   pinMode(out,INPUT);


  digitalWrite(s0,HIGH);
  digitalWrite(s1,HIGH);
  // Set software serial baud to 115200;
  Serial.begin(115200);

  // connecting to a WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  //connecting to a mqtt broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) {
      String client_id = "esp8266-client-";
      client_id += String(WiFi.macAddress());
      Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
      if (client.connect(client_id.c_str())) {
          Serial.println("Public emqx mqtt broker connected");
      } else {
          Serial.print("failed with state ");
          Serial.print(client.state());
          delay(2000);
      }
  }
  // publish and subscribe
  client.publish(topic, "hello emqx");
  client.subscribe(topic);
}

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
      Serial.print((char) payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");
}

void checkData(){
    if(Serial.available()){
        String msg = "";
        while(Serial.available()){
          msg += char(Serial.read());
          delay(50);
          }
        Serial.println(msg);
        client.publish(topic, msg.c_str());
      }
  }
void checkWarna(){
    digitalWrite(s2,LOW);        //S2/S3 levels define which set of photodiodes we are using LOW/LOW is for RED LOW/HIGH is for Blue and HIGH/HIGH is for green
   digitalWrite(s3,LOW);
   Serial.print("Red value= "); 
   GetData();                   //Executing GetData function to get the value

   digitalWrite(s2,LOW);
   digitalWrite(s3,HIGH);
   Serial.print("Blue value= ");
   GetData();

   digitalWrite(s2,HIGH);
   digitalWrite(s3,HIGH);
   Serial.print("Green value= ");
   GetData();

   Serial.println();

   delay(2000);
  }

  void GetData(){
   data=pulseIn(out,LOW);       //here we wait until "out" go LOW, we start measuring the duration and stops when "out" is HIGH again
   Serial.print(data);          //it's a time duration measured, which is related to frequency as the sensor gives a frequency depending on the color
   Serial.print("\t");          //The higher the frequency the lower the duration
   delay(20);
}

void loop() {
  client.loop();
  //client.publish(topic, "hello hanif");
  checkData();
  checkWarna();
  delay(1000);
}
