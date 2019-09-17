#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#define MAX_SIZE 1024

void serviceClient(int);

int main(int argc, char *argv[]){  
  int sd, client, portNumber, status;
  struct sockaddr_in servAdd;      // client socket address
 
 if(argc != 2){
    printf("Call model: %s <Port Number>\n", argv[0]);
    exit(0);
  }
  if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    fprintf(stderr, "Cannot create socket\n");
    exit(1);
  }
  sd = socket(AF_INET, SOCK_STREAM, 0);
  servAdd.sin_family = AF_INET;
  servAdd.sin_addr.s_addr = htonl(INADDR_ANY);
  sscanf(argv[1], "%d", &portNumber);
  servAdd.sin_port = htons((uint16_t)portNumber);
 
  bind(sd, (struct sockaddr *) &servAdd, sizeof(servAdd));
  listen(sd, 5);

  while(1){
    printf("Waiting for a client to chat with\n");
    client = accept(sd, NULL, NULL);
    printf("Got a client, start chatting\n");
 
    if(!fork())
      serviceClient(client);

    close(client);
    waitpid(0, &status, WNOHANG);
  }
}

void serviceClient(int sd)
{
char message[255];
int m,n, pid,size,status;
char *token,*copy;
char delim[40],*command[40],*cmdargs[40];
write(sd, "start chatting", 16);
pid=fork();
if(!pid)
{
	while(1)
	{
		int count=0;
		if(n=read(sd, message, 255))
		{
	     		message[n]='\0';
			fprintf(stderr,"%s", message);
			copy=strdup(message);
			token=strtok(message,";&|\n");
			while(token!=NULL)
			{
				delim[count]=copy[token - message + strlen(token)];
				command[count]=token;
				count++;
				token=strtok(NULL,";&|\n");
			}
			command[count]=NULL;
			for(int k=0;k<count;k++)
			{
				if(status > -1)
				{
					if(delim[k-1]=='&')
					{
						if(status!=0)
							break;
					}
					else if(delim[k-1]=='|')
					{
						if(status==0)
							break;
					}
				}
				token=strtok(command[k]," ");
				int total=0;
				while(token!=NULL)
				{
					cmdargs[total]=token;
					//printf("%s\n",cmdargs[total]);
					total++;
					token=strtok(NULL," ");
				}
				cmdargs[total]=NULL;
				int pr_id=fork();
				if(!pr_id)
				{
					dup2(sd,1);
					execvp(cmdargs[0],cmdargs);
					fprintf(stderr,"Command not recognised\n");
					exit(1);
				}
				else
				{
					m=wait(&status);
					if(WIFEXITED(status))
						printf("Child %d exited with status %d\n",pr_id,WEXITSTATUS(status));
				}
	

			}
		}
	}

}	 

if(pid)
 /*sending messages to the client */
	while(1)

		if(n=read(0, message, 255))
		{
			message[n]='\0';
			write(sd, message, strlen(message)+1);
		}

}

