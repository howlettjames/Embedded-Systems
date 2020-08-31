#!/bin/bash

arreglo=("12" "17" "21" "45" "89")

echo "El elemento 1 del areglo es: ${arreglo[0]}"
echo "El elemento 3 del areglo es: ${arreglo[3]}"
echo "Todos los elementos del arreglo son: ${arreglo[*]}"
echo "El número elementos del arreglo son: ${#arreglo[*]}"

exit 0