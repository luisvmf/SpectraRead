#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <locale.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include "graph.c"
#include "../bin_modules/FastMmapMQ/fastmmapmq.c"

//XXX TODO: Only things that are missing are to import datasources from json file.


#define lineplotpointsmax 10000000
#define maxpointsplot 1000 //Must be the same as the maximum number of points set on glade adjustment 1.
#define maxpeaksdatasource 100
int *mapid;
float *xaq;
float *yaq;
float *x2aq;
float *y2aq;
float *x3aq;
float *y3aq;
float *x4aq;
float *y4aq;
float *x5aq;
float *y5aq;
float *x6aq;
float *y6aq;
float *x7aq;
float *y7aq;
float *x8aq;
float *y8aq;
float *x9aq;
float *y9aq;
float *x10aq;
float *y10aq;
GtkWidget*  AboutWindow;
GtkWidget*  window;
GtkWidget*  scrollb;
GtkSwitch*  sw1;
GtkSwitch*  sw2;
GtkSwitch*  sw3;
GtkSwitch*  sw4;
GtkSwitch*  sw5;
GtkSwitch*  sw6;
GtkSwitch*  sw7;
GtkSwitch*  sw8;
GtkSwitch*  sw9;
GtkSwitch*  sw10;
int pointsshow;
int pointsmeasured;
GtkSpinButton*  pointsentry;
GtkToggleButton*  startstopbutton;
GtkAdjustment * scrollbarbottom;
GtkListStore * liststore;
int datasourcesnumber;
int *datasourcesids;
int *datasourceshastime;
GtkComboBox **datasourcescombo;
char** datasourcesmmapprogramnames;
char** datasourcesmmapkeys;
GtkAdjustment **peaksnumberadjustment;
int perfoptimncalls=0;
float *xupc;
float *yupc;
float *x2upc;
float *y2upc;
float *x3upc;
float *y3upc;
float *x4upc;
float *y4upc;
float *x5upc;
float *y5upc;
float *x6upc;
float *y6upc;
float *x7upc;
float *y7upc;
float *x8upc;
float *y8upc;
float *x9upc;
float *y9upc;
float *x10upc;
float *y10upc;
float realaddpeaks[10];
float realaddpeakstime[10];
int *pointsmeasureddatasource;
int initrealaddpeaks=0;
GtkBuilder* builder;
int graphnpointshaschanged=0;
float lastvalscrollbar=-1.0;

void save_to_file(char *filename){
	FILE *filewrite;
	setlocale(LC_ALL,"C");
	filewrite=fopen(filename,"w");
	if(!filewrite){
		return;
	}
	char *linecontent=(char *)malloc(9000*sizeof(char));
	for(int i=0;i<pointsmeasured;i++){
		linecontent[0]='\0';
		sprintf(linecontent,"%020.10f\t%020.10f\t%020.10f\t%020.10f\t%020.10f\t%020.10f\t%020.10f\t%020.10f\t%020.10f\t%020.10f\n",xaq[i],yaq[i],x2aq[i],y2aq[i],x3aq[i],y3aq[i],x4aq[i],y4aq[i],x5aq[i],y5aq[i],x6aq[i],y6aq[i],x7aq[i],y7aq[i],x8aq[i],y8aq[i],x9aq[i],y9aq[i],x10aq[i],y10aq[i]);
    	fwrite(linecontent, 1, strlen(linecontent), filewrite);
		
	}
	free(linecontent);
	fclose(filewrite);
}
void addpoint(float x, float y,float x2, float y2,float x3, float y3,float x4, float y4,float x5, float y5,float x6, float y6,float x7, float y7,float x8, float y8,float x9, float y9,float x10, float y10){
	if(pointsmeasured<lineplotpointsmax-10){
		xaq[pointsmeasured]=x;
		yaq[pointsmeasured]=y;
		x2aq[pointsmeasured]=x2;
		y2aq[pointsmeasured]=y2;
		x3aq[pointsmeasured]=x3;
		y3aq[pointsmeasured]=y3;
		x4aq[pointsmeasured]=x4;
		y4aq[pointsmeasured]=y4;
		x5aq[pointsmeasured]=x5;
		y5aq[pointsmeasured]=y5;
		x6aq[pointsmeasured]=x6;
		y6aq[pointsmeasured]=y6;
		x7aq[pointsmeasured]=x7;
		y7aq[pointsmeasured]=y7;
		x8aq[pointsmeasured]=x8;
		y8aq[pointsmeasured]=y8;
		x9aq[pointsmeasured]=x9;
		y9aq[pointsmeasured]=y9;
		x10aq[pointsmeasured]=x10;
		y10aq[pointsmeasured]=y10;
		pointsmeasured=pointsmeasured+1;
	}
}

