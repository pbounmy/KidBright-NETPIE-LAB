
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <WiFi.h>



#define BUZZER_PIN 13
#define WIFI_LED 2
#define IOT_LED 12
#define USB_IO 25
#define BT_LED 23

#define LM73_ADDR 0x4D

#include <DHT.h>
#define DHTTYPE DHT21
#define DHTPIN 19

DHT dht(DHTPIN, DHTTYPE);

float humiddht = 0;     // ค่าความชื้น
float tempdht  = 0;     // ค่าอุณหภูมิ

const char* ssid     = "Goyangle";
const char* password = "pi3thnop";


int analog_value = 0;
double temp=0;



Adafruit_8x16minimatrix matrix = Adafruit_8x16minimatrix();
///////////DHT sensor//////////////////////////////
void dhtreadTempAndHumid()
{
          humiddht = dht.readHumidity();     
          tempdht  = dht.readTemperature();  
         
          Serial.print("Humid: "); Serial.print(humiddht); Serial.print(" %, ");
          Serial.print("Temp: "); Serial.print(tempdht); Serial.println(" °C ");

          matrix.setTextSize(1);
      matrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
      matrix.setTextColor(LED_ON);
      matrix.setRotation(1);
      for (int8_t x=7; x>=-126; x--) {
        matrix.clear();
        matrix.setCursor(x,0);
        matrix.print("Temp: " + String(tempdht) + " C " + "Humid = " + String(humiddht) + " %");
        matrix.writeDisplay();
        delay(50);
      }
  
}




///////////read temperature/////////////////

float readTemperature() {
  Wire1.beginTransmission(LM73_ADDR);
  Wire1.write(0x00); // Temperature Data Register
  Wire1.endTransmission();
  
  uint8_t count = Wire1.requestFrom(LM73_ADDR, 2);
  float temp = 0.0;
  if (count == 2) {
    byte buff[2];
    buff[0] = Wire1.read();
    buff[1] = Wire1.read();
    temp += (int)(buff[0]<<1);
    if (buff[1]&0b10000000) temp += 1.0;
    if (buff[1]&0b01000000) temp += 0.5;
    if (buff[1]&0b00100000) temp += 0.25;
    if (buff[0]&0b10000000) temp *= -1.0;
  }
  return temp;
}




///show Temp and Light  ///////////////

void measure(){
 
   
      matrix.setTextSize(1);
      matrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
      matrix.setTextColor(LED_ON);
      matrix.setRotation(1);
      temp = readTemperature();
      Serial.print("Temp:");Serial.println(temp);
      analog_value = analogRead(36);
      if(analog_value > 1000){
        analog_value = 1000;
      }
      int ll = (1000 - analog_value)*100/1000;
      String Kidrightsensordata = "T: " + String(temp) + " C " + "L: " + String(ll) + " %";
      for (int8_t x=7; x>=-126; x--) {
        matrix.clear();
        matrix.setCursor(x,0);
        matrix.print(Kidrightsensordata);
        matrix.writeDisplay();
        delay(50);
      }
     
    

}


////IP show/////////////

String ipToString(IPAddress ip){
  String s="";
  for (int i=0; i<4; i++)
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  matrix.setTextSize(1);
  matrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
  matrix.setTextColor(LED_ON);
  matrix.setRotation(1);
  
  for (int8_t x=7; x>=-100; x--) {
    matrix.clear();
    matrix.setCursor(x,0);
    matrix.print("IP:"+s);
    matrix.writeDisplay();
    delay(50);
  }



    
  return s;
}




void setup() {
  //while (!Serial);
  Serial.begin(9600);
  Serial.println("16x8 LED Mini Matrix Test");

  Wire1.begin(4, 5);
 
 pinMode(WIFI_LED, OUTPUT); 
  pinMode(IOT_LED, OUTPUT); 
  pinMode(USB_IO, OUTPUT); 
  pinMode(BT_LED, OUTPUT); 
  digitalWrite(WIFI_LED, HIGH);
  digitalWrite(IOT_LED, HIGH);
  digitalWrite(USB_IO, HIGH);
  digitalWrite(BT_LED, HIGH);
  
  matrix.begin(0x70);  // pass in the address

   matrix.setTextSize(1);
  matrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
  matrix.setTextColor(LED_ON);
  matrix.setRotation(1);
  Serial.println("IoT");

  

  
  for (int8_t x=7; x>=-100; x--) {
    matrix.clear();
    matrix.setCursor(x,0);
    matrix.print("NETPIE Training");
    matrix.writeDisplay();
    delay(100);
  }
  


   
  WiFi.begin(ssid, password);
  int count =0;
  while (WiFi.status() != WL_CONNECTED) {    
    delay(500);
    Serial.print(".");
      matrix.clear();
     matrix.setCursor(0,0);
     count = count+1;
    matrix.print(String(count)+"..");
    matrix.writeDisplay();
  }
  
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

 digitalWrite(WIFI_LED, LOW);
 
  


 
  
}


void loop() {
  
 ipToString(WiFi.localIP());
  
  measure();
  delay(500);
  dhtreadTempAndHumid();
  delay(500);
}
