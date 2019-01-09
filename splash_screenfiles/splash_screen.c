#include <gtk/gtk.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "fastmmap.c"

GtkWidget *win;
GtkWidget *img;
GtkWidget *l1;
GtkWidget *l2;
GtkWidget *imgb[45];
GdkColor color;
GdkColor colorb;
gboolean setting=1;
GtkBuilder *builder;
int mapid=0;
static gint timer=0;
float p=0.1;
float paaa=0.1;
int d=1;
float progresspos=0;
gint TimerCallback(){
	char str[99]="";
	char straaa[99]="";
	if(paaa+0.1<p & (p-paaa)>0.5){
		paaa=paaa+0.1;
	}else{
	if(paaa+0.005<p & (p-paaa)>0.1){
		paaa=paaa+0.005;
	}else{
	if(paaa+0.001<p){
		paaa=paaa+0.001;
	}else{
	if(paaa+0.0001<p){
		paaa=paaa+0.0001;
	}
	}
	}
	}
	gtk_widget_set_margin_start(img,(paaa/10.0)*545);
	if(p<paaa){
		paaa=p;
	}
	char *tempa=straaa;
	char *readdata=readmmap(mapid,0);


	char *res, *strreplace, *varreplace;
	char *finalresultreplace;
	finalresultreplace=malloc(99);
	varreplace=strreplace=strdup(readdata);
	while ((res=strsep(&strreplace, ","))){
		finalresultreplace=res;
	}
	strcat(tempa,finalresultreplace);
	free(varreplace);
	char *tempimg=str;
	int upimg=0;
	int hhdddsgdha=1;
	if(tempa[0]==' '){
		tempa[0]=0;
		tempa[1]=0;
		hhdddsgdha=0;
	}
	if(tempa[0]=='\0'){
		tempa[0]=0;
		tempa[1]=0;
		hhdddsgdha=0;
	}
	if(hhdddsgdha!=0){
		if(tempa[0]=='\2'){
			gtk_main_quit();
		}else{
				int gdfkghdk=2;
				while(gdfkghdk<=25){
				if(tempa[gdfkghdk]=='\0'){
					tempa[gdfkghdk]=' ';
					upimg=1;
				}
				if((tempa[gdfkghdk]!='a') & (tempa[gdfkghdk]!='b') & (tempa[gdfkghdk]!='c') & (tempa[gdfkghdk]!='d') & (tempa[gdfkghdk]!='e') & (tempa[gdfkghdk]!='f') & (tempa[gdfkghdk]!='g') & (tempa[gdfkghdk]!='h') & (tempa[gdfkghdk]!='i') & (tempa[gdfkghdk]!='j') & (tempa[gdfkghdk]!='k') & (tempa[gdfkghdk]!='l') & (tempa[gdfkghdk]!='m') & (tempa[gdfkghdk]!='n') & (tempa[gdfkghdk]!='o') & (tempa[gdfkghdk]!='p') & (tempa[gdfkghdk]!='q') & (tempa[gdfkghdk]!='r') & (tempa[gdfkghdk]!='s') & (tempa[gdfkghdk]!='t') & (tempa[gdfkghdk]!='u') & (tempa[gdfkghdk]!='v') & (tempa[gdfkghdk]!='w') & (tempa[gdfkghdk]!='x') & (tempa[gdfkghdk]!='y') & (tempa[gdfkghdk]!='z') & (tempa[gdfkghdk]!='(') & (tempa[gdfkghdk]!=')') & (tempa[gdfkghdk]!='0') & (tempa[gdfkghdk]!='1') & (tempa[gdfkghdk]!='2') & (tempa[gdfkghdk]!='3') & (tempa[gdfkghdk]!='4') & (tempa[gdfkghdk]!='5') & (tempa[gdfkghdk]!='6') & (tempa[gdfkghdk]!='7') & (tempa[gdfkghdk]!='8') & (tempa[gdfkghdk]!='9')){
					tempa[gdfkghdk]=' ';
				}
				gdfkghdk=gdfkghdk+1;
				sprintf(tempimg,"res/l%c.svg",tempa[gdfkghdk-1]);
				if(upimg==0){
				gtk_image_set_from_file(GTK_IMAGE(imgb[gdfkghdk-3]),tempimg);}else{sprintf(tempimg,"res/l%c.svg",' ');gtk_image_set_from_file(GTK_IMAGE(imgb[gdfkghdk-3]),tempimg);}
			}
			progresspos=progresspos+((float)(((int)(tempa[0]-'0'))*10+((int)(tempa[1]-'0'))))/10.0;
			p=progresspos+0.1;
		}
	}
	return 1;
}
int main(int argc, char *argv[]){
	int iscontained=0;
	if(argc<=1){
		char arg1[90];
		char exepath[90] = {0};
		sprintf( arg1, "/proc/%d/exe", getpid() );
		readlink( arg1, exepath, 1024 );
		std::string ownPth=std::string(exepath);
		std::string aux(ownPth);
		int pos = aux.rfind('/');
		std::string path = aux.substr(0,pos+1);
		std::string name = aux.substr(pos+1);
		chdir(path.c_str());
		system("./spectraread");
		return 0;
	}
	mapid=createmmap(argv[argc-1],"rwx------");
	char arg1[90];
	char exepath[90] = {0};
	sprintf( arg1, "/proc/%d/exe", getpid() );
	readlink( arg1, exepath, 1024 );
	std::string ownPth=std::string(exepath);
	std::string aux(ownPth);
	int pos = aux.rfind('/');
	std::string path = aux.substr(0,pos+1);
	std::string name = aux.substr(pos+1);
	chdir(path.c_str());
	GtkWidget *window;
	gtk_init(&argc,&argv);
	builder=gtk_builder_new();
	gtk_builder_add_from_file(builder,"res/ui.glade",NULL);
	window=GTK_WIDGET(gtk_builder_get_object(builder,"window_main"));
	gtk_builder_connect_signals(builder, NULL);
	win=GTK_WIDGET(gtk_builder_get_object(builder,"window_main"));
	char str[9]="";
	char *tempimggladename=str;
	int i=0;
	while(i<=23){
		sprintf(tempimggladename,"image%i",(i+4));
		imgb[i]=GTK_WIDGET(gtk_builder_get_object(builder,tempimggladename));
		i=i+1;
	}
	gtk_window_set_keep_above(GTK_WINDOW(win),setting);
	img=GTK_WIDGET(gtk_builder_get_object(builder,"image2"));
	l1=GTK_WIDGET(gtk_builder_get_object(builder,"label1"));
	gdk_color_parse("black", &color);
	gdk_color_parse("white", &colorb);
	gtk_widget_modify_bg(win,GTK_STATE_NORMAL,&color);
	gtk_widget_modify_fg(l1,GTK_STATE_NORMAL,&colorb);
	gtk_widget_set_margin_start(img,0);
	g_object_unref(builder);
	gtk_widget_show(window);
	timer=g_timeout_add(15,TimerCallback,NULL);      
	gtk_main();
	return 0;
}
void on_window_main_destroy(){
	gtk_main_quit();
}