double maxgfgd=0;
static gboolean pointtimmer(void){
	setlocale(LC_ALL,"C");
	int measurestatus=gtk_toggle_button_get_active(startstopbutton);
	char *received=NULL;
	float datapeaks[maxpeaksdatasource+5];
	float datapeakstime[maxpeaksdatasource+5];
	float datapeakstimecopy[maxpeaksdatasource+5];
	for(int jjk=0;jjk<=maxpeaksdatasource;jjk++){
		datapeaks[jjk]=0.0;
		datapeakstime[jjk]=0.0;
	}
	for(int jjk=0;jjk<=9;jjk++){
		datapeaks[jjk]=0.0;
		datapeakstime[jjk]=0.0;
		if(initrealaddpeaks==0){
			realaddpeaks[jjk]=0.0;
			realaddpeakstime[jjk]=0.0;
		}
	}
	initrealaddpeaks=1;
	//For each data source we repeat the loop bellow
	for(int dsii=0;dsii<datasourcesnumber;dsii++){
							for(int jjk=0;jjk<=maxpeaksdatasource;jjk++){
								datapeaks[jjk]=0.0;
							}
							if(perfoptimncalls>10){
								if(mapid[dsii]==-1){
									//printf("Connection failled to %s %s, retrying...\n",datasourcesmmapprogramnames[dsii],datasourcesmmapkeys[dsii]);
									mapid[dsii]=fastmmapmq_connectmmap(datasourcesmmapprogramnames[dsii],datasourcesmmapkeys[dsii]);
								}
								perfoptimncalls=0;
							}
							perfoptimncalls=perfoptimncalls+1;
							int received_fastmmapmq_peaks=-1;
							if(mapid[dsii]!=-1){
								received=fastmmapmq_readmmap(mapid[dsii],0);
								if(received[0]!='\0'){
									char *token;
									char *tokenb;
									int kk;
									double thistime=0;
									token=strtok(received,"\n");
									while(token!=NULL){
										tokenb=strtok(token," ");
										if(datasourceshastime[dsii]==1){
											if(tokenb!=NULL){
												thistime=atof(tokenb);
												//printf("%f\n",thistime);
											}
											tokenb=strtok(NULL," ");
										}else{
											thistime=pointsmeasureddatasource[dsii];
											pointsmeasureddatasource[dsii]=pointsmeasureddatasource[dsii]+1;
										}
										kk=0;
										while(tokenb!=NULL){
											if(kk<=maxpeaksdatasource){
												datapeaks[kk]=atof(tokenb);
												//printf("%f\n",datapeaks[kk]);
												datapeakstime[kk]=thistime;
												received_fastmmapmq_peaks=1;
											}
											kk=kk+1;
											tokenb=strtok(NULL," ");
										}
												if(received_fastmmapmq_peaks==1){
													if(measurestatus==1){
														for(int jjk=0;jjk<=9;jjk++){
															//printf("%f ",datapeaks[jjk]);
															//printf("%f\n",gtk_adjustment_get_value(peaksnumberadjustment[jjk]));
															int peaknumintgh=gtk_adjustment_get_value(peaksnumberadjustment[jjk])-1;
															//Get the active data source:
															int indexfdsghfds=gtk_combo_box_get_active(datasourcescombo[jjk]);
															//Here we only change the realaddpeaks value if datasource selected corresponds to datasource on current loop.
															if(indexfdsghfds==datasourcesids[dsii]){
																if(peaknumintgh<=maxpeaksdatasource){
																	realaddpeaks[jjk]=datapeaks[peaknumintgh];
																	realaddpeakstime[jjk]=datapeakstime[peaknumintgh];
																}
															}
															}
														for(int jjk=0;jjk<=9;jjk++){
															if(realaddpeakstime[jjk]>maxgfgd){
																maxgfgd=realaddpeakstime[jjk];
															}
														}
														for(int jjk=0;jjk<=9;jjk++){
															datapeakstimecopy[jjk]=realaddpeakstime[jjk];
														}
													addpoint(datapeakstimecopy[0],realaddpeaks[0],datapeakstimecopy[1],realaddpeaks[1],datapeakstimecopy[2],realaddpeaks[2],datapeakstimecopy[3],realaddpeaks[3],datapeakstimecopy[4],realaddpeaks[4],datapeakstimecopy[5],realaddpeaks[5],datapeakstimecopy[6],realaddpeaks[6],datapeakstimecopy[7],realaddpeaks[7],datapeakstimecopy[8],realaddpeaks[8],datapeakstimecopy[9],realaddpeaks[9]);
													}
												}
										token=strtok(NULL,"\n");
								   }
								}
								if(received!=NULL){
									free(received);
								}
							}
		}
	if(measurestatus==1){
		if(aqrun==0){
			gtk_widget_set_sensitive(scrollb,false);
			gtk_widget_set_sensitive(tb1b,false);
			gtk_widget_set_sensitive(tb2b,false);
			gtk_adjustment_set_value(scrollbarbottom,100);
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tb1b),1);
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tb2b),0);
			gtk_databox_set_enable_selection(GTK_DATABOX(box),FALSE);
			mode=0;
			aqrun=1;
		}
	}else{
		if(aqrun==1){
			gtk_widget_set_sensitive(scrollb,true);
			gtk_widget_set_sensitive(tb1b,true);
			gtk_widget_set_sensitive(tb2b,true);
			aqrun=0;
		}
	}
	return TRUE;
}

