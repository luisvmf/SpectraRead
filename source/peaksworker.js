var cmdargs="";
process.argv.forEach(function (val, index, array) {
	if(val.indexOf("spectrareadcurrentprocid:")!=-1){
		if(val.indexOf("spectrareadprocid")!=-1){
			cmdargs=val.split("spectrareadcurrentprocid:")[1].split("spectrareadprocid")[0];
		}
	}
});
var cmdargsid="";
process.argv.forEach(function (val, index, array) {
	if(val.indexOf("specreadprocpeaks:")!=-1){
			cmdargsid=val.split("specreadprocpeaks:")[1];
	}
});
var cmdargsidb=0;
process.argv.forEach(function (val, index, array) {
	if(val.indexOf("spec2")!=-1){
			cmdargsidb=1;
	}
});
if(cmdargsid=="par"){
	cmdargsid="(";
}
var peakshs=require("./hspeaks.js");
console.log("Initializing hspeaks."+cmdargsid+cmdargsidb+cmdargs);
peakshs.inithspeaksprocess(cmdargsid,"p"+cmdargsid+cmdargsidb,0,0.01);
