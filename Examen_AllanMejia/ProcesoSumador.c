#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#define SHM_SIZE 1024
int sumarArreglo(int *arreglo, int inicio, int fin);

int main(int argc, char *args[]){

	int tam = atoi(args[0]);
	int inicio = atoi(args[1]);
	int fin = atoi(args[2]);
	key_t key;
    	int shmid;
    	int *arreglo;

    	if ((key = ftok("ProcesoSumador.c", 'B')) == -1) {
        	perror("ftok");
        	exit(1);
    	}

	shmid = shmget(key,SHM_SIZE, IPC_EXCL);
	arreglo = shmat(shmid, (void *)0, 0);

	int respuesta = sumarArreglo(arreglo, inicio, fin);

	printf("Suma del Proceso %d: %d\n", getpid(),respuesta);
	return respuesta;
}

int sumarArreglo(int *arreglo, int inicio, int fin){
	int suma = 0;
	for(int i = inicio; i<fin; i++){
		suma += arreglo[i];
	}
	return suma;
}