float addchangept=0;
static gboolean updatepeaksmainc(void){
	layersshown[0]=(gtk_switch_get_state(sw1)==TRUE);
	layersshown[1]=(gtk_switch_get_state(sw2)==TRUE);
	layersshown[2]=(gtk_switch_get_state(sw3)==TRUE);
	layersshown[3]=(gtk_switch_get_state(sw5)==TRUE);
	layersshown[4]=(gtk_switch_get_state(sw4)==TRUE);
	layersshown[5]=(gtk_switch_get_state(sw6)==TRUE);
	layersshown[6]=(gtk_switch_get_state(sw7)==TRUE);
	layersshown[7]=(gtk_switch_get_state(sw8)==TRUE);
	layersshown[8]=(gtk_switch_get_state(sw9)==TRUE);
	layersshown[9]=(gtk_switch_get_state(sw10)==TRUE);
	hidden=0;
	if((gtk_spin_button_get_value(pointsentry)-1)!=pointsshow){
		pointsshow=gtk_spin_button_get_value(pointsentry)-1;
		graphnpointshaschanged=1;
	}
	int currentpointsshow=pointsshow;
	int j=0;
	volatile float valscrollbar=gtk_adjustment_get_value(scrollbarbottom);
	volatile int valstsb=(pointsmeasured*valscrollbar)/100.0;//Final value for graph.
	//printf("%i %i %i\n",valstsb,pointsmeasured-currentpointsshow,pointsmeasured-valstsb );
	if(valstsb>pointsmeasured){
		valstsb=pointsmeasured;
	}
	int kki=valstsb-currentpointsshow;//Initial value for graph.
	if(kki<0){
		kki=0;
		if(pointsmeasured>currentpointsshow-1){
			valstsb=pointsshow;
		}else{
			valstsb=pointsmeasured;
		}
	}
	if(lastvalscrollbar!=valscrollbar){
	lastvalscrollbar=valscrollbar;
	graphnpointshaschanged=1;
	}
			if(addchangept==0){
			addchangept=yaq[kki];
			for(int i=kki;i<valstsb;i++){
				if(yaq[i]>addchangept){
					addchangept=yaq[i];
				}
				if(y2aq[i]>addchangept){
					addchangept=y2aq[i];
				}
				if(y3aq[i]>addchangept){
					addchangept=y3aq[i];
				}
				if(y4aq[i]>addchangept){
					addchangept=y4aq[i];
				}
				if(y5aq[i]>addchangept){
					addchangept=y5aq[i];
				}
				if(y6aq[i]>addchangept){
					addchangept=y6aq[i];
				}
				if(y7aq[i]>addchangept){
					addchangept=y7aq[i];
				}
				if(y8aq[i]>addchangept){
					addchangept=y8aq[i];
				}
				if(y9aq[i]>addchangept){
					addchangept=y9aq[i];
				}
				if(y10aq[i]>addchangept){
					addchangept=y10aq[i];
				}
			}
			addchangept=addchangept/1.0;
		hidden=1;
		}else{
			addchangept=0;
		}
		if(graphnpointshaschanged==0){
			addchangept=0;
			hidden=0;
		}
		if(hidden){
			gtk_databox_graph_set_hide(graph2brangea,TRUE);
			gtk_databox_graph_set_hide(graph2rangea,TRUE);
			gtk_databox_graph_set_hide(graph3brangea,TRUE);
			gtk_databox_graph_set_hide(graph3rangea,TRUE);
			gtk_databox_graph_set_hide(graph4brangea,TRUE);
			gtk_databox_graph_set_hide(graph4rangea,TRUE);
			gtk_databox_graph_set_hide(graph5brangea,TRUE);
			gtk_databox_graph_set_hide(graph5rangea,TRUE);
			gtk_databox_graph_set_hide(graph6brangea,TRUE);
			gtk_databox_graph_set_hide(graph6rangea,TRUE);
			gtk_databox_graph_set_hide(graph7brangea,TRUE);
			gtk_databox_graph_set_hide(graph7rangea,TRUE);
			gtk_databox_graph_set_hide(graph8brangea,TRUE);
			gtk_databox_graph_set_hide(graph8rangea,TRUE);
			gtk_databox_graph_set_hide(graph9brangea,TRUE);
			gtk_databox_graph_set_hide(graph9rangea,TRUE);
			gtk_databox_graph_set_hide(graph10brangea,TRUE);
			gtk_databox_graph_set_hide(graph10rangea,TRUE);
			gtk_databox_graph_set_hide(graphbrangea,TRUE);
			gtk_databox_graph_set_hide(graphbrangeb,TRUE);
			gtk_databox_graph_set_hide(graphbrangec,TRUE);
			gtk_databox_graph_set_hide(graphbranged,TRUE);
			gtk_databox_graph_set_hide(graphrangea,TRUE);
			gtk_databox_graph_set_hide(graphrangeb,TRUE);
			gtk_databox_graph_set_hide(graphrangec,TRUE);
			gtk_databox_graph_set_hide(graphranged,TRUE);
		}
		graphnpointshaschanged=0;
		float nn=0;
		float nnb=0;
		for(int i=kki;i<valstsb;i++){
		xupc[j]=xaq[i];
		yupc[j]=yaq[i]+addchangept;
		x2upc[j]=x2aq[i];
		y2upc[j]=y2aq[i]+addchangept;
		x3upc[j]=x3aq[i];
		y3upc[j]=y3aq[i]+addchangept;
		x4upc[j]=x4aq[i];
		y4upc[j]=y4aq[i]+addchangept;
		x5upc[j]=x5aq[i];
		y5upc[j]=y5aq[i]+addchangept;
		x6upc[j]=x6aq[i];
		y6upc[j]=y6aq[i]+addchangept;
		x7upc[j]=x7aq[i];
		y7upc[j]=y7aq[i]+addchangept;
		x8upc[j]=x8aq[i];
		y8upc[j]=y8aq[i]+addchangept;
		x9upc[j]=x9aq[i];
		y9upc[j]=y9aq[i]+addchangept;
		x10upc[j]=x10aq[i];
		y10upc[j]=y10aq[i]+addchangept;
		nn=xaq[i];
		nnb=yaq[i]+addchangept;
		j=j+1;
		x2upc[j]=x2aq[i];
		y2upc[j]=y2aq[i]+addchangept;
		x3upc[j]=x3aq[i];
		y3upc[j]=y3aq[i]+addchangept;
		x4upc[j]=x4aq[i];
		y4upc[j]=y4aq[i]+addchangept;
		x5upc[j]=x5aq[i];
		y5upc[j]=y5aq[i]+addchangept;
		x6upc[j]=x6aq[i];
		y6upc[j]=y6aq[i]+addchangept;
		x7upc[j]=x7aq[i];
		y7upc[j]=y7aq[i]+addchangept;
		x8upc[j]=x8aq[i];
		y8upc[j]=y8aq[i]+addchangept;
		x9upc[j]=x9aq[i];
		y9upc[j]=y9aq[i]+addchangept;
		x10upc[j]=x10aq[i];
		y10upc[j]=y10aq[i]+addchangept;
		}
		xupc[j]=nn;
		yupc[j]=nnb;
  	  updategraph(xupc,yupc,x2upc,y2upc,x3upc,y3upc,x4upc,y4upc,x5upc,y5upc,x6upc,y6upc,x7upc,y7upc,x8upc,y8upc,x9upc,y9upc,x10upc,y10upc,valstsb-kki+1, 1);
	return TRUE;
}


