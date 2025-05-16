#include <stdio.h>
#include <gtk/gtk.h>
#include "../bin_modules/pygtkdatabox/gtkdatabox/install/include/gtkdatabox.h"
#include "../bin_modules/pygtkdatabox/gtkdatabox/install/include/gtkdatabox_points.h"
#include "../bin_modules/pygtkdatabox/gtkdatabox/install/include/gtkdatabox_lines.h"
#include "../bin_modules/pygtkdatabox/gtkdatabox/install/include/gtkdatabox_util.h"
#include "../bin_modules/pygtkdatabox/gtkdatabox/install/include/gtkdatabox_grid.h"
#include "../bin_modules/pygtkdatabox/gtkdatabox/install/include/gtkdatabox_regions.h"
#include "../bin_modules/pygtkdatabox/gtkdatabox/install/include/gtkdatabox_markers.h"
#include <math.h>
#include <time.h>

//XXX XXX XXX XXX TODO 25/04/2024 Fix white box not appearing when on log scale, put on spectraread.

#define POINTS 13000 //This is the maximum number of points that can be shown in the graph.


#define  FRAME_RATE        7 //This is the minimum frame rate. The graph is also drawn on idle (g_idle_add_full).
						 	  // If this is set to high the window may not appear/ the graph can freeze.

#define  FRAME_RATEb        7 //This is the minimum frame rate. The graph is also drawn on idle (g_idle_add_full).
						 	  // If this is set to high the window may not appear/ the graph can freeze.

#define resizepercentage 0.0 //This is how much percent the scale must be off in relation to graph to trigger a resize.


#define borderporcent 3.0 //The white space between the graph and legend in percentage of the size of the graph.


#define popuptime 1.0 //In seconds.

int aqrun=0;
int logscale=0;
int oldislogscale=0;
int drawpoints=0;
int drawpointspeaks=0;
int *layersshown=NULL;
//New lines XXX
static gfloat *X = NULL;
static gfloat *Y = NULL;
double *X_old = NULL;
double *Y_old = NULL;
static gfloat *X2 = NULL;
static gfloat *Y2 = NULL;
double *X2_old = NULL;
double *Y2_old = NULL;
static gfloat *X3 = NULL;
static gfloat *Y3 = NULL;
double *X3_old = NULL;
double *Y3_old = NULL;
static gfloat *X4 = NULL;
static gfloat *Y4 = NULL;
double *X4_old = NULL;
double *Y4_old = NULL;
static gfloat *X5 = NULL;
static gfloat *Y5 = NULL;
double *X5_old = NULL;
double *Y5_old = NULL;

static gfloat *X6 = NULL;
static gfloat *Y6 = NULL;
double *X6_old = NULL;
double *Y6_old = NULL;
static gfloat *X7 = NULL;
static gfloat *Y7 = NULL;
double *X7_old = NULL;
double *Y7_old = NULL;
static gfloat *X8 = NULL;
static gfloat *Y8 = NULL;
double *X8_old = NULL;
double *Y8_old = NULL;
static gfloat *X9 = NULL;
static gfloat *Y9 = NULL;
double *X9_old = NULL;
double *Y9_old = NULL;
static gfloat *X10 = NULL;
static gfloat *Y10 = NULL;
double *X10_old = NULL;
double *Y10_old = NULL;





static gfloat *Y_notlogoffset = NULL;
//New lines XXX
static gfloat *Y2_notlogoffset = NULL;
static gfloat *Y3_notlogoffset = NULL;
static gfloat *Y4_notlogoffset = NULL;
static gfloat *Y5_notlogoffset = NULL;

static gfloat *Y6_notlogoffset = NULL;
static gfloat *Y7_notlogoffset = NULL;
static gfloat *Y8_notlogoffset = NULL;
static gfloat *Y9_notlogoffset = NULL;
static gfloat *Y10_notlogoffset = NULL;

static gfloat *Xpeaks = NULL;
static gfloat *Ypeaks = NULL;
static gfloat *Ypeaks_notlogoffset = NULL;
double *Xpeaks_old = NULL;
double *Ypeaks_old = NULL;
int isselecting=0;
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
float peaksaddlog=0;
long long int lastmouseinsidetime=0;
int setlimnext=0;
int hasupdated=1;
int showtooltip=0;
int oldnsdf=-1;
char *spectrareadproccessidnodegui;
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

//New lines XXX
GtkDataboxGraph *graph2rangea;
GtkDataboxGraph *graph3rangea;
GtkDataboxGraph *graph4rangea;
GtkDataboxGraph *graph5rangea;
GtkDataboxGraph *graph6rangea;
GtkDataboxGraph *graph7rangea;
GtkDataboxGraph *graph8rangea;
GtkDataboxGraph *graph9rangea;
GtkDataboxGraph *graph10rangea;
GtkDataboxGraph *graph2brangea;
GtkDataboxGraph *graph3brangea;
GtkDataboxGraph *graph4brangea;
GtkDataboxGraph *graph5brangea;
GtkDataboxGraph *graph6brangea;
GtkDataboxGraph *graph7brangea;
GtkDataboxGraph *graph8brangea;
GtkDataboxGraph *graph9brangea;
GtkDataboxGraph *graph10brangea;

