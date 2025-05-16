#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>

static const char* socket_path = "/dev/shm/notify.luisvmf.com";
static const unsigned int s_recv_len = 200;
static const unsigned int s_send_len = 100;

int main(int argc, char *argv[]){
	int sock = 0;
	int data_len = 0;
	struct sockaddr_un remote;
	char recv_msg[s_recv_len];
	char send_msg[s_send_len];
	memset(recv_msg, 0, s_recv_len*sizeof(char));
	memset(send_msg, 0, s_send_len*sizeof(char));
	if( (sock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1  ){
		printf("Client: Error on socket() call \n");
		return 1;
	}
	remote.sun_family = AF_UNIX;
	strcpy( remote.sun_path, socket_path );
	data_len = strlen(remote.sun_path) + sizeof(remote.sun_family);
	if( connect(sock, (struct sockaddr*)&remote, data_len) == -1 )
	{
		printf("Client: Error on connect() call \n");
		return 1;
	}
		if(argc>1){
		}else{
			return -1;
		}
		char *dest=malloc(9000);
		dest[0]='\0';
		int i=1;
		while(i<argc){
			strcat(dest, argv[i]);
			strcat(dest, " ");
			i=i+1;
		}
		printf("%s",dest);
		if( send(sock, dest, strlen(dest)*sizeof(char), 0 ) == -1 )
		{
			printf("Client: Error on send() call \n");
		}
	return 0;
}
