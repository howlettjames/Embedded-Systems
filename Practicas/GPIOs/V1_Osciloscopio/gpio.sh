#!/bin/bash

#Este programa configura el BCM_GPIO 17 usando el FHS y un script de shell

#export - Sirve para exportar el control del GPIO desde el kernel al espacio de usuario,
#escribiendo su número al archivo.
echo "17" > /sys/class/gpio/export

#direction - Atributo que configura la direccion de entrada (in) o salida (out) del gpio
echo "out" > /sys/class/gpio/gpio17/direction

#value - Atributo que permite escribir un valor digital al GPIO
while [ 1 ]
do
	echo "1" > /sys/class/gpio/gpio17/value
#	sleep 1
	echo "0" > /sys/class/gpio/gpio17/value
#	sleep 1
done

#unexport - Revierte el efecto de exportar al espacio de usuario,
#escribiendo su número al archivo.
echo "17" > /sys/class/gpio/unexport
exit 0

