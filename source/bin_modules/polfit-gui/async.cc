#include <nan.h>
#include <string>
#include <iostream>
#include <chrono>
#include <math.h>
#include <unistd.h>
int statlastnode=0;
#include "../peakproc.cc"
#include "../FastMmapMQ/cmodule.c"
#include <locale.h>
using namespace std; 
//Copyright (c) 2018 Luís Victor Muller Fabris. Apache License.
using namespace v8;
using v8::Function;
using v8::Local;
using v8::Number;
using v8::Value;
using Nan::AsyncQueueWorker;
using Nan::AsyncWorker;
using Nan::Callback;
using Nan::HandleScope;
using Nan::New;
using Nan::Null;
using Nan::To;
using v8::FunctionTemplate;
using namespace std::chrono;
#define TEMPDATASIZE 300000
double peaks[TEMPDATASIZE+9000];
int peakscount=0;
int lastsetlocaleint=0;
int mapidb=-1;
int mapidba=-1;
int getN(int len, std::string str){
	if(lastsetlocaleint==0){
		setlocale(LC_ALL,"C");
		lastsetlocaleint=1;
	}
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
	if(lastsetlocaleint==0){
		setlocale(LC_ALL,"C");
		lastsetlocaleint=1;
	}
	char* cptr;
	strtod(stringnum.c_str(),&cptr);
	if (*cptr){
		return 0;
	}
	return 1;
}
int getvector(int N,int len, std::string str,double *yb){
	if(lastsetlocaleint==0){
		setlocale(LC_ALL,"C");
		lastsetlocaleint=1;
	}
	int i,j,cntintnodestring,cntintnodestringb;
	cntintnodestringb=0;
	cntintnodestring=0;
	std::string auxb="";
	char* cptr;
	for (i=0;i<len;i++){
		if(str[i]!=' '){
			if(str[i]==','){
				auxb=auxb+'.';
			}else{
				auxb=auxb+str[i];
			}
		}else{
			if(cntintnodestring<=N){
				yb[cntintnodestring]=strtod(auxb.c_str(),&cptr);
			}else{
				perror("Seg");
				return -1;
			}
			auxb="";
			cntintnodestring=cntintnodestring+1;
		}
	}
	return 0;
}

void getspecsync(const FunctionCallbackInfo<Value>& info) {
				if(mapidb==-1)
					mapidb=fastmmapmq_createmmap("darkref","rwx------");
				if(mapidba==-1)
					mapidba=fastmmapmq_createmmap("scoperef","rwx------");
				char *darkcharpointer=fastmmapmq_getsharedstring_withsize(mapidb,99999*sizeof(float)/sizeof(char));
				float *darkfloatpointer=(float *)darkcharpointer;
				char *scopecharpointer=fastmmapmq_getsharedstring_withsize(mapidba,99999*sizeof(float)/sizeof(char));
				float *scopefloatpointer=(float *)scopecharpointer;
				double estimate=0;
				double returnarray[TEMPDATASIZE+900];
				int currint;
				int order=info[2]->NumberValue();
				int boxcarsizeint=info[3]->NumberValue();
				float risingthreshold=info[4]->NumberValue();
				v8::String::Utf8Value param1(info[0]->ToString());
				std::string internalstring = std::string(*param1);
				Nan::Utf8String intdatastraaa(info[0]);
				v8::String::Utf8Value param2(info[1]->ToString());
				std::string internalstring2 = std::string(*param2);
				Nan::Utf8String intdatastr2aaa(info[1]);
				int length = intdatastraaa.length();
				int lengthb = intdatastr2aaa.length();
				std::string str=internalstring;
				std::string strb=internalstring2;
				int len = length;
				int lenb = lengthb;
				int N=getN(len,str);
				int j=0;
				if((getN(lenb,strb))!=N){
					perror("198");
					return;
				}
				if(N<5){
					return;
				}
				//if(N>513){
				//	printf("Error here: N=%i\n",N);
				//	return;
				//}
				//////double yb[N+50000];
				//////double xb[N+50000];
				double *xb=(double *)malloc((N+50000)*sizeof(double));
				double *yb=(double *)malloc((N+50000)*sizeof(double));
				if(getvector(N-1,len,str,xb)==-1){
					return;
				}
				if(getvector(N-1,lenb,strb,yb)==-1){
					return;
				}
				peakscount=processpeaks(xb, yb,peaks, order, N, boxcarsizeint, risingthreshold,darkfloatpointer,scopefloatpointer);
				for (j=0;j<=N-1;j++){
					returnarray[j]=yb[j];
				}
				length=N;
				j=0;
				v8::Local<v8::Array> jsArr = Nan::New<v8::Array>(length);
				for (j=0;j<length;j++){
					Nan::Set(jsArr, j, Nan::New(returnarray[j]));
				}
				info.GetReturnValue().Set(jsArr);
				free(darkcharpointer);
				free(scopecharpointer);
				free(xb);
				free(yb);
}
void getpeakssync(const FunctionCallbackInfo<Value>& info) {
			int j=0;
			v8::Local<v8::Array> jsArr = Nan::New<v8::Array>(peakscount);
			for (j=0;j<peakscount;j++){
				Nan::Set(jsArr, j, Nan::New(peaks[j]));
			}
			info.GetReturnValue().Set(jsArr);
}
void getstat(const FunctionCallbackInfo<Value>& info) {
			int j=0;
			v8::Local<v8::Array> jsArr = Nan::New<v8::Array>(1);
			Nan::Set(jsArr, 0, Nan::New(statlastnode));
			info.GetReturnValue().Set(jsArr);
}

void initmmap(const FunctionCallbackInfo<Value>& info) {
	if(mapidb==-1)
		mapidb=fastmmapmq_createmmap("darkref","rwx------");
	if(mapidba==-1)
		mapidba=fastmmapmq_createmmap("scoperef","rwx------");
}

void nodesleep(const FunctionCallbackInfo<Value>& info){
	usleep((float)((info[0]->NumberValue())*1000000.0));
}
NAN_MODULE_INIT(InitAll) {
	NODE_SET_METHOD(target, "getspecsync", getspecsync);
	NODE_SET_METHOD(target, "getpeakssync", getpeakssync);
	NODE_SET_METHOD(target, "nodesleep", nodesleep);
	NODE_SET_METHOD(target, "getstat", getstat);
	NODE_SET_METHOD(target, "initmmap", initmmap);
}
NODE_MODULE(NativeExtension, InitAll)