int aqmodethisspecfromnode=-1;
GtkWidget *tb1b;
GtkWidget *tblogscale;
GtkWidget *tb2b;
GtkWidget *buttonlightb;
GtkWidget *buttonlightc;
GtkWidget *buttonlightd;
GtkWidget *buttonlighta;
GtkWidget *buttonlight;
int mapdatadarkid=-1;
int hidden=0;
//Performance optimization, number of points drawn on graph according with data provided, not POINTS defined above.
void enablegraphnpoints(int n){
	oldnsdf=n;
	if(!hidden){
			if(layersshown!=NULL){
				if(layersshown[1]==1){
					gtk_databox_graph_set_hide(graph2brangea,FALSE);
					gtk_databox_graph_set_hide(graph2rangea,FALSE);
				}
				if(layersshown[2]==1){
					gtk_databox_graph_set_hide(graph3brangea,FALSE);
					gtk_databox_graph_set_hide(graph3rangea,FALSE);
				}
				if(layersshown[3]==1){
					gtk_databox_graph_set_hide(graph4brangea,FALSE);
					gtk_databox_graph_set_hide(graph4rangea,FALSE);
				}
				if(layersshown[4]==1){
					gtk_databox_graph_set_hide(graph5brangea,FALSE);
					gtk_databox_graph_set_hide(graph5rangea,FALSE);
				}
				if(layersshown[5]==1){
					gtk_databox_graph_set_hide(graph6brangea,FALSE);
					gtk_databox_graph_set_hide(graph6rangea,FALSE);
				}
				if(layersshown[6]==1){
					gtk_databox_graph_set_hide(graph7brangea,FALSE);
					gtk_databox_graph_set_hide(graph7rangea,FALSE);
				}
				if(layersshown[7]==1){
					gtk_databox_graph_set_hide(graph8brangea,FALSE);
					gtk_databox_graph_set_hide(graph8rangea,FALSE);
				}
				if(layersshown[8]==1){
					gtk_databox_graph_set_hide(graph9brangea,FALSE);
					gtk_databox_graph_set_hide(graph9rangea,FALSE);
				}
				if(layersshown[9]==1){
					gtk_databox_graph_set_hide(graph10brangea,FALSE);
					gtk_databox_graph_set_hide(graph10rangea,FALSE);
				}
				if(layersshown[1]==0){
					gtk_databox_graph_set_hide(graph2brangea,TRUE);
					gtk_databox_graph_set_hide(graph2rangea,TRUE);
				}
				if(layersshown[2]==0){
					gtk_databox_graph_set_hide(graph3brangea,TRUE);
					gtk_databox_graph_set_hide(graph3rangea,TRUE);
				}
				if(layersshown[3]==0){
					gtk_databox_graph_set_hide(graph4brangea,TRUE);
					gtk_databox_graph_set_hide(graph4rangea,TRUE);
				}
				if(layersshown[4]==0){
					gtk_databox_graph_set_hide(graph5brangea,TRUE);
					gtk_databox_graph_set_hide(graph5rangea,TRUE);
				}
				if(layersshown[5]==0){
					gtk_databox_graph_set_hide(graph6brangea,TRUE);
					gtk_databox_graph_set_hide(graph6rangea,TRUE);
				}
				if(layersshown[6]==0){
					gtk_databox_graph_set_hide(graph7brangea,TRUE);
					gtk_databox_graph_set_hide(graph7rangea,TRUE);
				}
				if(layersshown[7]==0){
					gtk_databox_graph_set_hide(graph8brangea,TRUE);
					gtk_databox_graph_set_hide(graph8rangea,TRUE);
				}
				if(layersshown[8]==0){
					gtk_databox_graph_set_hide(graph9brangea,TRUE);
					gtk_databox_graph_set_hide(graph9rangea,TRUE);
				}
				if(layersshown[9]==0){
					gtk_databox_graph_set_hide(graph10brangea,TRUE);
					gtk_databox_graph_set_hide(graph10rangea,TRUE);
				}
			}

	if(n<500){
		showtooltip=1;
		//gtk_databox_graph_set_hide(graphpeaksrangea,FALSE);
		//gtk_databox_graph_set_hide(graphpeaksrangeb,TRUE);
		//gtk_databox_graph_set_hide(graphpeaksrangec,TRUE);
		//gtk_databox_graph_set_hide(graphpeaksranged,TRUE);
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
			showtooltip=1;
			//gtk_databox_graph_set_hide(graphpeaksrangea,TRUE);
			//gtk_databox_graph_set_hide(graphpeaksrangeb,FALSE);
			//gtk_databox_graph_set_hide(graphpeaksrangec,TRUE);
			//gtk_databox_graph_set_hide(graphpeaksranged,TRUE);
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
			showtooltip=1;
			//gtk_databox_graph_set_hide(graphpeaksrangea,TRUE);
			//gtk_databox_graph_set_hide(graphpeaksrangeb,TRUE);
			//gtk_databox_graph_set_hide(graphpeaksrangec,FALSE);
			//gtk_databox_graph_set_hide(graphpeaksranged,TRUE);
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
		showtooltip=0;
		//gtk_databox_graph_set_hide(graphpeaksrangea,TRUE);
		//gtk_databox_graph_set_hide(graphpeaksrangeb,TRUE);
		//gtk_databox_graph_set_hide(graphpeaksrangec,TRUE);
		//gtk_databox_graph_set_hide(graphpeaksranged,FALSE);
		gtk_databox_graph_set_hide(graphbrangea,TRUE);
		gtk_databox_graph_set_hide(graphbrangeb,TRUE);
		gtk_databox_graph_set_hide(graphbrangec,TRUE);
		gtk_databox_graph_set_hide(graphbranged,FALSE);
		gtk_databox_graph_set_hide(graphrangea,TRUE);
		gtk_databox_graph_set_hide(graphrangeb,TRUE);
		gtk_databox_graph_set_hide(graphrangec,TRUE);
		gtk_databox_graph_set_hide(graphranged,FALSE);
	}
			if(layersshown!=NULL){
				if(layersshown[0]==0){
					gtk_databox_graph_set_hide(graphbrangea,TRUE);
					gtk_databox_graph_set_hide(graphbrangeb,TRUE);
					gtk_databox_graph_set_hide(graphbrangec,TRUE);
					gtk_databox_graph_set_hide(graphbranged,TRUE);
					gtk_databox_graph_set_hide(graphrangea,TRUE);
					gtk_databox_graph_set_hide(graphrangeb,TRUE);
					gtk_databox_graph_set_hide(graphrangec,TRUE);
					gtk_databox_graph_set_hide(graphranged,TRUE);
				}
			}
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
		isselecting=0;
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
		isselecting=1;
	}
   return 0;
}
static gint handle_signal_selection_canceled(GtkDatabox * box ){
	if(mode==1){
		hasupdated=1;
		gtk_databox_ruler_set_draw_position(gtk_databox_get_ruler_x(GTK_DATABOX(box)),FALSE);
		gtk_databox_ruler_set_draw_position(gtk_databox_get_ruler_y(GTK_DATABOX(box)),FALSE);
		update=1;
		isselecting=0;
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
			if(layersshown[0]==1){
					//xmax=X[0];
					//xmin=X[0];
					ymax=Y[0];
					ymin=Y[0];
			}
			if(layersshown[1]==1){
					//xmax=X2[0];
					//xmin=X2[0];
					ymax=Y2[0];
					ymin=Y2[0];
			}
			if(layersshown[2]==1){
					//xmax=X3[0];
					//xmin=X3[0];
					ymax=Y3[0];
					ymin=Y3[0];
			}
			if(layersshown[3]==1){
					//xmax=X4[0];
					//xmin=X4[0];
					ymax=Y4[0];
					ymin=Y4[0];
			}
			if(layersshown[4]==1){
					//xmax=X5[0];
					//xmin=X5[0];
					ymax=Y5[0];
					ymin=Y5[0];
			}
			if(layersshown[5]==1){
					//xmax=X6[0];
				//	xmin=X6[0];
					ymax=Y6[0];
					ymin=Y6[0];
			}
			if(layersshown[6]==1){
					//xmax=X7[0];
					//xmin=X7[0];
					ymax=Y7[0];
					ymin=Y7[0];
			}
			if(layersshown[7]==1){
					//xmax=X8[0];
					//xmin=X8[0];
					ymax=Y8[0];
					ymin=Y8[0];
			}
			if(layersshown[8]==1){
					//xmax=X9[0];
					//xmin=X9[0];
					ymax=Y9[0];
					ymin=Y9[0];
			}
			if(layersshown[9]==1){
					//xmax=X10[0];
					//xmin=X10[0];
					ymax=Y10[0];
					ymin=Y10[0];
			}
  		 for (int i = 0; i < drawpoints; i++){
				if(X[i]>xmax)
					xmax=X[i];
				if(X[i]<xmin)
					xmin=X[i];
			if(layersshown[0]==1){

				if(Y[i]>ymax)
					ymax=Y[i];
				if(Y[i]<ymin)
					ymin=Y[i];
			}				if(X2[i]>xmax)
					xmax=X2[i];
				if(X2[i]<xmin)
					xmin=X2[i];
			if(layersshown[1]==1){

				if(Y2[i]>ymax)
					ymax=Y2[i];
				if(Y2[i]<ymin)
					ymin=Y2[i];
			}				if(X3[i]>xmax)
					xmax=X3[i];
				if(X3[i]<xmin)
					xmin=X3[i];
			if(layersshown[2]==1){

				if(Y3[i]>ymax)
					ymax=Y3[i];
				if(Y3[i]<ymin)
					ymin=Y3[i];
			}				if(X4[i]>xmax)
					xmax=X4[i];
				if(X4[i]<xmin)
					xmin=X4[i];
			if(layersshown[3]==1){

				if(Y4[i]>ymax)
					ymax=Y4[i];
				if(Y4[i]<ymin)
					ymin=Y4[i];
			}				if(X5[i]>xmax)
					xmax=X5[i];
				if(X5[i]<xmin)
					xmin=X5[i];
			if(layersshown[4]==1){

				if(Y5[i]>ymax)
					ymax=Y5[i];
				if(Y5[i]<ymin)
					ymin=Y5[i];
			}				if(X6[i]>xmax)
					xmax=X6[i];
				if(X6[i]<xmin)
					xmin=X6[i];
			if(layersshown[5]==1){

				if(Y6[i]>ymax)
					ymax=Y6[i];
				if(Y6[i]<ymin)
					ymin=Y6[i];
			}				if(X7[i]>xmax)
					xmax=X7[i];
				if(X7[i]<xmin)
					xmin=X7[i];
			if(layersshown[6]==1){

				if(Y7[i]>ymax)
					ymax=Y7[i];
				if(Y7[i]<ymin)
					ymin=Y7[i];
			}				if(X8[i]>xmax)
					xmax=X8[i];
				if(X8[i]<xmin)
					xmin=X8[i];
			if(layersshown[7]==1){

				if(Y8[i]>ymax)
					ymax=Y8[i];
				if(Y8[i]<ymin)
					ymin=Y8[i];
			}				if(X9[i]>xmax)
					xmax=X9[i];
				if(X9[i]<xmin)
					xmin=X9[i];
			if(layersshown[8]==1){

				if(Y9[i]>ymax)
					ymax=Y9[i];
				if(Y9[i]<ymin)
					ymin=Y9[i];
			}				if(X10[i]>xmax)
					xmax=X10[i];
				if(X10[i]<xmin)
					xmin=X10[i];
			if(layersshown[9]==1){

				if(Y10[i]>ymax)
					ymax=Y10[i];
				if(Y10[i]<ymin)
					ymin=Y10[i];
			}
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
		xmin=xmin-(0/100.0)*absolute(xmin-xmax);
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
void updateinfobox(void){
	if(isselecting)
		return;
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
static gint motion_notify(GtkDatabox * boxb, GdkEventButton * event ){
	if(isselecting)
		return 0;
	if(showtooltip==0){
		return 0;
	}
	if(update==0){
		hasupdated=1;
		if(!isselecting)
			update=1;
	}
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

static gboolean refresh_label_timeout(void){
	if(isselecting)
		return FALSE;
	if(hasupdated==0){
		usleep(1000);
		return FALSE;
	}
	updateinfobox();
	if(!GTK_IS_DATABOX(box))
		return FALSE;
	if(update==1)
		gtk_widget_queue_draw (GTK_WIDGET (box));
	return FALSE;
}
int oldmode=-3;



static gboolean update_graph (void){
	if(isselecting)
		return FALSE;
	time_t starttimeb;
	time(&starttimeb);
	long long int internaltime=starttimeb;
	if(absolute(internaltime-lastmouseinsidetime)>popuptime){
		if(showtooltip){
			updateinfobox();
			gtk_widget_queue_draw (GTK_WIDGET (box));
		}
	}
	if(logscale==1){
		if(oldislogscale!=1){
	  		gtk_databox_set_scale_type_y (GTK_DATABOX (box), GTK_DATABOX_SCALE_LOG);
			oldislogscale=1;
			hasupdated=1;
			update=1;
		}
	}
	if(logscale==0){
		if(oldislogscale!=0){
	  		gtk_databox_set_scale_type_y (GTK_DATABOX (box), GTK_DATABOX_SCALE_LINEAR);
			oldislogscale=0;
			hasupdated=1;
			update=1;
		}
	}
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
		update=0;//XXX TODO
	for (i = drawpoints; i < POINTS; i++){
		X[i]=X[drawpoints-1];
		Y[i]=Y[drawpoints-1];
		X2[i]=X2[drawpoints-1];
		Y2[i]=Y2[drawpoints-1];
		X3[i]=X3[drawpoints-1];
		Y3[i]=Y3[drawpoints-1];
		X4[i]=X4[drawpoints-1];
		Y4[i]=Y4[drawpoints-1];
		X5[i]=X5[drawpoints-1];
		Y5[i]=Y5[drawpoints-1];
		X6[i]=X6[drawpoints-1];
		Y6[i]=Y6[drawpoints-1];
		X7[i]=X7[drawpoints-1];
		Y7[i]=Y7[drawpoints-1];
		X8[i]=X8[drawpoints-1];
		Y8[i]=Y8[drawpoints-1];
		X9[i]=X9[drawpoints-1];
		Y9[i]=Y9[drawpoints-1];
		X10[i]=X10[drawpoints-1];
		Y10[i]=Y10[drawpoints-1];
	}
	if(mode==0){
		float xmin=X[0];
		float xmax=X[0];
		float ymin=Y[0];
		float ymax=Y[0];
			if(layersshown[0]==1){
					//xmax=X[0];
					if(X[0]>0.001)
					xmin=X[0];
					ymax=Y[0];
					ymin=Y[0];
			}
			if(layersshown[1]==1){
					//xmax=X2[0];
					if(X2[0]>0.001)
					xmin=X2[0];
					ymax=Y2[0];
					ymin=Y2[0];
			}
			if(layersshown[2]==1){
					//xmax=X3[0];
					if(X3[0]>0.001)
					xmin=X3[0];
					ymax=Y3[0];
					ymin=Y3[0];
			}
			if(layersshown[3]==1){
					//xmax=X4[0];
					if(X4[0]>0.001)
					xmin=X4[0];
					ymax=Y4[0];
					ymin=Y4[0];
			}
			if(layersshown[4]==1){
					//xmax=X5[0];
					if(X5[0]>0.001)
					xmin=X5[0];
					ymax=Y5[0];
					ymin=Y5[0];
			}
			if(layersshown[5]==1){
					//xmax=X6[0];
					if(X6[0]>0.001)
					xmin=X6[0];
					ymax=Y6[0];
					ymin=Y6[0];
			}
			if(layersshown[6]==1){
					//xmax=X7[0];
					if(X7[0]>0.001)
					xmin=X7[0];
					ymax=Y7[0];
					ymin=Y7[0];
			}
			if(layersshown[7]==1){
					//xmax=X8[0];
					if(X8[0]>0.001)
					xmin=X8[0];
					ymax=Y8[0];
					ymin=Y8[0];
			}
			if(layersshown[8]==1){
					//xmax=X9[0];
					if(X9[0]>0.001)
					xmin=X9[0];
					ymax=Y9[0];
					ymin=Y9[0];
			}
			if(layersshown[9]==1){
					//xmax=X10[0];
					if(X10[0]>0.001)
					xmin=X10[0];
					ymax=Y10[0];
					ymin=Y10[0];
			}
		xmax=0;
  		for (i = 0; i < drawpoints; i++){
			if(layersshown[0]==1){
				if(X[i]>xmax)
					xmax=X[i];
				if(X[i]<xmin)
					if(X[0]>0.001)
					xmin=X[i];
				if(Y[i]>ymax)
					ymax=Y[i];
				if(Y[i]<ymin)
					ymin=Y[i];
			}
			if(layersshown[1]==1){
				if(X2[i]>xmax)
					xmax=X2[i];
				if(X2[i]<xmin)
					if(X2[0]>0.001)
					xmin=X2[i];
				if(Y2[i]>ymax)
					ymax=Y2[i];
				if(Y2[i]<ymin)
					ymin=Y2[i];
			}	
			if(layersshown[2]==1){
				if(X3[i]>xmax)
					xmax=X3[i];
				if(X3[i]<xmin)
					if(X3[0]>0.001)
					xmin=X3[i];
				if(Y3[i]>ymax)
					ymax=Y3[i];
				if(Y3[i]<ymin)
					ymin=Y3[i];
			}
			if(layersshown[3]==1){
				if(X4[i]>xmax)
					xmax=X4[i];
				if(X4[i]<xmin)
					if(X4[0]>0.001)
					xmin=X4[i];
				if(Y4[i]>ymax)
					ymax=Y4[i];
				if(Y4[i]<ymin)
					ymin=Y4[i];
			}
			if(layersshown[4]==1){
				if(X5[i]>xmax)
					xmax=X5[i];
				if(X5[i]<xmin)
					if(X5[0]>0.001)
					xmin=X5[i];
				if(Y5[i]>ymax)
					ymax=Y5[i];
				if(Y5[i]<ymin)
					ymin=Y5[i];
			}
			if(layersshown[5]==1){
				if(X6[i]>xmax)
					xmax=X6[i];
				if(X6[i]<xmin)
					if(X6[0]>0.001)
					xmin=X6[i];
				if(Y6[i]>ymax)
					ymax=Y6[i];
				if(Y6[i]<ymin)
					ymin=Y6[i];
			}
			if(layersshown[6]==1){
				if(X7[i]>xmax)
					xmax=X7[i];
				if(X7[i]<xmin)
					if(X7[0]>0.001)
					xmin=X7[i];
				if(Y7[i]>ymax)
					ymax=Y7[i];
				if(Y7[i]<ymin)
					ymin=Y7[i];
			}
			if(layersshown[7]==1){
				if(X8[i]>xmax)
					xmax=X8[i];
				if(X8[i]<xmin)
					if(X8[0]>0.001)
					xmin=X8[i];
				if(Y8[i]>ymax)
					ymax=Y8[i];
				if(Y8[i]<ymin)
					ymin=Y8[i];
			}
			if(layersshown[8]==1){
				if(X9[i]>xmax)
					xmax=X9[i];
				if(X9[i]<xmin)
					if(X9[0]>0.001)
					xmin=X9[i];
				if(Y9[i]>ymax)
					ymax=Y9[i];
				if(Y9[i]<ymin)
					ymin=Y9[i];
			}
			if(layersshown[9]==1){
				if(X10[i]>xmax)
					xmax=X10[i];
				if(X10[i]<xmin)
					if(X10[0]>0.001)
					xmin=X10[i];
				if(Y10[i]>ymax)
					ymax=Y10[i];
				if(Y10[i]<ymin)
					ymin=Y10[i];
			}

		}
		if(isnan(xmin))
			xmin=0;
		if(isnan(xmax))
			xmax=xmin+0.5;
		if(isnan(ymin)){
			ymin=0;
		}
		if(isnan(ymax))
			ymax=ymin+0.5;
		if(xmax<=xmin)
			xmax=xmin+0.5;
		if(ymax<=ymin)
			ymax=ymin+0.5;
		xmin=xmin-(0/100.0)*absolute(xmin-xmax);
		xmax=xmax+(borderporcent/100.0)*absolute(xmin-xmax);
		ymin=ymin-(borderporcent/100.0)*absolute(ymin-ymax);
		ymax=ymax+(borderporcent/100.0)*absolute(ymin-ymax);
		Xreg[0]=xmin-(20.0/100.0)*absolute(xmin-xmax);
		Xreg[1]=xmax+(20.0/100.0)*absolute(xmin-xmax);
		Yreg[0]=ymax+(20.0/100.0)*absolute(ymin-ymax);
		Yregb[0]=ymin-(20.0/100.0)*absolute(ymin-ymax);
		Yreg[1]=ymax+(20.0/100.0)*absolute(ymin-ymax);
		Yregb[1]=ymin-(20.0/100.0)*absolute(ymin-ymax);
		if(logscale==1){
            //XXX XXX XXX XXX TODO 25/04/2024 Fix white box not appearing when on log scale, put on spectraread.
            //XXX XXX XXX XXX TODO 
            //XXX XXX XXX XXX TODO 
            //XXX XXX XXX XXX TODO 
            //XXX XXX XXX XXX TODO 
            //XXX XXX XXX XXX TODO 
            Yreg[0]=log10(Yreg[0]);
            Yreg[1]=log10(Yreg[1]);
            Yregb[0]=log10(Yregb[0]);
            Yregb[1]=log10(Yregb[1]);
            //XXX XXX XXX XXX TODO 
            //XXX XXX XXX XXX TODO 
            //XXX XXX XXX XXX TODO 
            //XXX XXX XXX XXX TODO 
            //XXX XXX XXX XXX TODO 
			if(ymin<1){
				ymin=1;
			}
			if(ymax<=1){
				ymax=2;
			}
		}
		xyxy[0]=xmin;
		xyxy[2]=xmax;
		xyxy[1]=ymax;
		xyxy[3]=ymin;
		Xlim[0]=xyxy[0];
		Xlim[1]=xyxy[2];
		Ylim[0]=xyxy[1];
		Ylim[1]=xyxy[3];
		if(firstup==0){
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
		if(logscale==1){
			if(ymin<1){
				ymin=1;
				xyxy[3]=1;
			}
			if(ymax<=1){
				ymax=2;
				xyxy[1]=2;
			}
		}
		Xreg[0]=xmin-(20.0/100.0)*absolute(xmin-xmax);
		Xreg[1]=xmax+(20.0/100.0)*absolute(xmin-xmax);
		Yreg[0]=ymax+(20.0/100.0)*absolute(ymin-ymax);
		Yregb[0]=ymin-(20.0/100.0)*absolute(ymin-ymax);;
		Yreg[1]=ymax+(20.0/100.0)*absolute(ymin-ymax);;
		Yregb[1]=ymin-(20.0/100.0)*absolute(ymin-ymax);;
		if(logscale==1){
            //XXX XXX XXX XXX TODO 25/04/2024 Fix white box not appearing when on log scale, put on spectraread.
            //XXX XXX XXX XXX TODO 
            //XXX XXX XXX XXX TODO 
            //XXX XXX XXX XXX TODO 
            //XXX XXX XXX XXX TODO 
            //XXX XXX XXX XXX TODO 
            Yreg[0]=log10(Yreg[0]);
            Yreg[1]=log10(Yreg[1]);
            Yregb[0]=log10(Yregb[0]);
            Yregb[1]=log10(Yregb[1]);
            //XXX XXX XXX XXX TODO 
            //XXX XXX XXX XXX TODO 
            //XXX XXX XXX XXX TODO 
            //XXX XXX XXX XXX TODO 
            //XXX XXX XXX XXX TODO 
        }
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

void updategraph(float *xupc, float *yupc,float *x2upc, float *y2upc,float *x3upc, float *y3upc,float *x4upc, float *y4upc,float *x5upc, float *y5upc,float *x6upc, float *y6upc,float *x7upc, float *y7upc,float *x8upc, float *y8upc,float *x9upc, float *y9upc,float *x10upc, float *y10upc,int pupdate, int aqmodethisspecfromnodeinternal){
	//int pupdate=0;
	//int aqmodethisspecfromnodeinternal=-1;

	aqmodethisspecfromnode=aqmodethisspecfromnodeinternal;
	drawpoints=pupdate;
	if(drawpoints+1>=POINTS)
		drawpoints=POINTS-1;
	int i=0;
	while(1){
		//if(!PyFloat_Check(next)){
		//	break;//XXX
		//}
		if(i>drawpoints)
			break;
		double foo=xupc[i];
		//if(roundf(100*X[i])/100.0!=roundf(100*foo)/100.0){// was if(roundf(10*X_old[i])/10.0!=roundf(10*foo)/10.0){, changed to fix graph plot error when number of points increases, decreases and then increses again.
			X[i]=foo;
			X_old[i]=foo;
			hasupdated=1;
			update=1;
	//	}
		aqmodethisspecfromnode=aqmodethisspecfromnodeinternal;
		i=i+1;
	}
	i=0;
	while(1){
		//if(!PyFloat_Check(next)){
		//	break;//XXX
		//}
		if(i>drawpoints)
			break;
		double foo=yupc[i];
	//	if(roundf(100*Y_notlogoffset[i])/100.0!=roundf(100*foo)/100.0){
			Y[i]=foo;
			Y_old[i]=foo;
			Y_notlogoffset[i]=foo;
			update=1;
			hasupdated=1;
	//	}
		aqmodethisspecfromnode=aqmodethisspecfromnodeinternal;
		i=i+1;
	}
	for (i = drawpoints; i < POINTS; i++){
		X[i]=X[drawpoints-1];
		Y[i]=Y[drawpoints-1];
	}











	i=0;
	while(1){
		//if(!PyFloat_Check(next)){
		//	break;//XXX
		//}
		if(i>drawpoints)
			break;
		double foo=x2upc[i];
	//	if(roundf(100*X2[i])/100.0!=roundf(100*foo)/100.0){// was if(roundf(10*X_old[i])/10.0!=roundf(10*foo)/10.0){, changed to fix graph plot error when number of points increases, decreases and then increses again.
			X2[i]=foo;
			X2_old[i]=foo;
			hasupdated=1;
			update=1;
	//	}
		aqmodethisspecfromnode=aqmodethisspecfromnodeinternal;
		i=i+1;
	}
	i=0;
	while(1){
		//if(!PyFloat_Check(next)){
		//	break;//XXX
		//}
		if(i>drawpoints)
			break;
		double foo=y2upc[i];
		//if(roundf(100*Y2_notlogoffset[i])/100.0!=roundf(100*foo)/100.0){
			Y2[i]=foo;
			Y2_old[i]=foo;
			Y2_notlogoffset[i]=foo;
			update=1;
			hasupdated=1;
	//	}
		aqmodethisspecfromnode=aqmodethisspecfromnodeinternal;
		i=i+1;
	}
	for (i = drawpoints; i < POINTS; i++){
		X2[i]=X2[drawpoints-1];
		Y2[i]=Y2[drawpoints-1];
	}



	i=0;
	while(1){
		//if(!PyFloat_Check(next)){
		//	break;//XXX
		//}
		if(i>drawpoints)
			break;
		double foo=x3upc[i];
		//if(roundf(100*X3[i])/100.0!=roundf(100*foo)/100.0){// was if(roundf(10*X_old[i])/10.0!=roundf(10*foo)/10.0){, changed to fix graph plot error when number of points increases, decreases and then increses again.
			X3[i]=foo;
			X3_old[i]=foo;
			hasupdated=1;
			update=1;
	//	}
		aqmodethisspecfromnode=aqmodethisspecfromnodeinternal;
		i=i+1;
	}
	i=0;
	while(1){
		//if(!PyFloat_Check(next)){
		//	break;//XXX
		//}
		if(i>drawpoints)
			break;
		double foo=y3upc[i];
		//if(roundf(100*Y3_notlogoffset[i])/100.0!=roundf(100*foo)/100.0){
			Y3[i]=foo;
			Y3_old[i]=foo;
			Y3_notlogoffset[i]=foo;
			update=1;
			hasupdated=1;
	//	}
		aqmodethisspecfromnode=aqmodethisspecfromnodeinternal;
		i=i+1;
	}
	for (i = drawpoints; i < POINTS; i++){
		X3[i]=X3[drawpoints-1];
		Y3[i]=Y3[drawpoints-1];
	}




	i=0;
	while(1){
		//if(!PyFloat_Check(next)){
		//	break;//XXX
		//}
		if(i>drawpoints)
			break;
		double foo=x4upc[i];
		//if(roundf(100*X4[i])/100.0!=roundf(100*foo)/100.0){// was if(roundf(10*X_old[i])/10.0!=roundf(10*foo)/10.0){, changed to fix graph plot error when number of points increases, decreases and then increses again.
			X4[i]=foo;
			X4_old[i]=foo;
			hasupdated=1;
			update=1;
	//	}
		aqmodethisspecfromnode=aqmodethisspecfromnodeinternal;
		i=i+1;
	}
	i=0;
	while(1){
		//if(!PyFloat_Check(next)){
		//	break;//XXX
		//}
		if(i>drawpoints)
			break;
		double foo=y4upc[i];
		//if(roundf(100*Y4_notlogoffset[i])/100.0!=roundf(100*foo)/100.0){
			Y4[i]=foo;
			Y4_old[i]=foo;
			Y4_notlogoffset[i]=foo;
			update=1;
			hasupdated=1;
	//	}
		aqmodethisspecfromnode=aqmodethisspecfromnodeinternal;
		i=i+1;
	}
	for (i = drawpoints; i < POINTS; i++){
		X4[i]=X4[drawpoints-1];
		Y4[i]=Y4[drawpoints-1];
	}



	i=0;
	while(1){
		//if(!PyFloat_Check(next)){
		//	break;//XXX
		//}
		if(i>drawpoints)
			break;
		double foo=x5upc[i];
		//if(roundf(100*X5[i])/100.0!=roundf(100*foo)/100.0){// was if(roundf(10*X_old[i])/10.0!=roundf(10*foo)/10.0){, changed to fix graph plot error when number of points increases, decreases and then increses again.
			X5[i]=foo;
			X5_old[i]=foo;
			hasupdated=1;
			update=1;
	//	}
		aqmodethisspecfromnode=aqmodethisspecfromnodeinternal;
		i=i+1;
	}
	i=0;
	while(1){
		//if(!PyFloat_Check(next)){
		//	break;//XXX
		//}
		if(i>drawpoints)
			break;
		double foo=y5upc[i];
	//	if(roundf(100*Y5_notlogoffset[i])/100.0!=roundf(100*foo)/100.0){
			Y5[i]=foo;
			Y5_old[i]=foo;
			Y5_notlogoffset[i]=foo;
			update=1;
			hasupdated=1;
		//}
		aqmodethisspecfromnode=aqmodethisspecfromnodeinternal;
		i=i+1;
	}
	for (i = drawpoints; i < POINTS; i++){
		X5[i]=X5[drawpoints-1];
		Y5[i]=Y5[drawpoints-1];
	}




	i=0;
	while(1){
		//if(!PyFloat_Check(next)){
		//	break;//XXX
		//}
		if(i>drawpoints)
			break;
		double foo=x6upc[i];
	//	if(roundf(100*X6[i])/100.0!=roundf(100*foo)/100.0){// was if(roundf(10*X_old[i])/10.0!=roundf(10*foo)/10.0){, changed to fix graph plot error when number of points increases, decreases and then increses again.
			X6[i]=foo;
			X6_old[i]=foo;
			hasupdated=1;
			update=1;
	//	}
		aqmodethisspecfromnode=aqmodethisspecfromnodeinternal;
		i=i+1;
	}
	i=0;
	while(1){
		//if(!PyFloat_Check(next)){
		//	break;//XXX
		//}
		if(i>drawpoints)
			break;
		double foo=y6upc[i];
	//	if(roundf(100*Y6_notlogoffset[i])/100.0!=roundf(100*foo)/100.0){
			Y6[i]=foo;
			Y6_old[i]=foo;
			Y6_notlogoffset[i]=foo;
			update=1;
			hasupdated=1;
	//	}
		aqmodethisspecfromnode=aqmodethisspecfromnodeinternal;
		i=i+1;
	}
	for (i = drawpoints; i < POINTS; i++){
		X6[i]=X6[drawpoints-1];
		Y6[i]=Y6[drawpoints-1];
	}



	i=0;
	while(1){
		//if(!PyFloat_Check(next)){
		//	break;//XXX
		//}
		if(i>drawpoints)
			break;
		double foo=x7upc[i];
	//	if(roundf(100*X7[i])/100.0!=roundf(100*foo)/100.0){// was if(roundf(10*X_old[i])/10.0!=roundf(10*foo)/10.0){, changed to fix graph plot error when number of points increases, decreases and then increses again.
			X7[i]=foo;
			X7_old[i]=foo;
			hasupdated=1;
			update=1;
	//	}
		aqmodethisspecfromnode=aqmodethisspecfromnodeinternal;
		i=i+1;
	}
	i=0;
	while(1){
		//if(!PyFloat_Check(next)){
		//	break;//XXX
		//}
		if(i>drawpoints)
			break;
		double foo=y7upc[i];
	//	if(roundf(100*Y7_notlogoffset[i])/100.0!=roundf(100*foo)/100.0){
			Y7[i]=foo;
			Y7_old[i]=foo;
			Y7_notlogoffset[i]=foo;
			update=1;
			hasupdated=1;
	//	}
		aqmodethisspecfromnode=aqmodethisspecfromnodeinternal;
		i=i+1;
	}
	for (i = drawpoints; i < POINTS; i++){
		X7[i]=X7[drawpoints-1];
		Y7[i]=Y7[drawpoints-1];
	}


	i=0;
	while(1){
		//if(!PyFloat_Check(next)){
		//	break;//XXX
		//}
		if(i>drawpoints)
			break;
		double foo=x8upc[i];
	//	if(roundf(100*X8[i])/100.0!=roundf(100*foo)/100.0){// was if(roundf(10*X_old[i])/10.0!=roundf(10*foo)/10.0){, changed to fix graph plot error when number of points increases, decreases and then increses again.
			X8[i]=foo;
			X8_old[i]=foo;
			hasupdated=1;
			update=1;
	//	}
		aqmodethisspecfromnode=aqmodethisspecfromnodeinternal;
		i=i+1;
	}
	i=0;
	while(1){
		//if(!PyFloat_Check(next)){
		//	break;//XXX
		//}
		if(i>drawpoints)
			break;
		double foo=y8upc[i];
	//	if(roundf(100*Y8_notlogoffset[i])/100.0!=roundf(100*foo)/100.0){
			Y8[i]=foo;
			Y8_old[i]=foo;
			Y8_notlogoffset[i]=foo;
			update=1;
			hasupdated=1;
	//	}
		aqmodethisspecfromnode=aqmodethisspecfromnodeinternal;
		i=i+1;
	}
	for (i = drawpoints; i < POINTS; i++){
		X8[i]=X8[drawpoints-1];
		Y8[i]=Y8[drawpoints-1];
	}


	i=0;
	while(1){
		//if(!PyFloat_Check(next)){
		//	break;//XXX
		//}
		if(i>drawpoints)
			break;
		double foo=x9upc[i];
	//	if(roundf(100*X9[i])/100.0!=roundf(100*foo)/100.0){// was if(roundf(10*X_old[i])/10.0!=roundf(10*foo)/10.0){, changed to fix graph plot error when number of points increases, decreases and then increses again.
			X9[i]=foo;
			X9_old[i]=foo;
			hasupdated=1;
			update=1;
	//	}
		aqmodethisspecfromnode=aqmodethisspecfromnodeinternal;
		i=i+1;
	}
	i=0;
	while(1){
		//if(!PyFloat_Check(next)){
		//	break;//XXX
		//}
		if(i>drawpoints)
			break;
		double foo=y9upc[i];
	//	if(roundf(100*Y9_notlogoffset[i])/100.0!=roundf(100*foo)/100.0){
			Y9[i]=foo;
			Y9_old[i]=foo;
			Y9_notlogoffset[i]=foo;
			update=1;
			hasupdated=1;
	//	}
		aqmodethisspecfromnode=aqmodethisspecfromnodeinternal;
		i=i+1;
	}
	for (i = drawpoints; i < POINTS; i++){
		X9[i]=X9[drawpoints-1];
		Y9[i]=Y9[drawpoints-1];
	}


	i=0;
	while(1){
		//if(!PyFloat_Check(next)){
		//	break;//XXX
		//}
		if(i>drawpoints)
			break;
		double foo=x10upc[i];
	//	if(roundf(100*X10[i])/100.0!=roundf(100*foo)/100.0){// was if(roundf(10*X_old[i])/10.0!=roundf(10*foo)/10.0){, changed to fix graph plot error when number of points increases, decreases and then increses again.
			X10[i]=foo;
			X10_old[i]=foo;
			hasupdated=1;
			update=1;
	//	}
		aqmodethisspecfromnode=aqmodethisspecfromnodeinternal;
		i=i+1;
	}
	i=0;
	while(1){
		//if(!PyFloat_Check(next)){
		//	break;//XXX
		//}
		if(i>drawpoints)
			break;
		double foo=y10upc[i];
	//	if(roundf(100*Y10_notlogoffset[i])/100.0!=roundf(100*foo)/100.0){
			Y10[i]=foo;
			Y10_old[i]=foo;
			Y10_notlogoffset[i]=foo;
			update=1;
			hasupdated=1;
	//	}
		aqmodethisspecfromnode=aqmodethisspecfromnodeinternal;
		i=i+1;
	}
	for (i = drawpoints; i < POINTS; i++){
		X10[i]=X10[drawpoints-1];
		Y10[i]=Y10[drawpoints-1];
	}

	if(hasupdated==1){
		if(logscale==1){
			//Check graph min y value.
			float yminbuplog=Y[0];
	  		for (i = 0; i <= drawpoints; i++){
				if(Y_notlogoffset[i]<yminbuplog)
					yminbuplog=Y_notlogoffset[i];
				if(Y2_notlogoffset[i]<yminbuplog)
					yminbuplog=Y2_notlogoffset[i];
				if(Y3_notlogoffset[i]<yminbuplog)
					yminbuplog=Y3_notlogoffset[i];
				if(Y4_notlogoffset[i]<yminbuplog)
					yminbuplog=Y4_notlogoffset[i];
				if(Y5_notlogoffset[i]<yminbuplog)
					yminbuplog=Y5_notlogoffset[i];
				if(Y6_notlogoffset[i]<yminbuplog)
					yminbuplog=Y6_notlogoffset[i];
				if(Y7_notlogoffset[i]<yminbuplog)
					yminbuplog=Y7_notlogoffset[i];
				if(Y8_notlogoffset[i]<yminbuplog)
					yminbuplog=Y8_notlogoffset[i];
				if(Y9_notlogoffset[i]<yminbuplog)
					yminbuplog=Y9_notlogoffset[i];
				if(Y10_notlogoffset[i]<yminbuplog)
					yminbuplog=Y10_notlogoffset[i];
			}
			//Add offset to graph if it has parts below zero.
			if(yminbuplog<=0){
				peaksaddlog=-yminbuplog+1;
		  		for (i = 0; i <= drawpoints; i++){
					if(Y[i]!=(Y_notlogoffset[i]-yminbuplog+1))
						update=1;
					if(Y2[i]!=(Y2_notlogoffset[i]-yminbuplog+1))
						update=1;
					if(Y3[i]!=(Y3_notlogoffset[i]-yminbuplog+1))
						update=1;
					if(Y4[i]!=(Y4_notlogoffset[i]-yminbuplog+1))
						update=1;
					if(Y5[i]!=(Y5_notlogoffset[i]-yminbuplog+1))
						update=1;
					if(Y6[i]!=(Y6_notlogoffset[i]-yminbuplog+1))
						update=1;
					if(Y7[i]!=(Y7_notlogoffset[i]-yminbuplog+1))
						update=1;
					if(Y8[i]!=(Y8_notlogoffset[i]-yminbuplog+1))
						update=1;
					if(Y9[i]!=(Y9_notlogoffset[i]-yminbuplog+1))
						update=1;
					if(Y10[i]!=(Y10_notlogoffset[i]-yminbuplog+1))
						update=1;
					Y[i]=Y_notlogoffset[i]-yminbuplog+1;
					Y2[i]=Y2_notlogoffset[i]-yminbuplog+1;
					Y3[i]=Y3_notlogoffset[i]-yminbuplog+1;
					Y4[i]=Y4_notlogoffset[i]-yminbuplog+1;
					Y5[i]=Y5_notlogoffset[i]-yminbuplog+1;
					Y6[i]=Y6_notlogoffset[i]-yminbuplog+1;
					Y7[i]=Y7_notlogoffset[i]-yminbuplog+1;
					Y8[i]=Y8_notlogoffset[i]-yminbuplog+1;
					Y9[i]=Y9_notlogoffset[i]-yminbuplog+1;
					Y10[i]=Y10_notlogoffset[i]-yminbuplog+1;
				}
			}else{
				peaksaddlog=1;
		  		for (i = 0; i <= drawpoints; i++){
					if(Y[i]!=(Y_notlogoffset[i]+1))
						update=1;
					if(Y2[i]!=(Y2_notlogoffset[i]+1))
						update=1;
					if(Y3[i]!=(Y3_notlogoffset[i]+1))
						update=1;
					if(Y4[i]!=(Y4_notlogoffset[i]+1))
						update=1;
					if(Y5[i]!=(Y5_notlogoffset[i]+1))
						update=1;
					if(Y6[i]!=(Y6_notlogoffset[i]+1))
						update=1;
					if(Y7[i]!=(Y7_notlogoffset[i]+1))
						update=1;
					if(Y8[i]!=(Y8_notlogoffset[i]+1))
						update=1;
					if(Y9[i]!=(Y9_notlogoffset[i]+1))
						update=1;
					if(Y10[i]!=(Y10_notlogoffset[i]+1))
						update=1;
					Y[i]=Y_notlogoffset[i]+1;
					Y2[i]=Y2_notlogoffset[i]+1;	
					Y3[i]=Y3_notlogoffset[i]+1;	
					Y4[i]=Y4_notlogoffset[i]+1;	
					Y5[i]=Y5_notlogoffset[i]+1;	
					Y6[i]=Y6_notlogoffset[i]+1;	
					Y7[i]=Y7_notlogoffset[i]+1;	
					Y8[i]=Y8_notlogoffset[i]+1;	
					Y9[i]=Y9_notlogoffset[i]+1;	
					Y10[i]=Y10_notlogoffset[i]+1;	
				}
			}
		}else{
			peaksaddlog=0;
		}
	}
	for (i = drawpoints; i < POINTS; i++){
		X[i]=X[drawpoints-1];
		Y[i]=Y[drawpoints-1];
		X2[i]=X2[drawpoints-1];
		Y2[i]=Y2[drawpoints-1];
		X3[i]=X3[drawpoints-1];
		Y3[i]=Y3[drawpoints-1];
		X4[i]=X4[drawpoints-1];
		Y4[i]=Y4[drawpoints-1];
		X5[i]=X5[drawpoints-1];
		Y5[i]=Y5[drawpoints-1];
		X6[i]=X6[drawpoints-1];
		Y6[i]=Y6[drawpoints-1];
		X7[i]=X7[drawpoints-1];
		Y7[i]=Y7[drawpoints-1];
		X8[i]=X8[drawpoints-1];
		Y8[i]=Y8[drawpoints-1];
		X9[i]=X9[drawpoints-1];
		Y9[i]=Y9[drawpoints-1];
		X10[i]=X10[drawpoints-1];
		Y10[i]=Y10[drawpoints-1];
	}
   timeoutid = g_timeout_add (1000/FRAME_RATE,(GSourceFunc)update_graph, NULL);
   timeoutidb = g_timeout_add (1000.0/FRAME_RATEb,(GSourceFunc)refresh_label_timeout, NULL);

}



void refresh_graph(){
   timeoutid = g_timeout_add (1000/FRAME_RATE,(GSourceFunc)update_graph, NULL);
   timeoutidb = g_timeout_add (1000.0/FRAME_RATEb,(GSourceFunc)refresh_label_timeout, NULL);
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

	//New lines XXX
	layersshown=(int *)malloc(100*sizeof(double));
   X2 = g_new0 (gfloat, POINTS);
   Y2 = g_new0 (gfloat, POINTS);
   X3 = g_new0 (gfloat, POINTS);
   Y3 = g_new0 (gfloat, POINTS);
   X4 = g_new0 (gfloat, POINTS);
   Y4 = g_new0 (gfloat, POINTS);
   X5 = g_new0 (gfloat, POINTS);
   Y5 = g_new0 (gfloat, POINTS);
   X6 = g_new0 (gfloat, POINTS);
   Y6 = g_new0 (gfloat, POINTS);
   X7 = g_new0 (gfloat, POINTS);
   Y7 = g_new0 (gfloat, POINTS);
   X8 = g_new0 (gfloat, POINTS);
   Y8 = g_new0 (gfloat, POINTS);
   X9 = g_new0 (gfloat, POINTS);
   Y9 = g_new0 (gfloat, POINTS);
   X10 = g_new0 (gfloat, POINTS);
   Y10 = g_new0 (gfloat, POINTS);
   X2_old = (double *)malloc(POINTS*sizeof(double));
   Y2_old =(double *)malloc(POINTS*sizeof(double));
   X3_old = (double *)malloc(POINTS*sizeof(double));
   Y3_old =(double *)malloc(POINTS*sizeof(double));
   X4_old = (double *)malloc(POINTS*sizeof(double));
   Y4_old =(double *)malloc(POINTS*sizeof(double));
   X5_old = (double *)malloc(POINTS*sizeof(double));
   Y5_old =(double *)malloc(POINTS*sizeof(double));
   X6_old = (double *)malloc(POINTS*sizeof(double));
   Y6_old =(double *)malloc(POINTS*sizeof(double));
   X7_old = (double *)malloc(POINTS*sizeof(double));
   Y7_old =(double *)malloc(POINTS*sizeof(double));
   X8_old = (double *)malloc(POINTS*sizeof(double));
   Y8_old =(double *)malloc(POINTS*sizeof(double));
   X9_old = (double *)malloc(POINTS*sizeof(double));
   Y9_old =(double *)malloc(POINTS*sizeof(double));
   X10_old = (double *)malloc(POINTS*sizeof(double));
   Y10_old =(double *)malloc(POINTS*sizeof(double));
	Y2_notlogoffset= g_new0 (gfloat, POINTS);
	Y3_notlogoffset= g_new0 (gfloat, POINTS);
	Y4_notlogoffset= g_new0 (gfloat, POINTS);
	Y5_notlogoffset= g_new0 (gfloat, POINTS);
	Y6_notlogoffset= g_new0 (gfloat, POINTS);
	Y7_notlogoffset= g_new0 (gfloat, POINTS);
	Y8_notlogoffset= g_new0 (gfloat, POINTS);
	Y9_notlogoffset= g_new0 (gfloat, POINTS);
	Y10_notlogoffset= g_new0 (gfloat, POINTS);


   Xpeaks = g_new0 (gfloat, POINTS);
   Ypeaks = g_new0 (gfloat, POINTS);
   X_old = (double *)malloc(POINTS*sizeof(double));
   Y_old =(double *)malloc(POINTS*sizeof(double));
   Xpeaks_old = (double *)malloc(POINTS*sizeof(double));
   Ypeaks_old =(double *)malloc(POINTS*sizeof(double));
   Ypeaks_notlogoffset =(gfloat *)malloc(POINTS*sizeof(double));
	Y_notlogoffset= g_new0 (gfloat, POINTS);
	for (int i = 0; i < POINTS; i++){
		X[i]=i;
		Y[i]=0;
		//New lines XXX
		X2[i]=i;
		Y2[i]=0;
		X3[i]=i;
		Y3[i]=0;
		X4[i]=i;
		Y4[i]=0;
		X5[i]=i;
		Y5[i]=0;
		X6[i]=i;
		Y6[i]=0;
		X7[i]=i;
		Y7[i]=0;
		X8[i]=i;
		Y8[i]=0;
		X9[i]=i;
		Y9[i]=0;
		X10[i]=i;
		Y10[i]=0;
		X2_old[i]=i;
		Y2_old[i]=0;
		X3_old[i]=i;
		Y3_old[i]=0;
		X4_old[i]=i;
		Y4_old[i]=0;
		X5_old[i]=i;
		Y5_old[i]=0;
		X6_old[i]=i;
		Y6_old[i]=0;
		X7_old[i]=i;
		Y7_old[i]=0;
		X8_old[i]=i;
		Y8_old[i]=0;
		X9_old[i]=i;
		Y9_old[i]=0;
		X10_old[i]=i;
		Y10_old[i]=0;
		Xpeaks[i]=i;
		Ypeaks[i]=0;
		X_old[i]=i;
		Y_old[i]=0;
		Xpeaks_old[i]=i;
		Ypeaks_old[i]=0;
		 Ypeaks_notlogoffset[i]=0;
		Y_notlogoffset[i]=0;
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
   color.red = 1;
   color.green = 0;
   color.blue = 0;
   color.alpha = 1;
   graphrangea = gtk_databox_points_new (600, X, Y, &color, 3);
   graphbrangea = gtk_databox_lines_new (600, X, Y, &color, 1);
   graphrangeb = gtk_databox_points_new (4000, X, Y, &color, 3);
   graphbrangeb = gtk_databox_lines_new (4000, X, Y, &color, 1);
   graphrangec = gtk_databox_points_new (10000, X, Y, &color, 3);
   graphbrangec = gtk_databox_lines_new (10000, X, Y, &color, 1);
   graphranged = gtk_databox_points_new (POINTS, X, Y, &color, 3);
   graphbranged = gtk_databox_lines_new (POINTS, X, Y, &color, 1);

		//New lines XXX
   color.red = 0;
   color.green = 1;
   color.blue = 0;
   graph2rangea = gtk_databox_points_new (POINTS, X2, Y2, &color, 3);
   graph2brangea = gtk_databox_lines_new (POINTS, X2, Y2, &color, 1);

   color.red = 0;
   color.green = 0;
   color.blue = 1;
   graph3rangea = gtk_databox_points_new (POINTS, X3, Y3, &color, 3);
   graph3brangea = gtk_databox_lines_new (POINTS, X3, Y3, &color, 1);
   color.red = 0;
   color.green = 1;
   color.blue = 1;
   graph4rangea = gtk_databox_points_new (POINTS, X4, Y4, &color, 3);
   graph4brangea = gtk_databox_lines_new (POINTS, X4, Y4, &color, 1);
   color.red = 0.4392;
   color.green = 0;
   color.blue = 0.3961;
   graph5rangea = gtk_databox_points_new (POINTS, X5, Y5, &color, 3);
   graph5brangea = gtk_databox_lines_new (POINTS, X5, Y5, &color, 1);



   color.red = 1;
   color.green = 1;
   color.blue = 0;
   graph6rangea = gtk_databox_points_new (POINTS, X6, Y6, &color, 3);
   graph6brangea = gtk_databox_lines_new (POINTS, X6, Y6, &color, 1);
   color.red = 0.3372;
   color.green = 0.1960;
   color.blue = 0.6588;
   graph7rangea = gtk_databox_points_new (POINTS, X7, Y7, &color, 3);
   graph7brangea = gtk_databox_lines_new (POINTS, X7, Y7, &color, 1);
   color.red = 1;
   color.green = 0;
   color.blue = 0.7333;
   graph8rangea = gtk_databox_points_new (POINTS, X8, Y8, &color, 3);
   graph8brangea = gtk_databox_lines_new (POINTS, X8, Y8, &color, 1);
   color.red = 1;
   color.green = 0.2509;
   color.blue = 0;
   graph9rangea = gtk_databox_points_new (POINTS, X9, Y9, &color, 3);
   graph9brangea = gtk_databox_lines_new (POINTS, X9, Y9, &color, 1);
   color.red = 0.4588;
   color.green = 0.3137;
   color.blue = 0.0627;
   graph10rangea = gtk_databox_points_new (POINTS, X10, Y10, &color, 3);
   graph10brangea = gtk_databox_lines_new (POINTS, X10, Y10, &color, 1);

   color.red = pointcolor[0];
   color.green = pointcolor[1];
   color.blue = pointcolor[2];
   color.alpha = 1;
   //graphpeaksrangea = gtk_databox_points_new (600, Xpeaks, Ypeaks, &color, 5);
   //graphpeaksrangeb = gtk_databox_points_new (4000, Xpeaks, Ypeaks, &color, 5);
   //graphpeaksrangec = gtk_databox_points_new (10000, Xpeaks, Ypeaks, &color, 5);
   //graphpeaksranged = gtk_databox_points_new (POINTS, Xpeaks, Ypeaks, &color, 5);
	//gtk_databox_graph_add (GTK_DATABOX (box), graphpeaksrangea);
   gtk_databox_graph_add (GTK_DATABOX (box), graphrangea);
   //gtk_databox_graph_add (GTK_DATABOX (box), graphbrangea);
	//gtk_databox_graph_add (GTK_DATABOX (box), graphpeaksrangeb);
   gtk_databox_graph_add (GTK_DATABOX (box), graphrangeb);
   //gtk_databox_graph_add (GTK_DATABOX (box), graphbrangeb);
	//gtk_databox_graph_add (GTK_DATABOX (box), graphpeaksrangec);
   gtk_databox_graph_add (GTK_DATABOX (box), graphrangec);
  // gtk_databox_graph_add (GTK_DATABOX (box), graphbrangec);
	//gtk_databox_graph_add (GTK_DATABOX (box), graphpeaksranged);
   gtk_databox_graph_add (GTK_DATABOX (box), graphranged);
   //gtk_databox_graph_add (GTK_DATABOX (box), graphbranged);
   gtk_databox_graph_add (GTK_DATABOX (box), graphlim);

		//New lines XXX
   gtk_databox_graph_add (GTK_DATABOX (box), graph2rangea);
   //gtk_databox_graph_add (GTK_DATABOX (box), graph2brangea);
   gtk_databox_graph_add (GTK_DATABOX (box), graph3rangea);
   //gtk_databox_graph_add (GTK_DATABOX (box), graph3brangea);
   gtk_databox_graph_add (GTK_DATABOX (box), graph4rangea);
  // gtk_databox_graph_add (GTK_DATABOX (box), graph4brangea);
   gtk_databox_graph_add (GTK_DATABOX (box), graph5rangea);
   //gtk_databox_graph_add (GTK_DATABOX (box), graph5brangea);
   gtk_databox_graph_add (GTK_DATABOX (box), graph6rangea);
   //gtk_databox_graph_add (GTK_DATABOX (box), graph6brangea);
   gtk_databox_graph_add (GTK_DATABOX (box), graph7rangea);
   //gtk_databox_graph_add (GTK_DATABOX (box), graph7brangea);
   gtk_databox_graph_add (GTK_DATABOX (box), graph8rangea);
   //gtk_databox_graph_add (GTK_DATABOX (box), graph8brangea);
   gtk_databox_graph_add (GTK_DATABOX (box), graph9rangea);
   //gtk_databox_graph_add (GTK_DATABOX (box), graph9brangea);
   gtk_databox_graph_add (GTK_DATABOX (box), graph10rangea);
   //gtk_databox_graph_add (GTK_DATABOX (box), graph10brangea);

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
	   color.alpha = 0.2;
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
   g_signal_connect (G_OBJECT (box), "selection-started", G_CALLBACK (handle_signal_selection_started), NULL);
   g_signal_connect (G_OBJECT (box), "selection-finalized",G_CALLBACK (handle_signal_selection_finalized), NULL);
   g_signal_connect (G_OBJECT (box), "selection-canceled", G_CALLBACK (handle_signal_selection_canceled), NULL);
   g_signal_connect (G_OBJECT (box), "button_press_event",G_CALLBACK (handle_signal_click), NULL);
   g_signal_connect_swapped (G_OBJECT (box),"motion_notify_event", G_CALLBACK (motion_notify), NULL);
}

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
int mapdatadarkidbdhdf=-1;

int aqmode=1;



static gint toogle2_click(GtkWidget *button ){
	setlimnext=1;
	hasupdated=1;
	update=1;
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
static gint toogle3_click(GtkWidget *button ){
	setlimnext=1;
	hasupdated=1;
	update=1;
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(tblogscale))==0){
		if(aqrun==0){
			gtk_widget_set_sensitive(tb1b,true);
			gtk_widget_set_sensitive(tb2b,true);
		}
		logscale=0;
	}else{
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tb1b),1);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tb2b),0);
		gtk_databox_set_enable_selection(GTK_DATABOX(box),FALSE);
		gtk_widget_set_sensitive(tb1b,false);
		gtk_widget_set_sensitive(tb2b,false);
		mode=0;
		logscale=1;
	}
	int i;
	for (i = drawpoints; i < POINTS; i++){
		X[i]=X[drawpoints-1];
		Y[i]=Y[drawpoints-1];
		X2[i]=X2[drawpoints-1];
		Y2[i]=Y2[drawpoints-1];
		X3[i]=X3[drawpoints-1];
		Y3[i]=Y3[drawpoints-1];
		X4[i]=X4[drawpoints-1];
		Y4[i]=Y4[drawpoints-1];
		X5[i]=X5[drawpoints-1];
		Y5[i]=Y5[drawpoints-1];
		X6[i]=X6[drawpoints-1];
		Y6[i]=Y6[drawpoints-1];
		X7[i]=X7[drawpoints-1];
		Y7[i]=Y7[drawpoints-1];
		X8[i]=X8[drawpoints-1];
		Y8[i]=Y8[drawpoints-1];
		X9[i]=X9[drawpoints-1];
		Y9[i]=Y9[drawpoints-1];
		X10[i]=X10[drawpoints-1];
		Y10[i]=Y10[drawpoints-1];
	}
	if(hasupdated==1){
		if(logscale==1){
			//Check graph min y value.
			float yminbuplog=Y[0];
	  		for (i = 0; i <= drawpoints; i++){
				if(Y[i]<yminbuplog)
					yminbuplog=Y[i];
				if(Y2[i]<yminbuplog)
					yminbuplog=Y2[i];
				if(Y3[i]<yminbuplog)
					yminbuplog=Y3[i];
				if(Y4[i]<yminbuplog)
					yminbuplog=Y4[i];
				if(Y5[i]<yminbuplog)
					yminbuplog=Y5[i];
				if(Y6[i]<yminbuplog)
					yminbuplog=Y6[i];
				if(Y7[i]<yminbuplog)
					yminbuplog=Y7[i];
				if(Y8[i]<yminbuplog)
					yminbuplog=Y8[i];
				if(Y9[i]<yminbuplog)
					yminbuplog=Y9[i];
				if(Y10[i]<yminbuplog)
					yminbuplog=Y10[i];
			}
			//Add offset to graph if it has parts below zero.
			if(yminbuplog<=0){
				peaksaddlog=-yminbuplog+1;
		  		for (i = 0; i <= drawpoints; i++){
					Y[i]=Y[i]-yminbuplog+1;	
					Y2[i]=Y2[i]-yminbuplog+1;	
					Y3[i]=Y3[i]-yminbuplog+1;	
					Y4[i]=Y4[i]-yminbuplog+1;	
					Y5[i]=Y5[i]-yminbuplog+1;
					Y6[i]=Y6[i]-yminbuplog+1;	
					Y7[i]=Y7[i]-yminbuplog+1;	
					Y8[i]=Y8[i]-yminbuplog+1;	
					Y9[i]=Y9[i]-yminbuplog+1;	
					Y10[i]=Y10[i]-yminbuplog+1;	
				}
			}else{
				peaksaddlog=1;
		  		for (i = 0; i <= drawpoints; i++){
					Y[i]=Y[i]+1;
					Y2[i]=Y2[i]+1;	
					Y3[i]=Y3[i]+1;	
					Y4[i]=Y4[i]+1;	
					Y5[i]=Y5[i]+1;
					Y6[i]=Y6[i]+1;	
					Y7[i]=Y7[i]+1;	
					Y8[i]=Y8[i]+1;	
					Y9[i]=Y9[i]+1;	
					Y10[i]=Y10[i]+1;	
				}
			}
		}else{
			for (i = 0; i < POINTS; i++){
				Y[i]=Y[i]-peaksaddlog;
				Y2[i]=Y2[i]-peaksaddlog;
				Y3[i]=Y3[i]-peaksaddlog;
				Y4[i]=Y4[i]-peaksaddlog;
				Y5[i]=Y5[i]-peaksaddlog;
				Y6[i]=Y6[i]-peaksaddlog;
				Y7[i]=Y7[i]-peaksaddlog;
				Y8[i]=Y8[i]-peaksaddlog;
				Y9[i]=Y9[i]-peaksaddlog;
				Y10[i]=Y10[i]-peaksaddlog;
			}
			peaksaddlog=0;
		}
	}
	for (i = drawpoints; i < POINTS; i++){
		X[i]=X[drawpoints-1];
		Y[i]=Y[drawpoints-1];
		X2[i]=X2[drawpoints-1];
		Y2[i]=Y2[drawpoints-1];
		X3[i]=X3[drawpoints-1];
		Y3[i]=Y3[drawpoints-1];
		X4[i]=X4[drawpoints-1];
		Y4[i]=Y4[drawpoints-1];
		X5[i]=X5[drawpoints-1];
		Y5[i]=Y5[drawpoints-1];
		X6[i]=X6[drawpoints-1];
		Y6[i]=Y6[drawpoints-1];
		X7[i]=X7[drawpoints-1];
		Y7[i]=Y7[drawpoints-1];
		X8[i]=X8[drawpoints-1];
		Y8[i]=Y8[drawpoints-1];
		X9[i]=X9[drawpoints-1];
		Y9[i]=Y9[drawpoints-1];
		X10[i]=X10[drawpoints-1];
		Y10[i]=Y10[drawpoints-1];
	}
	for (i = 0; i < POINTS; i++){
		Ypeaks[i]=Ypeaks_notlogoffset[i]+peaksaddlog;
		Ypeaks_old[i]=Ypeaks_notlogoffset[i]+peaksaddlog;
	}
	gboolean nullup=update_graph();
   return 0;
}
float oldlinecolor[3];
float oldpointcolor[3];
float oldtextcolor[3];
int olddarktheme=-1;


GtkWidget *export_graph(int rlineonc,int glineonc, int blineonc, int r2lineonc,int g2lineonc,int b2lineonc){
	GObject *observed;
	GtkWidget *widget;
	float linecolor[3];
	float pointcolor[3];
	float textcolor[3];
	int darktheme=0;
	//if(!PyArg_ParseTuple(args,"Offfffffffi",&object,&linecolor[0],&linecolor[1],&linecolor[2],&pointcolor[0],&pointcolor[1],&pointcolor[2],&textcolor[0],&textcolor[1],&textcolor[2],&darktheme))
		//return NULL;
    linecolor[0]=rlineonc;
    linecolor[1]=glineonc;
    linecolor[2]=blineonc;
    pointcolor[0]=0;
    pointcolor[1]=0;
    pointcolor[2]=0;
    textcolor[0]=r2lineonc;
    textcolor[1]=g2lineonc;
    textcolor[2]=b2lineonc;
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

	GtkToolItem *tblogscaleti;
	tblogscaleti=gtk_tool_item_new();
 	tblogscale = gtk_toggle_button_new();


	gtk_container_add (GTK_CONTAINER(tb2),tb2b);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tb2, -1);

		GtkToolItem *sepghgfdgf;	
		sepghgfdgf=gtk_separator_tool_item_new();
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), sepghgfdgf, -1);

	gtk_button_set_label(GTK_BUTTON(tblogscale),"LOG");

	gtk_container_add (GTK_CONTAINER(tblogscaleti),tblogscale);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tblogscaleti, -1);

   g_signal_connect (GTK_TOGGLE_BUTTON(tblogscale), "toggled",G_CALLBACK (toogle3_click), NULL);
	//-----
	//-----
	//-----
	//Buttons
	//-----

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
	//object=pygobject_new(G_OBJECT(widget));
	return widget;

}

