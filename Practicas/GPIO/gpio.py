#Este programa configura el BCM_GPIO 17 usando phyton
#El BCM_GPIO 17 corresponde con la terminal 11 del conector
#de 40 terminales de la raspberry

import RPi.GPIO as GPIO
import time

#Se inicializa los GPIOS
GPIO.setmode(GPIO.BOARD)
#Se configura el BCM_GPIO 17 como salida usando la terminal 11
GPIO.setup(11, GPIO.OUT)

#Se escribe un valor digital al BCM_GPIO 17
while(1):
	GPIO.output(11,GPIO.HIGH)
#	time.sleep(1)
	GPIO.output(11,GPIO.LOW)
#	time.sleep(1)
