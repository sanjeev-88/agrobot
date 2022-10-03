m #include <Servo.h>

#include <ESP8266Firebase.h>

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include <ESP8266Firebase.h>

#include "FirebaseESP8266.h" // Install Firebase ESP8266 library
#include <esp8266wifi.h><ESP8266WiFi.h>
#define FIREBASE_HOST "wheeloo-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "RICEDxY1HnCboetLf9Eq79aq6ixEWectI4cz07vf"
//Motor PINs
#define ENA D0
#define IN1 D1
#define IN2 D2
#define IN3 D3
#define IN4 D4
#define ENB D5

const int trigPin = D7;
const int echoPin = D8;

#define analogpin         A0
int led = D6;  
int pos;
bool forward = 0;
bool backward = 0;
bool left = 0;
bool right = 0;
int Speed;
char auth[] = " hTD_A0ClYajC3ZNOI7a-PfVfUmntGE3-"; //Enter your Blynk application auth token
char ssid[] = "wheeloo"; //Enter your WIFI name
char pass[] = "sanjeev88"; //Enter your WIFI passowrd
Servo servo;
 FirebaseData firebaseData;
FirebaseData ledData;
FirebaseJson json; 
 
void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(led,OUTPUT);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  Blynk.begin(auth, ssid, pass);
  servo.attach(15); // 15 means D8 pin 
}
 
BLYNK_WRITE(V0) {
  forward = param.asInt();
}
 
BLYNK_WRITE(V1) {
  backward = param.asInt();
}
 
BLYNK_WRITE(V2) {
  left = param.asInt();
}
 
BLYNK_WRITE(V3) {
  right = param.asInt();
}
 
BLYNK_WRITE(V4) {
  Speed = param.asInt();
}
BLYNK_WRITE(V5) {

servo.write(param.asInt());

}

 BLYNK_WRITE(V6)
 {
  servo.write(0);
 }
 BLYNK_WRITE(V7)
 {
  servo.write(180);
 }
void smartcar() {
  if (forward == 1) {
    carforward();
    Serial.println("carforward");
  } else if (backward == 1) {
    carbackward();
    Serial.println("carbackward");
  } else if (left == 1) {
    carturnleft();
    Serial.println("carfleft");
  } else if (right == 1) {
    carturnright();
    Serial.println("carright");
  } else if (forward == 0 && backward == 0 && left == 0 && right == 0) {
    carStop();
    Serial.println("carstop");
  }
}
 
void loop() {
   digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
 int duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  int SOUND_VELOCITY;
    float distanceCm = duration * SOUND_VELOCITY/2;
  
for(pos=0;pos<=180;pos++)
{
  servo.attach(pos);
  delay(15);
}
for(pos=180;pos>=0;pos--)
{
  servo.write(pos);
  delay(15);
}
  delay(1000);
  Blynk.run();
  smartcar();

}
 
void carforward() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void carbackward() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void carturnleft() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void carturnright() 
{
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void carStop() 
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
