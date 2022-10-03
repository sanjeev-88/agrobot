
#include <DHT.h>
#include <DHT_U.h>
#include "MQ135.h"
//const int trigPin = D0;
//const int echoPin = D1;


#include <ESP8266Firebase.h>

#include "FirebaseESP8266.h" // Install Firebase ESP8266 library
#include <esp8266wifi.h><ESP8266WiFi.h>
//#include <dht.h><DHT.h> // Install DHT11 Library and Adafruit Unified Sensor Library</dht.h></esp8266wifi.h>
#define FIREBASE_HOST "wheeloo-default-rtdb.firebaseio.com" //Without <a href="http:// or"> http:// or </a> <a href="https:// schemes"> https:// schemes
//</a>
#define FIREBASE_AUTH "RICEDxY1HnCboetLf9Eq79aq6ixEWectI4cz07vf"
#define WIFI_SSID "wheeloo"
#define WIFI_PASSWORD "sanjeev88"
#define DHTPIN    D7 // Connect Data pin of DHT to D2
int led = D5; // Connect LED to D5
#define DHTTYPE DHT11
#define buzzer            D0
#define S0                D3
#define S1                D4
#define analogpin         A0
int proximity;
int light;
int methane;
float CO2;
int sense_pin;
int value=0;

long duration;
int distance;
DHT dht(DHTPIN, DHTTYPE);
//Define FirebaseESP8266 data object
FirebaseData firebaseData;
FirebaseData ledData;
FirebaseJson json;

