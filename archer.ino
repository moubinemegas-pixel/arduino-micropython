#include <HardwareSerial.h>

#define RX_PIN 9
#define TX_PIN 10

HardwareSerial SerialModbus(UART_NUM_1);

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  SerialModbus.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  
  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║  TESTING UART1                        ║");
  Serial.println("║  RX: GPIO9   TX: GPIO10               ║");
  Serial.println("║  Baud: 9600                           ║");
  Serial.println("╚════════════════════════════════════════╝\n");
}

void loop() {
  Serial.println("Envoi requête...");
  
  uint8_t request[8] = {0x09, 0x03, 0x10, 0x00, 0x00, 0x3D, 0x81, 0x6C};
  
  SerialModbus.write(request, 8);
  SerialModbus.flush();
  
  delay(200);
  
  uint8_t buffer[150];
  int count = 0;
  
  while (SerialModbus.available() && count < 150) {
    buffer[count] = SerialModbus.read();
    Serial.print("0x");
    if (buffer[count] < 0x10) Serial.print("0");
    Serial.print(buffer[count], HEX);
    Serial.print(" ");
    count++;
  }
  
  if (count > 0) {
    Serial.print("\n✅ Reçu ");
    Serial.print(count);
    Serial.println(" octets!\n");
  } else {
    Serial.println("\n❌ Aucune réponse\n");
  }
  
  delay(3000);
}