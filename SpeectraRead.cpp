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
static uid_t euid,ruid;
static gid_t egid,rgid;


using namespace std;

main(int argc, char ** argv, char ** envp){
srand(time(NULL));

/*
Look for unsanitised command line parameters directly passed to a system() call
Look for unsanitised PATH variables directly passed to a system() call
Look for unchecked data being loaded directly into static buffers
Look for unchecked data being loaded directly into a heap buffer
*/

ofstream myfile;
myfile.open ("/dev/shm/spectrareadenv-shm-1823019506");
for (char **env = envp; *env != 0; env++)
{
    char *thisEnv = *env;
    myfile << thisEnv<<"\n";
}
myfile.close();
environ = NULL;/* Clear all enviroment variables to prevent code injection on systhem call */
envp = 0; /* blocks IFS attack on non-bash shells */
rgid=getgid();
egid=getegid();
ruid=getuid();
euid=geteuid();
static gid_t groups[]={egid};
setgroups(1,groups); /* Add permission to control devices owned by egid group */
setuid(ruid); /* Drop real root privileges */
seteuid(ruid);/* Drop effetive root privileges (can't gain root privileges again after this line) */
char arg1[20];
char exepath[30] = {0};
sprintf( arg1, "/proc/%d/exe", getpid() );
readlink( arg1, exepath, 1024 );
string ownPth=string(exepath);
std::string aux(ownPth);
//   int pos = aux.rfind('\\');
int pos = aux.rfind('/');
// Get the path and the name
std::string path = aux.substr(0,pos+1);
std::string name = aux.substr(pos+1);
// Use above module handle to get the path using GetModuleFileName()
int a=chdir(path.c_str());


string randomstring;
stringstream aaa;
aaa << abs(rand()*rand()*rand()*rand());
randomstring = aaa.str();
std::string execcommand="LD_LIBRARY_PATH=atlas-base DISPLAY=':0' PYTHONPATH="+path+"Python/lib/python2.7 PYTHONHOME="+path+"Python/ LD_PRELOAD='"+path+"Python/lib/libpython2.7.so.1.0 "+path+"atlas-base/libcblas.so.3.0 "+path+"atlas-base/libatlas.so.3 "+path+"libgfortran/libgfortran.so.3 "+path+"atlas-base/liblapack.so.3 "+path+"atlas-base/libblas.so.3 "+path+"atlas-base/libf77blas.so.3' "+path+"node-v4.4.2 "+path+"main.js "+randomstring;
system(execcommand.c_str());
}
