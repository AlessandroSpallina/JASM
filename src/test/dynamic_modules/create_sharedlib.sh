gcc -fPIC -c module.c
gcc -shared -o module.so module.o
rm module.o
