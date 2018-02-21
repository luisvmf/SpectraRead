#include <nan.h>
#include <string>
#include <iostream>
#include <chrono>
#include "peaks.cc"

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

class AsyncPeaksWorker : public AsyncWorker {
	public:
		 AsyncPeaksWorker(Callback *callback, std::string internalstring,std::string internalstring2, int length,int lengthb, int order, int boxcarsizeint)
			:AsyncWorker(callback),internalstring(internalstring),internalstring2(internalstring2),length(length),lengthb(lengthb),order(order),boxcarsizeint(boxcarsizeint) ,estimate(0) {}
		 ~AsyncPeaksWorker() {}
		  void Execute () {
				int reductionfactor=7; //This is the reduction factor to be used in the baseline removal algorithm (not all data points are used in fit to improve speed).
				std::string str=internalstring;
				std::string strb=internalstring2;
				int len = length;
				int lenb = lengthb;
				int j=0;
				int N=getN(len,str);
				double yb[N];
				double ybb[N];
				double xb[N];
				int oldN=N;
				float y[(int)N/reductionfactor];


	//high_resolution_clock::time_point t1=high_resolution_clock::now();
				getvector(oldN,len,str,yb);
				getvector(oldN,lenb,strb,xb);
	//high_resolution_clock::time_point t2=high_resolution_clock::now();
	//auto duration=duration_cast<microseconds>(t2-t1).count();
	//std::cout<<"\n Tempo 1:"<<duration;
				N=(int)N/reductionfactor;

				fitbaseline(y,yb,order,N,reductionfactor);
				removebaseline(y,yb,oldN,reductionfactor);
				boxcar(yb,boxcarsizeint,oldN);
				for (j=0;j<oldN;j++){
					ybb[j]=yb[j];
				}
				derivada(ybb,xb,1,oldN);
				double peaks[oldN];
				int zeroderivativepoints[oldN];
				int zeroderivativepointscount=0;
				int peakscount=0;
				for (j=0;j<oldN;j++){
					if(ybb[j]<0){
						if(ybb[j+1]>0){
							if(yb[j]>0.5){
							zeroderivativepoints[zeroderivativepointscount]=j;
							zeroderivativepointscount=zeroderivativepointscount+1;
							}
						}
					}
					if(ybb[j]>0){
						if(ybb[j+1]<0){
							if(yb[j]>0.5){
							zeroderivativepoints[zeroderivativepointscount]=j;
							zeroderivativepointscount=zeroderivativepointscount+1;
							}
						}
					}
				}
				derivada(ybb,xb,1,oldN);
				boxcar(ybb,boxcarsizeint,oldN);
				for (j=0;j<zeroderivativepointscount;j++){
					//if(ybb[zeroderivativepoints[j]]>0){
						peaks[peakscount]=xb[zeroderivativepoints[j]];
						peakscount=peakscount+1;
						std::cout<<peaks[peakscount-1]<<"\n";
					//}
				}
				for (j=0;j<oldN;j++){
					returnarray[j]=yb[j];
				}
				
				length=oldN;
		  }
		  void HandleOKCallback () {
			HandleScope scope;
			v8::Local<v8::Array> jsArr = Nan::New<v8::Array>(length);
			int j=0;
			Local<Value> argv[] = {
				Null()
			  , jsArr
			};
			for (j=0;j<length;j++){
				Nan::Set(jsArr, j, Nan::New(returnarray[j]));
			}
			callback->Call(2, argv);
		  }
	private:
		double estimate=0;
		double returnarray[MAXDATASIZE];
		std::string internalstring="";
		std::string internalstring2="";
		int length;
		int lengthb;
		int order;
		int boxcarsizeint;
};
NAN_METHOD(peaks) {
   // get the param
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
	Callback *callback = new Callback(info[4].As<Function>());
	AsyncQueueWorker(new AsyncPeaksWorker(callback,internalstring,internalstring2,length,lengthb,order,boxcarsizeint));
}
#ifndef ASYNC_TEST_H_
#define ASYNC_TEST_H_
NAN_METHOD(peaks);
#endif
NAN_MODULE_INIT(InitAll) {
	Nan::Set(target, Nan::New("peaks").ToLocalChecked(),
		Nan::GetFunction(Nan::New<FunctionTemplate>(peaks)).ToLocalChecked());
}
NODE_MODULE(NativeExtension, InitAll)
