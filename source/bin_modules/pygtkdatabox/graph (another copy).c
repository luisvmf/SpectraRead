#include <Python.h>
#include <pygobject.h>
#include <stdio.h>

#include <gtk/gtk.h>
#include "gtkdatabox/install/include/gtkdatabox.h"
#include "gtkdatabox/install/include/gtkdatabox_points.h"
#include "gtkdatabox/install/include/gtkdatabox_lines.h"
#include "gtkdatabox/install/include/gtkdatabox_util.h"
#include "gtkdatabox/install/include/gtkdatabox_grid.h"
#include "gtkdatabox/install/include/gtkdatabox_regions.h"
#include "gtkdatabox/install/include/gtkdatabox_markers.h"
#include <math.h>
#include <time.h>
#include "fft.c"


#define POINTS 999999 //This is the maximum number of points that can be shown in the graph.


#define  FRAME_RATE        7 //This is the minimum frame rate. The graph is also drawn on idle (g_idle_add_full).
						 	  // If this is set to high the window may not appear/ the graph can freeze.

#define  FRAME_RATEb        7 //This is the minimum frame rate. The graph is also drawn on idle (g_idle_add_full).
						 	  // If this is set to high the window may not appear/ the graph can freeze.

#define resizepercentage 1.0 //This is how much percent the scale must be off in relation to graph to trigger a resize.


#define borderporcent 3.0 //The white space between the graph and legend in percentage of the size of the graph.


#define popuptime 1.0 //In seconds.

int drawpoints=0;
int drawpointspeaks=0;
static gfloat *X = NULL;
static gfloat *Y = NULL;
static gfloat *Xpeaks = NULL;
static gfloat *Ypeaks = NULL;
static gfloat *Xreg = NULL;
static gfloat *Yreg = NULL;
static gfloat *Yregb = NULL;
static gfloat *Xlim = NULL;
static gfloat *Ylim = NULL;
static gfloat *Xregp = NULL;
static gfloat *Yregp = NULL;
static gfloat *Yregbp = NULL;
GtkDataboxGraph *marker=NULL;
char *label=NULL;
int firstup=0;
static gint timeoutid = 0;
static gint timeoutidb = 0;
int update=1;
gfloat xyxy[4];
gfloat txyxy[4];
static guint counter = 0;
int mode=0; //0=autoscale, 1=zoom
GtkWidget *box;
float xminprev=0;
float yminprev=0;
float xmaxprev=0;
float ymaxprev=0;
int lastmousexpos=0;
int lastmouseypos=0;
long long int lastmouseinsidetime=0;
int setlimnext=0;
int hasupdated=1;
GtkDataboxGraph *graphgrid;
GtkDataboxGraph *graphregionbox;

GtkDataboxGraph *graphbrangea;
GtkDataboxGraph *graphrangea;
GtkDataboxGraph *graphpeaksrangea;

GtkDataboxGraph *graphbrangeb;
GtkDataboxGraph *graphrangeb;
GtkDataboxGraph *graphpeaksrangeb;

GtkDataboxGraph *graphbrangec;
GtkDataboxGraph *graphrangec;
GtkDataboxGraph *graphpeaksrangec;

GtkDataboxGraph *graphbranged;
GtkDataboxGraph *graphranged;
GtkDataboxGraph *graphpeaksranged;
//Performance optimization, number of points on graph according with data provided.
void enablegraphnpoints(int n){
n=90000;
	if(n<500){
		gtk_databox_graph_set_hide(graphpeaksrangea,FALSE);
		gtk_databox_graph_set_hide(graphpeaksrangeb,TRUE);
		gtk_databox_graph_set_hide(graphpeaksrangec,TRUE);
		gtk_databox_graph_set_hide(graphpeaksranged,TRUE);

		gtk_databox_graph_set_hide(graphbrangea,FALSE);
		gtk_databox_graph_set_hide(graphbrangeb,TRUE);
		gtk_databox_graph_set_hide(graphbrangec,TRUE);
		gtk_databox_graph_set_hide(graphbranged,TRUE);

		gtk_databox_graph_set_hide(graphrangea,FALSE);
		gtk_databox_graph_set_hide(graphrangeb,TRUE);
		gtk_databox_graph_set_hide(graphrangec,TRUE);
		gtk_databox_graph_set_hide(graphranged,TRUE);
	}
	if(n>=500){
		if(n<3000){
			gtk_databox_graph_set_hide(graphpeaksrangea,TRUE);
			gtk_databox_graph_set_hide(graphpeaksrangeb,FALSE);
			gtk_databox_graph_set_hide(graphpeaksrangec,TRUE);
			gtk_databox_graph_set_hide(graphpeaksranged,TRUE);

			gtk_databox_graph_set_hide(graphbrangea,TRUE);
			gtk_databox_graph_set_hide(graphbrangeb,FALSE);
			gtk_databox_graph_set_hide(graphbrangec,TRUE);
			gtk_databox_graph_set_hide(graphbranged,TRUE);

			gtk_databox_graph_set_hide(graphrangea,TRUE);
			gtk_databox_graph_set_hide(graphrangeb,FALSE);
			gtk_databox_graph_set_hide(graphrangec,TRUE);
			gtk_databox_graph_set_hide(graphranged,TRUE);
		}
	}
	if(n>=3000){
		if(n<9000){
			gtk_databox_graph_set_hide(graphpeaksrangea,TRUE);
			gtk_databox_graph_set_hide(graphpeaksrangeb,TRUE);
			gtk_databox_graph_set_hide(graphpeaksrangec,FALSE);
			gtk_databox_graph_set_hide(graphpeaksranged,TRUE);

			gtk_databox_graph_set_hide(graphbrangea,TRUE);
			gtk_databox_graph_set_hide(graphbrangeb,TRUE);
			gtk_databox_graph_set_hide(graphbrangec,FALSE);
			gtk_databox_graph_set_hide(graphbranged,TRUE);

			gtk_databox_graph_set_hide(graphrangea,TRUE);
			gtk_databox_graph_set_hide(graphrangeb,TRUE);
			gtk_databox_graph_set_hide(graphrangec,FALSE);
			gtk_databox_graph_set_hide(graphranged,TRUE);
		}
	}
	if(n>=9000){
		gtk_databox_graph_set_hide(graphpeaksrangea,TRUE);
		gtk_databox_graph_set_hide(graphpeaksrangeb,TRUE);
		gtk_databox_graph_set_hide(graphpeaksrangec,TRUE);
		gtk_databox_graph_set_hide(graphpeaksranged,FALSE);

		gtk_databox_graph_set_hide(graphbrangea,TRUE);
		gtk_databox_graph_set_hide(graphbrangeb,TRUE);
		gtk_databox_graph_set_hide(graphbrangec,TRUE);
		gtk_databox_graph_set_hide(graphbranged,FALSE);

		gtk_databox_graph_set_hide(graphrangea,TRUE);
		gtk_databox_graph_set_hide(graphrangeb,TRUE);
		gtk_databox_graph_set_hide(graphrangec,TRUE);
		gtk_databox_graph_set_hide(graphranged,FALSE);
	}
}

