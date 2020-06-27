#include "ESP8266WiFi.h"
const char* ssid = "isef";
const char* password =  "mshmsh22";
#define enA 5
#define in1 13
#define in2 12
#define enB 16
#define in3 14
#define in4 4
int motorSpeedA = 0;
int motorSpeedB = 0;
int xAxis;
int yAxis;
int c;
int a;
int t=4; 
int msg[4];
WiFiServer wifiServer(1180);
 
void setup() {
 
  Serial.begin(115200);
  delay(1000);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting..");
  }
 
  Serial.print("Connected to WiFi. IP:");
  Serial.println(WiFi.localIP());
 
  wifiServer.begin();
  
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

}
 
void loop() {
 

WiFiClient client = wifiServer.available();
 
  if (client) {
 
    while (client.connected()) {
 
      while (client.available()>0) {
        c = client.read();
        if (c == 255){
          for(int b=0 ; b<=t ; b++){
            c=client.read();
            if (c==0){
              b=t;
            }
            else {
            msg[b]= c;
            }
             
          }
          //write the code you want to act with here 
          Serial.print(msg[0]);
          Serial.print("\t");
          Serial.print(msg[1]);
          Serial.print("\t");
          Serial.println(msg[2]);
  xAxis =msg[0];
  yAxis= msg[2];
  // Y-axis used for forward and backward control
  if (yAxis < 110) {
    // Set Motor A backward
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    // Set Motor B backward
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    // Convert the declining Y-axis readings for going backward from 470 to 0 into 0 to 255 value for the PWM signal for increasing the motor speed
    motorSpeedA = map(yAxis, 110, 0, 0, 1024);
    motorSpeedB = map(yAxis, 110, 0, 0, 1024);
  }
  else if (yAxis > 135) {
    // Set Motor A forward
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    // Set Motor B forward
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    // Convert the increasing Y-axis readings for going forward from 550 to 1023 into 0 to 255 value for the PWM signal for increasing the motor speed
    motorSpeedA = map(yAxis, 135, 254, 0, 1024);
    motorSpeedB = map(yAxis, 135, 254, 0, 1024);
  }
  // If joystick stays in middle the motors are not moving
  else {
    motorSpeedA = 0;
    motorSpeedB = 0;
  }
  // X-axis used for left and right control
  if (xAxis < 110) {
    // Convert the declining X-axis readings from 470 to 0 into increasing 0 to 255 value
    int xMapped = map(xAxis, 110, 0, 0, 1024);
    // Move to left - decrease left motor speed, increase right motor speed
    motorSpeedA = motorSpeedA - xMapped;
    motorSpeedB = motorSpeedB + xMapped;
    // Confine the range from 0 to 255
    if (motorSpeedA < 0) {
      motorSpeedA = 0;
    }
    if (motorSpeedB > 1024) {
      motorSpeedB = 1024;
    }
  }
  if (xAxis > 135) {
    // Convert the increasing X-axis readings from 550 to 1023 into 0 to 255 value
    int xMapped = map(xAxis, 135, 254, 0, 1024);
    // Move right - decrease right motor speed, increase left motor speed
    motorSpeedA = motorSpeedA + xMapped;
    motorSpeedB = motorSpeedB - xMapped;
    // Confine the range from 0 to 255
    if (motorSpeedA > 1024) {
      motorSpeedA = 1024;
    }
    if (motorSpeedB < 0) {
      motorSpeedB = 0;
    }
  }
  // Prevent buzzing at low speeds (Adjust according to your motors. My motors couldn't start moving if PWM value was below value of 70)
  /*if (motorSpeedA < 70) {
    motorSpeedA = 0;
  }
  if (motorSpeedB < 70) {
    motorSpeedB = 0;
  }*/
  analogWrite(enA, motorSpeedA); // Send PWM signal to motor A
  analogWrite(enB, motorSpeedB); // Send PWM signal to motor B
          Serial.print(motorSpeedA);
          Serial.print("\t");
          Serial.println(motorSpeedB);


        
        }

      }

    }
 
    client.stop();
    Serial.println("Client disconnected");
 
  }
}
