#include <nan.h>
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include <stdio.h>
#include "../FastMmapMQ/cmodule.c"
#include <sstream>
int statlastnode=0;
#include "../peakproc.cc"
#include <locale.h>
//Copyright (c) 2019 Luís Victor Muller Fabris. Apache License.
using namespace v8;
using namespace std::chrono;
#define TEMPDATASIZE 300000
int mapid=-1;
int mapidb=-1;
int order;
int boxcarsizeint;
float risingthreshold;
int nruns;
int savepeaks;
int savespectra;
float savespectrainterval;
std::string spectrafilename;
std::string peaksfilename;
int mapidbn=-1;
int mapidban=-1;

void connectmappfhs(char *internalconnectid,char *internalconnectidpeaks,int createconnect, char *cmdargsscope){
	setlocale(LC_ALL,"C");
	while(mapidbn==-1){
		mapidbn=fastmmapmq_connectmmap(cmdargsscope,"darkref");
		printf("Connection failed to darkref on polfit-hs/pf.cc, reconnecting...\n");
		usleep(0.5*1000000.0);
	}
	while(mapidban==-1){
		mapidban=fastmmapmq_connectmmap(cmdargsscope,"scoperef");
		printf("Connection failed to scoperef on polfit-hs/pf.cc, reconnecting...\n");
		usleep(0.5*1000000.0);
	}
	mapid=connectmmap("daemon.js",internalconnectid);
	while(mapid==-1){
		mapid=connectmmap("daemon.js",internalconnectid);
		//perror("Connection failed, reconnecting...");
		usleep(0.1*1000000.0);
	}
	if(createconnect==0){
		mapidb=createmmap(internalconnectidpeaks,"rwx------");
	}else{
		mapidb=connectmmap("peaksworker.js",internalconnectidpeaks);
		while(mapidb==-1){
			mapidb=connectmmap("peaksworker.js",internalconnectidpeaks);
			//perror("Connection failed, reconnecting...");
			usleep(0.1*1000000.0);
		}
	}
}
int getN(int len, std::string str){
	int i=0;
	int N=1;
	for (i=0;i<=len;i++){
		if(str[i]==' '){
			N=N+1;
		}
	}
	//N=N-1;
	return N;
}
int isNumbera(std::string stringnum){
	char* cptr;
	strtod(stringnum.c_str(),&cptr);
	if (*cptr){
		return 0;
	}
	return 1;
}
int getvector(int N,int len, std::string str,double *yb){
	int i,cntintnodestring;
	cntintnodestring=0;
	std::string auxb="";
	char* cptr;
	for (i=0;i<len;i++){
		if(str[i]!=' '){
			//auxb=auxb+str[i];
			if(str[i]==','){
				auxb=auxb+'.';
			}else{
				auxb=auxb+str[i];
			}
		}else{
			if(isNumbera(auxb)==1){
	yb[cntintnodestring]=strtod(auxb.c_str(),&cptr);
			}else{
	return -1;
			}
			auxb="";
			cntintnodestring=cntintnodestring+1;
		}
	}
	return 0;
}
float absolute(float x){
	if(x<0)
		x=x*(-1);
	return x;
}
int mainproclastN=-3;
double *yb=NULL;
double *xb=NULL;
char *chartemppeakbuffer=(char *)malloc(9000*sizeof(char));
long long int internaltimecheckscope;
char *darkcharpointer;
float *darkfloatpointer=(float *)darkcharpointer;
char *scopecharpointer;
float *scopefloatpointer=(float *)scopecharpointer;
int alocatedscope=0;
int mainproc(std::string internalstringauxreadx,std::string internalstringauxready,int order,int boxcarsizeint,float risingthreshold,float timestamp){
	std::string internalstring=internalstringauxreadx;
	std::string internalstring2=internalstringauxready;
	int length = internalstring.length();
	int lengthb = internalstring2.length();
	double peaks[TEMPDATASIZE];
	int peakscount=0;
	std::string str=internalstring;
	std::string strb=internalstring2;
	int len = length;
	int lenb = lengthb;
	int j=0;
	int N=getN(len,str);
	if((getN(lenb,strb))!=N){
		return -1;
	}
	if(N<5){
		return 0;
	}
	if(N>mainproclastN){
		if(mainproclastN>0){
			free(xb);
			free(yb);
		}
		yb=(double *)malloc((N+900)*sizeof(double));
		if(yb==NULL){
			perror("Error on pf.cc. yb=(double *)malloc(..) fail.");
			exit(-1);
		}
		xb=(double *)malloc((N+900)*sizeof(double));
		if(xb==NULL){
			perror("Error on pf.cc. xb=(double *)malloc(..) fail.");
			exit(-1);
		}
		mainproclastN=N;
	}
	//double yb[N+90];
	//double xb[N+90];
	int currentnruns=0;
	if(getvector(N-1,len,str,xb)==-1){
		return -1;
	}
	if(getvector(N-1,lenb,strb,yb)==-1){
		return -1;
	}
	time_t starttime;
	time(&starttime);
	if(alocatedscope==0){
		alocatedscope=1;
		darkcharpointer=fastmmapmq_getsharedstring_withsize(mapidbn,99999*sizeof(float)/sizeof(char));
		darkfloatpointer=(float *)darkcharpointer;
		scopecharpointer=fastmmapmq_getsharedstring_withsize(mapidban,99999*sizeof(float)/sizeof(char));
		scopefloatpointer=(float *)scopecharpointer;
	}
	long long int internaltimecheckscopeb=starttime;;
	if(absolute(internaltimecheckscopeb-internaltimecheckscope)>0.3){//In seconds
		internaltimecheckscope=starttime;
		if(alocatedscope==1){
			free(darkcharpointer);
			free(scopecharpointer);
		}
		alocatedscope=1;
		darkcharpointer=fastmmapmq_getsharedstring_withsize(mapidbn,99999*sizeof(float)/sizeof(char));
		darkfloatpointer=(float *)darkcharpointer;
		scopecharpointer=fastmmapmq_getsharedstring_withsize(mapidban,99999*sizeof(float)/sizeof(char));
		scopefloatpointer=(float *)scopecharpointer;
	}
	peakscount=processpeaks(xb, yb,peaks, order, N, boxcarsizeint, risingthreshold,darkfloatpointer,scopefloatpointer);
	//char strdhdhsghdaold[9000]="";
	//chartemppeakbuffer=strdhdhsghdaold;
	sprintf(chartemppeakbuffer,"%f ",timestamp);
	for (j=0;j<peakscount;j++){
		sprintf(chartemppeakbuffer+strlen(chartemppeakbuffer),"%f ",peaks[j]);
	}
	sprintf(chartemppeakbuffer+strlen(chartemppeakbuffer),"%s","\n");
	writemmap(mapidb,chartemppeakbuffer);
	length=N-1;
	currentnruns=currentnruns+1;
	return 0;
}
char *fastchararrcnt=malloc(9000);
char *fastchararrb=malloc(1000);
int alloccountera=10;
char *fastchararrc=malloc(1000);
int alloccounterb=10;
int procdatath(std::string internalstringauxread){
	int countrawstrconvertb=0;
	int strselect=0;
	std::string internalstringauxreadx="";
	std::string internalstringauxready="";
	if(internalstringauxread.length()+10>alloccountera){
		fastchararrb=realloc(fastchararrb,internalstringauxread.length()+10);
		alloccountera=internalstringauxread.length()+10;
	}
	int fastchararrbcounter=0;
	if(internalstringauxread.length()+10>alloccounterb){
		fastchararrc=realloc(fastchararrc,internalstringauxread.length()+10);
		alloccounterb=internalstringauxread.length()+10;
	}
	int fastchararrccounter=0;
	float thistimestamp=-1;
	int realnspectra=0;
	while(countrawstrconvertb<internalstringauxread.length()){
		if(internalstringauxread[countrawstrconvertb]!='?'){
			if(strselect==0){
				fastchararrb[fastchararrbcounter]=internalstringauxread[countrawstrconvertb];
				fastchararrb[fastchararrbcounter+1]='\0';
				fastchararrbcounter=fastchararrbcounter+1;
			}else{
				fastchararrc[fastchararrccounter]=internalstringauxread[countrawstrconvertb];
				fastchararrc[fastchararrccounter+1]='\0';
				fastchararrccounter=fastchararrccounter+1;
			}
		}else{
			strselect=1;
			int kk=0;
			countrawstrconvertb=countrawstrconvertb+1;
			while(internalstringauxread[countrawstrconvertb]!='?'){
				fastchararrcnt[kk]=internalstringauxread[countrawstrconvertb];
				countrawstrconvertb=countrawstrconvertb+1;
				if(countrawstrconvertb>=internalstringauxread.length()){
					break;
				}
				kk=kk+1;
			}
			fastchararrcnt[kk]='\0';
			char* cptr;
			thistimestamp=strtod(fastchararrcnt,&cptr);
			if (*cptr){
				printf("Invalid timestamp data on polfiths");
				return 0;
			}
			//free(fastchararrcnt);
		}
		countrawstrconvertb=countrawstrconvertb+1;
	}
	if(fastchararrbcounter>3){
		internalstringauxreadx=std::string(fastchararrb);
		internalstringauxready=std::string(fastchararrc);
		int retmainprocstatus=mainproc(internalstringauxreadx,internalstringauxready,order,boxcarsizeint,risingthreshold,thistimestamp);
		if(retmainprocstatus==0){
			realnspectra=realnspectra+1;
		}
	}
	return realnspectra;
}