float absolute(float x){
	if(x<0)
		x=x*(-1);
	return x;
}
static gint handle_signal_selection_finalized(GtkDatabox * box,GtkDataboxValueRectangle * selectionValues ){
	if(mode==1){
		hasupdated=1;
		xyxy[0]=selectionValues->x1;
		xyxy[1]=selectionValues->y1;
		xyxy[2]=selectionValues->x2;
		xyxy[3]=selectionValues->y2;
		gfloat changevar;
		if(xyxy[0]>xyxy[2]){
			changevar=xyxy[0];
			xyxy[0]=xyxy[2];
			xyxy[2]=changevar;
		}
		if(xyxy[1]<xyxy[3]){
			changevar=xyxy[1];
			xyxy[1]=xyxy[3];
			xyxy[3]=changevar;
		}
		gtk_databox_ruler_set_draw_position(gtk_databox_get_ruler_x(GTK_DATABOX(box)),FALSE);
		gtk_databox_ruler_set_draw_position(gtk_databox_get_ruler_y(GTK_DATABOX(box)),FALSE);
		update=1;
	}
   return 0;
}

static gint handle_signal_selection_started(GtkDatabox * box ){
	if(mode==1){
		hasupdated=1;
		sprintf(label,"\n");
		gtk_databox_markers_set_label(GTK_DATABOX_MARKERS(marker),0,GTK_DATABOX_MARKERS_TEXT_E,label,FALSE);
		if(GTK_IS_DATABOX(box))
			gtk_widget_queue_draw (GTK_WIDGET (box));
		gtk_databox_ruler_set_draw_position(gtk_databox_get_ruler_x(GTK_DATABOX(box)),TRUE);
		gtk_databox_ruler_set_draw_position(gtk_databox_get_ruler_y(GTK_DATABOX(box)),TRUE);
		update=0;
	}
   return 0;
}
static gint handle_signal_selection_canceled(GtkDatabox * box ){
	if(mode==1){
		hasupdated=1;
		gtk_databox_ruler_set_draw_position(gtk_databox_get_ruler_x(GTK_DATABOX(box)),FALSE);
		gtk_databox_ruler_set_draw_position(gtk_databox_get_ruler_y(GTK_DATABOX(box)),FALSE);
		update=1;
	}
   return 0;
}
static gint handle_signal_click(GtkDatabox * box ,GdkEventButton * event){
	hasupdated=1;
	if(mode==1){
		if(event->button==3){
		Xlim[0]=X[0];
		Xlim[1]=X[0];
		Ylim[0]=Y[0];
		Ylim[1]=Y[0];
		float xmin=X[0];
		float xmax=X[0];
		float ymin=Y[0];
		float ymax=Y[0];
  		 for (int i = 0; i < drawpoints; i++){
			if(X[i]>xmax)
				xmax=X[i];
			if(X[i]<xmin)
				xmin=X[i];
			if(Y[i]>ymax)
				ymax=Y[i];
			if(Y[i]<ymin)
				ymin=Y[i];
		}
		if(isnan(xmin))
			xmin=0;
		if(isnan(xmax))
			xmax=xmin+0.5;
		if(isnan(ymin))
			ymin=0;
		if(isnan(ymax))
			ymax=ymin+0.5;
		if(xmax<=xmin)
			xmax=xmin+0.5;
		if(ymax<=ymin)
			ymax=ymin+0.5;
		xmin=xmin-(borderporcent/100.0)*absolute(xmin-xmax);
		xmax=xmax+(borderporcent/100.0)*absolute(xmin-xmax);
		ymin=ymin-(borderporcent/100.0)*absolute(ymin-ymax);
		ymax=ymax+(borderporcent/100.0)*absolute(ymin-ymax);
		xyxy[0]=xmin;
		xyxy[2]=xmax;
		xyxy[1]=ymax;
		xyxy[3]=ymin;

		}
	}
   return 0;
}


static gint motion_notify(GtkDatabox * boxb, GdkEventButton * event ){
	if(update==0)
		hasupdated=1;
	lastmousexpos=event->x;
	lastmouseypos=event->y;
	time_t starttime;
    time(&starttime);
	lastmouseinsidetime=starttime;
	if(update==1){
		updateinfobox();
		gtk_widget_queue_draw (GTK_WIDGET (box));
	}
   return 0;
}



