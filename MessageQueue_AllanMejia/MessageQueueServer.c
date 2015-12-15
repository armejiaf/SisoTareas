#include <stdio.h>
#include <stdlib.h>
#include <string.h>  
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
 
 
/*struct my_msgbuf {
    int option;
    char mtext[250];
};*/ 
struct my_userbuf {    
    char User[50];
    char *Messages;
};
 char message[250];
int main(void)
{  
    struct my_userbuf users[3];
    //struct my_msgbuf buf;
    int msqid;    
    key_t key;    
    for(int i=0;i<3;i++){
        users[i].Messages="";
    }
 
    if ((key = ftok("MessageQueueClient.c", 'B')) == -1) {  
        perror("ftok");
        exit(1);
    }
 
    if ((msqid = msgget(key, 0666| IPC_CREAT)) == -1) {
        perror("msgget");
        exit(1);
    }
    char userName[50];
    int userCount=0;
    int position=0;
    char serverResponse[1];
    int option;
    int len;
    char * messagesToSend;
    int messageslen;
     /* Server never quits! */
    for(;;) {
       if (msgrcv(msqid, &message, sizeof message, 0, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }
	strcpy(userName,message);
	serverResponse[0]='Y';
	if (msgsnd(msqid,(void*)serverResponse, 1, 0) == -1){
		       	perror("msgsnd");
			exit(1);
	}
	if (msgrcv(msqid, &message,sizeof message, 0, 0) == -1) {
                	perror("msgrcv");
			exit(1);
        } 
	option=atoi(message);
        
	if(option==1){
	    
            if(userCount > 2){
		serverResponse[0]='N';
		if (msgsnd(msqid,(void*)serverResponse, 1, 0) == -1){
		       	perror("msgsnd");
			exit(1);
	    	}
	    }
            else{  
		    strcpy(users[position].User,userName);
		    userCount++;
		    position++;
		    serverResponse[0]='Y';
		    if (msgsnd(msqid,(void*)serverResponse, 1, 0) == -1){
		       perror("msgsnd");
			exit(1);
		    }
	    }
        }
 
        if(option==2){   
            serverResponse[0]='Y';
	    if (msgsnd(msqid,(void*)serverResponse, 1, 0) == -1){
               perror("msgsnd");
		exit(1);
	    }
	    if (msgrcv(msqid, &message,sizeof message, 0, 0) == -1) {
		    perror("msgrcv");
		    exit(1);
            }
	    
	    serverResponse[0]='N';
            char msg[250];
	    strcpy(msg,message);
            int msglen = strlen(msg);
            msg[msglen]=',';
            msg[msglen+1]='\0';
            char* userMessages;
            for(int i=0;i<3;i++){
                if(strcmp(users[i].User,userName)==0){
                    userMessages=malloc(strlen(users[i].Messages)+strlen(msg));
                    strcpy(userMessages,users[i].Messages);
                    strcat(userMessages,msg);
                    users[i].Messages = userMessages;
                    serverResponse[0]='Y';
		    break;
                }
            }
            if (msgsnd(msqid,(void*)serverResponse, 1, 0) == -1){
               perror("msgsnd");
		exit(1);
            }

        }
 
        if(option==3){
            serverResponse[0]='N';
            for(int i=0;i<3;i++){
                if(strcmp(users[i].User,userName)==0){
                    messagesToSend=users[i].Messages;
                    serverResponse[0]='Y';
                }
            }
 	    
            if (msgsnd(msqid,(void*)serverResponse,1, 0) == -1){
                perror("msgsnd");
                exit(1);
	    }

            if(serverResponse[0]=='Y'){
		
		if (msgrcv(msqid, &message,sizeof message, 0, 0) == -1) {
                	perror("msgrcv");
			exit(1);
        	} 

		if(message[0]=='Y'){	
		        messageslen=strlen(messagesToSend);
			
			if (msgsnd(msqid,(void*)messagesToSend,messageslen, 0) == -1){
			    perror("msgsnd");
			    exit(1);
			}
		}
		
            } 
        }

	if(option == 4){
	    serverResponse[0]='N';
	    for(int i=0;i<3;i++){
                if(strcmp(users[i].User,userName)==0){
                    users[i].User[0]='\0';
		    users[i].Messages="";
		    userCount--;
		    position=i;
		     serverResponse[0]='Y';
		    break;
                }
            }
	     
	      if (msgsnd(msqid,(void*)serverResponse,1, 0) == -1){
                perror("msgsnd");
                exit(1);
	      }
	}
    }
 
    return 0;
 

    }

