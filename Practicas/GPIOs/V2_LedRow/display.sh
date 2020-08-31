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
  echo "Values written"
}

#value - Atributo que permite escribir un valor digital al GPIO
function writeToLed
{
  echo $1 > /sys/class/gpio/gpio$2/value
  echo "Value $1 written to BCM$2"
}

function writeToDisplay
{
  case $1 in
    0)
      writeToLed 1 5
      writeToLed 1 6
      writeToLed 1 17
      writeToLed 1 22
      writeToLed 1 23
      writeToLed 1 24
      writeToLed 0 25
      echo "Written 0"   
    ;;
    1)
      writeToLed 0 5
      writeToLed 1 6
      writeToLed 1 17
      writeToLed 0 22
      writeToLed 0 23
      writeToLed 0 24
      writeToLed 0 25
      echo "Written 1"
    ;;
    2)
      writeToLed 1 5
      writeToLed 1 6
      writeToLed 0 17
      writeToLed 1 22
      writeToLed 1 23
      writeToLed 0 24
      writeToLed 1 25
      echo "Written 2"   
    ;;
    3)
      writeToLed 1 5
      writeToLed 1 6
      writeToLed 1 17
      writeToLed 1 22
      writeToLed 0 23
      writeToLed 0 24
      writeToLed 1 25
      echo "Written 3"   
    ;;
    4)
      writeToLed 0 5
      writeToLed 1 6
      writeToLed 1 17
      writeToLed 0 22
      writeToLed 0 23
      writeToLed 1 24
      writeToLed 1 25
      echo "Written 4"   
    ;;
    6)
      writeToLed 1 5
      writeToLed 0 6
      writeToLed 1 17
      writeToLed 1 22
      writeToLed 1 23
      writeToLed 1 24
      writeToLed 1 25
      echo "Written 6"   
    ;;
    *)
      echo "Incorrect option"
    ;;
  esac 
}

#unexport - Revierte el efecto de exportar al espacio de usuario,
function unexportGPIOs
{
  for (( i=0 ; $i<${#arreglo_ios[*]} ; i=$i+1 )) 
  do
    echo ${arreglo_ios[i]} > /sys/class/gpio/unexport
  done
  echo "Unexport done"
}

exportGPIOs
configureAsOut
writeToDisplay 2
sleep 1
writeToDisplay 0
sleep 1
writeToDisplay 1
sleep 1
writeToDisplay 6
sleep 1
writeToDisplay 6
sleep 1
writeToDisplay 3
sleep 1
writeToDisplay 0
sleep 1
writeToDisplay 0
sleep 1
writeToDisplay 2
sleep 1
writeToDisplay 4
sleep 1

unexportGPIOs

exit 0
