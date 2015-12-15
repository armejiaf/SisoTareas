#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <unistd.h>
#define SHM_SIZE 1024
char* IntToCharPointer(int intToConvert);

int main(){

	int tam;
	int numeroProcesos;
	key_t key;
    	int shmid;
    	int *arreglo;
	
    	if ((key = ftok("ProcesoSumador.c", 'B')) == -1) {
        	perror("ftok");
        	exit(1);
    	}
	printf("Ingresar tamaño del arreglo: ");
	scanf("%d", &tam);
	if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1) {
		perror("shmget");
		exit(1);
    	}

	arreglo = shmat(shmid, (void *)0, 0);
	if (arreglo == (int *)(-1)) {
		perror("shmat");
		exit(1);
    	} 
  	printf("Ingrese numeros:\n");
	for(int i = 0; i < tam; i++){	
		scanf("%d", &arreglo[i]);
	}
	printf("Ingresar numero de procesos: ");
	scanf("%d", &numeroProcesos);
	
	if(tam % numeroProcesos != 0){
          printf("El tamaño del arreglo y la cantidad de procesos tienen que ser divisible entre si.\n");
          return -1;
        }
	if(tam < numeroProcesos){
          printf("La cantidad de procesos no puede ser mayor a el tamaño del arreglo.\n");
          return -1;
        }
	int valorMaximo = 0;
	int procesoMayor = -1;
	for(int i = 0; i<numeroProcesos; i++){
		int resultado = 0;
		int inicio = (tam/numeroProcesos)*i;
		int fin = inicio + (tam/numeroProcesos);
		int pid = fork();
		if(pid == 0)
			execl("./ProcesoSumador", IntToCharPointer(tam), IntToCharPointer(inicio), IntToCharPointer(fin), (char *)0);
		else{
			waitpid (pid, &resultado, 0);
			if( WEXITSTATUS(resultado) > valorMaximo){
				procesoMayor = pid;
				valorMaximo = WEXITSTATUS(resultado);
			}
		}
	}

	printf("Proceso de mayor valor: %d\nSuma: %d\n", procesoMayor, valorMaximo);
}

char* IntToCharPointer(int intToConvert){
	char* str = malloc(4);
	sprintf( str, "%d", intToConvert );
	return str;
}