void updateinfobox(void){
	if(!GTK_IS_DATABOX(box))
		return;
	double realxpos=gtk_databox_pixel_to_value_x (GTK_DATABOX(box),lastmousexpos);
	double realypos=gtk_databox_pixel_to_value_y (GTK_DATABOX(box),lastmouseypos);
	time_t starttime;
	time(&starttime);
	long long int internaltime=starttime;
	if(absolute(internaltime-lastmouseinsidetime)<popuptime){
		Xregp[0]=realxpos;
		Xregp[1]=realxpos;
		Yregp[0]=realypos;
		Yregbp[0]=realypos;
		Yregp[1]=realypos;
		Yregbp[1]=realypos;
		sprintf(label,"x:%f\ny:%f",realxpos,realypos);
		gtk_databox_markers_set_label(GTK_DATABOX_MARKERS(marker),0,GTK_DATABOX_MARKERS_TEXT_E,label,FALSE);
	}else{
		Xregp[0]=0;
		Xregp[1]=0;
		Yregp[0]=0;
		Yregbp[0]=0;
		Yregp[1]=0;
		Yregbp[1]=0;
		sprintf(label,"\n");
		gtk_databox_markers_set_label(GTK_DATABOX_MARKERS(marker),0,GTK_DATABOX_MARKERS_TEXT_E,label,FALSE);
	}
}


static gboolean refresh_label_timeout(void){
	updateinfobox();
	if(!GTK_IS_DATABOX(box))
		return FALSE;
	if(update==1)
		gtk_widget_queue_draw (GTK_WIDGET (box));
	return FALSE;
}

PyObject *obj;
PyObject *objb;
PyObject *iter;
PyObject *iterb;
PyObject *next;



static gboolean update_graph (void){
   gint i;
   if (!GTK_IS_DATABOX (box))
      return FALSE;
	enablegraphnpoints(drawpoints);
	if(hasupdated==0){
		usleep(1000);
		return FALSE;
	}
	hasupdated=0;
	if(update==1){
//	double real[POINTS*2];
//	double img[POINTS*2];
 //  for (i = 0; i < POINTS*2; i++){
//		real[i] = 100.0*sin(0.001*counter*i)+100.0*sin(0.002*counter*i)+100.0*sin(0.003*counter*i)+100.0*sin(0.01*counter*i+counter/100.0)+(rand()/RAND_MAX)*1000.0;
//		img[i]=0;
//	}
//	Fft_transform(real,img, POINTS*2-2);
	//double max=0;
  // for (i = 0; i < POINTS; i++)
 //  {
  //    X[i] = i;
  //    Y[i] = sqrt(pow(real[i],2)+pow(img[i],2));
	//	if(Y[i]>max)
	//		max=Y[i];
  // }
  // for (i = 0; i < POINTS; i++){
	//	Y[i]=Y[i]/max/(counter+1.0);
	//}
	for (i = drawpoints; i < POINTS; i++){
		X[i]=X[drawpoints-1];
		Y[i]=Y[drawpoints-1];
	}
	if(mode==0){
		float xmin=X[0];
		float xmax=X[0];
		float ymin=Y[0];
		float ymax=Y[0];	
  		 for (i = 0; i < drawpoints; i++){
			if(X[i]>xmax)
				xmax=X[i];
			if(X[i]<xmin)
				xmin=X[i];
			if(Y[i]>ymax)
				ymax=Y[i];
			if(Y[i]<ymin)
				ymin=Y[i];
		}
		if(isnan(xmin))
			xmin=0;
		if(isnan(xmax))
			xmax=xmin+0.5;
		if(isnan(ymin))
			ymin=0;
		if(isnan(ymax))
			ymax=ymin+0.5;
		if(xmax<=xmin)
			xmax=xmin+0.5;
		if(ymax<=ymin)
			ymax=ymin+0.5;
		xmin=xmin-(borderporcent/100.0)*absolute(xmin-xmax);
		xmax=xmax+(borderporcent/100.0)*absolute(xmin-xmax);
		ymin=ymin-(borderporcent/100.0)*absolute(ymin-ymax);
		ymax=ymax+(borderporcent/100.0)*absolute(ymin-ymax);
		Xreg[0]=xmin-(20.0/100.0)*absolute(xmin-xmax);
		Xreg[1]=xmax+(20.0/100.0)*absolute(xmin-xmax);
		Yreg[0]=ymax+(20.0/100.0)*absolute(ymin-ymax);
		Yregb[0]=ymin-(20.0/100.0)*absolute(ymin-ymax);
		Yreg[1]=ymax+(20.0/100.0)*absolute(ymin-ymax);
		Yregb[1]=ymin-(20.0/100.0)*absolute(ymin-ymax);
		xyxy[0]=xmin;
		xyxy[2]=xmax;
		xyxy[1]=ymax;
		xyxy[3]=ymin;
		Xlim[0]=xyxy[0];
		Xlim[1]=xyxy[2];
		Ylim[0]=xyxy[1];
		Ylim[1]=xyxy[3];
		if(firstup==0){
			//gtk_databox_get_total_limits(GTK_DATABOX(box),&xyxy[0],&xyxy[2],&xyxy[1],&xyxy[3]);
			xminprev=xmin;
			yminprev=ymin;
			xmaxprev=xmax;
			ymaxprev=ymax;
			gtk_databox_set_total_limits(GTK_DATABOX(box),(xmin),(xmax),(ymax),(ymin));
			gtk_databox_set_visible_limits(GTK_DATABOX(box),(xmin),(xmax),(ymax),(ymin));
			updateinfobox();
			firstup=1;
		}
		if( (setlimnext==1) || (absolute(xmin-xminprev)>((resizepercentage/100.0)*(absolute(xmax-xmin)))) || (absolute(xmax-xmaxprev)>((resizepercentage/100.0)*(absolute(xmax-xmin)))) || (absolute(ymin-yminprev)>((resizepercentage/100.0)*(absolute(ymax-ymin)))) || (absolute(ymax-ymaxprev)>((resizepercentage/100.0)*(absolute(ymax-ymin))))){
			gtk_databox_set_total_limits(GTK_DATABOX(box),(xmin),(xmax),(ymax),(ymin));
			gtk_databox_set_visible_limits(GTK_DATABOX(box),(xmin),(xmax),(ymax),(ymin));
			xminprev=xmin;
			yminprev=ymin;
			xmaxprev=xmax;
			ymaxprev=ymax;
			setlimnext=0;
			updateinfobox();
		}
		gtk_widget_queue_draw (GTK_WIDGET (box));
	}else{
		if(isnan(xyxy[0]))
			xyxy[0]=0;
		if(isnan(xyxy[2]))
			xyxy[2]=xyxy[0]+0.5;
		if(isnan(xyxy[1]))
			xyxy[1]=0;
		if(isnan(xyxy[3]))
			xyxy[3]=xyxy[1]+0.5;
		if(xyxy[0]>=xyxy[2])
			xyxy[0]=xyxy[2]-0.5;
		if(xyxy[1]<=xyxy[3])
			xyxy[1]=xyxy[3]+0.5;
		Xlim[0]=xyxy[0];
		Xlim[1]=xyxy[2];
		Ylim[0]=xyxy[1];
		Ylim[1]=xyxy[3];
		float xmin=X[0];
		float xmax=X[0];
		float ymin=Y[0];
		float ymax=Y[0];
		xmin=xyxy[0];
		xmax=xyxy[2];
		ymax=xyxy[1];
		ymin=xyxy[3];
		Xreg[0]=xmin-(20.0/100.0)*absolute(xmin-xmax);
		Xreg[1]=xmax+(20.0/100.0)*absolute(xmin-xmax);
		Yreg[0]=ymax+(20.0/100.0)*absolute(ymin-ymax);
		Yregb[0]=ymin-(20.0/100.0)*absolute(ymin-ymax);;
		Yreg[1]=ymax+(20.0/100.0)*absolute(ymin-ymax);;
		Yregb[1]=ymin-(20.0/100.0)*absolute(ymin-ymax);;
		gtk_databox_set_total_limits(GTK_DATABOX(box),(xyxy[0]),(xyxy[2]),(xyxy[1]),(xyxy[3]));
		gtk_databox_set_visible_limits(GTK_DATABOX(box),(xyxy[0]),(xyxy[2]),(xyxy[1]),(xyxy[3]));
		gtk_widget_queue_draw (GTK_WIDGET (box));
		if(firstup==0){
			gtk_databox_get_total_limits(GTK_DATABOX(box),&xyxy[0],&xyxy[2],&xyxy[1],&xyxy[3]);
			xminprev=xmin;
			yminprev=ymin;
			xmaxprev=xmax;
			ymaxprev=ymax;
			firstup=1;
		}
	}
	counter++;
	counter++;
	counter++;
	if(counter>300)
		counter=0;
	}
   return FALSE;
}

