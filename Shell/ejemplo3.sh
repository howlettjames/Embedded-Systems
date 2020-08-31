#!/bin/bash

cadena="Hola mundo"
arreglo=(12 17 21 45 89)

function sumaAcc1
{   
    let suma=0
    for i in 0 1 2 3 4
    do
        let suma=suma+arreglo[i]
    done
    echo "La suma acumulada es: $suma"
}

function sumaAcc2
{   
    let suma=0
    for i in ${arreglo[*]}
    do
        let suma=suma+i
    done
    echo "La suma acumulada es: $suma"
}

function sumaAcc3
{   
    let suma=0
    for (( i=0 ; $i<${#arreglo[*]} ; i=$i+1 )) 
    do
        let suma=suma+arreglo[i]
    done
    echo "La suma acumulada3 es: $suma"
}

echo  "La cadena es: $cadena"
echo "El elemento 1 del areglo es: ${arreglo[0]}"
echo "El elemento 3 del areglo es: ${arreglo[3]}"
echo "Todos los elementos del arreglo son: ${arreglo[*]}"
echo "El número elementos del arreglo son: ${#arreglo[*]}"

sumaAcc1
sumaAcc2
sumaAcc3

exit 0
