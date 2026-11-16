from machine import Pin, 12C
import time
import onewire, ds18x20
from i2c_lcd import I2cLcd
lcd = I2cLcd(i2c, 0x27, 4, 20)
# Initialisation I2C pour LCD (SDA=GPIO21, SCL=GPIO22)
i2c = I2C(0, scl=Pin(22), sda=Pin(21), freq=400000)
# Adresse I2C du LCD (souvent 0x27 ou 0x3F)

# Initialisation capteur DS18B20 (DATA sur GPIO4)
datapin = Pin(4)
ds = ds18x20.DS18X20(onewire.OneWire(datapin))
roms = ds. scan()
if not roms:
lcd.putstr(" Aucun capteur DS18B20 detecte. Verifie cablage et resistance.")

# Boucle principale
while True:
    ds.convert_temp()
    time.sleep_ms(750)
    temp = ds.read_temp(roms[0])
    lcd.clear()
    lcd.putstr("Temp: { :. 2f} C".format(temp)) # Affichage sur LCD
    time.sleep(2)

# Assure-toi d'avoir copie i2c_lcd.py sur l'ESP32


# Temps de conversion du DS18B20
# Lecture température