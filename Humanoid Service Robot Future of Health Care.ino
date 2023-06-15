#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <WiFiClient.h>

#define IN1 13
#define IN2 14
#define IN3 27
#define IN4 26
#define EnA 22
#define EnB 23
#define SL 25
#define SR 33

int control_mode = 1;

int t_no = 2; 
int checkpoint = 0;

int x = 50;
int y = 50;
int speed = 100;

int pwmChannelEnA = 0;
int frequenceEnA = 1000;
int resolutionEnA = 8;
int pwmChannelEnB = 0;
int frequenceEnB = 1000;
int resolutionEnB = 8;

char auth[] = "xW3lAbV9bDly6c-c01rO3FLmH0aaEjQn";
char ssid[] = "Emon";
char pass[] = "12345678";

void setup() {
  Serial.begin(9600); 

  pinMode(EnA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(EnB, OUTPUT);

  pinMode(SL, INPUT);
  pinMode(SR, INPUT);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  ledcSetup(pwmChannelEnA, frequenceEnA, resolutionEnA);
  ledcAttachPin(EnA, pwmChannelEnA);
  ledcWrite(pwmChannelEnA, speed);
  ledcSetup(pwmChannelEnB, frequenceEnB, resolutionEnB);
  ledcAttachPin(EnB, pwmChannelEnB);
  ledcWrite(pwmChannelEnB, speed);

  //Initialize the Blynk library
  Blynk.begin(auth, ssid, pass);   
}

// GET BLYNK vPIN VALUES

BLYNK_WRITE(V0) {
  control_mode = param.asInt();
}
BLYNK_WRITE(V1) {
  x = param[0].asInt();
}
BLYNK_WRITE(V2) {
  y = param[0].asInt();
}
BLYNK_WRITE(V3) {
  t_no = param[0].asInt();
}

// To Controll Robot using WiFi
void smartcar() {
  if (y > 70) {
    carForward();
    Serial.println("WiFi >> carForward");
  } else if (y < 30) {
    carBackward();
    Serial.println("WiFi >> carBackward");
  } else if (x < 30) {
    carLeft();
    Serial.println("WiFi >> carLeft");
  } else if (x > 70) {
    carRight();
    Serial.println("WiFi >> carRight");
  } else if (x < 70 && x > 30 && y < 70 && y > 30) {
    carStop();
    Serial.println("WiFi >> carStop");
  }
}




void loop() {

  Blynk.run();
  
  if(control_mode==1) {
    smartcar();
  }
  else {

    if(checkpoint > 3) {
      checkpoint = 0;
    }

    if(digitalRead(SL) == LOW && digitalRead(SR) == LOW) {

      checkpoint = checkpoint + 1;
      carStop();
      Serial.println("==================================================");
      Serial.print("New Checkpoint Detected for ");
      Serial.print("Table No: ");
      Serial.print(t_no);
      Serial.println("");
      Serial.print("Current Checkpoint: ");
      Serial.print(checkpoint);
      Serial.println(" ");
      Serial.println("==================================================");
      delay(1000);
      carForward();
      delay(100);
    
      if(t_no == 1 && checkpoint == 1) {
        carLeft();
        delay(1000);
      }

      if(t_no == 2 && checkpoint == 1) {  
        carRight();
        delay(1000);
        //carForward();
        //delay(500);
      }

      if(t_no == 3 && checkpoint == 2) {
        carLeft();
        delay(1000);
      }

      if(t_no == 4 && checkpoint == 2) {
        carRight();
        delay(1000);
        //carForward();
        //delay(500);
      }
      if ( (t_no == 1 || t_no == 2) && checkpoint == 2) {
        carStop();
        Serial.println("Car Stopped for 7 Seconds!");
        delay(7000);
        checkpoint = 0;  
      }
      if ( (t_no == 3 || t_no == 4) && checkpoint == 3) {
        carStop();
        Serial.println("Car Stopped for 7 Seconds!");
        delay(7000);
        checkpoint = 0;
      }
    } 
    else {
      /***************  FOR LINE FOLLOWING  *****************/
      delay(100);

      if(digitalRead(SL) == HIGH && digitalRead(SR) == HIGH) {
        carForward();
      }
      else if(digitalRead(SL) == HIGH && digitalRead(SR) == LOW) {
        carRight();
      }
      else if(digitalRead(SL) == LOW && digitalRead(SR) == HIGH) {
      carLeft();
      } 
      /* if(digitalRead(SL)==LOW  && digitalRead(SR)==LOW)
      {
        carStop(); 
      } */
    }
  }
}

/**************Motor movement functions*****************/
void carRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Car Right");
}
void carLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("Car Left");
}
void carBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Car Backward");
}
void carForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
 // Serial.println("Car Forward");
}
void carStop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);  
  Serial.println("Car Stop");
}