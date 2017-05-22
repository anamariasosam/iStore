#include "HX711.h"
#include<Servo.h>

HX711 cell(4,5 );
long val = 0;
float cont  = 0;
float cantidad = 0;

const int buzzer =  3;
const int switch_pin = 2;
int estadoSwitch = 0;


int s_temperatura = 0;
int valor_temperatura = 0;
int valorConvertido = 0;
int MAX_TEMP = 20;

int pinIn1 = 8;
int pinIn2 = 9;
int numPeoples = 0;
int oneTime = 0;
int oneTimePIR = 0;
boolean isOpen = false;
Servo servoMotor;

int pinPIR = 11;


void setup() {
  Serial.begin(9600);

  pinMode(buzzer, OUTPUT);
  pinMode(switch_pin, INPUT);

  pinMode(s_temperatura, INPUT);


  pinMode(pinIn1,INPUT);
  pinMode(pinIn2,INPUT);
  servoMotor.attach(10);
  servoMotor.write(90);

  pinMode(pinPIR,INPUT);
}

void loop() {
  asesor();
  delay(100);
  temperatura();
  delay(100);
  peopleInside();
  delay(100);
  countProduct();
}


void asesor() {
  if (digitalRead(switch_pin) == HIGH) {
      digitalWrite(buzzer, HIGH);
      Serial.println("Necesitamos un asesor");
  } else {
      digitalWrite(buzzer, LOW);
  }
}

void temperatura() {
  valorConvertido = (5.0 * analogRead(s_temperatura) * 100.0) / 1024;
  if ( valorConvertido > MAX_TEMP)
  {
    Serial.println(valorConvertido);
  }
}

void openDoor(){
  servoMotor.write(180);
   Serial.println("Puerta Abierta");
}
void closeDoor(){
  servoMotor.write(90);
  Serial.println("Puerta cerrada");
}

void peopleInside(){
    int in1 = digitalRead(pinIn1);
    int in2 = digitalRead(pinIn2);
    int movement = digitalRead(pinPIR);

  if(movement == 1 && oneTimePIR == 0){
    if(numPeoples > 0){
      Serial.println("Se han aproximado a un producto");
      oneTimePIR = 1;
    }
  }
  if(movement == 0 && oneTimePIR == 1){
    if(numPeoples > 0){
      Serial.println("Ninguna proximidad al producto");
      oneTimePIR = 0;
    }
  }

      if(in1 == 0&& in2 == 0 && isOpen == false){
        if(oneTime == 0){
          closeDoor();
          oneTime = 1;
        }
      }

      if(isOpen == false && in1 == 1 && in2 == 0 ){
          openDoor();
          oneTime = 0;
          isOpen = true;
      }
      if(isOpen == true && in1 == 0 && in2 == 1){
         numPeoples = numPeoples + 1;
         Serial.print("# Personas: ");
         Serial.println(numPeoples);
         isOpen = false;
      }
}


void countProduct(){
  cont = cont+1;
    val = cell.read();
    cantidad = ((( val - (-175484))/498.48f)+5);
  Serial.println(cantidad/200);
}
