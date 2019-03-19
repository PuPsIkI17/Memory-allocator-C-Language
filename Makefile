#Variabile
CC = gcc
Cflags = -Wall -lm
ts1 = main2.c
ts = alocator
 
#Compilare
build:
	$(CC) $(ts1) -o $(ts) $(Cflags)
#Stergere
clean :
	rm -f $(ts)