int main(int argc, char* argv[]){
	//***************************************
	//***************************************
	//XXX Here we should initialize the datasources, from json config. Datasources are initialized on start, from spectrareads instances running, and are not modified later.
	//Number of data sources
	int idmmappeakssplash=fastmmapmq_connectmmap("peakss","1");
	while(idmmappeakssplash==-1){
		printf("Connecting peakss fastmmapmq on peaks main.c...\n");
		idmmappeakssplash=fastmmapmq_connectmmap("peakss",argv[argc-1]);
		usleep(100000);
	}
	fastmmapmq_writemmap(idmmappeakssplash,",10initializing");
	usleep(500000);
	argc=argc-1;
	if((argc-1)%4!=0){
		printf("Peaksfit: Invalid number of arguments\n");
		return 0;
	}
	datasourcesnumber=(argc-1)/4;
	printf("Number of data sources on command line: %i\n",datasourcesnumber);
	datasourcesids=(int *)malloc(datasourcesnumber*sizeof(int));
	mapid=(int *)malloc(datasourcesnumber*sizeof(int));
	pointsmeasureddatasource=(int *)malloc(datasourcesnumber*sizeof(int));

	char** datasourcesnames=(char **)malloc((datasourcesnumber*sizeof(char*)));
	datasourceshastime=(int *)malloc((datasourcesnumber*sizeof(int)));
	datasourcesmmapprogramnames=(char **)malloc((datasourcesnumber*sizeof(char*)));
	datasourcesmmapkeys=(char **)malloc((datasourcesnumber*sizeof(char*)));
	int datasourceiik=0;
	for(int kkcountsource=1;kkcountsource<argc;kkcountsource=kkcountsource+4){
		printf("Source name %s hastimestamp %s progname %s key %s\n",argv[kkcountsource],argv[kkcountsource+1],argv[kkcountsource+2],argv[kkcountsource+3]);
		datasourcesnames[datasourceiik]=argv[kkcountsource];
		datasourceshastime[datasourceiik]=atoi(argv[kkcountsource+1]);
		datasourcesmmapprogramnames[datasourceiik]=argv[kkcountsource+2];
		datasourcesmmapkeys[datasourceiik]=argv[kkcountsource+3];
		datasourceiik=datasourceiik+1;
	}
	//char* datasourcesnames[] = { "Qimon", "Spectraread" };
	//int datasourceshastimecolumn[] = { 0, 1 };// If first column of fastmmapdata is a time stamp (1) or peak (0)
	//datasourceshastime=datasourceshastimecolumn;
	//char* datasourcesmmapprogramnames_local[] = { "qimon", "spectrareadd" };
	//datasourcesmmapprogramnames=datasourcesmmapprogramnames_local;
	//char* datasourcesmmapkeys_local[] = { "qimonpeaks", "pfgs" };
	//datasourcesmmapkeys=datasourcesmmapkeys_local;
	for(int dsii=0;dsii<datasourcesnumber;dsii++){
		mapid[dsii]=-1;
		datasourcesids[dsii]=dsii;
		pointsmeasureddatasource[dsii]=0;
	}
	//***************************************
	//***************************************
	fastmmapmq_writemmap(idmmappeakssplash,",10load ui");


		srand(time(NULL));
        GtkWidget*  box;
        gtk_init(&argc, &argv);
  builder = gtk_builder_new ();
  gtk_builder_add_from_file (builder, "main.glade", NULL);

        window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
        box = GTK_WIDGET(gtk_builder_get_object(builder, "box_graph"));
        pointsentry = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "pointsshow"));
     startstopbutton=GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "startstop"));
	AboutWindow=GTK_WIDGET(gtk_builder_get_object(builder, "Aboutwin"));
	scrollb=GTK_WIDGET(gtk_builder_get_object(builder, "scrollb"));
	scrollbarbottom=GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adjustment2"));
	liststore=GTK_LIST_STORE(gtk_builder_get_object(builder, "liststore1"));
	peaksnumberadjustment=(GtkAdjustment **)malloc(12*sizeof(GtkAdjustment *));
	peaksnumberadjustment[0]=GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adjustment4"));
	peaksnumberadjustment[1]=GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adjustment5"));
	peaksnumberadjustment[2]=GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adjustment6"));
	peaksnumberadjustment[3]=GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adjustment8"));
	peaksnumberadjustment[4]=GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adjustment7"));
	peaksnumberadjustment[5]=GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adjustment9"));
	peaksnumberadjustment[6]=GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adjustment10"));
	peaksnumberadjustment[7]=GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adjustment11"));
	peaksnumberadjustment[8]=GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adjustment12"));
	peaksnumberadjustment[9]=GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adjustment13"));
	GtkTreeIter myiter;
	for(int dsii=0;dsii<datasourcesnumber;dsii++){
		gtk_list_store_append(liststore,&myiter);
		gtk_list_store_set(liststore,&myiter,0,datasourcesnames[dsii],-1);
	}
	gtk_combo_box_set_active(GTK_COMBO_BOX(gtk_builder_get_object(builder, "c1")),0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(gtk_builder_get_object(builder, "c2")),0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(gtk_builder_get_object(builder, "c3")),0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(gtk_builder_get_object(builder, "c4")),0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(gtk_builder_get_object(builder, "c5")),0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(gtk_builder_get_object(builder, "c6")),0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(gtk_builder_get_object(builder, "c7")),0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(gtk_builder_get_object(builder, "c8")),0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(gtk_builder_get_object(builder, "c9")),0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(gtk_builder_get_object(builder, "c10")),0);
	sw1=GTK_SWITCH(gtk_builder_get_object(builder, "line1switch"));
	sw2=GTK_SWITCH(gtk_builder_get_object(builder, "line2switch"));
	sw3=GTK_SWITCH(gtk_builder_get_object(builder, "line3switch"));
	sw4=GTK_SWITCH(gtk_builder_get_object(builder, "line4switch"));
	sw5=GTK_SWITCH(gtk_builder_get_object(builder, "line5switch"));
	sw6=GTK_SWITCH(gtk_builder_get_object(builder, "line6switch"));
	sw7=GTK_SWITCH(gtk_builder_get_object(builder, "line7switch"));
	sw8=GTK_SWITCH(gtk_builder_get_object(builder, "line8switch"));
	sw9=GTK_SWITCH(gtk_builder_get_object(builder, "line9switch"));
	sw10=GTK_SWITCH(gtk_builder_get_object(builder, "line10switch"));