char *fastchararr=malloc(1000);
int alloccounterc=10;
void polfit(const FunctionCallbackInfo<Value>& info) {
	high_resolution_clock::time_point t1b=high_resolution_clock::now();
	order=info[0]->NumberValue();
	boxcarsizeint=info[1]->NumberValue();;
	risingthreshold=info[2]->NumberValue();
	nruns=info[3]->NumberValue();
	high_resolution_clock::time_point t2b=high_resolution_clock::now();
	float durationtotalacquisition=duration_cast<microseconds>(t2b-t1b).count();
	std::string internalstringauxread="";
	int counternspectra=0;
	int counterrealnspectra=0;
	int readlastinteractionb=0;
	while(durationtotalacquisition/1000000<nruns){
		t2b=high_resolution_clock::now();
		durationtotalacquisition=duration_cast<microseconds>(t2b-t1b).count();
		//if(readlastinteractionb<1000){
			//usleep(0.01*1000000.0);
		//}
		if(readlastinteractionb<150){
			usleep(0.01*1000000.0);
		}
		if(readlastinteractionb<550){
			usleep(0.001*1000000.0);
		}
		if(readlastinteractionb<2){
			usleep(0.05*1000000.0);
		}
		readlastinteractionb=0;
		char *intchararrtostringtemp=readmmap(mapid,0);
		if(intchararrtostringtemp==NULL){
			return;
		}
		std::string rawreaddata(intchararrtostringtemp);
		free(intchararrtostringtemp);
		int countrawstrconvert=0;
		while(countrawstrconvert<rawreaddata.length()){
			if(rawreaddata[countrawstrconvert]==';'){
				break;
			}
			countrawstrconvert=countrawstrconvert+1;
		}
		internalstringauxread="";
		if(rawreaddata.length()+10>alloccounterc){
			fastchararr=realloc(fastchararr,rawreaddata.length()+10);
			if(!fastchararr){
				perror("realloc(fastchararr,rawreaddata.length()+10); failled.\n");
				return;
			}
			alloccounterc=rawreaddata.length()+10;
		}
		int fastchararrcounter=0;
		while(countrawstrconvert<rawreaddata.length()){
			if(rawreaddata[countrawstrconvert]!=';'){
				fastchararr[fastchararrcounter]=rawreaddata[countrawstrconvert];
				fastchararr[fastchararrcounter+1]='\0';
				fastchararrcounter=fastchararrcounter+1;
			}else{
				internalstringauxread=std::string(fastchararr);
				counterrealnspectra=counterrealnspectra+procdatath(internalstringauxread);
				counternspectra=counternspectra+1;
				readlastinteractionb=readlastinteractionb+1;
				internalstringauxread="";
				fastchararrcounter=0;
			}
			countrawstrconvert=countrawstrconvert+1;
		}
		}
		info.GetReturnValue().Set(counterrealnspectra);
}
void nodesleep(const FunctionCallbackInfo<Value>& info){
	usleep((float)((info[0]->NumberValue())*1000000.0));
}
void connect(const FunctionCallbackInfo<Value>& info){
	if(mapid==-1){
		v8::String::Utf8Value param1b(info[0]->ToString());
		std::string internalstringtth = std::string(*param1b);
		v8::String::Utf8Value param1c(info[1]->ToString());
		std::string internalstringtthb = std::string(*param1c);
		v8::String::Utf8Value param1caa(info[3]->ToString());
		std::string internalstringtthbaa = std::string(*param1caa);
		int createconnect=info[2]->NumberValue();
		connectmappfhs(internalstringtth.c_str(),internalstringtthb.c_str(),createconnect,internalstringtthbaa.c_str());
	}
}
NAN_MODULE_INIT(InitAll) {
	NODE_SET_METHOD(target, "polfit", polfit);
	NODE_SET_METHOD(target, "connect", connect);
	NODE_SET_METHOD(target, "sleep", nodesleep);
}
NODE_MODULE(NativeExtension, InitAll)
