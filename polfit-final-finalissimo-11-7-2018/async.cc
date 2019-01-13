#include <nan.h>
#include <string>
#include <iostream>
#include <chrono>
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
double peaks[TEMPDATASIZE];
int peakscount=0;
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
	int i,j,cntintnodestring,cntintnodestringb;
	cntintnodestringb=0;
	cntintnodestring=0;
	std::string auxb="";
	char* cptr;
	for (i=0;i<len;i++){
		if(str[i]!=' '){
			auxb=auxb+str[i];
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
		int res=fit(y,yb,N,order,coef);
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
void getspecsync(const FunctionCallbackInfo<Value>& info) {
				double estimate=0;
				double returnarray[TEMPDATASIZE];
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
				int j=0;
				int N=getN(len,str);
				if((getN(lenb,strb))!=N){
					perror("198");
					return;
				}
				double yb[N+5];
				double ybb[N+5];
				double xb[N+5];
				int oldN=N-1;
				double y[(int)N];
				if(getvector(oldN,len,str,xb)==-1){
					return;
				}
				if(getvector(oldN,lenb,strb,yb)==-1){
					return;
				}
				fitbaseline(xb,yb,order,N-1);
				boxcar(yb,boxcarsizeint,oldN);
				double valmaxderivada=0;
				for (j=0;j<=oldN;j++){
					ybb[j]=yb[j];
					if(ybb[j]>valmaxderivada){
						valmaxderivada=ybb[j];
					}
				}
				derivada(ybb,xb,1,oldN);
				double adjustsection[3+5];
				double adjustsectionx[3+5];
				peakscount=0;
				double adjustsectioncoef[3+5];
				int zeroderivativepoints[oldN+5];
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
						int resfitpeak=fit(adjustsectionx,adjustsection,3,2,adjustsectioncoef);
						peaks[peakscount]=(-1*adjustsectioncoef[1]/(2*adjustsectioncoef[2]));
						peakscount=peakscount+1;
				}
				for (j=0;j<=oldN;j++){
					returnarray[j]=yb[j];
				}
				length=oldN+1;

			j=0;
			v8::Local<v8::Array> jsArr = Nan::New<v8::Array>(length);
			for (j=0;j<length;j++){
				Nan::Set(jsArr, j, Nan::New(returnarray[j]));
			}

	info.GetReturnValue().Set(jsArr);
}
void getpeakssync(const FunctionCallbackInfo<Value>& info) {
			int j=0;
			v8::Local<v8::Array> jsArr = Nan::New<v8::Array>(peakscount);
			for (j=0;j<peakscount;j++){
				Nan::Set(jsArr, j, Nan::New(peaks[j]));
			}
			info.GetReturnValue().Set(jsArr);
}
NAN_MODULE_INIT(InitAll) {
	NODE_SET_METHOD(target, "getspecsync", getspecsync);
	NODE_SET_METHOD(target, "getpeakssync", getpeakssync);
}
NODE_MODULE(NativeExtension, InitAll)
