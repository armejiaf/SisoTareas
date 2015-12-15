#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Server{
    int option;
    char User[50];
    char Message[200];
    char serverResponse[100];
};

struct UserInfo{
    int msgCount;
    char User[50];
    char * Messages[200];
};

int main()
{
    struct UserInfo users[3];
    for(int i=0;i<3;i++){
        users[i].msgCount=0;
    }
    
    int shmid;
    key_t key;
    if ((key = ftok("ShareMemoryServer.c", 'B')) == -1) {
        perror("ftok");
        exit(1);
    }
    if ((shmid = shmget(key, sizeof(struct Server),IPC_CREAT | 0666)) == -1) {
        perror("shmget");
        exit(1);
    }
    int userCount=0;
    int position=0;
    int messageSnd;
    struct Server client;
    struct Server *shmdata;
    shmdata = shmat(shmid,(void *)0,0);

    for(;;){
        memcpy(&client, &shmdata[0], sizeof(struct Server));
        if(client.option==1){
            if(userCount > 2){
		strcpy(client.serverResponse,"Usuario no se agrego.\nCantidad de usuarios conectados esta al limite.\n");
	    }else{
		    strcpy(users[position].User,client.User);
		    position++;
		    userCount++;
		    strcpy(client.serverResponse,"Usuario se agrego.\n");
		   
	    }
	    client.option=0;
        }
        if(client.option==2){
	    messageSnd = 0;
            for(int i=0;i<userCount;i++){
                if(strcmp(users[i].User,client.User)==0){
                    users[i].Messages[users[i].msgCount] = (char *)malloc(200);
                    strcpy(users[i].Messages[users[i].msgCount],client.Message);
                    users[i].msgCount=users[i].msgCount+1;
		    messageSnd=1;
                    strcpy(client.serverResponse,"Mensaje Enviado.\n" );
                }
            }
	    if(!messageSnd)
		 strcpy(client.serverResponse,"Usuario no existe.\n" );
            client.option=0;
        }
        if(client.option==3){
	    messageSnd = 0;
            for(int i=0;i<userCount;i++){
                if(strcmp(users[i].User,client.User)==0){
                    char msg[2000];
                    strcpy(msg,"------------MENSAJES------------\n");
                    for(int j=0;j<users[i].msgCount;j++){
			strcat(msg,"-");                        
			strcat(msg,users[i].Messages[j]);
                        strcat(msg,"\n");
                    }
		    strcat(msg,"--------------------------------\n");
		    messageSnd=1;
                    strcpy(client.serverResponse, msg);
                    break;
                }
            }
	     if(!messageSnd)
		 strcpy(client.serverResponse,"Usuario no existe.\n" );
            client.option=0;
        }
	if(client.option == 4){
	    messageSnd = 0;
	    for(int i=0;i<userCount;i++){
                if(strcmp(users[i].User,client.User)==0){
                    users[i].User[0]='\0';
		    for(int i=0;i< users[i].msgCount;i++)
		    	users[i].Messages[i]="";
		    users[i].msgCount = 0;
		    userCount--;
		    position=i;
		    messageSnd=1;
		    strcpy(client.serverResponse,"Usuario se desconecto.\n");
		    break;
                }
            }
	    if(!messageSnd)
		 strcpy(client.serverResponse,"Usuario no existe.\n" );
	    client.option=0;
	}  
        memcpy(&shmdata[0], &client, sizeof(struct Server));
        while(shmdata[0].option == 0);
    }
    return 0;
}
