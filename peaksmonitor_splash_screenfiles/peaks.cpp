#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string> 
#include <time.h>
#include <grp.h>
#include <string>
#include <stdio.h>
#include <sstream>
#include <stdlib.h>
#include <fstream>
#include <sys/resource.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <sys/time.h>
#include <sys/file.h>
#include <errno.h>
#include <fcntl.h>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
static uid_t euid,ruid;
static gid_t egid,rgid;
using namespace std;
/*
Look for unsanitised command line parameters directly passed to a system() call
Look for unsanitised PATH variables directly passed to a system() call
Look for unchecked data being loaded directly into static buffers
Look for unchecked data being loaded directly into a heap buffer
*/
//Part of the code is from http://www.itp.uzh.ch/~dpotter/howto/daemonize and is under public domain.
static void daemonize(void){
    pid_t pid, sid;
    /* already a daemon */
    if ( getppid() == 1 ) return;
    /* Fork off the parent process */
    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    /* If we got a good PID, then we can exit the parent process. */
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }
    /* At this point we are executing as the child process */
    /* Change the file mode mask */
    umask(0);
    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }
    /* Change the current working directory.  This prevents the current
       directory from being locked; hence not being able to remove it. */
    if ((chdir("/")) < 0) {
        exit(EXIT_FAILURE);
    }
    /* Redirect standard files to /dev/null */
    freopen( "/dev/null", "r", stdin);
    freopen( "/dev/null", "w", stdout);
    freopen( "/dev/null", "w", stderr);
}
int search(char *fname, char *str) {
	FILE *fp;
	int line_num = 1;
	int find_result = 0;
	char temp[512];
	if((fp=fopen(fname,"r"))==NULL){
		return 0;
	}
	while(fgets(temp, 512, fp)!=NULL){
		if((strstr(temp, str))!=NULL){
			return 1;
			find_result++;
		}
		line_num++;
	}
	if(fp) {
		fclose(fp);
	}
   	return 0;
}
long getTime(){
	struct timeval ct;
	gettimeofday(&ct, NULL);
	return ct.tv_sec * (int)1e6 + ct.tv_usec;
}
main(int argc, char ** argv, char ** envp){
	//daemonize();
	srand(time(NULL));
	string randomstring;
	stringstream aaa;
	aaa << abs(rand()*rand()*rand()*rand()*2/100000);
	stringstream aaab;
	aaab << abs(getTime()/100000);
	randomstring=aaa.str()+aaab.str();
	int alreadyrunning=1;
	while(alreadyrunning==1){
		alreadyrunning=0;
		DIR *d;
		struct dirent *dir;
		d=opendir("/proc/");
		if(d){
		while((dir=readdir(d))!=NULL){
		int tmpnum = atoi(dir->d_name);
		if(tmpnum==0&&(dir->d_name)[0]!='0'){}else{
			std::string cmdlineuri="/proc/"+(std::string(dir->d_name))+"/cmdline";
			if(search(cmdlineuri.c_str(),randomstring.c_str())){
			if(search(cmdlineuri.c_str(),"\0peaksfindluisvmfcomcmdarggui")){ //The \0 is to check that the string is not contained in the folder were the program is stored, because file names cannot contain the null character.
			if(search(cmdlineuri.c_str(),"\0peaksfindluisvmfcomcmdargguib")==0){ //The \0 is to check that the string is not contained in the folder were the program is stored, because file names cannot contain the null character.
				randomstring=randomstring+"sr";
				alreadyrunning=1;
			}
			}
			}
		}
		}
		closedir(d);
		}
		}
		int pidbsafe=fork();
		if(pidbsafe==0){
			char arg1[900];
			char exepath[900] = {0};
			sprintf( arg1, "/proc/%d/exe", getpid() );
			readlink( arg1, exepath, 1024 );
			string ownPth=string(exepath);
			std::string aux(ownPth);
			int pos = aux.rfind('/');
			std::string path = aux.substr(0,pos+1);
			std::string name = aux.substr(pos+1);
			path=path+"source/";
			int a=chdir(path.c_str());
			//std::string execcommandb=path+"peakss "+randomstring+"";
			//system(execcommandb.c_str()); //Start splash screen.
			execl((path+"peakss").c_str(), (path+"peakss").c_str(), (""+randomstring+"").c_str(), NULL);
		}else{
			char arg1[900];
			char exepath[900] = {0};
			sprintf( arg1, "/proc/%d/exe", getpid() );
			readlink( arg1, exepath, 1024 );
			string ownPth=string(exepath);
			std::string aux(ownPth);
			int pos = aux.rfind('/');
			std::string path = aux.substr(0,pos+1);
			std::string name = aux.substr(pos+1);
			path=path+"source/";
			int a=chdir(path.c_str());
			//std::string execcommand="GI_TYPELIB_PATH=girepository-1.0 LD_LIBRARY_PATH=atlas-base DISPLAY=':0' PYTHONPATH="+path+"Python/lib/python2.7 PYTHONHOME="+path+"Python/ LD_PRELOAD='"+path+"Python/lib/libpython2.7.so.1.0 "+path+"atlas-base/libcblas.so.3.0 "+path+"atlas-base/libatlas.so.3 "+path+"libgfortran/libgfortran.so.3 "+path+"atlas-base/liblapack.so.3 "+path+"atlas-base/libblas.so.3 "+path+"atlas-base/libf77blas.so.3' "+path+"spectrareadd "+path+"peaksmain.js peakscurrentprocid:"+randomstring+"peaksprocid peaksfindluisvmfcomcmdarggui";
			   setenv("GI_TYPELIB_PATH", "girepository-1.0", 1);
			   setenv("LD_LIBRARY_PATH", "atlas-base", 1);
			   setenv("PYTHONPATH", (path+"Python/lib/python2.7").c_str(), 1);
			   setenv("PYTHONHOME", (path+"Python/").c_str(), 1);
			   setenv("LD_PRELOAD", (path+"Python/lib/libpython2.7.so.1.0 "+path+"atlas-base/libcblas.so.3.0 "+path+"atlas-base/libatlas.so.3 "+path+"libgfortran/libgfortran.so.3 "+path+"atlas-base/liblapack.so.3 "+path+"atlas-base/libblas.so.3 "+path+"atlas-base/libf77blas.so.3").c_str(), 1);
				execl((path+"spectrareadd").c_str(), (path+"spectrareadd").c_str(),(path+"peaksmain.js").c_str(), ("peakscurrentprocid:"+randomstring+"peaksprocid").c_str(), "peaksfindluisvmfcomcmdarggui", NULL);
			//system(execcommand.c_str()); //Start splash screen.	

		}
	//------------------------------------------------
	//------------------------------------------------
	//Safe part end
	//------------------------------------------------
	//------------------------------------------------
}

