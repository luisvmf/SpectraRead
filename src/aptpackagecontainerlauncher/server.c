#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static const char* socket_path = "/dev/shm/notify.luisvmf.com";
static const unsigned int nIncomingConnections = 5;


#define BD_NO_CHDIR          01
#define BD_NO_CLOSE_FILES    02 
#define BD_NO_REOPEN_STD_FDS 04 
#define BD_NO_UMASK0        010 
#define BD_MAX_CLOSE       8192 

int become_daemon(int flags){
  int maxfd, fd;
  switch(fork())
  {
    case -1: return -1;
    case 0: break; 
    default: _exit(EXIT_SUCCESS);
  }
  if(setsid() == -1)
    return -1;
  switch(fork())
  {
    case -1: return -1;
    case 0: break;
    default: _exit(EXIT_SUCCESS);
  }
  if(!(flags & BD_NO_UMASK0))
    umask(0);
  if(!(flags & BD_NO_CHDIR))
    chdir("/");
  if(!(flags & BD_NO_CLOSE_FILES))
  {
    maxfd = sysconf(_SC_OPEN_MAX);
    if(maxfd == -1)
      maxfd = BD_MAX_CLOSE;
    for(fd = 0; fd < maxfd; fd++)
      close(fd);
  }
  if(!(flags & BD_NO_REOPEN_STD_FDS))
  {
    close(STDIN_FILENO);
    fd = open("/dev/null", O_RDWR);
    if(fd != STDIN_FILENO)
      return -1;
    if(dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO)
      return -2;
    if(dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO)
      return -3;
  }
  return 0;
}
int main(){
	system("./client quit");
	become_daemon(0);
	int s = 0;
	int s2 = 0;
	struct sockaddr_un local, remote;
	int len = 0;

	s = socket(AF_UNIX, SOCK_STREAM, 0);
	if( -1 == s ){
		printf("Error on socket() call \n");
		return 1;
	}
	local.sun_family = AF_UNIX;
	strcpy( local.sun_path, socket_path );
	unlink(local.sun_path);
	len = strlen(local.sun_path) + sizeof(local.sun_family);
	if( bind(s, (struct sockaddr*)&local, len) != 0)
	{
		printf("Error on binding socket \n");
		return 1;
	}

	if( listen(s, nIncomingConnections) != 0 )
	{
		printf("Error on listen call \n");
	}

	int bWaiting = 1;
	while (bWaiting)
	{
		unsigned int sock_len = 0;
		printf("Waiting for connection.... \n");
		if( (s2 = accept(s, (struct sockaddr*)&remote, &sock_len)) == -1 )
		{
			printf("Error on accept() call \n");
			return 1;
		}

		printf("Server connected \n");

		int data_recv = 0;
		char recv_buf[900];
		char send_buf[950];
		do{
			memset(recv_buf, 0, 900*sizeof(char));
			memset(send_buf, 0, 950*sizeof(char));
			data_recv = recv(s2, recv_buf, 899, 0);
			if(data_recv > 0)
			{

				char *dest=malloc(90+data_recv);
				dest[0]='\0';
				strcat(dest, "notify-send \0");
				strcat(dest, recv_buf);
				if(strstr(recv_buf, "quit")!=0)
				{
					printf("Exit command received -> quitting \n");
					bWaiting = 0;
					break;
				}
				system(dest);
				free(dest);
			}
			else
			{
				printf("Error on recv() call \n");
			}
		}while(data_recv > 0);

		close(s2);
	}
	return 0;
}
