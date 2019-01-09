var polfit=require("./polfit.node");
var fs=require("fs");
var i=0;
var j=0;
var startmem=Number(process.memoryUsage().heapUsed / 1024 / 1024);
function calc(){
	//console.log("depth:"+polfit.getcallstackdepth());
//console.log((fs.readFileSync("/proc/"+process.pid+"/status")+"").split("Threads:\t")[1].split("\n")[0]);
	if((Number(process.memoryUsage().heapUsed / 1024 / 1024)-startmem)<20){
	polfit.process("1 2 3 4 5 6 7 ","1 4 9 29 25 36 49 ",2,1,i,function(err,data,l){
		k=0;
		//polfit.returned(Math.random());
		savebuffer="";
		while(k<data.length){
			savebuffer=savebuffer+data[k]+"\n";
		k=k+1;
		}
		//fs.writeFile("test"+k+".dat",savebuffer,function(err){});
	});
	i=i+1;
	if(j>6000){
		console.log(Number(process.memoryUsage().heapUsed / 1024 / 1024)-startmem);
		j=0;
	}
	j=j+1;
	}
}
var nproc=0;
//if we just put calc() in a while(true) it leaks memory???
while(nproc<25){
setInterval(function (){

calc();
},0);
nproc=nproc+1;
}
