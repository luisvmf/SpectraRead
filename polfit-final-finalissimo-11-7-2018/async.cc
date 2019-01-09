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
void getvector(int N,int len, std::string str,double *yb){
	int i,j,cntintnodestring,cntintnodestringb;
	cntintnodestringb=0;
	cntintnodestring=0;
	std::string auxb="";
	for (i=0;i<len;i++){
		if(str[i]!=' '){
			auxb=auxb+str[i];
		}else{
			yb[cntintnodestring]=std::stof(auxb);
			auxb="";
			cntintnodestring=cntintnodestring+1;
		}
	}
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
class AsyncPeaksWorker : public AsyncWorker {
	public:
		 AsyncPeaksWorker(Callback *callback, std::string internalstring,std::string internalstring2, int length,int lengthb, int order, int boxcarsizeint,int currint,float risingthreshold)
			:AsyncWorker(callback),internalstring(internalstring),internalstring2(internalstring2),length(length),lengthb(lengthb),order(order),boxcarsizeint(boxcarsizeint) ,currint(currint),risingthreshold(risingthreshold),estimate(0) {}
		 ~AsyncPeaksWorker() {
			delete callback;
		}
		  void Execute () {
				std::string str=internalstring;
				std::string strb=internalstring2;
				int len = length;
				int lenb = lengthb;
				int j=0;
				int N=getN(len,str)-1;
				double yb[N];
				double ybb[N];
				double xb[N];
				int oldN=N;
				double y[(int)N];
				//high_resolution_clock::time_point t1=high_resolution_clock::now();
				getvector(oldN,len,str,xb);
				getvector(oldN,lenb,strb,yb);
				//high_resolution_clock::time_point t2=high_resolution_clock::now();
				//auto duration=duration_cast<microseconds>(t2-t1).count();
				//std::cout<<"\n Tempo 1:"<<duration;
							//x y
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
					// Local minimum.
					//if(ybb[j]<0){
					//	if(ybb[j+1]>0){
					//		if(ybb[j]<-risingthreshold){  // rising threshold=0.5.
					//		zeroderivativepoints[zeroderivativepointscount]=j;
					//		zeroderivativepointscount=zeroderivativepointscount+1;
					//		}
					//	}
					//}
					// Local maximum.
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
					//if(ybb[zeroderivativepoints[j]]>0){ //Maximum, not minimum.
						adjustsection[0]=yb[zeroderivativepoints[j]];
						adjustsection[1]=yb[zeroderivativepoints[j]+1];
						adjustsection[2]=yb[zeroderivativepoints[j]+2];
						adjustsectionx[0]=xb[zeroderivativepoints[j]];
						adjustsectionx[1]=xb[zeroderivativepoints[j]+1];
						adjustsectionx[2]=xb[zeroderivativepoints[j]+2];
						int resfitpeak=fit(adjustsectionx,adjustsection,3,2,adjustsectioncoef);
						peaks[peakscount]=(-1*adjustsectioncoef[1]/(2*adjustsectioncoef[2]));
						peakscount=peakscount+1;
					//}
				}
				for (j=0;j<=oldN;j++){
					returnarray[j]=yb[j];
				}
				length=oldN+1;
		  }
		  void HandleOKCallback () {
			Nan::HandleScope scope;
			v8::Local<v8::Array> jsArr = Nan::New<v8::Array>(length);
			v8::Local<v8::Array> jsArrb = Nan::New<v8::Array>(peakscount);
			v8::Local<v8::Integer> aaaretint = Nan::New(currint);
			int j=0;
			Local<Value> argv[] = {
				Null()
			  , jsArr
			  , jsArrb
			  , aaaretint
			};
			for (j=0;j<length;j++){
				Nan::Set(jsArr, j, Nan::New(returnarray[j]));
			}
			for (j=0;j<peakscount;j++){
				Nan::Set(jsArrb, j, Nan::New(peaks[j]));
			}
			callback->Call(3, argv);
		  }
	private:
		double estimate=0;
		double returnarray[TEMPDATASIZE];
		double peaks[TEMPDATASIZE];
		std::string internalstring="";
		std::string internalstring2="";
		int length;
		int lengthb;
		int peakscount=0;
		int order;
		int boxcarsizeint;
		int currint;
		float risingthreshold;
};
NAN_METHOD(peaks) {
    v8::String::Utf8Value param1(info[0]->ToString());
    std::string internalstring = std::string(*param1);  
	Nan::Utf8String intdatastr(info[0]);
    v8::String::Utf8Value param2(info[1]->ToString());
    std::string internalstring2 = std::string(*param2);  
	Nan::Utf8String intdatastr2(info[1]);
	int length = intdatastr.length();
	int lengthb = intdatastr2.length();
	int order = To<int>(info[2]).FromJust();
	int boxcarsizeint = To<int>(info[3]).FromJust();
	int currint = To<int>(info[4]).FromJust();
	float risingthreshold = To<double>(info[5]).FromJust();
	Callback *callback = new Callback(info[6].As<Function>());
	AsyncQueueWorker(new AsyncPeaksWorker(callback,internalstring,internalstring2,length,lengthb,order,boxcarsizeint,currint,risingthreshold));
}
NAN_MODULE_INIT(InitAll) {
	Nan::Set(target, Nan::New("process").ToLocalChecked(),Nan::GetFunction(Nan::New<FunctionTemplate>(peaks)).ToLocalChecked());
}
NODE_MODULE(NativeExtension, InitAll)
