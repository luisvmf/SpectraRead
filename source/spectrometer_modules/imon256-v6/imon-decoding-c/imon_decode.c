#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imon_decoding_utils.h"
//Copyright (c) 2019 Luís Victor Muller Fabris. Apache License.

#define spectrabufferfullsize 100*256*sizeof(char)
char *sb;
char *s;
int currentspacealocatedtosb=-1;
static PyObject* addhexdata(PyObject* self,  PyObject *args) {
	if(buffersinitiated==0){
		char *tempidstringa;
		char *tempidstringb=malloc(900*sizeof(char));
		if(!PyArg_ParseTuple(args, "ss",&s, &tempidstringa)){
			return NULL;
		}
		sprintf(tempidstringb,"imdec%s",tempidstringa);
		initiatebuffers(tempidstringb);
		return Py_BuildValue("i", 0);
	}
	if (!PyArg_ParseTuple(args, "s", &s)) {
		return NULL;
	}
	int l=strlen(s);
	if(l>currentspacealocatedtosb){
		sb=malloc(5*l+9);
		currentspacealocatedtosb=l;
	}
	int i=0;
	int skip=0;
	for(i=0;i!=l;i++){
		sprintf(tempswap,"\\x%02x", (unsigned char)s[i]);
		if(strcmp(tempswap,"\\x01\0")==0){
			sprintf(tempswap,"\\x%02x", (unsigned char)s[i+1]);
			if(strcmp(tempswap,"\\x03\0")==0){
				sprintf(tempswap,"\\x01\0");
				for(int j=0;j<4;j++){
					sb[4*(i-skip)+j]=tempswap[j];
				}
				i=i+1;
				skip=skip+1;
			}else{
				if(strcmp(tempswap,"\\x02\0")==0){
					sprintf(tempswap,"\\x00\0");
					for(int j=0;j<4;j++){
						sb[4*(i-skip)+j]=tempswap[j];
					}
					i=i+1;
					skip=skip+1;
				}else{
					sprintf(tempswap,"\\x01\0");
					for(int j=0;j<4;j++){
						sb[4*(i-skip)+j]=tempswap[j];
					}
				}
			}
		}else{
			for(int j=0;j<4;j++){
				sb[4*(i-skip)+j]=tempswap[j];
			}
		}
	}
	sb[4*(i-skip)]='\0';
	//printf("Received and decoded:%s\n",sb);
	adddatapoints(sb);
	if(spectrabuffercurpos>spectrabufferfullsize){
		senddata();//This function sends data using fastmmapmq and then clears the buffer.
	}
	return Py_BuildValue("i", 0);
}
static PyObject* gethexdata(PyObject* self,  PyObject *args) {
		int i;
		if (!PyArg_ParseTuple(args, "i", &i)) {
			return NULL;
		}
	   return Py_BuildValue("s", convert16bitinttohex(i));
}
static PyObject* addterminator(PyObject* self,  PyObject *args) {
		if(spectrabuffercurpos!=0){
			spectrabuffer[spectrabuffercurpos-1]='?';
			spectrabuffercurpos=spectrabuffercurpos+1;
		}
		return Py_BuildValue("i", 0);
}
//This function is run after aquisition ends or is canceled to make sure no data remains in spectrabuffer or in hexnotconvbuffer.
static PyObject* forcesenddata(PyObject* self,  PyObject *args) {
	senddata();//This function sends data using fastmmapmq and then clears the buffer.
	hexnotconvbuffer[0]='\0';
	return Py_BuildValue("i", 0);
}
static PyMethodDef imondecode_funcs[] = {
	{"addhexdata", (PyCFunction)addhexdata, METH_VARARGS, NULL},
	{"gethexdata", (PyCFunction)gethexdata, METH_VARARGS, NULL},
	{"addterminator", (PyCFunction)addterminator, METH_VARARGS, NULL},
	{"forcesenddata", (PyCFunction)forcesenddata, METH_VARARGS, NULL},
	{ NULL, NULL, 0, NULL}
};
void initimondecode(void) {
	Py_InitModule3("imondecode", imondecode_funcs,"imondecode");
}
