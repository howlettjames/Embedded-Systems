#!/bin/bash

function operSuma
{
    let suma=$1+$2
    echo "La suma de $1 + $2 es $suma"
}

function operResta
{
    let resta=$1-$2
    echo "La resta de $1 - $2 es $resta"
}

echo "1. Suma"
echo "2. Resta"
echo "Dame la opcion: "
read opcion

    
case $opcion in
    1)
    echo "Dame el numero 1: "
    read n1
    echo "Dame el numero 2: "
    read n2
    operSuma $n1 $n2
    ;;
    2)
    echo "Dame el numero 1: "
    read n1
    echo "Dame el numero 2: "
    read n2
    operResta $n1 $n2   
    ;;
    *)
    echo "Opcion incorrecta[1-2]"
    ;;
esac