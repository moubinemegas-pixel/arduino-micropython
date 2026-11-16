#include <HardwareSerial.h>

#define RX_PIN 16
#define TX_PIN 17

// ⭐ CRÉER UNE NOUVELLE INSTANCE UART2
HardwareSerial SerialModbus(UART_NUM_2);

void setup() {
  Serial.begin(115200);  // Pour USB (debug)
  delay(2000);
  
  // Initialiser UART2 pour Modbus
  SerialModbus.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  
  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║  CORRECT UART2 CONFIG                 ║");
  Serial.println("║  RX: GPIO16  TX: GPIO17               ║");
  Serial.println("║  Baud: 9600                           ║");
  Serial.println("╚════════════════════════════════════════╝\n");
}

void loop() {
  Serial.println("Envoi requête...");
  
  uint8_t request[8] = {0x09, 0x03, 0x10, 0x00, 0x00, 0x3D, 0x81, 0x6C};
  
  // Envoyer sur UART2 (pas Serial!)
  SerialModbus.write(request, 8);
  SerialModbus.flush();
  
  delay(200);
  
  int count = 0;
  while (SerialModbus.available() && count < 150) {
    uint8_t byte = SerialModbus.read();
    Serial.print("0x");
    if (byte < 0x10) Serial.print("0");
    Serial.print(byte, HEX);
    Serial.print(" ");
    count++;
  }
  
  if (count > 0) {
    Serial.print("\n✅ Reçu ");
    Serial.print(count);
    Serial.println(" octets!\n");
    
    // Si ce n'est pas un echo
    if (count > 8 || byte[0] != 0x09) {
      Serial.println("✅ C'EST UNE VRAIE RÉPONSE!\n");
    }
  } else {
    Serial.println("\n❌ Aucune réponse\n");
  }
  
  delay(3000);
}