import network
import time
import json
from umqtt.simple import MQTTClient
import machine
import random

# ⚙️ Paramètres WiFi
SSID = "TON_SSID"
PASSWORD = "TON_PASSWORD"

# ⚙️ Broker MQTT (EMQX public)
MQTT_SERVER = "broker.emqx.io"
MQTT_PORT = 1883
MQTT_TOPIC = b"moubine/energie"

# ⚙️ Connexion WiFi
def connect_wifi():
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    wlan.connect(SSID, PASSWORD)
    print("Connexion WiFi...")
    while not wlan.isconnected():
        time.sleep(0.5)
    print("✅ WiFi connecté :", wlan.ifconfig())

# ⚙️ Connexion MQTT
def connect_mqtt():
    client = MQTTClient("ESP32Client", MQTT_SERVER, port=MQTT_PORT)
    client.connect()
    print("✅ Connecté au broker MQTT")
    return client

# ⚙️ Programme principal
def main():
    connect_wifi()
    client = connect_mqtt()

    while True:
        # ⚡ Simuler des valeurs (remplace par tes capteurs)
        solaire = random.randint(50, 200)       # W
        eolienne = random.randint(20, 150)      # W
        temperature = random.randint(20, 35)    # °C
        consommation = random.randint(100, 300) # W
        etat = random.randint(0, 100)           # %
        percent = random.randint(0, 100)        # %

        # ⚙️ Créer le JSON
        data = {
            "solaire": solaire,
            "eolienne": eolienne,
            "temperature": temperature,
            "consommation": consommation,
            "etat": etat,
            "percent": percent
        }
        payload = json.dumps(data)

        # 📤 Publier sur le topic
        client.publish(MQTT_TOPIC, payload)
        print("Données envoyées :", payload)

        time.sleep(5)  # toutes les 5 secondes

# 🚀 Lancer
main()