void setup()
{
Serial.begin(115200);
dht.begin();
pinMode(led,OUTPUT);
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
Serial.print("Connecting to Wi-Fi");
while (WiFi.status() != WL_CONNECTED)
{
Serial.print(".");
delay(300);
}
Serial.println();
Serial.print("Connected with IP: ");
Serial.println(WiFi.localIP());
Serial.println();
Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
Firebase.reconnectWiFi(true);
}
void sensorUpdate(){
// Reading temperature or humidity takes about 250 milliseconds!
// Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
float h = dht.readHumidity();
// Read temperature as Celsius (the default)
float t = dht.readTemperature();
// Read temperature as Fahrenheit (isFahrenheit = true)
float f = dht.readTemperature(true);
// Check if any reads failed and exit early (to try again).
if (isnan(h) || isnan(t) || isnan(f)) 
{
Serial.println(F("Failed to read from DHT sensor!"));
return;
}
Serial.print(F("Humidity: "));
Serial.print(h);
Serial.print(F("% Temperature: "));
Serial.print(t);
Serial.print(F("C ,"));
Serial.print(f);
Serial.println(F("F "));
if (Firebase.setFloat(firebaseData, "/FirebaseIOT/temperature", t))
{
Serial.println("PASSED");
Serial.println("PATH: " + firebaseData.dataPath());
Serial.println("TYPE: " + firebaseData.dataType());
Serial.println("ETag: " + firebaseData.ETag());
Serial.println("------------------------------------");
Serial.println();
}
else
{
Serial.println("FAILED");
Serial.println("REASON: " + firebaseData.errorReason());
Serial.println("------------------------------------");
Serial.println();
}
if (Firebase.setFloat(firebaseData, "/FirebaseIOT/humidity", h))
{
Serial.println("PASSED");
Serial.println("PATH: " + firebaseData.dataPath());
Serial.println("TYPE: " + firebaseData.dataType());
Serial.println("ETag: " + firebaseData.ETag());
Serial.println("------------------------------------");
Serial.println();
}
else
{
Serial.println("FAILED");
Serial.println("REASON: " + firebaseData.errorReason());
Serial.println("------------------------------------");
Serial.println();
}
}
void loop() {
 
// Calculating the distance
distance= duration*0.034/2;
// Prints the distance on the Serial Monitor
//Serial.print("Distance: ");
//Serial.println(distance);
delay(2000);
sensorUpdate();
if (Firebase.getString(ledData, "/FirebaseIOT/led")){
Serial.println(ledData.stringData());
if (ledData.stringData() == "1") {
digitalWrite(led, HIGH);
}
else if (ledData.stringData() == "0"){
digitalWrite(led, LOW);
}
}
delay(100);

// Address 00
  digitalWrite(S0, LOW);
  digitalWrite(S1, LOW);
  methane = analogRead(analogpin);
  const int R_0 = 945;
  float v_o = methane * 5 / 1023; // convert reading to volts
   float R_S = (5-v_o) * 1000 / v_o; // apply formula for getting RS
   float PPM = pow(R_S/R_0,-2.95) * 1000; //apply formula for getting PPM
  
  Serial.print("methane gas - "); 
  Serial.println(PPM);
  if (Firebase.setFloat(firebaseData, "/FirebaseIOT/methane", PPM))
{
Serial.println("PASSED");
Serial.println("PATH: " + firebaseData.dataPath());
Serial.println("TYPE: " + firebaseData.dataType());
Serial.println("ETag: " + firebaseData.ETag());
Serial.println("------------------------------------");
Serial.println();
}
else
{
Serial.println("FAILED");
Serial.println("REASON: " + firebaseData.errorReason());
Serial.println("------------------------------------");
Serial.println();
}
  // Address 10
 // digitalWrite(S0, HIGH);
//  digitalWrite(S1, LOW);
 // motion = analogRead(analogpin);
 // if (motion > 512)
 // {
  //  motion = 1;
   // Serial.print("obstacle!!");
   // Serial.println(motion);
 // }
 // else
  
    //motion = 0;
    
 // }


// Address 11
  digitalWrite(S0, HIGH);
  digitalWrite(S1, HIGH);
  int raw_light = analogRead(analogpin);
  light = map(raw_light, 1024, 0, 0, 100);
  Serial.print("Light - "); 
  Serial.println(light);
  if(light>20)
  {
    Serial.print("light is detected");
    digitalWrite(light,HIGH);
  }
  else{
    Serial.print("light is not detected");
    digitalWrite(light,LOW);
  }
  Serial.println();
  delay(1000);
  if (Firebase.setFloat(firebaseData, "/FirebaseIOT/light", light))
{
Serial.println("PASSED");
Serial.println("PATH: " + firebaseData.dataPath());
Serial.println("TYPE: " + firebaseData.dataType());
Serial.println("ETag: " + firebaseData.ETag());
Serial.println("------------------------------------");
Serial.println();
}
else
{
Serial.println("FAILED");
Serial.println("REASON: " + firebaseData.errorReason());
Serial.println("------------------------------------");
Serial.println();
} 
//rain 10
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  delay(100);
  int rain = analogRead(analogpin);
  rain = constrain(rain, 150, 440);
  rain = map(rain, 0, 1023, 0,100);
  
  Serial.print("Rain value: ");
  Serial.println(rain);
  if(rain>50)
  {
    Serial.print("rain is detected");
    digitalWrite(rain,HIGH);
  }
  else{
    Serial.print("rain is not detected");
    digitalWrite(rain,LOW);
  }
  Serial.println();
  delay(100);
  if (Firebase.setFloat(firebaseData, "/FirebaseIOT/rain", rain))
{
Serial.println("PASSED");
Serial.println("PATH: " + firebaseData.dataPath());
Serial.println("TYPE: " + firebaseData.dataType());
Serial.println("ETag: " + firebaseData.ETag());
Serial.println("------------------------------------");
Serial.println();
}
else
{
Serial.println("FAILED");
Serial.println("REASON: " + firebaseData.errorReason());
Serial.println("------------------------------------");
Serial.println();
} 
//soil moisture
 digitalWrite(S0,LOW);
 digitalWrite(S1,LOW);
  sense_pin=analogRead(analogpin);
 Serial.print("MOISTURE LEVEL : ");
 float  moisture_percentage =  ( (analogRead(analogpin)/1023.00) * 100.00 ) ;
   Serial.print("soil moisture(in percentage)= ");
   delay(1000);
   Serial.print(moisture_percentage); 
   
 
 //mq135 01
   digitalWrite(S0, LOW);
   digitalWrite(S1, HIGH);
   CO2= analogRead(analogpin);
 // float rzero = CO2.getRZero();
 // float ppm = CO2.getPPM();
  
   Serial.print("ppm: "); 
   Serial.println(CO2); 
   if (Firebase.setFloat(firebaseData, "/FirebaseIOT/co2", CO2))
{
Serial.println("PASSED");
Serial.println("PATH: " + firebaseData.dataPath());
Serial.println("TYPE: " + firebaseData.dataType());
Serial.println("ETag: " + firebaseData.ETag());
Serial.println("------------------------------------");
Serial.println();
}
else
{
Serial.println("FAILED");
Serial.println("REASON: " + firebaseData.errorReason());
Serial.println("------------------------------------");
Serial.println();
  
}
}   
