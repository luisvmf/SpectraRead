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
	mapid=connectmmap("spectrareadd","dproc");
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
				//writemmap(mapid,";0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100 101 102 103 104 105 106 107 108 109 110 111 112 113 114 115 116 117 118 119 120 121 122 123 124 125 126 127 128 129 130 131 132 133 134 135 136 137 138 139 140 141 142 143 144 145 146 147 148 149 150 151 152 153 154 155 156 157 158 159 160 161 162 163 164 165 166 167 168 169 170 171 172 173 174 175 176 177 178 179 180 181 182 183 184 185 186 187 188 189 190 191 192 193 194 195 196 197 198 199 200 201 202 203 204 205 206 207 208 209 210 211 212 213 214 215 216 217 218 219 220 221 222 223 224 225 226 227 228 229 230 231 232 233 234 235 236 237 238 239 240 241 242 243 244 245 246 247 248 249 250 251 252 253 254 255 256?0.0 0.663850432328 3.40362893765 8.18075920113 2.2497806941 7.72966109328 23.2252698787 8.6733144822 38.9860024319 30.5649377306 73.673592571 94.7199477825 78.1160223391 108.793341111 81.6438992659 97.2012260673 80.985703027 43.7184637148 38.1577656989 113.648277174 354.215688391 360.137377729 308.764532773 381.846339094 342.779893479 227.76718057 627.159265454 58.0113033448 635.299091238 187.89505542 787.641705748 104.856920053 578.190647747 202.119949196 1071.74422878 1013.46116311 984.562500015 612.022494873 1300.44940531 595.810745318 444.503224591 606.974777192 1452.4784878 1660.22941855 386.054814393 643.449801553 415.384769928 437.789229596 1952.17515908 279.702726121 950.086914431 392.508479878 1622.0635201 2755.15964079 2901.27541402 1305.69682333 2543.56424129 2569.8538432 2481.28001038 452.728424891 3053.59596527 1342.71656511 1312.71020068 3222.13514025 2348.61371869 3526.64628248 3092.34909445 1042.02464642 1161.6574108 3240.0897546 3522.68855672 3782.48067344 3879.75411496 109.403304042 1885.82124085 5612.5657096 4224.34944481 906.125816235 3633.25562006 605.929234294 6230.70573235 4443.16556356 5348.10087561 4980.27395364 1056.51496855 747.313044818 6927.63843932 4363.79114052 1114.54725448 3294.64310186 5067.76933632 5756.86913751 1306.03720539 2817.07039665 2509.90590605 1194.19070288 5857.66214377 7019.81300348 8514.42478556 2689.83227718 1751.82518485 3317.06817577 3142.77371034 4209.00690316 7662.98627402 246.322835196 9774.82292339 400.160723412 442.160724573 732.358768688 2499.61441246 3729.87515551 9699.76863434 9685.48908181 6915.94213993 7186.7379372 8026.29988969 10411.1402976 13257.9452 4718.77828377 3209.12795983 7221.67760923 10999.1895518 6594.79944567 8086.41415681 14789.6685097 5203.69846961 505.662546227 9345.23592226 6556.44099238 978.621061948 9098.96889744 10214.6941761 5116.46480225 12922.2615206 8717.72135054 3864.9969779 10501.3477419 3491.54926126 1381.4944496 7837.75127691 17915.0092623 2766.58231055 1012.09140018 15336.9575416 1546.27020192 4170.23115286 10323.5740182 3815.20497764 3087.76458437 20875.3489141 10545.4543723 5138.08454491 17419.7238528 8192.07865409 680.209316155 23583.2699801 9690.01552102 8979.89808543 16645.4346342 5571.42060346 384.575598063 18381.4413224 5741.56430083 6580.21715496 10132.8117772 5100.76819504 12934.4353469 775.99980407 557.060584708 23650.3202438 28255.673106 24556.3925167 14286.7661764 28933.2507606 2093.07251893 2170.60549652 21956.9041547 23973.8644643 31237.1777083 9058.60953323 6762.51759426 3468.11912059 1775.35032147 8222.1681961 3514.70099402 24615.9883784 19545.4268416 25347.5731908 17878.0902492 11187.561963 18670.9933667 8690.8582136 13743.8512705 36287.8877971 35597.1899362 35652.6180909 36029.3053854 33830.6183651 11138.7336559 28579.9272436 34952.8777299 27405.5975825 27341.1298336 20250.215983 1364.40290296 29816.2825241 1998.7842807 36291.3651509 28702.871353 25207.4454217 36853.3042959 24571.5722043 37258.9566425 19327.4252352 39387.8754047 23124.5679073 38502.4899666 18036.4368961 44042.4076699 20777.16196 116.083881314 22978.1495009 8153.13866435 42408.8081933 8359.76821726 38793.8839469 20832.6607982 50024.9506511 31380.0634663 47411.1469245 41491.2431475 16494.934869 17448.2700989 52891.8112031 8829.91826313 51318.2735071 48151.893527 17339.0337047 17819.720934 56580.7922592 5244.46087898 9906.61135909 52577.2881993 59262.4511097 57414.3077595 32558.8887908 21769.214371 2206.85942309 61088.7781985 12036.4214746 6086.73805595 62390.4718145 15130.1834083 30397.8079457 52321.8771331 41748.0614011; ;0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100 101 102 103 104 105 106 107 108 109 110 111 112 113 114 115 116 117 118 119 120 121 122 123 124 125 126 127 128 129 130 131 132 133 134 135 136 137 138 139 140 141 142 143 144 145 146 147 148 149 150 151 152 153 154 155 156 157 158 159 160 161 162 163 164 165 166 167 168 169 170 171 172 173 174 175 176 177 178 179 180 181 182 183 184 185 186 187 188 189 190 191 192 193 194 195 196 197 198 199 200 201 202 203 204 205 206 207 208 209 210 211 212 213 214 215 216 217 218 219 220 221 222 223 224 225 226 227 228 229 230 231 232 233 234 235 236 237 238 239 240 241 242 243 244 245 246 247 248 249 250 251 252 253 254 255 256?0.0 0.663850432328 3.40362893765 8.18075920113 2.2497806941 7.72966109328 23.2252698787 8.6733144822 38.9860024319 30.5649377306 73.673592571 94.7199477825 78.1160223391 108.793341111 81.6438992659 97.2012260673 80.985703027 43.7184637148 38.1577656989 113.648277174 354.215688391 360.137377729 308.764532773 381.846339094 342.779893479 227.76718057 627.159265454 58.0113033448 635.299091238 187.89505542 787.641705748 104.856920053 578.190647747 202.119949196 1071.74422878 1013.46116311 984.562500015 612.022494873 1300.44940531 595.810745318 444.503224591 606.974777192 1452.4784878 1660.22941855 386.054814393 643.449801553 415.384769928 437.789229596 1952.17515908 279.702726121 950.086914431 392.508479878 1622.0635201 2755.15964079 2901.27541402 1305.69682333 2543.56424129 2569.8538432 2481.28001038 452.728424891 3053.59596527 1342.71656511 1312.71020068 3222.13514025 2348.61371869 3526.64628248 3092.34909445 1042.02464642 1161.6574108 3240.0897546 3522.68855672 3782.48067344 3879.75411496 109.403304042 1885.82124085 5612.5657096 4224.34944481 906.125816235 3633.25562006 605.929234294 6230.70573235 4443.16556356 5348.10087561 4980.27395364 1056.51496855 747.313044818 6927.63843932 4363.79114052 1114.54725448 3294.64310186 5067.76933632 5756.86913751 1306.03720539 2817.07039665 2509.90590605 1194.19070288 5857.66214377 7019.81300348 8514.42478556 2689.83227718 1751.82518485 3317.06817577 3142.77371034 4209.00690316 7662.98627402 246.322835196 9774.82292339 400.160723412 442.160724573 732.358768688 2499.61441246 3729.87515551 9699.76863434 9685.48908181 6915.94213993 7186.7379372 8026.29988969 10411.1402976 13257.9452 4718.77828377 3209.12795983 7221.67760923 10999.1895518 6594.79944567 8086.41415681 14789.6685097 5203.69846961 505.662546227 9345.23592226 6556.44099238 978.621061948 9098.96889744 10214.6941761 5116.46480225 12922.2615206 8717.72135054 3864.9969779 10501.3477419 3491.54926126 1381.4944496 7837.75127691 17915.0092623 2766.58231055 1012.09140018 15336.9575416 1546.27020192 4170.23115286 10323.5740182 3815.20497764 3087.76458437 20875.3489141 10545.4543723 5138.08454491 17419.7238528 8192.07865409 680.209316155 23583.2699801 9690.01552102 8979.89808543 16645.4346342 5571.42060346 384.575598063 18381.4413224 5741.56430083 6580.21715496 10132.8117772 5100.76819504 12934.4353469 775.99980407 557.060584708 23650.3202438 28255.673106 24556.3925167 14286.7661764 28933.2507606 2093.07251893 2170.60549652 21956.9041547 23973.8644643 31237.1777083 9058.60953323 6762.51759426 3468.11912059 1775.35032147 8222.1681961 3514.70099402 24615.9883784 19545.4268416 25347.5731908 17878.0902492 11187.561963 18670.9933667 8690.8582136 13743.8512705 36287.8877971 35597.1899362 35652.6180909 36029.3053854 33830.6183651 11138.7336559 28579.9272436 34952.8777299 27405.5975825 27341.1298336 20250.215983 1364.40290296 29816.2825241 1998.7842807 36291.3651509 28702.871353 25207.4454217 36853.3042959 24571.5722043 37258.9566425 19327.4252352 39387.8754047 23124.5679073 38502.4899666 18036.4368961 44042.4076699 20777.16196 116.083881314 22978.1495009 8153.13866435 42408.8081933 8359.76821726 38793.8839469 20832.6607982 50024.9506511 31380.0634663 47411.1469245 41491.2431475 16494.934869 17448.2700989 52891.8112031 8829.91826313 51318.2735071 48151.893527 17339.0337047 17819.720934 56580.7922592 5244.46087898 9906.61135909 52577.2881993 59262.4511097 57414.3077595 32558.8887908 21769.214371 2206.85942309 61088.7781985 12");
				std::string rawreaddata=readmmap(mapid,1);
				//std::cout<<rawreaddata;
				int countrawstrconvert=0;
				int countrawstrconvertb=0;
				std::string internalstringauxread="";
				std::string internalstring="";
				std::string internalstring2="";

				while(countrawstrconvert<rawreaddata.length()){
				std::cout<<rawreaddata.length()<<"aaa";
					countrawstrconvert=countrawstrconvert+1;
					if(rawreaddata[countrawstrconvert-1]==';'){
						break;
					}
				}

				while(countrawstrconvert<rawreaddata.length()){
						std::cout<<countrawstrconvert<<" "<<rawreaddata.length()<<" "<<rawreaddata[countrawstrconvert]<<"\n";
						internalstringauxread="";
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
