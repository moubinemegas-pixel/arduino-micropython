from machine import Pin, I2C
import time
import onewire, ds18x20
from i2c_lcd import I2cLcd   # Assure-toi d’avoir copié i2c_lcd.py sur l’ESP32

# Initialisation I2C pour LCD (SDA=GPIO21, SCL=GPIO22)
i2c = I2C(0, scl=Pin(22), sda=Pin(21), freq=400000)
lcd = I2cLcd(i2c, 0x27, 4, 20)   # Adresse I2C du LCD (souvent 0x27 ou 0x3F)

# Initialisation capteur DS18B20 (DATA sur GPIO4)
datapin = Pin(4)
ds = ds18x20.DS18X20(onewire.OneWire(datapin))
roms = ds.scan()

# Boucle principale
while True:
    ds.convert_temp()
    time.sleep_ms(750)   # Temps de conversion du DS18B20
    temp = ds.read_temp(roms[0])   # Lecture température
    lcd.clear()
    lcd.putstr("Temp: {:.2f} C".format(temp))  # Affichage sur LCD
    time.sleep(2)   # Attente avant la prochaine mesure