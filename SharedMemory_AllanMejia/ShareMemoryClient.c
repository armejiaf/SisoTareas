#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct Client{
    int option;
    char User[50];
    char Message[200];
    char serverResponse[100];
};
int main()
{
    int shmid;
    key_t key;
    if ((key = ftok("ShareMemoryServer.c", 'B')) == -1) {
        perror("ftok");
        exit(1);
    }
    if ((shmid = shmget(key, sizeof(struct Client), 0666)) == -1) {
        perror("shmget");
        exit(1);
    } 
    int option =0; 
    struct Client client;
    struct Client *shmdata;
    shmdata = shmat(shmid,(void *)0,0);

    do{  
       printf("------------MENU------------\n");
        printf("1. Agregar nuevo usuario\n");
        printf("2. Mandar nuevo mensaje\n");
        printf("3. Leer Mensajes\n");    
        printf("4. Desconectar Usuario\n");	
        printf("----------------------------\n"); 
	printf("Escoja opcion: ");
        scanf("%d",&option);
	getchar();    
        client.option=option;
        switch(option) {
            case 1 :
                printf("Ingrese nombre de usuario: ");
	        scanf("%50[^\n]", client.User);
	 	getchar();
                break;
            case 2 :
                printf("Ingrese usuario a cual enviar mensaje: " );
                scanf("%50[^\n]", client.User);
		getchar();
		printf("Ingrese mensaje: " );
		scanf("%200[^\n]", client.Message);
		getchar();
                break;
            case 3 :
                printf("Ingrese usuario para mostrar sus menajes: " );
                scanf("%50[^\n]", client.User);
		getchar();
                break;
            case 4:
                printf("Ingrese usuario que se va desconectar: " );
                scanf("%50[^\n]", client.User);
		getchar();
                break;
        }
        memcpy(&shmdata[0], &client, sizeof(struct Client));
        while(shmdata[0].option!=0);
        printf("%s\n",shmdata[0].serverResponse);
    }while(option<5 && option > 0);
    return 0;
}
