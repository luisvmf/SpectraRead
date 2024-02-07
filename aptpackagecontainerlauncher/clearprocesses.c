#include "fastmmapmq.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main(void){
	int ntry=10;
	int mapid=fastmmapmq_connectmmap("spectrareadd","pfgs");
	while(mapid==-1){
		mapid=fastmmapmq_connectmmap("spectrareadd","pfgs");
		sleep(0.1);
		if(ntry<0){
			break;
		}
		ntry=ntry-1;
	}
	ntry=10;
	while(mapid==-1){
		mapid=fastmmapmq_connectmmap("qimon","qimonpeaks");
		sleep(0.1);
		if(ntry<0){
			break;
		}
		ntry=ntry-1;
	}
	if(mapid==-1){
		printf("Spectraread not running\n");
		system("pkill -f -9 /usr/lib/luisvmf.com-spectraread/nstest");
		system("pkill -f -9 /usr/lib/luisvmf.com-qimon/nstest");
		return 0;
	}
	printf("Spectraread already running\n");
	return 0;
}
