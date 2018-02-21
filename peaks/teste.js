var aaa=require("./polfit.node");
var peaks=require('./peaks.js');



var grafico=[];
var graficox=[];
aux1=peaks.readasciispectrumfile("tabela.txt","utf8");
grafico=peaks.split_t(aux1,1);
graficox=peaks.split_t(aux1,0);


var cnt=0;
var strgrafico="";
while(cnt<grafico.length){
	strgrafico=strgrafico+grafico[cnt]/1000+" ";
	cnt=cnt+1;
}
var cnt=0;
var strgraficox="";
while(cnt<graficox.length){
	strgraficox=strgraficox+graficox[cnt]+" ";
	cnt=cnt+1;
}

var t1=process.hrtime();
var n=0;
while(n<1){
	//aaa.peaks('10.2 90.3 78.4 34.6 22.1',2,function(err,data){
aaa.peaks(strgrafico+"",strgraficox+"",7,2,function(err,data){
		//console.log(data);
	});
n=n+1;
}

aaa.peaks(strgrafico+"",strgraficox+"",7,2,function(err,data){
	peaks.saveasciispectrumfile("tabela2.dat",graficox,data)
});

var t2=process.hrtime(t1);
console.log(t2[0], t2[1]/1000000);







