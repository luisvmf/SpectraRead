#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <sys/mount.h>
#include <sys/syscall.h>
#include <sys/resource.h>
#include <grp.h>

static uid_t euid,ruid;
static gid_t egid,rgid;

int main(int argc, char **argv, char ** envp){
						//Dangerous part start
						rgid=getgid();
						egid=getegid();
						ruid=getuid();
						euid=geteuid();
						int pid = getpid();
						int retnice=setpriority((int)PRIO_PROCESS, pid, (int)-15);
						if(retnice<0){
							printf("Error setting nice: %s.\n",strerror(errno));
							//exit(-1);
						}
						printf("uid=%i\ngid=%i/neuid=%i/negid=%i\n",getuid(),getgid(),geteuid(),getegid());
						gid_t groups[]={egid};
						setgroups(1,groups); /* Add permission to control devices owned by egid group */
						if(setgid(rgid)<0){ /* Drop real root privileges */
							fprintf(stderr, "Danger!!! Couldn't drop real root privileges. Terminating.\n");
							exit(-1);
						}
						if(setegid(rgid)<0){ /* Drop effetive root privileges (can't gain root privileges again after this line) */
							fprintf(stderr, "Danger!!! Couldn't drop effetive root privileges. Terminating.\n");
							exit(-1);
						}
						if(setuid(ruid)<0){ /* Drop real root privileges */
							fprintf(stderr, "Danger!!! Couldn't drop real root privileges. Terminating.\n");
							exit(-1);
						}
						if(seteuid(ruid)<0){ /* Drop effetive root privileges (can't gain root privileges again after this line) */
							fprintf(stderr, "Danger!!! Couldn't drop effetive root privileges. Terminating.\n");
							exit(-1);
						}
						if(getgid()==0){
								fprintf(stderr, "Danger!!! Running as root. Terminating.\n");
								exit(-1);
						}
						if(getegid()==0){
								fprintf(stderr, "Danger!!! Running as root. Terminating.\n");
								exit(-1);
						}
						if(getuid()==0){
								fprintf(stderr, "Danger!!! Running as root. Terminating.\n");
								exit(-1);
						}
						if(geteuid()==0){
								fprintf(stderr, "Danger!!! Running as root. Terminating.\n");
								exit(-1);
						}
						printf("uid=%i\ngid=%i/neuid=%i/negid=%i\n",getuid(),getgid(),geteuid(),getegid());
						//Dangerous part end.
	//HERE STARTS THE MEDIUM DANGER PART FOR PROCESS 1, WE CAN'T GAIN ROOT PRIVILEGES AGAIN AFTER THIS LINE,
	//BUT WE ARE RUNING WITH egid GROUP, SO WE CAN DIRECTLY TALK TO THE SPECTROMETER.
	//------------------------------------------------
	//------------------------------------------------
	if(argv[1][0]=='0'){
		execl("/usr/lib/luisvmf.com-spectraread/nstest","/usr/lib/luisvmf.com-spectraread/nstest", "/usr/lib/luisvmf.com-Runtimelibs/Runtime","1","1","1","1", "/opt/SpectraRead/spectraread", NULL);
	}
	if(argv[1][0]=='1'){
		execl("/usr/lib/luisvmf.com-spectraread/enterns","/usr/lib/luisvmf.com-spectraread/enterns",argv[2], argv[3], argv[4], argv[5], argv[6], NULL);		
	}

}
