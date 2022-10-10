#include <math.h>
using namespace std; 
//Copyright (c) 2022 Luís Victor Muller Fabris. Apache License.

//fit function was unstable on high orders (spectrum baseline was on -2000) why?. Problem was solved by using point index instead of wavelength by changing line 252 from mem.xbb[j]=xb[j]; to mem.xbb[j]=j;.

//Lets keep memory allocated across function calls insted of doing malloc/free every time for speed.
typedef struct{
	int countplusorder=-1;
	int lastN=-1;
	long double **A=NULL;
	long double **xpower=NULL;
	double *xbb=NULL;
	double *ybb=NULL;
	double *dadosb;
	double *vetorretornaderivada=NULL;
	double vetorretornaderivadalen=-1;
	long double *coefbaseline=NULL;
	int alloccoefbaseline=-1;
	int dadosblen=-1;
	int allocinit=-1;
	double *adjustsection=NULL;
	double *adjustsectionx=NULL;
	double *adjustsectioncoef=NULL;
	int *zeroderivativepoints=NULL;
}allocatedarrays;

allocatedarrays mem;

void boxcarint(double *dados,int dadoslen,int loopselect){
	int contador1=1;
	int N=dadoslen+1000;
	if(mem.dadosblen<N){
		if(mem.dadosblen>0){
			free(mem.dadosb);
		}
	mem.dadosblen=N;
	mem.dadosb=(double *)malloc((N+50000)*sizeof(double));
	}
	if(mem.dadosb==NULL){
		perror("Malloc failed");
		return;
	}
	mem.dadosb[0]=(dados[0]+dados[1])/2;
	if(loopselect==0){
		while(contador1<dadoslen){
			mem.dadosb[contador1]=((dados[contador1-1])+(dados[contador1])+(dados[contador1+1]))/3;
			contador1=contador1+1;
		}
	}else{
		while(contador1<dadoslen){
			mem.dadosb[contador1]=((dados[contador1])+(dados[contador1+1]))/2;
			contador1=contador1+1;
		}
	}
	mem.dadosb[dadoslen]=(dados[dadoslen]+dados[dadoslen-1])/2;
	contador1=0;
	while(contador1<=dadoslen){
		dados[contador1]=mem.dadosb[contador1];
		contador1=contador1+1;
	}
}
void boxcar(double *dados, int boxcarsize,int dadoslen){
	int cnt_boxcar=0;
	int loopselect=0;
	while(cnt_boxcar<boxcarsize){
		boxcarint(dados,dadoslen,loopselect);
		cnt_boxcar=cnt_boxcar+1;
	}
}
void derivada(double *dados,double *dadosx, int ordemderivada,int dadoslen){
	int contador1=0;
	int N=dadoslen+300;
	if(mem.vetorretornaderivadalen<N){
		if(mem.vetorretornaderivadalen>0){
			free(mem.vetorretornaderivada);
		}
	mem.vetorretornaderivadalen=N;
	mem.vetorretornaderivada=(double *)malloc((N+50000)*sizeof(double));
	}
	if(mem.vetorretornaderivada==NULL){
		perror("Malloc failed");
		return;
	}
	while(contador1<dadoslen){
		mem.vetorretornaderivada[contador1]=((dados[contador1+1])-(dados[contador1]))/((dadosx[contador1+1])-(dadosx[contador1]));
		contador1=contador1+1;
	}
	mem.vetorretornaderivada[contador1]=mem.vetorretornaderivada[contador1-1];
	contador1=0;
	while(contador1<=dadoslen){
		dados[contador1]=mem.vetorretornaderivada[contador1];
		contador1=contador1+1;
	}
}
int fit(double* x,double* y,int count,int order, long double* coef){
	if(mem.countplusorder!=(count+order)){
		if(mem.countplusorder>0){
			for(int imat=0;imat<(2*(mem.countplusorder+300));imat++) free(mem.xpower[imat]);
			free(mem.xpower);
			for(int imat=0;imat<(mem.countplusorder+30000);imat++) free(mem.A[imat]);
			free(mem.A);
		}
		mem.countplusorder=count+order;
		mem.A = (long double **)malloc((count+order+30000)*sizeof(long double*));
		for(int imat=0;imat<(count+order+30000);imat++) mem.A[imat]=(long double *)malloc((count+order+30000)*sizeof(long double));
		mem.xpower = (long double **)malloc((2*(count+order+30000))*sizeof(long double*));
		for(int imat=0;imat<(2*(count+order+300));imat++) mem.xpower[imat]=(long double *)malloc((2*(count+order+30000))*sizeof(long double));
	}
	long double ratio=0;
	long double aux;
	int i,j,k,l;
	if(count<=order){
		return -1;
	}
	order=order+1;
	for(i=0;i<count;i++){
		mem.xpower[i][0]=1;
		for(j=1;j<=2*order;j++){
			mem.xpower[i][j]=mem.xpower[i][j-1]*x[i];
		}
	}
	for(i=0;i<order;i++){
		mem.A[i][order]=0;
		for(k=0;k<count;k++){
			mem.A[i][order]=mem.A[i][order]+mem.xpower[k][i]*y[k];
		}
		for(j=0;j<order;j++){
			mem.A[i][j]=0;
			for(k=0;k<count;k++){
				mem.A[i][j]=mem.A[i][j]+mem.xpower[k][j+i];
			}
		}	
	}
	int oldcount=count;
	count=order;
	i=j=k=l=0;
	for (i=0;i<count;i++){
		if(mem.A[i][i]==0){
			l=1;
			while(mem.A[i+l][i]==0&&(i+l)<count){
				l++;
			}
			if ((i+l)==count){
				break;
			}
			for (j=i,k=0;k<=count;k++){
				aux=mem.A[j][k];
				mem.A[j][k]=mem.A[j+l][k];
				mem.A[j+l][k]=aux;
			}
		}
		for (j=0;j<count;j++){
			if (i!=j){
				if(mem.A[i][i]==0){
					return -1;
				}
				ratio=mem.A[j][i]/mem.A[i][i];
				for (k=0;k<=count;k++)
					mem.A[j][k]=mem.A[j][k]-(mem.A[i][k])*ratio;
			}
		}
	}
	for(int i=0;i<=order;i++){
		coef[i]=mem.A[i][count]/mem.A[i][i];
	}
	return 0;
}
void fitbaseline(double *y,double *yb, int order,int N){
	if(order>0){
		int i=0;
		int j=0;
		if(mem.alloccoefbaseline==-1){
			mem.coefbaseline=(long double *)malloc(50000*sizeof(long double));
			mem.alloccoefbaseline=0;
		}
		int res=fit(y,yb,N,order,mem.coefbaseline);
		double aux=0;
		double auxb=0;
		for (i=0;i<=N;i++){
			aux=0;
			auxb=1;
			for (j=0;j<=order;j++){
				aux=aux+auxb*mem.coefbaseline[j];
				auxb=auxb*y[i];
			}
			yb[i]=yb[i]-aux;
		}
	}
}
int processpeaks(double* xb, double* yb, double* peaksng ,int order, int N, int boxcarsizeint,float risingthreshold){
				if(xb==NULL){
					perror("Error.xb points to NULL.");
					return 0;
				}
				if(yb==NULL){
					perror("Error.yb points to NULL.");
					return 0;
				}
				if(peaksng==NULL){
					perror("Error.peaksng points to NULL.");
					return 0;
				}
				if(mem.allocinit==-1){
					mem.adjustsection=(double *)malloc((3+50000)*sizeof(double));
					mem.adjustsectionx=(double *)malloc((3+50000)*sizeof(double));
					mem.adjustsectioncoef=(double *)malloc((3+50000)*sizeof(double));
					mem.zeroderivativepoints=(int *)malloc((256+50000)*sizeof(int));
					if(mem.zeroderivativepoints==NULL)
						return 0;
					if(mem.adjustsectioncoef==NULL)
						return 0;
					if(mem.adjustsectionx==NULL)
						return 0;
					if(mem.adjustsection==NULL)
						return 0;
					mem.allocinit=0;
				}
				int j=0;
				int oldN=N-1;
				if(mem.lastN<N){
					if(mem.lastN>0){
						free(mem.xbb);
						free(mem.ybb);
					}
					mem.lastN=N;
					mem.xbb=(double *)malloc((N+50000)*sizeof(double));
					mem.ybb=(double *)malloc((N+50000)*sizeof(double));
					if(mem.ybb==NULL)
						return 0;
					if(mem.xbb==NULL)
						return 0;
				}
				for (j=0;j<=oldN;j++){
					mem.ybb[j]=yb[j];
					mem.xbb[j]=j;
				}
				boxcar(yb,boxcarsizeint,oldN);
				fitbaseline(mem.xbb,yb,order,N-1);
				double valmaxderivada=yb[0];
				//-------------------------------------------
				//-------------------------------------------
				//Here we update the processed spectrum to the auxiliar arrays "xbb" and "ybb".
				for (j=0;j<=oldN;j++){
					mem.ybb[j]=yb[j];
					mem.xbb[j]=xb[j];
				}
				//-------------------------------------------
				//-------------------------------------------
				derivada(mem.ybb,mem.xbb,1,oldN);
				for (j=0;j<=oldN;j++){
					if(yb[j]>valmaxderivada){
						valmaxderivada=yb[j];
					}
				}
				int peakscountng=0;
				int zeroderivativepointscount=0;
				if(mem.xbb[0]>mem.xbb[1]){//This if is used to handle inverted spectra correctly.
					for (j=4;j<oldN-5;j++){
						if(mem.ybb[j]<0){
							if(mem.ybb[j+1]>0){
								if(yb[j+1]>risingthreshold*valmaxderivada){
								mem.zeroderivativepoints[zeroderivativepointscount]=j;
								zeroderivativepointscount=zeroderivativepointscount+1;
								}
							}
						}
					}
				}else{//This else is used to handle inverted spectra correctly.
					for (j=4;j<oldN-5;j++){
						if(mem.ybb[j]>0){
							if(mem.ybb[j+1]<0){
								if(yb[j+1]>risingthreshold*valmaxderivada){
								mem.zeroderivativepoints[zeroderivativepointscount]=j;
								zeroderivativepointscount=zeroderivativepointscount+1;
								}
							}
						}
					}
				} 
				int subtractpeakcount=0;
				int realpeakscount=0;
				for (j=0;j<zeroderivativepointscount;j++){
							double highpos=yb[mem.zeroderivativepoints[j]-3];
							int jkk=-3;
							int thebigpeakindex=-3;
							while(jkk<4){
								if(yb[mem.zeroderivativepoints[j]+jkk]>highpos){
									highpos=yb[mem.zeroderivativepoints[j]+jkk];
									thebigpeakindex=jkk;
								}
								jkk=jkk+1;
							}
						mem.adjustsection[0]=yb[mem.zeroderivativepoints[j]+thebigpeakindex-1];
						mem.adjustsection[1]=yb[mem.zeroderivativepoints[j]+thebigpeakindex];
						mem.adjustsection[2]=yb[mem.zeroderivativepoints[j]+thebigpeakindex+1];
						//The spectrum should have only positive values for the gaussian fit.
						while(true){
							int changedfdhdhfdfh=1;
							if(mem.adjustsection[0]<=0){
								mem.adjustsection[0]=mem.adjustsection[0]+50000;
								mem.adjustsection[1]=mem.adjustsection[1]+50000;
								mem.adjustsection[2]=mem.adjustsection[2]+50000;
								changedfdhdhfdfh=0;
							}
							if(mem.adjustsection[1]<=0){
								mem.adjustsection[0]=mem.adjustsection[0]+50000;
								mem.adjustsection[1]=mem.adjustsection[1]+50000;
								mem.adjustsection[2]=mem.adjustsection[2]+50000;
								changedfdhdhfdfh=0;
							}
							if(mem.adjustsection[2]<=0){
								mem.adjustsection[0]=mem.adjustsection[0]+50000;
								mem.adjustsection[1]=mem.adjustsection[1]+50000;
								mem.adjustsection[2]=mem.adjustsection[2]+50000;
								changedfdhdhfdfh=0;
							}
							if(changedfdhdhfdfh==1){
								break;
							}
						}
						mem.adjustsectionx[1]=xb[mem.zeroderivativepoints[j]+thebigpeakindex-1];
						mem.adjustsectionx[0]=xb[mem.zeroderivativepoints[j]+thebigpeakindex];
						mem.adjustsectionx[2]=xb[mem.zeroderivativepoints[j]+thebigpeakindex+1];
						//This is the equation for the peak position of a gaussian that passes through points (adjustsectionx[0],adjustsection[0]), (adjustsectionx[1],adjustsection[1]), (adjustsectionx[2],adjustsection[2]).
						double thispeak=(-log(mem.adjustsection[0]/mem.adjustsection[1])*mem.adjustsectionx[2]*mem.adjustsectionx[2]+(log(mem.adjustsection[0]/mem.adjustsection[1])-log(mem.adjustsection[0]/mem.adjustsection[2]))*mem.adjustsectionx[1]*mem.adjustsectionx[1]+log(mem.adjustsection[0]/mem.adjustsection[2])*mem.adjustsectionx[0]*mem.adjustsectionx[0])/(-2*log(mem.adjustsection[0]/mem.adjustsection[1])*mem.adjustsectionx[2]+(2*log(mem.adjustsection[0]/mem.adjustsection[1])-2*log(mem.adjustsection[0]/mem.adjustsection[2]))*mem.adjustsectionx[1]+2*log(mem.adjustsection[0]/mem.adjustsection[2])*mem.adjustsectionx[0]);
double a=((mem.adjustsectionx[2]-mem.adjustsectionx[0])*(mem.adjustsection[1]-mem.adjustsection[0])-(mem.adjustsectionx[1]-mem.adjustsectionx[0])*(mem.adjustsection[2]-mem.adjustsection[0]))*((mem.adjustsectionx[2]-mem.adjustsectionx[0])*(mem.adjustsectionx[1]*mem.adjustsectionx[1]-mem.adjustsectionx[0]*mem.adjustsectionx[0])-(mem.adjustsectionx[1]-mem.adjustsectionx[0])*(mem.adjustsectionx[2]*mem.adjustsectionx[2]-mem.adjustsectionx[0]*mem.adjustsectionx[0]));
double b=((mem.adjustsection[2]-mem.adjustsection[0])/(mem.adjustsectionx[2]-mem.adjustsectionx[0]))-a*(mem.adjustsectionx[2]+mem.adjustsectionx[0]);
double c=mem.adjustsection[0]-a*mem.adjustsectionx[0]*mem.adjustsectionx[0]-b*mem.adjustsectionx[0];
//double thispeak=(-b*b+4*a*c)/(4*a);


						//Test convergence. TODO fix this for the case of normal/ inverted spectra selecting the right if.
						int convergencefailed=1;
						if(thispeak<mem.adjustsectionx[2]){
							if(thispeak>mem.adjustsectionx[1]){
								convergencefailed=0;
							}
						}
						if(thispeak>mem.adjustsectionx[2]){
							if(thispeak<mem.adjustsectionx[1]){
								convergencefailed=0;
							}
						}
						if(convergencefailed==0){
						peaksng[realpeakscount]=thispeak;
						realpeakscount=realpeakscount+1;
						}else{
							peaksng[realpeakscount]=thispeak;
							realpeakscount=realpeakscount+1;
						}
						peakscountng=peakscountng+1;
				}
				peakscountng=peakscountng-subtractpeakcount;
				return peakscountng;
}
