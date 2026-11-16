#include <WiFi.h>
#include <PubSubClient.h>

// ⚙️ Paramètres WiFi
const char* ssid = "TON_SSID";
const char* password = "TON_PASSWORD";

// ⚙️ Broker MQTT (EMQX public)
const char* mqtt_server = "broker.emqx.io";
const int mqtt_port = 1883; // port TCP classique
const char* mqtt_topic = "moubine/energie";

// ⚙️ Client WiFi + MQTT
WiFiClient espClient;
PubSubClient client(espClient);

// Fonction de connexion WiFi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connexion à ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("✅ WiFi connecté");
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());
}

// Fonction de reconnexion MQTT
void reconnect() {
  while (!client.connected()) {
    Serial.print("Connexion au broker MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("✅ Connecté");
    } else {
      Serial.print("Échec, rc=");
      Serial.print(client.state());
      Serial.println(" → nouvel essai dans 5s");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // ⚡ Simuler des valeurs (remplace par tes capteurs)
  int solaire = random(50, 200);       // W
  int eolienne = random(20, 150);      // W
  int temperature = random(20, 35);    // °C
  int consommation = random(100, 300); // W
  int etat = random(0, 100);           // %
  int percent = random(0, 100);        // %

  // ⚙️ Créer le JSON
  String payload = "{";
  payload += "\"solaire\":" + String(solaire) + ",";
  payload += "\"eolienne\":" + String(eolienne) + ",";
  payload += "\"temperature\":" + String(temperature) + ",";
  payload += "\"consommation\":" + String(consommation) + ",";
  payload += "\"etat\":" + String(etat) + ",";
  payload += "\"percent\":" + String(percent);
  payload += "}";

  // Publier sur le topic
  client.publish(mqtt_topic, payload.c_str());
  Serial.println("📤 Données envoyées : " + payload);

  delay(5000); // toutes les 5 secondes
}
