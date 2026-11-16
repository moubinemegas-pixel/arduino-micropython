import network
import urequests
import time

# Configuration WiFi
ssid = "VotreNomDeRéseau"
password = "VotreMotDePasse"

# URL Firebase
firebase_url = "https://votre-projet.firebaseio.com/donnees.json"

def connect_wifi():
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    wlan.connect(ssid, password)
    while not wlan.isconnected():
        time.sleep(1)
        print("Connexion en cours...")
    print("Connecté au WiFi :", wlan.ifconfig())

def envoyer_donnees(data):
    try:
        response = urequests.post(firebase_url, json=data)
        print("Données envoyées :", response.text)
        response.close()
    except Exception as e:
        print("Erreur :", e)

connect_wifi()

# Exemple de données de capteur
while True:
    temperature = 25.5  # Remplacez par la lecture réelle de votre capteur
    humidite = 60.2     # Remplacez par la lecture réelle de votre capteur
    data = {"temperature": temperature, "humidite": humidite}
    envoyer_donnees(data)
    time.sleep(10)  # Envoi toutes les 10 secondes
