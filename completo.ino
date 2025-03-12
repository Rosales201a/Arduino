#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"
#include <SoftwareSerial.h>
#include <NewPing.h>

#define TRIGGER_PIN  9
#define ECHO_PIN     10
#define VIBRATION_PIN 6
#define MAX_DISTANCE 2
#define MIN_DISTANCE 0

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
SoftwareSerial Serial1(12, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

bool isObjectDetected = false;  // Variable de estado para la detección del objeto

void setup() {
  Serial1.begin(9600);
  Serial.begin(9600);
  
  if (!myDFPlayer.begin(Serial1)) {
    Serial.println("Error inicializando DFPlayer Mini");
    while (true);  // Detiene la ejecución si hay un error
  }
  
  delay(2000);
  myDFPlayer.volume(30);
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);

  pinMode(VIBRATION_PIN, OUTPUT);
  Serial.println("Setup completado");
}

void loop() {
  unsigned int distance = sonar.ping_cm();
  Serial.print("Distancia medida: ");
  Serial.println(distance);

  if (distance > MIN_DISTANCE && distance <= MAX_DISTANCE) {
    if (!isObjectDetected) {
      Serial.println("Detección de objeto dentro del rango.");

      myDFPlayer.playFolder(1, 1);  // Reproduce el archivo 1 en la carpeta 1
      Serial.println("Reproduciendo pista 1");

      digitalWrite(VIBRATION_PIN, HIGH);

      isObjectDetected = true;  // Actualiza el estado del objeto
    }
  } else {
    if (isObjectDetected) {
      delay (200); 
      myDFPlayer.stop();  // Detiene la reproducción del sonido
      Serial.println("Deteniendo reproducción");
      
      digitalWrite(VIBRATION_PIN, LOW);  // Detiene la vibración

      isObjectDetected = false;  // Actualiza el estado del objeto
    }
  }
  
  delay(100);
}

