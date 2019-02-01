#include <nan.h>
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include <stdio.h>
#include "fastmmapmq.c"
#include <sstream>
//Copyright (c) 2018 Luís Victor Muller Fabris. Apache License.
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
void connectmappfhs(char *internalconnectid,char *internalconnectidpeaks,int createconnect){
	mapid=connectmmap("daemon.js",internalconnectid);
	while(mapid==-1){
		mapid=connectmmap("daemon.js",internalconnectid);
		perror("Connection failed, reconnecting...");
		usleep(0.05*1000000.0);
	}
	if(createconnect==0){
		mapidb=createmmap(internalconnectidpeaks,"rwx------");
	}else{
		mapidb=connectmmap("peaksworker.js",internalconnectidpeaks);
		while(mapidb==-1){
			mapidb=connectmmap("peaksworker.js",internalconnectidpeaks);
			perror("Connection failed, reconnecting...");
			usleep(0.05*1000000.0);
		}
	}
}
void boxcarint(double *dados,int dadoslen,int loopselect){
	int contador1=1;
	double dadosb[dadoslen+1];
	dadosb[0]=(dados[0]+dados[1])/2;
	if(loopselect==0){
		while(contador1<dadoslen){
			dadosb[contador1]=((dados[contador1-1])+(dados[contador1])+(dados[contador1+1]))/3;
			contador1=contador1+1;
		}
	}else{
		while(contador1<dadoslen){
			dadosb[contador1]=((dados[contador1])+(dados[contador1+1]))/2;
			contador1=contador1+1;
		}
	}
	dadosb[dadoslen]=(dados[dadoslen]+dados[dadoslen-1])/2;
	contador1=0;
	while(contador1<=dadoslen){
		dados[contador1]=dadosb[contador1];
		contador1=contador1+1;
	}
}
void boxcar(double *dados, int boxcarsize,int dadoslen){
	int cnt_boxcar=0;
	int loopselect=0;
	loopselect=1;
	while(cnt_boxcar<boxcarsize){
		boxcarint(dados,dadoslen,loopselect);
		cnt_boxcar=cnt_boxcar+1;
	}
}
void derivada(double *dados,double *dadosx, int ordemderivada,int dadoslen){
	int contador1=0;
	double vetorretornaderivada[dadoslen+3];
	while(contador1<dadoslen){
		vetorretornaderivada[contador1]=((dados[contador1+1])-(dados[contador1]))/((dadosx[contador1+1])-(dadosx[contador1]));
		contador1=contador1+1;
	}
	vetorretornaderivada[contador1]=vetorretornaderivada[contador1-1];
	contador1=0;
	while(contador1<=dadoslen){
		dados[contador1]=vetorretornaderivada[contador1];
		contador1=contador1+1;
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
	N=N-1;
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
			auxb=auxb+str[i];
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
int fit(double* x,double* y,int count,int order, double* coef){
	double A[order+1]={0.0};
	double B[order*2+3]={0.0};
	B[0]=count;
	double C[2*order*order+4*order+2]={0.0};
	double xint,yint,px;
	int i,j,k;
	if(count<=order)
		return -1;
	for(i=0;i<count;i++){
		xint=x[i];
		yint=y[i];		
		px=1;
		for(j=0;j<=order; j++){
			A[j]=A[j]+yint*px;
			px=px*xint;
		}
		px=xint;
		for(j=1;j<=(2*order+2);j++){
			B[j]=B[j]+px;
			px=px*xint;
		}
	}
	for(i=0;i<=order;i++){
		k=i*(2*order+2);
		for(j=0;j<=order;j++){
			C[k+j]=B[i+j];
		}
		C[k+i+order+1]=1;
	}
	for(i=0;i<=order;i++){
		xint=C[i*(2*order+2)+i];
		k=i*(2*order+2);
		if(xint!=0){
			for(j=0;j<=(2*order+1);j++){
	C[k+j]=C[k+j]/xint;
			}
			for(j=0;j<=order;j++){
	if(j!=i){
		yint=C[j*(2*order+2)+i];
		for(k=0;k<=(2*order+1);k++){
			C[j*(2*order+2)+k]=C[j*(2*order+2)+k]-yint*C[i*(2*order+2)+k];
		}
	}
			}
		}else{return -1;}
	}
	for(i=0;i<=order;i++){
		for(j=0;j<=order;j++){
			xint=0;
			for(k=0;k<=order;k++){
	xint=xint+C[i*(2*order+2)+k+order+1]*A[k];
			}
			coef[i]=xint;
		}
	}
	return 0;
}
void fitbaseline(double *y,double *yb, int order,int N){
	if(order>0){
		double coef[order+1];
		if(fit(y,yb,N,order,coef)==-1){
			return;
		}
		int i=0;
		int j=0;
		double aux=0;
		double auxb=0;
		for (i=0;i<=N;i++){
			aux=0;
			auxb=1;
			for (j=0;j<=order+1;j++){
				aux=aux+auxb*coef[j];
				auxb=auxb*y[i];
			}
			yb[i]=yb[i]-aux;
		}
	}
}
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
	int N=getN(len,str)-1;
	if((getN(lenb,strb)-1)!=N){
		return -1;
	}
	double yb[N];
	double ybb[N];
	double xb[N];
	int currentnruns=0;
	int oldN=N;
	if(getvector(oldN,len,str,xb)==-1){
		return -1;
	}
	if(getvector(oldN,lenb,strb,yb)==-1){
		return -1;
	}
	fitbaseline(xb,yb,order,N);
	boxcar(yb,boxcarsizeint,oldN);
	double valmaxderivada=0;
	for (j=0;j<=oldN;j++){
		ybb[j]=yb[j];
		if(ybb[j]>valmaxderivada){
			valmaxderivada=ybb[j];
		}
	}
	derivada(ybb,xb,1,oldN);
	double adjustsection[3];
	double adjustsectionx[3];
	peakscount=0;
	double adjustsectioncoef[3];
	int zeroderivativepoints[oldN];
	int zeroderivativepointscount=0;
	for (j=0;j<oldN;j++){
		if(ybb[j]>0){
			if(ybb[j+1]<0){
	if(ybb[j]>risingthreshold*valmaxderivada){ // rising threshold=0.5.
	zeroderivativepoints[zeroderivativepointscount]=j;
	zeroderivativepointscount=zeroderivativepointscount+1;
	}
			}
		}
	}
	for (j=0;j<zeroderivativepointscount;j++){
			adjustsection[0]=yb[zeroderivativepoints[j]];
			adjustsection[1]=yb[zeroderivativepoints[j]+1];
			adjustsection[2]=yb[zeroderivativepoints[j]+2];
			adjustsectionx[0]=xb[zeroderivativepoints[j]];
			adjustsectionx[1]=xb[zeroderivativepoints[j]+1];
			adjustsectionx[2]=xb[zeroderivativepoints[j]+2];
			if(fit(adjustsectionx,adjustsection,3,2,adjustsectioncoef)==-1){
				return -1;
			}
			peaks[peakscount]=(-1*adjustsectioncoef[1]/(2*adjustsectioncoef[2]));
			peakscount=peakscount+1;
	}
	char strdhdhsghdaold[900]="";
char *chartemppeakbuffer;
chartemppeakbuffer=strdhdhsghdaold;
	sprintf(chartemppeakbuffer,"%f ",timestamp);
	//writemmap(mapidb,chartemppeakbuffer);
	for (j=0;j<peakscount;j++){
		sprintf(chartemppeakbuffer+strlen(chartemppeakbuffer),"%f ",peaks[j]);
		//writemmap(mapidb,chartemppeakbuffer);
	}
	//free(chartemppeakbuffer);
	sprintf(chartemppeakbuffer+strlen(chartemppeakbuffer),"%s","\n");
	writemmap(mapidb,chartemppeakbuffer);
	//writemmap(mapidb,"\n");
	//printf("%s",readmmap(mapidb,0));
	length=oldN+1;
	currentnruns=currentnruns+1;
	return 0;
}
char *fastchararrcnt=malloc(90);
char *fastchararrb=malloc(10);
int alloccountera=10;
char *fastchararrc=malloc(10);
int alloccounterb=10;
int procdatath(std::string internalstringauxread){
	int countrawstrconvertb=0;
	int strselect=0;
	std::string internalstringauxreadx="";
	std::string internalstringauxready="";
	if(internalstringauxread.length()+10>alloccountera){
		fastchararrb=realloc(fastchararrb,internalstringauxread.length()+10);
		alloccountera=alloccountera+1;
	}
	//char *fastchararrb=malloc(internalstringauxread.length()+10);
	int fastchararrbcounter=0;
	//char *fastchararrc=malloc(internalstringauxread.length()+10);
	if(internalstringauxread.length()+10>alloccounterb){
		fastchararrc=realloc(fastchararrc,internalstringauxread.length()+10);
		alloccounterb=alloccounterb+1;
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
	//free(fastchararrb);
	//free(fastchararrc);
	return realnspectra;
}
char *fastchararr=malloc(10);
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
		if(readlastinteractionb<1000){
			usleep(0.01*1000000.0);
		}
		if(readlastinteractionb<250){
			usleep(0.05*1000000.0);
		}
		if(readlastinteractionb<550){
			usleep(0.05*1000000.0);
		}
		if(readlastinteractionb<5){
			usleep(0.05*1000000.0);
		}
		readlastinteractionb=0;
		std::string rawreaddata=readmmap(mapid,0);
		int countrawstrconvert=0;
		while(countrawstrconvert<rawreaddata.length()){
			if(rawreaddata[countrawstrconvert]==';'){
				break;
			}
			countrawstrconvert=countrawstrconvert+1;
		}
		internalstringauxread="";
		//char *fastchararr=malloc(rawreaddata.length()+10);
	if(rawreaddata.length()+10>alloccounterc){
		fastchararr=realloc(fastchararr,rawreaddata.length()+10);
		alloccounterc=alloccounterc+1;
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
		//free(fastchararr);
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
		int createconnect=info[2]->NumberValue();
		connectmappfhs(internalstringtth.c_str(),internalstringtthb.c_str(),createconnect);
	}
}
NAN_MODULE_INIT(InitAll) {
	NODE_SET_METHOD(target, "polfit", polfit);
	NODE_SET_METHOD(target, "connect", connect);
	NODE_SET_METHOD(target, "sleep", nodesleep);
}
NODE_MODULE(NativeExtension, InitAll)
