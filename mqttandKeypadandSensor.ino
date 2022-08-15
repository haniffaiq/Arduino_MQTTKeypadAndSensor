#include <ArduinoJson.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8

int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

const byte ROWS = 4;
const byte COLS = 3;


char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {43, 41, 39, 37};
byte colPins[COLS] = {35, 33, 31};
 
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
LiquidCrystal_I2C lcd(0x3F, 16, 2);//coba juga 0x27
 
char stringAngka[17];
int indexKeypad = 0;
String data = "";
int jmlJawaban = 0;

const int capacity = JSON_ARRAY_SIZE(10) + 10*JSON_OBJECT_SIZE(2);
StaticJsonDocument<capacity> doc;
void setup() {
  Serial3.begin(115200);
  Serial.begin(9600);
  
    // Setting the outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  
  // Setting the sensorOut as an input
  pinMode(sensorOut, INPUT);
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);

  Serial.println();
        for(int i = 0; i< sizeof(stringAngka); i++){
            stringAngka[i] = '0';
          }
  Wire.begin();
  Wire.beginTransmission(0x3F);
  if (Wire.endTransmission())
  {
    lcd = LiquidCrystal_I2C(0x27, 16, 2);
  }
  lcd.init();
  lcd.backlight();
  lcd.print("Hallo");
  delay(3000);
  lcd.clear();
}

void loop() {
  getNumber();
  getwarna();
}

void getwarna(){
    // Setting RED (R) filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  
  // Reading the output frequency
  redFrequency = pulseIn(sensorOut, LOW);
  
   // Printing the RED (R) value
  Serial.print("R = ");
  Serial.print(redFrequency);
  delay(100);
  
  // Setting GREEN (G) filtered photodiodes to be read
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  
  // Reading the output frequency
  greenFrequency = pulseIn(sensorOut, LOW);
  
  // Printing the GREEN (G) value  
  Serial.print(" G = ");
  Serial.print(greenFrequency);
  delay(100);
 
  // Setting BLUE (B) filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  
  // Reading the output frequency
  blueFrequency = pulseIn(sensorOut, LOW);
  
  // Printing the BLUE (B) value 
  Serial.print(" B = ");
  Serial.println(blueFrequency);
  delay(100);
  }



void sendData(int number, int id){
   Serial.println(id);
    //data += "{ data : ";
    //data += String(number);
    //DynamicJsonDocument doc(1024);
    
    
    if(id > 9 ){
        
        //serializeJson(doc, Serial3);
        serializeJson(doc, Serial3);
        jmlJawaban = 0;
      }
    else{
        doc[id]["id"] = id + 1;
        doc[id]["data"] = number;
        delay(5000);
      }
    //serializeJson(doc, Serial3);
    //Serial3.println(doc);
      if(Serial3.available()){
        String msg = "";
        while(Serial3.available()){
          msg += char(Serial3.read());

          delay(50);
          }

         Serial.println(msg);
          
        
      }
    
    data = ""; 
  }

void getNumber(){
  int nilaiAngka;
  
  char key = keypad.getKey();
  lcd.setCursor(0, 0);
  lcd.print("Input Angka:");
  lcd.print(stringAngka);
  if (key) {
    
    //Serial.println(key);
    switch (key)
    {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        if (!indexKeypad)
        {
          lcd.clear();
        }
        stringAngka[indexKeypad++] = key;
        //lcd.print(key);
        break;
      case '*'://reset
        for(int i = 0; i< sizeof(stringAngka); i++){
            stringAngka[i] = '0';
          }
        lcd.clear();
        indexKeypad = 0;
        break;
      case '#':
        
        stringAngka[indexKeypad] = 0;
        lcd.setCursor(0, 1);
 
        nilaiAngka = atoi(stringAngka);
        lcd.print(nilaiAngka);
        lcd.print(" Dikirim");
        sendData(nilaiAngka, jmlJawaban);
        jmlJawaban += 1;
        for(int i = 0; i< sizeof(stringAngka); i++){
            stringAngka[i] = '0';
          }
        indexKeypad = 0;
        break;
    }
    
  }
  }
