#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h> 

/*struct my_msgbuf {
    long mtype;
    char mtext[250];
};*/
char message[250];
char Messages[2000];
int main(void)
{
    char serverResponse[1]; 
    char clientM[1];
    int msqid;    
    key_t key; 
    int messagelen;
    int option;	
 	
    if ((key = ftok("MessageQueueClient.c", 'B')) == -1) {    
        perror("ftok");    
        exit(1);    
    }
 
    if ((msqid = msgget(key, 0666)) == -1) {    
        perror("msgget");
        exit(1);
    }
    int len;
    do {  
        printf("------------MENU------------\n");
        printf("1. Agregar nuevo usuario\n");
        printf("2. Mandar nuevo mensaje\n");
        printf("3. Leer Mensajes\n");    
        printf("4. Desconectar Usuario\n");	
        printf("----------------------------\n"); 
	printf("Escoja opcion: ");
        scanf("%d",&option);
	getchar();    
        switch(option){
            case 1:
	
	        printf("Ingrese nombre de usuario: ");
	        scanf("%50[^\n]", message);
		getchar();
		len = strlen(message);
		if (msgsnd(msqid,(void*)message, len, 0) == -1){
                	perror("msgsnd");
			exit(1);
		}
		
		if (msgrcv(msqid, &serverResponse,1, 0, 0) == -1) {
                	perror("msgrcv");
			exit(1);
            	} 
		if(serverResponse[0]=='Y'){
			message[0]='1';
			message[1]='\0';
			if (msgsnd(msqid,(void*)message, 1, 0) == -1){
			       	perror("msgsnd");
				exit(1);
			}
		}
            	
            	if (msgrcv(msqid, &serverResponse,1, 0, 0) == -1) {
                	perror("msgrcv");
			exit(1);
            	}    
            	if(serverResponse[0]=='Y')
                	printf("Usuario se agrego.\n" );
            	else
                	printf("Usuario no se agrego.\nCantidad de usuarios conectados esta al limite.\n" );
            	break;
            case 2:
                printf("Ingrese usuario a cual enviar mensaje: " );
                scanf("%50[^\n]", message);
		getchar();
                len = strlen(message);
                if ( msgsnd( msqid,(void*)message, len, 0 ) == -1 ){
                    perror("msgsnd");
		    exit(1);
		}
		if (msgrcv(msqid, &serverResponse,1, 0, 0) == -1) {
                	perror("msgrcv");
			exit(1);
            	} 
		if(serverResponse[0]=='Y'){
			message[0]='2';
			message[1]='\0';
			if (msgsnd(msqid,(void*)message, 1, 0) == -1){
			       	perror("msgsnd");
				exit(1);
			}
		}

		if ( msgrcv( msqid, &serverResponse,1, 0, 0 ) == -1 ){
                    perror("msgrcv"); 
		    exit(1);
		}
		if( serverResponse[0] == 'Y'){
                	printf("Ingrese mensaje: " );
			scanf("%200[^\n]", message);
			getchar();
			len = strlen(message);
			if ( msgsnd( msqid,(void*)message, len, 0 ) == -1 ){
                    		perror("msgsnd");
				exit(1);
			}
			if ( msgrcv( msqid, &serverResponse,1, 0, 0 ) == -1 ){
                    		perror("msgrcv");      
				exit(1);
			}
		        if(serverResponse[0]=='Y'){
		            printf("Mensaje Enviado.\n" );
		        }else{
		            printf("Usuario no existe.\n");
		        }
 		} 
                break;  
 	    case 3:
                printf("Ingrese usuario para mostrar sus menajes: " );
                scanf("%50[^\n]", message);
		getchar();
                len = strlen(message);
                if (msgsnd(msqid,(void*)message, len, 0) == -1){  
                    perror("msgsnd");
		    exit(1);
		}
		
		if (msgrcv(msqid, &serverResponse,1, 0, 0) == -1) {
                	perror("msgrcv");
			exit(1);
            	} 
		if(serverResponse[0]=='Y'){
			message[0]='3';
			message[1]='\0';
			if (msgsnd(msqid,(void*)message, 1, 0) == -1){
			       	perror("msgsnd");
				exit(1);
			}
		}


                if (msgrcv(msqid, &serverResponse,1, 0, 0) == -1){
                    perror("msgrcv");
		    exit(1);
		}
 
                if(serverResponse[0]=='Y'){
                    message[0]='Y';
		    message[1]='\0';
		    if (msgsnd(msqid,(void*)message, 1, 0) == -1){
		       	perror("msgsnd");
			exit(1);
		    }
                		
                    if (msgrcv(msqid,&Messages,sizeof Messages, 0, 0) == -1){
                        perror("msgrcv");
			exit(1);
		    }
		   
                    printf("------------MENSAJES------------\n");    
                    char *msg;
                    msg = strtok(Messages,",");
 
                    while( msg != NULL ){
                        printf("-%s\n",msg);
                        msg = strtok(NULL,",");
                    }  
		     printf("------------------------------\n");  
                }else
                    printf("Usuario no exsite.\n" );
                break;  
            
            case 4:
                printf("Ingrese usuario que se va desconectar: " );
                scanf("%50[^\n]", message);
		getchar();
                len = strlen(message);
                if ( msgsnd( msqid,(void*)message, len, 0 ) == -1 ){
                    perror("msgsnd");
		    exit(1);
		}
		if (msgrcv(msqid, &serverResponse,1, 0, 0) == -1) {
                	perror("msgrcv");
			exit(1);
            	} 
		if(serverResponse[0]=='Y'){
			message[0]='4';
			message[1]='\0';
			if (msgsnd(msqid,(void*)message, 1, 0) == -1){
			       	perror("msgsnd");
				exit(1);
			}
			if (msgrcv(msqid, &serverResponse,1, 0, 0) == -1) {
		        	perror("msgrcv");
				exit(1);
            		} 
			if(serverResponse[0]=='Y')
				printf("Usuario se desconecto.\n");
			else
				printf("Usuario no existe.\n");
		} 
                break;
		
 	}
    }while(option < 5 && option > 0);   
    return 0;
}

