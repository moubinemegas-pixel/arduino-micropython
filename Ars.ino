#include <ModbusMaster.h>

// Instance Modbus
ModbusMaster node;

void setup() {
  Serial.begin(9600); // Moniteur série
  Serial2.begin(9600, SERIAL_8N1, 16, 17); 
  // RX=16, TX=17 (ESP32) → adapte selon ton câblage RS485

  node.begin(6, Serial2); 
  // Slave ID = 6 (par défaut sur JNGE)
}

void loop() {
  uint8_t result;
  uint16_t data[2];
  float value;

  // --- Puissance solaire (registre 18-19) ---
  result = node.readHoldingRegisters(18, 2);
  if (result == node.ku8MBSuccess) {
    data[0] = node.getResponseBuffer(0);
    data[1] = node.getResponseBuffer(1);
    uint32_t rawValue = ((uint32_t)data[0] << 16) | data[1];
    Serial.print("Puissance solaire : ");
    Serial.print(rawValue);
    Serial.println(" W");
  }

  // --- Puissance éolienne (registre 22-23) ---
  result = node.readHoldingRegisters(22, 2);
  if (result == node.ku8MBSuccess) {
    data[0] = node.getResponseBuffer(0);
    data[1] = node.getResponseBuffer(1);
    uint32_t rawValue = ((uint32_t)data[0] << 16) | data[1];
    Serial.print("Puissance éolienne : ");
    Serial.print(rawValue);
    Serial.println(" W");
  }

  // --- Consommation actuelle (registre 60-61) ---
  result = node.readHoldingRegisters(60, 2);
  if (result == node.ku8MBSuccess) {
    data[0] = node.getResponseBuffer(0);
    data[1] = node.getResponseBuffer(1);
    uint32_t rawValue = ((uint32_t)data[0] << 16) | data[1];
    Serial.print("Consommation actuelle : ");
    Serial.print(rawValue);
    Serial.println(" W");
  }

  // --- Température batterie (registre 42) ---
  result = node.readHoldingRegisters(42, 1);
  if (result == node.ku8MBSuccess) {
    value = node.getResponseBuffer(0);
    Serial.print("Température batterie : ");
    Serial.print(value);
    Serial.println(" °C");
  }

  // --- Niveau batterie (SOC %) (registre 40) ---
  result = node.readHoldingRegisters(40, 1);
  if (result == node.ku8MBSuccess) {
    value = node.getResponseBuffer(0);
    Serial.print("Niveau batterie : ");
    Serial.print(value);
    Serial.println(" %");
  }

  delay(3000); // Pause 3s entre chaque lecture
}