datasourcescombo=(GtkComboBox **)malloc(10*sizeof(GtkComboBox *));
datasourcescombo[0]=GTK_COMBO_BOX(gtk_builder_get_object(builder, "c1"));
datasourcescombo[1]=GTK_COMBO_BOX(gtk_builder_get_object(builder, "c2"));
datasourcescombo[2]=GTK_COMBO_BOX(gtk_builder_get_object(builder, "c3"));
datasourcescombo[3]=GTK_COMBO_BOX(gtk_builder_get_object(builder, "c5"));
datasourcescombo[4]=GTK_COMBO_BOX(gtk_builder_get_object(builder, "c4"));
datasourcescombo[5]=GTK_COMBO_BOX(gtk_builder_get_object(builder, "c6"));
datasourcescombo[6]=GTK_COMBO_BOX(gtk_builder_get_object(builder, "c7"));
datasourcescombo[7]=GTK_COMBO_BOX(gtk_builder_get_object(builder, "c8"));
datasourcescombo[8]=GTK_COMBO_BOX(gtk_builder_get_object(builder, "c9"));
datasourcescombo[9]=GTK_COMBO_BOX(gtk_builder_get_object(builder, "c10"));


        gtk_builder_connect_signals(builder, NULL);


	  GtkWidget* graph=export_graph(0,1,0,0,0,0);//line color r,g,b, text color r,g,b.
	layersshown[0]=(gtk_switch_get_state(sw1)==TRUE);
	layersshown[1]=(gtk_switch_get_state(sw2)==TRUE);
	layersshown[2]=(gtk_switch_get_state(sw3)==TRUE);
	layersshown[3]=(gtk_switch_get_state(sw5)==TRUE);
	layersshown[4]=(gtk_switch_get_state(sw4)==TRUE);
	layersshown[5]=(gtk_switch_get_state(sw6)==TRUE);
	layersshown[6]=(gtk_switch_get_state(sw7)==TRUE);
	layersshown[7]=(gtk_switch_get_state(sw8)==TRUE);
	layersshown[8]=(gtk_switch_get_state(sw9)==TRUE);
	layersshown[9]=(gtk_switch_get_state(sw10)==TRUE);
           gtk_box_pack_start (GTK_BOX (box), graph, TRUE, TRUE, 0);
	usleep(1000000);
	fastmmapmq_writemmap(idmmappeakssplash,",10load graph lib");