static PyObject *updategraph(PyObject*self,PyObject*args){
	hasupdated=1;
	int pupdate=0;
	if(!PyArg_ParseTuple(args,"OOi",&obj,&objb,&pupdate)){
		goto err;
	}
	drawpoints=pupdate;
	iter=PyObject_GetIter(obj);
	iterb=PyObject_GetIter(objb);
	if(!iter){
		goto err;
	}
	if(!iterb){
		goto err;
	}
	int i=0;
	while(1){
		next=PyIter_Next(iter);
		if(!next){
			break;
		}
		if(!PyFloat_Check(next)){
			//break;
		}
		double foo=PyFloat_AsDouble(next);
		Py_DECREF(next);// Prevent memory leak.
		X[i]=foo;
		i=i+1;
	}
	i=0;
	while(1){
		next=PyIter_Next(iterb);
		if(!next){
			break;
		}
		if(!PyFloat_Check(next)){
			//break;
		}
		double foo=PyFloat_AsDouble(next);
  		Py_DECREF(next);// Prevent memory leak.
		Y[i]=foo;
		i=i+1;
	}
 	Py_DECREF(obj);// Prevent memory leak.
 	Py_DECREF(objb);// Prevent memory leak.
	Py_DECREF(iter);// Prevent memory leak.
 	Py_DECREF(iterb);// Prevent memory leak.
   timeoutid = g_timeout_add (1000/FRAME_RATE,(GSourceFunc)update_graph, NULL);
   timeoutidb = g_timeout_add (1000.0/FRAME_RATEb,(GSourceFunc)refresh_label_timeout, NULL);
	return Py_BuildValue("s","");
	err:
		PyErr_SetString(PyExc_TypeError,"Error on update");
		return NULL;
}

static PyObject *refresh_graph(PyObject*self,PyObject*args){
   timeoutid = g_timeout_add (1000/FRAME_RATE,(GSourceFunc)update_graph, NULL);
   timeoutidb = g_timeout_add (1000.0/FRAME_RATEb,(GSourceFunc)refresh_label_timeout, NULL);
	return Py_BuildValue("s","");
}







