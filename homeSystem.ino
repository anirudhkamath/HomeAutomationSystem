#include <Servo.h>

Servo servo_test; //Servo Motor object
int angle=0; //angle of servo axle
int previous_angle=0;

int led=13; //LED connected to BT module
int sensor=2; //PIR sensor
int state=LOW;
int val=0;

//Ultrasonic sensor variables
long duration; 
int distance;
int trigPin=7;
int echoPin=8;
int hcLed=4;

char data; //BT module data

void setup() {
  // put your setup code here, to run once:
  pinMode(led,OUTPUT);
  pinMode(sensor, INPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(hcLed, OUTPUT);
  Serial.begin(9600);
  int val=0;
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); //sends 8 bursts in 10 microseconds
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); //time taken to detect reflection
  distance = duration*0.034/2; //in cms

  if(distance<10){
    digitalWrite(hcLed, HIGH);
  }
  else if(distance>=10){
    digitalWrite(hcLed, LOW);
  }
  
  val=digitalRead(sensor);
  if (val == HIGH) {           // check if the sensor is HIGH
    //digitalWrite(led, HIGH);   // turn LED ON
    //delay(10);                // delay 100 milliseconds

    servo_test.attach(3); //attaches servo object to the PWM pin 3
    
    if (state == LOW) { //state tells you if motion was already detected by the PIR sensor
      Serial.println("Motion detected!"); 
      state = HIGH;       // update variable state to HIGH
      for(angle = 0; angle < 90; angle += 1)    // command to move from 0 degrees to 90 degrees 
      {                                  
        servo_test.write(angle);                 //command to rotate the servo to the specified angle
        delay(15);                       
      } 
    }
  } 
  else {
      if(previous_angle!=90){ //to stop servo from spinning erratically
        servo_test.write(90);
        previous_angle=90;
      }
      //digitalWrite(led, LOW); // turn LED OFF
      //delay(20);             // delay 200 milliseconds
       for(angle = 180; angle>=0; angle-=5)     // command to move from 180 degrees to 0 degrees 
       {                                
          servo_test.write(angle);              //command to rotate the servo to the specified angle
          delay(30);                       
       } 
      
      if (state == HIGH){
        state = LOW;       // update variable state to LOW
        servo_test.detach(); //to detach servo motor off from PWM
    }
  }

  if(Serial.available()>0){ //When the smartphone connects to the module via Bluetooth, it sends serial data. Serial.available() shows positive when data is detected on Serial monitor
    data = Serial.read(); //reading Bluetooth serial data
    if(data=='1'){ //ON
      digitalWrite(led,HIGH);
    }
    else if(data=='0'){ //OFF
      digitalWrite(led,LOW);
    }
  }
}
