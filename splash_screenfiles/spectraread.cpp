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
static uid_t euid,ruid;
static gid_t egid,rgid;
using namespace std;
/*
Look for unsanitised command line parameters directly passed to a system() call
Look for unsanitised PATH variables directly passed to a system() call
Look for unchecked data being loaded directly into static buffers
Look for unchecked data being loaded directly into a heap buffer
*/
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
				//------------------------------------------------
				//------------------------------------------------
				//------------------------------------------------
				//HERE STARTS THE DANGEROUS PART, WE ARE RUNNING AS ROOT.
				//------------------------------------------------
				//------------------------------------------------
						rgid=getgid();
						egid=getegid();
						ruid=getuid();
						euid=geteuid();
						srand(time(NULL));
						string randomstring;
						stringstream aaa;
						aaa << abs(rand()*rand()*rand()*rand()*2/100000000);
						stringstream aaab;
						aaab << abs(getTime()/1000000);
						randomstring=aaa.str()+aaab.str();
						int pidb=fork();
						if ( pidb==0 ){
						int pid = getpid();
						int retnice=setpriority((int)PRIO_PROCESS, pid, (int)-15);
						if(retnice<0){
							printf("Error setting nice: %s.\n",strerror(errno));
							//exit(-1);
						}
						static gid_t groups[]={egid};
						setgroups(1,groups); /* Add permission to control devices owned by egid group */
						if(setuid(ruid)<0){ /* Drop real root privileges */
							fprintf(stderr, "Danger!!! Couldn't drop real root privileges. Terminating.\n");
							exit(-1);
						}
						if(seteuid(ruid)<0){ /* Drop effetive root privileges (can't gain root privileges again after this line) */
							fprintf(stderr, "Danger!!! Couldn't drop effetive root privileges. Terminating.\n");
							exit(-1);
						}
				//------------------------------------------------
				//------------------------------------------------
				//HERE STOPS THE DANGEROUS PART FOR PROCESS 1, WE CAN'T GAIN ROOT PRIVILEGES AGAIN AFTER THIS LINE.
				//------------------------------------------------
				//------------------------------------------------
	//HERE STARTS THE MEDIUM DANGER PART FOR PROCESS 1, WE CAN'T GAIN ROOT PRIVILEGES AGAIN AFTER THIS LINE,
	//BUT WE ARE RUNING WITH egid GROUP, SO WE CAN DIRECTLY TALK TO THE SPECTROMETER.
	//------------------------------------------------
	//------------------------------------------------
		environ = NULL;/* Clear all enviroment variables to prevent code injection on system call */
		envp = 0; /* blocks IFS attack on non-bash shells */
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
				if(search(cmdlineuri.c_str(),"\0spectrareadcmdargdaemon")){ //The \0 is to check that the string is not contained in the folder were the program is stored, because file names cannot contain the null character.
				if(search(cmdlineuri.c_str(),"\0spectrareadcmdarggui")==0){ //The \0 is to check that the string is not contained in the folder were the program is stored, because file names cannot contain the null character.
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
		char arg1[900];
		char exepath[900] = {0};
		sprintf( arg1, "/proc/%d/exe", getpid() );
		readlink( arg1, exepath, 1024 );
		string ownPth=string(exepath);
		std::string aux(ownPth);
		int pos = aux.rfind('/');
		std::string path = aux.substr(0,pos+1);
		std::string name = aux.substr(pos+1);
		int a=chdir(path.c_str());
		std::string execcommand="GI_TYPELIB_PATH=girepository-1.0 LD_LIBRARY_PATH=atlas-base DISPLAY=':0' PYTHONPATH="+path+"Python/lib/python2.7 PYTHONHOME="+path+"Python/ LD_PRELOAD='"+path+"Python/lib/libpython2.7.so.1.0 "+path+"atlas-base/libcblas.so.3.0 "+path+"atlas-base/libatlas.so.3 "+path+"libgfortran/libgfortran.so.3 "+path+"atlas-base/liblapack.so.3 "+path+"atlas-base/libblas.so.3 "+path+"atlas-base/libf77blas.so.3' "+path+"spectrareadd "+path+"daemon.js spectrareadcurrentprocid:"+randomstring+"spectrareadprocid spectrareadcmdargdaemon";
		system(execcommand.c_str()); //Start device control daemon.
	//HERE STOPS THE MEDIUM DANGER PART FOR PROCESS 1
	//------------------------------------------------
	//------------------------------------------------
	//------------------------------------------------
}else{
				//------------------------------------------------
				//------------------------------------------------
				//------------------------------------------------
				//HERE STARTS THE DANGEROUS PART FOR PROCESS 2, WE ARE RUNNING AS ROOT.
						if(setuid(ruid)<0){ /* Drop real root privileges */
							fprintf(stderr, "Danger!!! Couldn't drop real root privileges. Terminating.\n");
							exit (-1);
						}
						if(seteuid(ruid)<0){ /* Drop effetive root privileges (can't gain root privileges again after this line) */
							fprintf(stderr, "Danger!!! Couldn't drop effetive root privileges. Terminating.\n");
							exit (-1);
						}
				//------------------------------------------------
				//------------------------------------------------
				//HERE STOPS THE DANGEROUS PART FOR PROCESS 2, WE CAN'T GAIN ROOT PRIVILEGES AGAIN AFTER THIS LINE.
				//------------------------------------------------
				//------------------------------------------------
	//HERE WE ARE RUNNING WITH GROUPS OF THE USER WHO CALLED THE PROGRAM, SO WE CANNOT DIRECTLY TALK TO THE SPECTROMETER.
	//------------------------------------------------
	//------------------------------------------------
	//Safe part start
	//------------------------------------------------
	//------------------------------------------------
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
			if(search(cmdlineuri.c_str(),"\0spectrareadcmdarggui")){ //The \0 is to check that the string is not contained in the folder were the program is stored, because file names cannot contain the null character.
			if(search(cmdlineuri.c_str(),"\0spectrareadcmdargdaemon")==0){ //The \0 is to check that the string is not contained in the folder were the program is stored, because file names cannot contain the null character.
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
			int a=chdir(path.c_str());
			std::string execcommandb=path+"spectrareads "+randomstring+"";
			system(execcommandb.c_str()); //Start splash screen.
		}else{
			int pidbsafeb=fork();
			if(pidbsafeb==0){
				char arg1[900];
				char exepath[900] = {0};
				sprintf( arg1, "/proc/%d/exe", getpid() );
				readlink( arg1, exepath, 1024 );
				string ownPth=string(exepath);
				std::string aux(ownPth);
				int pos = aux.rfind('/');
				std::string path = aux.substr(0,pos+1);
				std::string name = aux.substr(pos+1);
				int a=chdir(path.c_str());
				std::string execcommand="GI_TYPELIB_PATH=girepository-1.0 LD_LIBRARY_PATH=atlas-base DISPLAY=':0' PYTHONPATH="+path+"Python/lib/python2.7 PYTHONHOME="+path+"Python/ LD_PRELOAD='"+path+"Python/lib/libpython2.7.so.1.0 "+path+"atlas-base/libcblas.so.3.0 "+path+"atlas-base/libatlas.so.3 "+path+"libgfortran/libgfortran.so.3 "+path+"atlas-base/liblapack.so.3 "+path+"atlas-base/libblas.so.3 "+path+"atlas-base/libf77blas.so.3' "+path+"spectrareadd "+path+"main.js spectrareadcurrentprocid:"+randomstring+"spectrareadprocid spectrareadcmdarggui";
				system(execcommand.c_str()); //Start splash screen.
			}else{
				int pidbsafec=fork();
				if(pidbsafec==0){
					int pidbsafed=fork();
					if(pidbsafed==0){
						int pidbsafee=fork();
						if(pidbsafee==0){
							int pidbsafef=fork();
							if(pidbsafef==0){
								//here
								char arg1[900];
								char exepath[900] = {0};
								sprintf( arg1, "/proc/%d/exe", getpid() );
								readlink( arg1, exepath, 1024 );
								string ownPth=string(exepath);
								std::string aux(ownPth);
								int pos = aux.rfind('/');
								std::string path = aux.substr(0,pos+1);
								std::string name = aux.substr(pos+1);
								int a=chdir(path.c_str());
								std::string execcommand="GI_TYPELIB_PATH=girepository-1.0 LD_LIBRARY_PATH=atlas-base DISPLAY=':0' PYTHONPATH="+path+"Python/lib/python2.7 PYTHONHOME="+path+"Python/ LD_PRELOAD='"+path+"Python/lib/libpython2.7.so.1.0 "+path+"atlas-base/libcblas.so.3.0 "+path+"atlas-base/libatlas.so.3 "+path+"libgfortran/libgfortran.so.3 "+path+"atlas-base/liblapack.so.3 "+path+"atlas-base/libblas.so.3 "+path+"atlas-base/libf77blas.so.3' "+path+"spectrareadd "+path+"peaksworker.js spectrareadcurrentprocid:"+randomstring+"spectrareadprocid spectrareadcmdarggui specreadprocpeaks:[ spec1";
								system(execcommand.c_str()); //Start splash screen.
							}else{
								//here
								char arg1[900];
								char exepath[900] = {0};
								sprintf( arg1, "/proc/%d/exe", getpid() );
								readlink( arg1, exepath, 1024 );
								string ownPth=string(exepath);
								std::string aux(ownPth);
								int pos = aux.rfind('/');
								std::string path = aux.substr(0,pos+1);
								std::string name = aux.substr(pos+1);
								int a=chdir(path.c_str());
								std::string execcommand="GI_TYPELIB_PATH=girepository-1.0 LD_LIBRARY_PATH=atlas-base DISPLAY=':0' PYTHONPATH="+path+"Python/lib/python2.7 PYTHONHOME="+path+"Python/ LD_PRELOAD='"+path+"Python/lib/libpython2.7.so.1.0 "+path+"atlas-base/libcblas.so.3.0 "+path+"atlas-base/libatlas.so.3 "+path+"libgfortran/libgfortran.so.3 "+path+"atlas-base/liblapack.so.3 "+path+"atlas-base/libblas.so.3 "+path+"atlas-base/libf77blas.so.3' "+path+"spectrareadd "+path+"peaksworker.js spectrareadcurrentprocid:"+randomstring+"spectrareadprocid spectrareadcmdarggui specreadprocpeaks:{ spec1";
								system(execcommand.c_str()); //Start splash screen.
							}							
						}else{
							//here
							char arg1[900];
							char exepath[900] = {0};
							sprintf( arg1, "/proc/%d/exe", getpid() );
							readlink( arg1, exepath, 1024 );
							string ownPth=string(exepath);
							std::string aux(ownPth);
							int pos = aux.rfind('/');
							std::string path = aux.substr(0,pos+1);
							std::string name = aux.substr(pos+1);
							int a=chdir(path.c_str());
							std::string execcommand="GI_TYPELIB_PATH=girepository-1.0 LD_LIBRARY_PATH=atlas-base DISPLAY=':0' PYTHONPATH="+path+"Python/lib/python2.7 PYTHONHOME="+path+"Python/ LD_PRELOAD='"+path+"Python/lib/libpython2.7.so.1.0 "+path+"atlas-base/libcblas.so.3.0 "+path+"atlas-base/libatlas.so.3 "+path+"libgfortran/libgfortran.so.3 "+path+"atlas-base/liblapack.so.3 "+path+"atlas-base/libblas.so.3 "+path+"atlas-base/libf77blas.so.3' "+path+"spectrareadd "+path+"peaksworker.js spectrareadcurrentprocid:"+randomstring+"spectrareadprocid spectrareadcmdarggui specreadprocpeaks:par spec1";
							system(execcommand.c_str()); //Start splash screen.
						}
					}else{
						//here
						char arg1[900];
						char exepath[900] = {0};
						sprintf( arg1, "/proc/%d/exe", getpid() );
						readlink( arg1, exepath, 1024 );
						string ownPth=string(exepath);
						std::string aux(ownPth);
						int pos = aux.rfind('/');
						std::string path = aux.substr(0,pos+1);
						std::string name = aux.substr(pos+1);
						int a=chdir(path.c_str());
						std::string execcommand="GI_TYPELIB_PATH=girepository-1.0 LD_LIBRARY_PATH=atlas-base DISPLAY=':0' PYTHONPATH="+path+"Python/lib/python2.7 PYTHONHOME="+path+"Python/ LD_PRELOAD='"+path+"Python/lib/libpython2.7.so.1.0 "+path+"atlas-base/libcblas.so.3.0 "+path+"atlas-base/libatlas.so.3 "+path+"libgfortran/libgfortran.so.3 "+path+"atlas-base/liblapack.so.3 "+path+"atlas-base/libblas.so.3 "+path+"atlas-base/libf77blas.so.3' "+path+"spectrareadd "+path+"peaksworker.js spectrareadcurrentprocid:"+randomstring+"spectrareadprocid spectrareadcmdarggui specreadprocpeaks:[ spec2";
						system(execcommand.c_str()); //Start splash screen.
					}
				}else{
					int pidbsafek=fork();
					if(pidbsafek==0){
					//here
					char arg1[900];
					char exepath[900] = {0};
					sprintf( arg1, "/proc/%d/exe", getpid() );
					readlink( arg1, exepath, 1024 );
					string ownPth=string(exepath);
					std::string aux(ownPth);
					int pos = aux.rfind('/');
					std::string path = aux.substr(0,pos+1);
					std::string name = aux.substr(pos+1);
					int a=chdir(path.c_str());
					std::string execcommand="GI_TYPELIB_PATH=girepository-1.0 LD_LIBRARY_PATH=atlas-base DISPLAY=':0' PYTHONPATH="+path+"Python/lib/python2.7 PYTHONHOME="+path+"Python/ LD_PRELOAD='"+path+"Python/lib/libpython2.7.so.1.0 "+path+"atlas-base/libcblas.so.3.0 "+path+"atlas-base/libatlas.so.3 "+path+"libgfortran/libgfortran.so.3 "+path+"atlas-base/liblapack.so.3 "+path+"atlas-base/libblas.so.3 "+path+"atlas-base/libf77blas.so.3' "+path+"spectrareadd "+path+"peaksworker.js spectrareadcurrentprocid:"+randomstring+"spectrareadprocid spectrareadcmdarggui specreadprocpeaks:{ spec2";
					system(execcommand.c_str()); //Start splash screen.
					}else{
						int pidbsafel=fork();
						if(pidbsafel==0){
							//here
							char arg1[900];
							char exepath[900] = {0};
							sprintf( arg1, "/proc/%d/exe", getpid() );
							readlink( arg1, exepath, 1024 );
							string ownPth=string(exepath);
							std::string aux(ownPth);
							int pos = aux.rfind('/');
							std::string path = aux.substr(0,pos+1);
							std::string name = aux.substr(pos+1);
							int a=chdir(path.c_str());
							std::string execcommand="GI_TYPELIB_PATH=girepository-1.0 LD_LIBRARY_PATH=atlas-base DISPLAY=':0' PYTHONPATH="+path+"Python/lib/python2.7 PYTHONHOME="+path+"Python/ LD_PRELOAD='"+path+"Python/lib/libpython2.7.so.1.0 "+path+"atlas-base/libcblas.so.3.0 "+path+"atlas-base/libatlas.so.3 "+path+"libgfortran/libgfortran.so.3 "+path+"atlas-base/liblapack.so.3 "+path+"atlas-base/libblas.so.3 "+path+"atlas-base/libf77blas.so.3' "+path+"spectrareadd "+path+"peaksworker.js spectrareadcurrentprocid:"+randomstring+"spectrareadprocid spectrareadcmdarggui specreadprocpeaks:par spec2";
							system(execcommand.c_str()); //Start splash screen.
						}else{
							//here
							char arg1[900];
							char exepath[900] = {0};
							sprintf( arg1, "/proc/%d/exe", getpid() );
							readlink( arg1, exepath, 1024 );
							string ownPth=string(exepath);
							std::string aux(ownPth);
							int pos = aux.rfind('/');
							std::string path = aux.substr(0,pos+1);
							std::string name = aux.substr(pos+1);
							int a=chdir(path.c_str());
							std::string execcommand="GI_TYPELIB_PATH=girepository-1.0 LD_LIBRARY_PATH=atlas-base DISPLAY=':0' PYTHONPATH="+path+"Python/lib/python2.7 PYTHONHOME="+path+"Python/ LD_PRELOAD='"+path+"Python/lib/libpython2.7.so.1.0 "+path+"atlas-base/libcblas.so.3.0 "+path+"atlas-base/libatlas.so.3 "+path+"libgfortran/libgfortran.so.3 "+path+"atlas-base/liblapack.so.3 "+path+"atlas-base/libblas.so.3 "+path+"atlas-base/libf77blas.so.3' "+path+"spectrareadd "+path+"peaksworker.js spectrareadcurrentprocid:"+randomstring+"spectrareadprocid spectrareadcmdarggui specreadprocpeaks:~ spec1";
							system(execcommand.c_str()); //Start splash screen.
						}

					}
				}

			}

		}
	//------------------------------------------------
	//------------------------------------------------
	//Safe part end
	//------------------------------------------------
	//------------------------------------------------
}
}