static PyObject *update_peaks(PyObject*self,PyObject*args){
	hasupdated=1;
	int pupdate=0;
	if(!PyArg_ParseTuple(args,"OOi",&obj,&objb,&pupdate)){
		goto err;
	}
	if(pupdate==0){
		for (int i = 0; i < POINTS; i++){
		Xpeaks[i]=X[0]-borderporcent*X[0]-30;
		Ypeaks[i]=Y[0]-borderporcent*Y[0]-30;
		}
 		Py_DECREF(obj);// Prevent memory leak.
 		Py_DECREF(objb);// Prevent memory leak.
		return Py_BuildValue("s","");
	}
	drawpointspeaks=pupdate;
	iter=PyObject_GetIter(obj);
	iterb=PyObject_GetIter(objb);
	if(!iter){
		goto err;
	}
	if(!iterb){
		goto err;
	}
	int i=0;
	while(1){
		next=PyIter_Next(iter);
		if(!next){
			break;
		}
		if(!PyFloat_Check(next)){
			//break;
		}
		double foo=PyFloat_AsDouble(next);
		Py_DECREF(next);// Prevent memory leak.
		Xpeaks[i]=foo;
		i=i+1;
	}
	i=0;
	while(1){
		next=PyIter_Next(iterb);
		if(!next){
			break;
		}
		if(!PyFloat_Check(next)){
			//break;
		}
		double foo=PyFloat_AsDouble(next);
  		Py_DECREF(next);// Prevent memory leak.
		Ypeaks[i]=foo;
		i=i+1;
	}
	for (i = drawpointspeaks; i < POINTS; i++){
		Xpeaks[i]=Xpeaks[drawpointspeaks-1];
		Ypeaks[i]=Ypeaks[drawpointspeaks-1];
	}
 	Py_DECREF(obj);// Prevent memory leak.
 	Py_DECREF(objb);// Prevent memory leak.
	Py_DECREF(iter);// Prevent memory leak.
 	Py_DECREF(iterb);// Prevent memory leak.
	return Py_BuildValue("s","");
	err:
		PyErr_SetString(PyExc_TypeError,"Error on update");
		return NULL;
}












