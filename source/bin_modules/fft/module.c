#include <stdio.h>
#include <stdlib.h>
#include "fft.c"
#include <math.h>
#define POINTS 9000

//XXX parece que o array de frequencias retornado não está certo, ver linha 35.

//Call double **data=FFT_get_frequency_spectrum(x,y,&len);
//Receives uniformely sampled signal (x,y) and pointer to length of x or y array.
//Modifies the len pointer content to match the length of the fft result.
//Returns "data" where data[0][i] are the the frequencies and data[1][i] are the magnitudes.
//After used free(data[0]); free(data[1]); free(data);
double **FFT_get_frequency_spectrum(double *x, double *y, int *len){
	if(*len % 2 != 0)
		*len=*len-1;
	double **result=(double **)malloc(2*sizeof(double *));
	double *imag=(double *)malloc(*len*sizeof(double));
	for(int i=0; i< 2;i++) result[i]= (double *)malloc(*len*sizeof(double));
	for (int i = 0; i < *len; i++){
		result[0][i]=x[i];
		result[1][i]=y[i];
		imag[i]=0;
	}
	Fft_transform(result[1],imag, *len-1);
	double max=0;
	for (int i = 0; i < *len; i++){
		result[1][i] = sqrt(pow(result[1][i],2)+pow(imag[i],2));
		if(result[1][i]>max)
			max=result[1][i];
	}
	*len=*len/2;
	double sample_rate=1.0/(result[0][1]-result[0][0]);
	for (int i = 0; i < *len; i++){
		result[0][i]=result[0][i]*sample_rate/(*len);
		result[1][i]=result[1][i]/max;
	}
	free(imag);
	return result;
}


int main(){
	double counter=90;
	double x[POINTS];
	double y[POINTS];
	int i;
	for (i = 0; i < POINTS; i++){
		y[i] = 100.0*sin(0.001*counter*1*i);
		x[i]=1*i;
	}
	int len=POINTS;
	double **result=FFT_get_frequency_spectrum(x,y,&len);
	for (i = 0; i < len; i++){
		printf("%f %f\n",result[0][i],result[1][i]);
	}
	free(result[0]);
	free(result[1]);
	free(result);
}