xupc=malloc(lineplotpointsmax*sizeof(float));
yupc=malloc(lineplotpointsmax*sizeof(float));
xaq=malloc(lineplotpointsmax*sizeof(float));
yaq=malloc(lineplotpointsmax*sizeof(float));


x2upc=malloc(lineplotpointsmax*sizeof(float));
y2upc=malloc(lineplotpointsmax*sizeof(float));
x2aq=malloc(lineplotpointsmax*sizeof(float));
y2aq=malloc(lineplotpointsmax*sizeof(float));

x3upc=malloc(lineplotpointsmax*sizeof(float));
y3upc=malloc(lineplotpointsmax*sizeof(float));
x3aq=malloc(lineplotpointsmax*sizeof(float));
y3aq=malloc(lineplotpointsmax*sizeof(float));

x4upc=malloc(lineplotpointsmax*sizeof(float));
y4upc=malloc(lineplotpointsmax*sizeof(float));
x4aq=malloc(lineplotpointsmax*sizeof(float));
y4aq=malloc(lineplotpointsmax*sizeof(float));

x5upc=malloc(lineplotpointsmax*sizeof(float));
y5upc=malloc(lineplotpointsmax*sizeof(float));
x5aq=malloc(lineplotpointsmax*sizeof(float));
y5aq=malloc(lineplotpointsmax*sizeof(float));