static void create_graph (GtkWidget *box1,float *linecolor, float *pointcolor,float *textcolor,int darktheme){
   GtkWidget *table;
   GtkDataboxGraph *graphlim;
   GdkRGBA color;
	gtk_databox_create_box_with_scrollbars_and_rulers_positioned(&box,&table,FALSE,FALSE,TRUE,TRUE,FALSE,TRUE);
		gtk_databox_ruler_set_text_orientation(gtk_databox_get_ruler_y(GTK_DATABOX(box)),GTK_ORIENTATION_HORIZONTAL);
		gtk_databox_ruler_set_invert_edge(gtk_databox_get_ruler_x(GTK_DATABOX(box)),FALSE);
   g_object_set(G_OBJECT(box), "expand", TRUE, NULL);
   gtk_box_pack_start (GTK_BOX (box1), table, TRUE, TRUE, 0);
   color.red = linecolor[0];
   color.green = linecolor[1];
   color.blue = linecolor[2];
   color.alpha = 1;
	gtk_databox_set_enable_selection(GTK_DATABOX(box),FALSE);
	gtk_databox_set_enable_zoom(GTK_DATABOX(box),FALSE);
	gtk_databox_ruler_set_draw_position(gtk_databox_get_ruler_x(GTK_DATABOX(box)),FALSE);
	gtk_databox_ruler_set_draw_position(gtk_databox_get_ruler_y(GTK_DATABOX(box)),FALSE);
   X = g_new0 (gfloat, POINTS);
   Y = g_new0 (gfloat, POINTS);
   Xpeaks = g_new0 (gfloat, POINTS);
   Ypeaks = g_new0 (gfloat, POINTS);
	for (int i = 0; i < POINTS; i++){
		X[i]=i;
		Y[i]=0;
		Xpeaks[i]=i;
		Ypeaks[i]=0;
	}
   Xlim = g_new0 (gfloat, 2);
   Ylim = g_new0 (gfloat, 2);
	Xlim[0]=0;
	Ylim[0]=0;
	Xlim[1]=0;
	Ylim[1]=0;


   color.red = 0;
   color.green = 0;
   color.blue = 0;
   color.alpha = 0;
   graphlim = gtk_databox_points_new (2, Xlim, Ylim, &color, 1);
   color.red = linecolor[0];
   color.green = linecolor[1];
   color.blue = linecolor[2];
   color.alpha = 1;
   graphrangea = gtk_databox_points_new (600, X, Y, &color, 3);
   graphbrangea = gtk_databox_lines_new (600, X, Y, &color, 1);
   graphrangeb = gtk_databox_points_new (4000, X, Y, &color, 3);
   graphbrangeb = gtk_databox_lines_new (4000, X, Y, &color, 1);
   graphrangec = gtk_databox_points_new (10000, X, Y, &color, 3);
   graphbrangec = gtk_databox_lines_new (10000, X, Y, &color, 1);
   graphranged = gtk_databox_points_new (POINTS, X, Y, &color, 3);
   graphbranged = gtk_databox_lines_new (POINTS, X, Y, &color, 1);
   color.red = pointcolor[0];
   color.green = pointcolor[1];
   color.blue = pointcolor[2];
   color.alpha = 1;
   graphpeaksrangea = gtk_databox_points_new (600, Xpeaks, Ypeaks, &color, 5);
   graphpeaksrangeb = gtk_databox_points_new (4000, Xpeaks, Ypeaks, &color, 5);
   graphpeaksrangec = gtk_databox_points_new (10000, Xpeaks, Ypeaks, &color, 5);
   graphpeaksranged = gtk_databox_points_new (POINTS, Xpeaks, Ypeaks, &color, 5);
	gtk_databox_graph_add (GTK_DATABOX (box), graphpeaksrangea);
   gtk_databox_graph_add (GTK_DATABOX (box), graphrangea);
   gtk_databox_graph_add (GTK_DATABOX (box), graphbrangea);
	gtk_databox_graph_add (GTK_DATABOX (box), graphpeaksrangeb);
   gtk_databox_graph_add (GTK_DATABOX (box), graphrangeb);
   gtk_databox_graph_add (GTK_DATABOX (box), graphbrangeb);
	gtk_databox_graph_add (GTK_DATABOX (box), graphpeaksrangec);
   gtk_databox_graph_add (GTK_DATABOX (box), graphrangec);
   gtk_databox_graph_add (GTK_DATABOX (box), graphbrangec);
	gtk_databox_graph_add (GTK_DATABOX (box), graphpeaksranged);
   gtk_databox_graph_add (GTK_DATABOX (box), graphranged);
   gtk_databox_graph_add (GTK_DATABOX (box), graphbranged);
   gtk_databox_graph_add (GTK_DATABOX (box), graphlim);
	color.red = textcolor[0];
	color.green = textcolor[1];
	color.blue = textcolor[2];
   color.alpha = 0.3;
   graphgrid = gtk_databox_grid_new (7, 7, &color, 1);
   gtk_databox_graph_add (GTK_DATABOX (box), graphgrid);
   gtk_databox_grid_set_line_style (GTK_DATABOX_GRID(graphgrid), GTK_DATABOX_GRID_DOTTED_LINES);
	if(darktheme==1){
	   color.red = 1;
	   color.green = 1;
	   color.blue = 1;
	   color.alpha = 0.2;
	}else{
	   color.red = 1;
	   color.green = 1;
	   color.blue = 1;
	   color.alpha = 0.7;
	}
   Xreg= g_new0 (gfloat, 2);
   Yreg = g_new0 (gfloat, 2);
   Yregb = g_new0 (gfloat, 2);
	Xreg[0]=0;
	Xreg[1]=1000;
	Yreg[0]=0.005;
	Yregb[0]=0.003;
	Yreg[1]=0.005;
	Yregb[1]=0.003;
    graphregionbox = gtk_databox_regions_new (2,Xreg ,Yreg,Yregb, &color);
   color.red = 1;
   color.green = 1;
   color.blue = 1;
   color.alpha = 0.7;
   Xregp= g_new0 (gfloat, 2);
   Yregp = g_new0 (gfloat, 2);
   Yregbp = g_new0 (gfloat, 2);
	Xregp[0]=0;
	Xregp[1]=0;
	Yregp[0]=0;
	Yregbp[0]=0;
	Yregp[1]=0;
	Yregbp[1]=0;
	color.red = textcolor[0];
	color.green = textcolor[1];
	color.blue = textcolor[2];
   color.alpha = 1;
	marker=gtk_databox_markers_new (1, Xregp, Yregp,  &color, 0,  GTK_DATABOX_MARKERS_NONE);
	label=malloc(900*sizeof(char));
	sprintf(label,"\n");
	gtk_databox_markers_set_label(GTK_DATABOX_MARKERS(marker),0,GTK_DATABOX_MARKERS_TEXT_E,label,TRUE);
   gtk_databox_graph_add (GTK_DATABOX (box), graphregionbox);
   gtk_databox_graph_add (GTK_DATABOX (box), marker);
   gtk_databox_auto_rescale (GTK_DATABOX (box), 0.05);
   //timeoutid = g_timeout_add (1000/FRAME_RATE,(GSourceFunc)update_graph, NULL);
	//g_idle_add_full(G_PRIORITY_DEFAULT_IDLE,(GSourceFunc)update_graph, NULL,NULL);
  // timeoutidb = g_timeout_add (1000.0/20.0,(GSourceFunc)refresh_label_timeout, NULL);
   g_signal_connect (G_OBJECT (box), "selection-started", G_CALLBACK (handle_signal_selection_started), NULL);
   g_signal_connect (G_OBJECT (box), "selection-finalized",G_CALLBACK (handle_signal_selection_finalized), NULL);
   g_signal_connect (G_OBJECT (box), "selection-canceled", G_CALLBACK (handle_signal_selection_canceled), NULL);
   g_signal_connect (G_OBJECT (box), "button_press_event",G_CALLBACK (handle_signal_click), NULL);
   g_signal_connect_swapped (G_OBJECT (box),"motion_notify_event", G_CALLBACK (motion_notify), NULL);
}
GtkWidget *tb1b;
GtkWidget *tb2b;
static gint toogle1_click(GtkWidget *button ){
	setlimnext=1;
	hasupdated=1;
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(tb1b))==0){
		if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(tb2b))==0){
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tb1b),1);
			gtk_databox_set_enable_selection(GTK_DATABOX(box),FALSE);
			mode=0;
		}
	}else{
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tb2b),0);
		gtk_databox_set_enable_selection(GTK_DATABOX(box),FALSE);
		mode=0;
	}
   return 0;
}
static gint toogle2_click(GtkWidget *button ){
	setlimnext=1;
	hasupdated=1;
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(tb2b))==0){
		if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(tb1b))==0){
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tb2b),1);
			gtk_databox_set_enable_selection(GTK_DATABOX(box),TRUE);
			mode=1;
		}
	}else{
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tb1b),0);
		gtk_databox_set_enable_selection(GTK_DATABOX(box),TRUE);
		mode=1;
	}
   return 0;
}
float oldlinecolor[3];
float oldpointcolor[3];
float oldtextcolor[3];
int olddarktheme=-1;
static PyObject *update_graph_colors(PyObject*self,PyObject*args){
	float linecolor[3];
	float pointcolor[3];
	float textcolor[3];
	int darktheme=0;
	hasupdated=1;
	if(!PyArg_ParseTuple(args,"fffffffffi",&linecolor[0],&linecolor[1],&linecolor[2],&pointcolor[0],&pointcolor[1],&pointcolor[2],&textcolor[0],&textcolor[1],&textcolor[2],&darktheme))
		return NULL;
	if ((oldlinecolor[0]==linecolor[0]) && (oldlinecolor[1]==linecolor[1]) && (oldlinecolor[2]==linecolor[2]) && (oldpointcolor[0]==pointcolor[0]) && (oldpointcolor[1]==pointcolor[1]) && (oldpointcolor[2]==pointcolor[2]) && (oldtextcolor[0]==textcolor[0]) && (oldtextcolor[1]==textcolor[1]) && (oldtextcolor[2]==textcolor[2]) && (olddarktheme==darktheme)){
		return Py_BuildValue("s","");
	}
	if(!PyArg_ParseTuple(args,"fffffffffi",&oldlinecolor[0],&oldlinecolor[1],&oldlinecolor[2],&oldpointcolor[0],&oldpointcolor[1],&oldpointcolor[2],&oldtextcolor[0],&oldtextcolor[1],&oldtextcolor[2],&olddarktheme))
		return NULL;
	GdkRGBA color;
	if(linecolor[0]>1)
		linecolor[0]=1;
	if(linecolor[1]>1)
		linecolor[1]=1;
	if(linecolor[2]>1)
		linecolor[2]=1;
	if(pointcolor[0]>1)
		pointcolor[0]=1;
	if(pointcolor[1]>1)
		pointcolor[1]=1;
	if(pointcolor[2]>1)
		pointcolor[2]=1;
	if(textcolor[0]>1)
		textcolor[0]=1;
	if(textcolor[1]>1)
		textcolor[1]=1;
	if(textcolor[2]>1)
		textcolor[2]=1;
	color.red = pointcolor[0];
	color.green = pointcolor[1];
	color.blue = pointcolor[2];
	color.alpha = 1;
	gtk_databox_graph_remove (GTK_DATABOX (box), graphpeaksrangea);
	gtk_databox_graph_remove (GTK_DATABOX (box), graphpeaksrangeb);
	gtk_databox_graph_remove (GTK_DATABOX (box), graphpeaksrangec);
	gtk_databox_graph_remove (GTK_DATABOX (box), graphpeaksranged);
   graphpeaksrangea = gtk_databox_points_new (600, Xpeaks, Ypeaks, &color, 5);
   graphpeaksrangeb = gtk_databox_points_new (4000, Xpeaks, Ypeaks, &color, 5);
   graphpeaksrangec = gtk_databox_points_new (10000, Xpeaks, Ypeaks, &color, 5);
   graphpeaksranged = gtk_databox_points_new (POINTS, Xpeaks, Ypeaks, &color, 5);
	gtk_databox_graph_add (GTK_DATABOX (box), graphpeaksrangea);
	gtk_databox_graph_add (GTK_DATABOX (box), graphpeaksrangeb);
	gtk_databox_graph_add (GTK_DATABOX (box), graphpeaksrangec);
	gtk_databox_graph_add (GTK_DATABOX (box), graphpeaksranged);
	color.red = linecolor[0];
	color.green = linecolor[1];
	color.blue = linecolor[2];
	color.alpha = 1;
	gtk_databox_graph_remove (GTK_DATABOX (box), graphregionbox);
	gtk_databox_graph_remove (GTK_DATABOX (box), graphrangea);
	gtk_databox_graph_remove (GTK_DATABOX (box), graphrangeb);
	gtk_databox_graph_remove (GTK_DATABOX (box), graphrangec);
	gtk_databox_graph_remove (GTK_DATABOX (box), graphranged);
	gtk_databox_graph_remove (GTK_DATABOX (box), graphbrangea);
	gtk_databox_graph_remove (GTK_DATABOX (box), graphbrangeb);
	gtk_databox_graph_remove (GTK_DATABOX (box), graphbrangec);
	gtk_databox_graph_remove (GTK_DATABOX (box), graphbranged);
   graphrangea = gtk_databox_points_new (600, X, Y, &color, 3);
   graphbrangea = gtk_databox_lines_new (600, X, Y, &color, 1);
   graphrangeb = gtk_databox_points_new (4000, X, Y, &color, 3);
   graphbrangeb = gtk_databox_lines_new (4000, X, Y, &color, 1);
   graphrangec = gtk_databox_points_new (10000, X, Y, &color, 3);
   graphbrangec = gtk_databox_lines_new (10000, X, Y, &color, 1);
   graphranged = gtk_databox_points_new (POINTS, X, Y, &color, 3);
   graphbranged = gtk_databox_lines_new (POINTS, X, Y, &color, 1);
   gtk_databox_graph_add (GTK_DATABOX (box), graphrangea);
   gtk_databox_graph_add (GTK_DATABOX (box), graphbrangea);
   gtk_databox_graph_add (GTK_DATABOX (box), graphrangeb);
   gtk_databox_graph_add (GTK_DATABOX (box), graphbrangeb);
   gtk_databox_graph_add (GTK_DATABOX (box), graphrangec);
   gtk_databox_graph_add (GTK_DATABOX (box), graphbrangec);
   gtk_databox_graph_add (GTK_DATABOX (box), graphranged);
   gtk_databox_graph_add (GTK_DATABOX (box), graphbranged);
	color.red = textcolor[0];
	color.green = textcolor[1];
	color.blue = textcolor[2];
	color.alpha = 1;
	gtk_databox_graph_remove (GTK_DATABOX (box), marker);
	marker=gtk_databox_markers_new (1, Xregp, Yregp,  &color, 0,  GTK_DATABOX_MARKERS_NONE);
	gtk_databox_graph_add (GTK_DATABOX (box), marker);
	color.red = textcolor[0];
	color.green = textcolor[1];
	color.blue = textcolor[2];
   color.alpha = 0.3;
	gtk_databox_graph_remove (GTK_DATABOX (box), graphgrid);
   graphgrid = gtk_databox_grid_new (7, 7, &color, 1);
	gtk_databox_graph_add (GTK_DATABOX (box), graphgrid);
	if(darktheme==1){
	   color.red = 1;
	   color.green = 1;
	   color.blue = 1;
	   color.alpha = 0.1;
	}else{
	   color.red = 1;
	   color.green = 1;
	   color.blue = 1;
	   color.alpha = 0.5;
	}
    graphregionbox = gtk_databox_regions_new (2,Xreg ,Yreg,Yregb, &color);
	gtk_databox_graph_add (GTK_DATABOX (box), graphregionbox);
   timeoutid = g_timeout_add (1000/FRAME_RATE,(GSourceFunc)update_graph, NULL);
   timeoutidb = g_timeout_add (1000.0/20.0,(GSourceFunc)refresh_label_timeout, NULL);
	return Py_BuildValue("s","");
}
static PyObject *export_graph(PyObject*self,PyObject*args){
	PyObject *object;
	GObject *observed;
	GtkWidget *widget;
	float linecolor[3];
	float pointcolor[3];
	float textcolor[3];
	int darktheme=0;
	if(!PyArg_ParseTuple(args,"Offfffffffi",&object,&linecolor[0],&linecolor[1],&linecolor[2],&pointcolor[0],&pointcolor[1],&pointcolor[2],&textcolor[0],&textcolor[1],&textcolor[2],&darktheme))
		return NULL;
	observed=object==Py_None?NULL:pygobject_get(object);
	if((observed!=NULL)&&(!G_IS_OBJECT(observed)))
		goto err;
   GtkWidget *separator;
   GtkWidget *box1;
   GtkWidget *box2;
   GtkWidget *box3;
	GtkWidget * toolbar;
   box3 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
   box1 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	GtkWidget *viewport=gtk_viewport_new(NULL,NULL);
	gtk_container_add (GTK_CONTAINER(viewport),box1);
   gtk_box_pack_start (GTK_BOX (box3), viewport, TRUE, TRUE, 0);//------
	toolbar=gtk_toolbar_new();
	if(linecolor[0]>1)
		linecolor[0]=1;
	if(linecolor[1]>1)
		linecolor[1]=1;
	if(linecolor[2]>1)
		linecolor[2]=1;
	if(pointcolor[0]>1)
		pointcolor[0]=1;
	if(pointcolor[1]>1)
		pointcolor[1]=1;
	if(pointcolor[2]>1)
		pointcolor[2]=1;
	if(textcolor[0]>1)
		textcolor[0]=1;
	if(textcolor[1]>1)
		textcolor[1]=1;
	if(textcolor[2]>1)
		textcolor[2]=1;
	gtk_orientable_set_orientation (GTK_ORIENTABLE(toolbar),GTK_ORIENTATION_VERTICAL);
	GtkToolItem *tb1;
	tb1 = gtk_tool_item_new();
 	tb1b = gtk_toggle_button_new();
	gtk_container_add (GTK_CONTAINER(tb1),tb1b);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tb1, -1);
	GtkToolItem *tb2;
	tb2 = gtk_tool_item_new();
 	tb2b = gtk_toggle_button_new();
	GtkWidget *image=gtk_image_new_from_icon_name("zoom-original",GTK_ICON_SIZE_LARGE_TOOLBAR);
	GtkWidget *imageb=gtk_image_new_from_icon_name("zoom-in",GTK_ICON_SIZE_LARGE_TOOLBAR);
	gtk_button_set_image (GTK_BUTTON (tb1b), image);
	gtk_button_set_image (GTK_BUTTON (tb2b), imageb);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tb1b),1);
   g_signal_connect (GTK_TOGGLE_BUTTON(tb1b), "toggled",G_CALLBACK (toogle1_click), NULL);
   g_signal_connect (GTK_TOGGLE_BUTTON(tb2b), "toggled",G_CALLBACK (toogle2_click), NULL);
	gtk_container_add (GTK_CONTAINER(tb2),tb2b);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tb2, -1);
   gtk_box_pack_start (GTK_BOX (box3), toolbar, FALSE, FALSE, 0);
   separator = gtk_separator_new (GTK_ORIENTATION_HORIZONTAL);
   gtk_box_pack_start (GTK_BOX (box1), separator, FALSE, FALSE, 0);
   separator = gtk_separator_new (GTK_ORIENTATION_HORIZONTAL);
   gtk_box_pack_start (GTK_BOX (box1), separator, FALSE, FALSE, 0);
   create_graph(box1,linecolor,pointcolor,textcolor,darktheme);
   separator = gtk_separator_new (GTK_ORIENTATION_HORIZONTAL);
   box2 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 10);
   gtk_container_set_border_width (GTK_CONTAINER (box2), 10);
	widget=box3;
	object=pygobject_new(G_OBJECT(widget));
	return object;
	err:
		PyErr_SetString(PyExc_TypeError,"Argument must be a GObject");
		return NULL;
}
static PyMethodDef methods[]={
	{"export_graph",export_graph,METH_VARARGS,""},
	{"update",updategraph,METH_VARARGS,""},
	{"update_peaks",update_peaks,METH_VARARGS,""},
	{"update_graph_colors",update_graph_colors,METH_VARARGS,""},
	{"refresh_graph",refresh_graph,METH_VARARGS,""},
	{NULL,NULL,0,NULL}
};
PyMODINIT_FUNC initpygtkdatabox(void){
	PyObject*module=Py_InitModule("pygtkdatabox",methods);
	if(module==NULL)
		return;
	pygobject_init(-1,-1,-1);
}
