#include <nan.h>
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include "fastmmapmq.c"
//Copyright (c) 2018 Luís Victor Muller Fabris. Apache License.
using namespace v8;
using namespace std::chrono;
#define TEMPDATASIZE 300000
int mapid=-1;
void connectmappfhs(){
	mapid=connectmmap("None","datamapa");
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
//void polfit(order,boxcar,risingthreshold,nruns,savepeaks,savespectra,savespectrainterval,spectrafilename,peaksfilename)
void polfit(const FunctionCallbackInfo<Value>& info) {
				if(mapid==-1){
					connectmappfhs();
				}
				high_resolution_clock::time_point t1=high_resolution_clock::now();
				high_resolution_clock::time_point t1s=high_resolution_clock::now();
				high_resolution_clock::time_point t1b=high_resolution_clock::now();
				std::ofstream peaksfile;
				int order=info[0]->NumberValue();
				int boxcarsizeint=info[1]->NumberValue();;
				float risingthreshold=info[2]->NumberValue();
				int nruns=info[3]->NumberValue();
				int savepeaks=info[4]->NumberValue();
				int savespectra=info[5]->NumberValue();
				float savespectrainterval=info[6]->NumberValue(); //seconds
				v8::String::Utf8Value param1(info[7]->ToString());
				std::string spectrafilename = std::string(*param1);
				v8::String::Utf8Value param2(info[8]->ToString());
				std::string peaksfilename = std::string(*param2);
				int currentnruns=0;
				if(savepeaks==1){
					peaksfile.open(peaksfilename+".dat");
				}
				high_resolution_clock::time_point t2b=high_resolution_clock::now();
				float durationtotalacquisition=duration_cast<microseconds>(t2b-t1b).count();
				while(durationtotalacquisition/1000000<nruns){
				t2b=high_resolution_clock::now();
				durationtotalacquisition=duration_cast<microseconds>(t2b-t1b).count();
				writemmap(mapid,";0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100 101 102 103 104 105 106 107 108 109 110 111 112 113 114 115 116 117 118 119 120 121 122 123 124 125 126 127 128 129 130 131 132 133 134 135 136 137 138 139 140 141 142 143 144 145 146 147 148 149 150 151 152 153 154 155 156 157 158 159 160 161 162 163 164 165 166 167 168 169 170 171 172 173 174 175 176 177 178 179 180 181 182 183 184 185 186 187 188 189 190 191 192 193 194 195 196 197 198 199 200 201 202 203 204 205 206 207 208 209 210 211 212 213 214 215 216 217 218 219 220 221 222 223 224 225 226 227 228 229 230 231 232 233 234 235 236 237 238 239 240 241 242 243 244 245 246 247 248 249 250 251 252 253 254 255 256?0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100 101 102 103 104 105 106 107 108 109 110 111 112 113 114 115 116 117 118 119 120 121 122 123 124 125 126 127 128 129 130 131 132 133 134 135 136 137 138 139 140 141 142 143 144 145 146 147 148 149 150 151 152 153 154 155 156 157 158 159 160 161 162 163 164 165 166 167 168 169 170 171 172 173 174 175 176 177 178 179 180 181 182 183 184 185 186 187 188 189 190 191 192 193 194 195 196 197 198 199 200 201 202 203 204 205 206 207 208 209 210 211 212 213 214 215 216 217 218 219 220 221 222 223 224 225 226 227 228 229 230 231 232 233 234 235 236 237 238 239 240 241 242 243 244 245 246 247 248 249 250 251 252 253 254 255 256;;0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100 101 102 103 104 105 106 107 108 109 110 111 112 113 114 115 116 117 118 119 120 121 122 123 124 125 126 127 128 129 130 131 132 133 134 135 136 137 138 139 140 141 142 143 144 145 146 147 148 149 150 151 152 153 154 155 156 157 158 159 160 161 162 163 164 165 166 167 168 169 170 171 172 173 174 175 176 177 178 179 180 181 182 183 184 185 186 187 188 189 190 191 192 193 194 195 196 197 198 199 200 201 202 203 204 205 206 207 208 209 210 211 212 213 214 215 216 217 218 219 220 221 222 223 224 225 226 227 228 229 230 231 232 233 234 235 236 237 238 239 240 241 242 243 244 245 246 247 248 249 250 251 252 253 254 255 256?0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100 101 102 103 104 105 106 107 108 109 110 111 112 113 114 115 116 117 118 119 120 121 122 123 124 125 126 127 128 129 130 131 132 133 134 135 136 137 138 139 140 141 142 143 144 145 146 147 148 149 150 151 152 153 154 155 156 157 158 159 160 161 162 163 164 165 166 167 168 169 170 171 172 173 174 175 176 177 178 179 180 181 182 183 184 185 186 187 188 189 190 191 192 193 194 195 196 197 198 199 200 201 202 203 204 205 206 207 208 209 210 211 212 213 214 215 216 217 218 219 220 221 222 223 224 225 226 227 228 229 230 231 232 233 234 235 236 237 238 239 240 241 242 243 244 245 246 247 248 249 250 251 252 253 254 255 256;;0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100 101 102 103 104 105 106 107 108 109 110 111 112 113 114 115 116 117 118 119 120 121 122 123 124 125 126 127 128 129 130 131 132 133 134 135 136 137 138 139 140 141 142 143 144 145 146 147 148 149 150 151 152 153 154 155 156 157 158 159 160 161 162 163 164 165 166 167 168 169 170 171 172 173 174 175 176 177 178 179 180 181 182 183 184 185 186 187 188 189 190 191 192 193 194 195 196 197 198 199 200 201 202 203 204 205 206 207 208 209 210 211 212 213 214 215 216 217 218 219 220 221 222 223 224 225 226 227 228 229 230 231 232 233 234 235 236 237 238 239 240 241 242 243 244 245 246 247 248 249 250 251 252 253 254 255 256?0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100 101 102 103 104 105 106 107 108 109 110 111 112 113 114 115 116 117 118 119 120 121 122 123 124 125 126 127 128 129 130 131 132 133 134 135 136 137 138 139 140 141 142 143 144 145 146 147 148 149 150 151 152 153 154 155 156 157 158 159 160 161 162 163 164 165 166 167 168 169 170 171 172 173 174 175 176 177 178 179 180 181 182 183 184 185 186 187 188 189 190 191 192 193 194 195 196 197 198 199 200 201 202 203 204 205 206 207 208 209 210 211 212 213 214 215 216 217 218 219 220 221 222 223 224 225 226 227 228 229 230 231 232 233 234 235 236 237 238 239 240 241 242 243 244 245 246 247 248 249 250 251 252 253 254 255 256;");
				std::string rawreaddata=readmmap(mapid,0);
				int countrawstrconvert=0;
				int countrawstrconvertb=0;
				std::string internalstringauxread="";
				std::string internalstring="";
				std::string internalstring2="";
				while(countrawstrconvert<rawreaddata.length()){
					countrawstrconvert=countrawstrconvert+1;
					if(rawreaddata[countrawstrconvert-1]==';'){
						break;
					}
				}
				while(countrawstrconvert<rawreaddata.length()){
						if(rawreaddata[countrawstrconvert]!=';'){
							internalstringauxread=internalstringauxread+rawreaddata[countrawstrconvert];
						}else{
							internalstring="";
							internalstring2="";
							int skipauxb=0;
							while(countrawstrconvertb<internalstringauxread.length()){

								if((internalstringauxread[countrawstrconvertb]!='?')&(skipauxb==0)){
									internalstring=internalstring+internalstringauxread[countrawstrconvertb];
								}else{
									if(skipauxb==1){
										internalstring2=internalstring2+internalstringauxread[countrawstrconvertb];
									}
									skipauxb=1;
								}
								countrawstrconvertb=countrawstrconvertb+1;
							}
							if(internalstring!=""){
								if(internalstring!=" "){
									if(internalstring2!=""){
										if(internalstring2!=" "){
											//std::cout<<internalstring<<"\n";
											//std::cout<<internalstring2<<"\n";
											int length = internalstring.length();
											int lengthb = internalstring2.length();
											double estimate=0;
											double returnarray[TEMPDATASIZE];
											double peaks[TEMPDATASIZE];
											int peakscount=0;
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
											getvector(oldN,len,str,xb);
											getvector(oldN,lenb,strb,yb);
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
											high_resolution_clock::time_point t2s=high_resolution_clock::now();
											float durationspectra=duration_cast<microseconds>(t2s-t1s).count();
											high_resolution_clock::time_point t2=high_resolution_clock::now();
											long double duration=duration_cast<nanoseconds>(t2-t1).count();
											if((durationspectra/1000000)>savespectrainterval){
												std::ofstream spectrafile;
												if(savespectra==1){
													spectrafile.open(spectrafilename+"-"+std::to_string((long int)duration)+".dat");
												}
												for (j=0;j<=oldN;j++){
													returnarray[j]=yb[j];
													spectrafile<<xb[j]<<"\t"<<returnarray[j]<<"\n";
												}
												spectrafile.close();
												t1s=high_resolution_clock::now();
											}
											if(savepeaks==1){
												peaksfile<<duration/1000000000<<"\t";
												for (j=0;j<peakscount;j++){
													peaksfile<<peaks[j]<<"\t";
												}
												peaksfile<<"\n";
											}
											length=oldN+1;
											currentnruns=currentnruns+1;
										}
									}
								}
							}
						}
						countrawstrconvert=countrawstrconvert+1;
				}
			}
			peaksfile.close();
		  }
void Init(Local<Object> exports, Local<Object> module) {
	NODE_SET_METHOD(module, "exports", polfit);
}
NODE_MODULE(addon, Init)
