#include <QTRSensors.h>

#define NUM_SENSORS             5  // Número de sensores a usar
#define NUM_SAMPLES_PER_SENSOR  4  // Tomar cuatro muestras durante la calibración
#define EMITTER_PIN             2  // El pin digital 2 

// Se van a usar 5 sensores, del 2 al 6
QTRSensorsAnalog qtra((unsigned char[]) {0, 1, 2, 3, 4}, 
  NUM_SENSORS, NUM_SAMPLES_PER_SENSOR, EMITTER_PIN);
unsigned int sensorValues[NUM_SENSORS];
bool s2, s3, s4, s5, s6, f, l, r;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  // Se enciende el LED para indicar que se puede calibrar el sensor QTR
  digitalWrite(LED_BUILTIN, HIGH);

  // Calibrar el sensor durante 10s aproximadamente
  for (int i = 0; i < 400; i++)
  {
    qtra.calibrate();
  }

  // Se apaga el LED para indicar que ya se ha terminado la calibración del sensor QTR
  digitalWrite(LED_BUILTIN, LOW);

  // Los pines 3 y 4 se van a utilizar para controlar el motor derecho
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  
  // Los pines 5 y 6 se van a utilizar para controlar el motor izquierdo
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);  
}

void loop() {
  qtra.readCalibrated(sensorValues);

  s2 = sensorValues[0] < 500 ? 0 : 1;
  s3 = sensorValues[1] < 500 ? 0 : 1;
  s4 = sensorValues[2] < 500 ? 0 : 1;
  s5 = sensorValues[3] < 500 ? 0 : 1;
  s6 = sensorValues[4] < 500 ? 0 : 1;

  // Decidir si el robot debe girar a la izquierda
  l = s5 && !s4 && !s3 && !s2 || s6 && !s4 && !s3 && !s2;

  if (l) {
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
    
    digitalWrite(5, LOW);
    digitalWrite(6, HIGH);
  }

  // Decidir si el robot debe ir hacia adelante
  f = s4 || s5 && s2 || s5 && s3 || s6 && s2 || s6 && s3;
  
  if (f) {
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
    digitalWrite(6, LOW);
  }

  // Decidir si el robot debe girar a la derecha
  r = !s6 && !s5 && !s4 && s2 || !s6 && !s5 && !s4 && s3;

  if (r) {
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, LOW);
  }

  if (!l && !f && !r) {
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);    
  }
  
  delay(250);
}
