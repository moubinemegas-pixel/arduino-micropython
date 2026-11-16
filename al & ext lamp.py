from machine import Pin
import time
p=Pin(2,Pin.OUT)
while True :
    p.on()
    time.sleep_ms(1000)
    p.off()
    time.sleep_ms(1000)