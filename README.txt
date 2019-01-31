COMPILAR PROGRAMA CON OMP:
gcc -fopenmp primsSTPserial.cpp -o primsSTPSerial
CORRER PROGRAMA:
primsSTPSerial

CONFIGURAR DEVC PARA CORRER OPENMP
Tools > Compiler Options > Check the option "Add the following commands when compiler is called" > in the text area put "-fopenmp"
https://stackoverflow.com/questions/8032654/openmp-on-dev-c