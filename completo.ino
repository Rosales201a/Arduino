#include "Arduino.h"  // Incluye la biblioteca base de Arduino.
#include "DFRobotDFPlayerMini.h"  // Incluye la biblioteca para el módulo DFPlayer Mini.
#include <SoftwareSerial.h>  // Incluye la biblioteca para crear puertos seriales software.
#include <NewPing.h>  // Incluye la biblioteca para el sensor ultrasónico.

// Define los pines del TRIG y ECHO del sensor
#define TRIGGER_PIN  9
#define ECHO_PIN     10
// Define el pin para el módulo de vibración Yeelur
#define VIBRATION_PIN 6
// Define el máximo rango de medición en centímetros
#define MAX_DISTANCE 1
// Define el rango mínimo de detección en centímetros
#define MIN_DISTANCE 0

// Inicializa el objeto NewPing
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// Inicializa la comunicación serie con el DFPlayer Mini
SoftwareSerial Serial1(12, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;  // Crea un objeto para controlar el DFPlayer Mini.

void setup() {
  // Inicia la comunicación serial
  Serial1.begin(9600);  // Inicia la comunicación serial en el puerto Serial1 a 9600 bps.
  Serial.begin(9600);  // Inicia la comunicación serial en el puerto USB a 9600 bps.
  
  myDFPlayer.begin(Serial1);  // Inicia el DFPlayer Mini en el puerto Serial1.
  delay(2000);  // Espera 2 segundos para que el módulo esté listo.
  myDFPlayer.volume(1);  // Establece el volumen a 20.
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);  // Establece el ecualizador a normal.
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);  // Selecciona la tarjeta SD como dispositivo de salida.

  pinMode(VIBRATION_PIN, OUTPUT);  // Configura el pin de vibración como salida.
}

void loop() {
  // Realiza la medición de la distancia
  unsigned int distance = sonar.ping_cm();

  // Comprueba si la distancia está dentro del rango deseado
  if (distance > MIN_DISTANCE && distance <= MAX_DISTANCE) {
    // Muestra la distancia medida en el monitor serie
    Serial.print("Distancia: ");
    Serial.print(distance);
    Serial.println(" cm");
    
    // Reproduce el sonido en el DFPlayer Mini durante 3 segundos
    myDFPlayer.playFolder(1, 1);  // Reproduce el primer archivo en el primer folder.
    Serial.println("Reproduciendo pista 1");  // Imprime un mensaje en el monitor serial.
    
    delay(9000);  // Espera 9 segundos
    myDFPlayer.stop();  // Detiene la reproducción del sonido
    
    // Activa la vibración
    digitalWrite(VIBRATION_PIN, HIGH);  // Activa el pin de vibración
    delay(2000);  // Mantiene la vibración durante 2 segundos
    digitalWrite(VIBRATION_PIN, LOW);  // Desactiva el pin de vibración
    
    delay(3000);  // Espera 3 segundos antes de realizar otra medición
  }
}
