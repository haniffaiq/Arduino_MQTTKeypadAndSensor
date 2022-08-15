//#define BLYNK_TEMPLATE_ID "TMPLjXwjEeXW"
//#define BLYNK_DEVICE_NAME "Tugas Akhir"
//#define BLYNK_AUTH_TOKEN "rsHReQUIa4_E_Ow3gB7AY16rGi-g5LSr"

#define BLYNK_TEMPLATE_ID "TMPLxWgIj1Nn"
#define BLYNK_DEVICE_NAME "Heart Rate Monitor"
#define BLYNK_AUTH_TOKEN "9XTFrUr9ROuIcHTGjL3q2AYS8BkemEOw"


#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h> 
#include <BlynkSimpleEsp8266.h>
// GPIO where the DS18B20 is connected to
#define BLYNK_PRINT Serial

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "ZTE_2.4G_6a67KT";  // type your wifi name
char pass[] = "Siji2345";
BlynkTimer timer;
const int oneWireBus = 4;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);
  // Start the DS18B20 sensor
  sensors.begin();
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi..");
  }
  Blynk.config(auth);

}

void loop() {
  Blynk.run();
  getSuhu();

}

void getSuhu(){
  
    sensors.requestTemperatures(); 
    float temperatureC = sensors.getTempCByIndex(0);
    float temperatureF = sensors.getTempFByIndex(0);
    Serial.print(temperatureC);
    Serial.println("ºC");
    Serial.print(temperatureF);
    Serial.println("ºF");
    Blynk.virtualWrite(V1, temperatureC);
    delay(1000);
  }
