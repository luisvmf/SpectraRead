#include <nan.h>
#include <string>
#include <iostream>
#include <chrono>
using namespace std::chrono;

#define MAXDATASIZE 3000
void boxcarint(double *dados,int dadoslen){
	int contador1=1;
	while(contador1<dadoslen-1){
		dados[contador1]=((dados[contador1-1])+(dados[contador1])+(dados[contador1+1]))/3;
		dados[0]=dados[2];
		contador1=contador1+1;
	}
	dados[dadoslen]=dados[dadoslen-2];
	dados[dadoslen-1]=dados[dadoslen-2];
}
void boxcar(double *dados, int boxcarsize,int dadoslen){
	int cnt_boxcar=0;
	while(cnt_boxcar<boxcarsize){
		boxcarint(dados,dadoslen);
		cnt_boxcar=cnt_boxcar+1;
	}
}
void derivada(double *dados,double *dadosx, int ordemderivada,int dadoslen){
	int contador1=0;
	double vetorretornaderivada[dadoslen];
	while(contador1<dadoslen-1){
		vetorretornaderivada[contador1]=((dados[contador1+1])-(dados[contador1]))/((dadosx[contador1+1])-(dadosx[contador1]));
		contador1=contador1+1;
	}
	vetorretornaderivada[contador1-1]=vetorretornaderivada[contador1-2];
	contador1=0;
	while(contador1<dadoslen){
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

void fitbaseline(float *y,double *yb, int order,int N,int reductionfactor){
	int i=0;
	for (i=0;i<N;i++){
		y[i]=yb[i*reductionfactor];
	}
	i=0;
	long int powaux=1;
	int n = order;
	//Some parts of the code bellow were adapted from http://www.bragitoff.com/2015/09/c-program-for-polynomial-fit-least-squares/
	int j,k,l;
	double aux;
	double X[2*n+1];
	double B[n+1][n+2],a[n+1];
	double Y[n+1];
	for (i=0;i<n+1;i++){
		for (j=0;j<n+2;j++){
			B[i][j]=0;
		}
	a[i]=0;
	Y[i]=0;
	}
	for (i=0;i<2*n+1;i++){
		X[i]=0;
		for (j=0;j<N;j++)
			X[i]=X[i]+pow(j,i);
	}
	for (i=0;i<=n;i++){
		Y[i]=0;
		for (j=0;j<=n;j++)
			B[i][j]=X[i+j];
		for (j=0;j<N;j++)
	   		 Y[i]=Y[i]+pow(j,i)*y[j];
		B[i][n+1]=Y[i];
	}
	l=n+1;
	for (i=0;i<=n;i++)
		for (k=i+1;k<=n;k++)
			if (B[i][i]<B[k][i])
			    for (j=0;j<=n+1;j++){
			        aux=B[i][j];
			        B[i][j]=B[k][j];
			        B[k][j]=aux;
			    }
	for (i=0;i<n;i++)
		for (k=i+1;k<=n;k++){
			    aux=B[k][i]/B[i][i];
			    for (j=0;j<=n+1;j++)
			        B[k][j]=B[k][j]-aux*B[i][j];
			}
	for (i=n;i>=0;i--){
		a[i]=B[i][l];
		for (j=0;j<=n;j++)
			if (j!=i)
			    a[i]=a[i]-B[i][j]*a[j];
		a[i]=a[i]/B[i][i];
	}
	powaux=1;
	for (j=0;j<N;j++){
		aux=0;
		y[j]=0;
		powaux=1;
		for (i=0;i<=n;i++){
			//y[j]=y[j]+a[i]*pow(j,i);
			y[j]=y[j]+a[i]*powaux;
			powaux=powaux*j;
			//std::cout<<y[j]<<"\n";
		}
	}
}

void removebaseline(float *y,double *yb, int oldN,int reductionfactor){
	int i,j,cntintnodestring,cntintnodestringb;
	cntintnodestring=0;
	cntintnodestringb=0;
		for (j=0;j<oldN;j++){
			yb[j]=yb[j]-y[cntintnodestringb];
			cntintnodestring=cntintnodestring+1;
			if(cntintnodestring>=reductionfactor){
				cntintnodestring=0;
				cntintnodestringb=cntintnodestringb+1;
			}

		}
}
