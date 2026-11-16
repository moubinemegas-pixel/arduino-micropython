import network
import time

ssid = "VotreNomDeRéseau"
password = "VotreMotDePasse"

def connect_wifi():
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    wlan.connect(ssid, password)

    print("Connexion au réseau...")
    while not wlan.isconnected():
        time.sleep(1)
        print("Connexion en cours...")
    print("Connecté !")
    print("Adresse IP :", wlan.ifconfig()[0])

connect_wifi()