x6upc=malloc(lineplotpointsmax*sizeof(float));
y6upc=malloc(lineplotpointsmax*sizeof(float));
x6aq=malloc(lineplotpointsmax*sizeof(float));
y6aq=malloc(lineplotpointsmax*sizeof(float));
	usleep(500000);
	fastmmapmq_writemmap(idmmappeakssplash,",10main ui load");

x7upc=malloc(lineplotpointsmax*sizeof(float));
y7upc=malloc(lineplotpointsmax*sizeof(float));
x7aq=malloc(lineplotpointsmax*sizeof(float));
y7aq=malloc(lineplotpointsmax*sizeof(float));

x8upc=malloc(lineplotpointsmax*sizeof(float));
y8upc=malloc(lineplotpointsmax*sizeof(float));
x8aq=malloc(lineplotpointsmax*sizeof(float));
y8aq=malloc(lineplotpointsmax*sizeof(float));

x9upc=malloc(lineplotpointsmax*sizeof(float));
y9upc=malloc(lineplotpointsmax*sizeof(float));
x9aq=malloc(lineplotpointsmax*sizeof(float));
y9aq=malloc(lineplotpointsmax*sizeof(float));

x10upc=malloc(lineplotpointsmax*sizeof(float));
y10upc=malloc(lineplotpointsmax*sizeof(float));
x10aq=malloc(lineplotpointsmax*sizeof(float));
y10aq=malloc(lineplotpointsmax*sizeof(float));

