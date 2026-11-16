#include <WiFi.h>
#include <PubSubClient.h>
#include <ModbusMaster.h>

// --- WiFi ---
const char* ssid = "TonSSID";
const char* password = "TonPassword";

// --- HiveMQ Cloud ---
const char* mqtt_server = "128f659d3e694c8ea95298fc0285627f.s1.eu.hivemq.cloud";
const int   mqtt_port   = 8883; // TLS obligatoire
const char* mqtt_user   = "esp_32_solarwind";
const char* mqtt_pass   = "M10u3b4i5n6e7.";

WiFiClientSecure espClient;
PubSubClient client(espClient);

// --- Modbus ---
ModbusMaster node;

// --- Timers ---
unsigned long last30s = 0;
unsigned long last1h  = 0;

void setup() {
  Serial.begin(115200);

  // WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connecté");

  // TLS (HiveMQ Cloud exige SSL)
  espClient.setInsecure(); // pour test, en production utiliser certificat

  // MQTT
  client.setServer(mqtt_server, mqtt_port);
  reconnect();

  // Modbus init
  node.begin(1, Serial2); // Esclave ID=1
  Serial2.begin(9600, SERIAL_8N1, 16, 17); // RX=16, TX=17
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  unsigned long now = millis();

  if (now - last30s >= 30000) {
    last30s = now;
    readAndPublish30s();
  }

  if (now - last1h >= 3600000) {
    last1h = now;
    readAndPublish1h();
  }
}

void readAndPublish30s() {
  uint8_t result = node.readInputRegisters(0x0000, 5);
  if (result == node.ku8MBSuccess) {
    float consommation = node.getResponseBuffer(0) / 10.0;
    float solaire      = node.getResponseBuffer(1) / 10.0;
    float eolienne     = node.getResponseBuffer(2) / 10.0;
    float tempBatt     = node.getResponseBuffer(3) / 10.0;
    float niveauBatt   = node.getResponseBuffer(4) / 10.0;

    String payload = "{";
    payload += "\"consommation\":" + String(consommation) + ",";
    payload += "\"solaire\":" + String(solaire) + ",";
    payload += "\"eolienne\":" + String(eolienne) + ",";
    payload += "\"temperature_batterie\":" + String(tempBatt) + ",";
    payload += "\"niveau_batterie\":" + String(niveauBatt);
    payload += "}";

    client.publish("solarwind/30s", payload.c_str());
    Serial.println("MQTT publish 30s: " + payload);
  }
}

void readAndPublish1h() {
  uint8_t result = node.readInputRegisters(0x0005, 1);
  if (result == node.ku8MBSuccess) {
    int etatBatt = node.getResponseBuffer(0);

    String payload = "{";
    payload += "\"etat_batterie\":" + String(etatBatt);
    payload += "}";

    client.publish("solarwind/1h", payload.c_str());
    Serial.println("MQTT publish 1h: " + payload);
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connexion MQTT...");
    if (client.connect("ESP32Client", mqtt_user, mqtt_pass)) {
      Serial.println("connecté");
    } else {
      Serial.print("échec, rc=");
      Serial.print(client.state());
      delay(2000);
    }
  }
}
