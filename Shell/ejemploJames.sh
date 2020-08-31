#!/bin/bash

arreglo_ios=("5" "6" "17" "22" "23" "24" "25" "27")

function exportGPIOs
{
	for (( i=0 ; $i<${#arreglo_ios[*]} ; i=$i+1 )) 
  do
    echo ${arreglo_ios[i]} > /sys/class/gpio/export
  done
  echo "Export done"
}

function configureAsOut
{
	for (( i=0 ; $i<${#arreglo_ios[*]} ; i=$i+1 )) 
  do
    echo "out" > /sys/class/gpio/gpio"${arreglo_ios[i]}"/direction
  done
  echo "Configured as outputs"
}

#value - Atributo que permite escribir un valor digital al GPIO
function writeToLeds
{
  for (( i=0 ; $i<${#arreglo_ios[*]} ; i=$i+1 )) 
  do
    echo "1" > /sys/class/gpio/gpio${arreglo_ios[i]}/value
    sleep 1
    echo "0" > /sys/class/gpio/gpio${arreglo_ios[i]}/value
    #sleep 1
  done
  echo "Configured as outputs"
}

#unexport - Revierte el efecto de exportar al espacio de usuario,
function unexportGPIOs
{
  for (( i=0 ; $i<${#arreglo_ios[*]} ; i=$i+1 )) 
  do
    echo ${arreglo_ios[i]} > /sys/class/gpio/unexport
  done
  echo "Export done"
}

exportGPIOs
configureAsOut
writeToLeds
unexportGPIOs

exit 0
