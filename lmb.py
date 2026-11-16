from machine import ADC, Pin

# Choisir une broche ADC, par ex. GPIO32
adc = ADC(Pin(32))
adc.atten(ADC.ATTN_11DB)   # permet de lire jusqu'à ~3.3V

valeur = adc.read()        # lecture brute (0 à 4095)
print("Valeur ADC =", valeur)