pointsshow=0;
pointsmeasured=0;
for(int i=0;i<lineplotpointsmax;i++){
xupc[i]=i;
xaq[i]=i;
yupc[i]=0;
yaq[i]=0;


x2upc[i]=i;
x2aq[i]=i;
y2upc[i]=0;
y2aq[i]=0;
x3upc[i]=i;
x3aq[i]=i;
y3upc[i]=0;
y3aq[i]=0;
x4upc[i]=i;
x4aq[i]=i;
y4upc[i]=0;
y4aq[i]=0;
x5upc[i]=i;
x5aq[i]=i;
y5upc[i]=0;
y5aq[i]=0;

x6upc[i]=i;
x6aq[i]=i;
y6upc[i]=0;
y6aq[i]=0;
x7upc[i]=i;
x7aq[i]=i;
y7upc[i]=0;
y7aq[i]=0;
x8upc[i]=i;
x8aq[i]=i;
y8upc[i]=0;
y8aq[i]=0;
x9upc[i]=i;
x9aq[i]=i;
y9upc[i]=0;
y9aq[i]=0;
x10upc[i]=i;
x10aq[i]=i;
y10upc[i]=0;
y10aq[i]=0;


}
updategraph(xupc,yupc,xupc,yupc,xupc,yupc,xupc,yupc,xupc,yupc,xupc,yupc,xupc,yupc,xupc,yupc,xupc,yupc,xupc,yupc,1, 1);

	usleep(1000000);
        gtk_widget_show(window);
	fastmmapmq_writemmap(idmmappeakssplash,",\2");
gtk_widget_show_all(graph);
   g_timeout_add (30.0,(GSourceFunc)updatepeaksmainc, NULL);
   g_timeout_add (30.0,(GSourceFunc)pointtimmer, NULL);
        gtk_main();
        return 0;
}

extern "C" void on_save(){
GtkWidget *dialog;
GtkFileChooser *chooser;
GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
gint res;

dialog = gtk_file_chooser_dialog_new ("Save File",GTK_WINDOW(window),action,("_Cancel"),GTK_RESPONSE_CANCEL,("_Save"),GTK_RESPONSE_ACCEPT,NULL);
chooser = GTK_FILE_CHOOSER (dialog);

gtk_file_chooser_set_do_overwrite_confirmation (chooser, TRUE);

gtk_file_chooser_set_current_folder (chooser, ("/home/"));
gtk_file_chooser_set_current_name (chooser, ("Untitled document"));

res = gtk_dialog_run (GTK_DIALOG (dialog));
if (res == GTK_RESPONSE_ACCEPT)
  {
    char *filename;

    filename = gtk_file_chooser_get_filename (chooser);
    save_to_file(filename);
    g_free (filename);
  }

gtk_widget_destroy (dialog);
}

extern "C" void hide_about()
{
        gtk_widget_hide(AboutWindow);
}

extern "C" void on_help()
{
        gtk_widget_show(AboutWindow);
}

extern "C" void on_reset_clicked()
{
	pointsshow=0;
	pointsmeasured=0;
	for(int i=0;i<=maxpointsplot;i++){
		xupc[i]=i;
		xaq[i]=i;
		yupc[i]=0;
		yaq[i]=0;
		x2upc[i]=i;
		x2aq[i]=i;
		y2upc[i]=0;
		y2aq[i]=0;
		x3upc[i]=i;
		x3aq[i]=i;
		y3upc[i]=0;
		y3aq[i]=0;
		x4upc[i]=i;
		x4aq[i]=i;
		y4upc[i]=0;
		y4aq[i]=0;
		x5upc[i]=i;
		x5aq[i]=i;
		y5upc[i]=0;
		y5aq[i]=0;
		x6upc[i]=i;
		x6aq[i]=i;
		y6upc[i]=0;
		y6aq[i]=0;
		x7upc[i]=i;
		x7aq[i]=i;
		y7upc[i]=0;
		y7aq[i]=0;
		x8upc[i]=i;
		x8aq[i]=i;
		y8upc[i]=0;
		y8aq[i]=0;
		x9upc[i]=i;
		x9aq[i]=i;
		y9upc[i]=0;
		y9aq[i]=0;
		x10upc[i]=i;
		x10aq[i]=i;
		y10upc[i]=0;
		y10aq[i]=0;
	}
}

extern "C" void on_window_main_destroy()
{
        gtk_main_quit();
}
