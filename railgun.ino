#include <ModbusMaster.h>

#define RXD2 16   // RO du MAX485 → RX ESP32
#define TXD2 17   // DI du MAX485 → TX ESP32
#define DE_RE 4   // Broche DE/RE commune → GPIO4 ESP32

ModbusMaster node;

// Variables pour tes mesures
float temperature = 0.0;
float consommation = 0.0;
float etat = 0.0;
float solaire = 0.0;
float eolien = 0.0;
float percent = 0.0;
uint16_t modeCharge = 0;

void preTransmission() {
  digitalWrite(DE_RE, HIGH); // Activer émission
}

void postTransmission() {
  digitalWrite(DE_RE, LOW);  // Repasser en réception
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  pinMode(DE_RE, OUTPUT);
  digitalWrite(DE_RE, LOW); // Par défaut en réception

  node.begin(6, Serial2); // ID Modbus du régulateur
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

  Serial.println("Lecture des registres Modbus du JN-T-MPPT...");
}

void loop() {
  uint8_t result;

  // 🔹 Lecture tension batterie (0x1000)
  result = node.readHoldingRegisters(0x1000, 1);
  if (result == node.ku8MBSuccess) etat = node.getResponseBuffer(0) * 0.1;

  // 🔹 Lecture puissance solaire (0x1007)
  result = node.readHoldingRegisters(0x1007, 1);
  if (result == node.ku8MBSuccess) solaire = node.getResponseBuffer(0) * 1.0;

  // 🔹 Lecture puissance éolienne (0x1008)
  result = node.readHoldingRegisters(0x1008, 1);
  if (result == node.ku8MBSuccess) eolien = node.getResponseBuffer(0) * 1.0;

  // 🔹 Lecture puissance charge (0x1009)
  result = node.readHoldingRegisters(0x1009, 1);
  if (result == node.ku8MBSuccess) consommation = node.getResponseBuffer(0) * 1.0;

  // 🔹 Lecture SOC batterie (0x100A)
  result = node.readHoldingRegisters(0x100A, 1);
  if (result == node.ku8MBSuccess) percent = node.getResponseBuffer(0) * 1.0;

  // 🔹 Lecture mode de charge (0x1026)
  result = node.readHoldingRegisters(0x1026, 1);
  if (result == node.ku8MBSuccess) modeCharge = node.getResponseBuffer(0);

  // Affichage
  Serial.println("---- Données régulateur ----");
  Serial.print("Tension batterie : "); Serial.print(etat); Serial.println(" V");
  Serial.print("Puissance solaire : "); Serial.print(solaire); Serial.println(" W");
  Serial.print("Puissance éolienne : "); Serial.print(eolien); Serial.println(" W");
  Serial.print("Puissance charge : "); Serial.print(consommation); Serial.println(" W");
  Serial.print("SOC batterie : "); Serial.print(percent); Serial.println(" %");
  Serial.print("Mode de charge : ");
  switch (modeCharge) {
    case 0: Serial.println("OFF"); break;
    case 1: Serial.println("MPPT"); break;
    case 2: Serial.println("BOOST"); break;
    case 3: Serial.println("FLOAT"); break;
    default: Serial.println("Inconnu"); break;
  }
  Serial.println("-----------------------------");

  delay(2000);
}
