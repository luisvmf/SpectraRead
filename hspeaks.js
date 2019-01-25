var cmdargs="";
process.argv.forEach(function (val, index, array) {
	if(val.indexOf("spectrareadcurrentprocid:")!=-1){
		if(val.indexOf("spectrareadprocid")!=-1){
			cmdargs=val.split("spectrareadcurrentprocid:")[1].split("spectrareadprocid")[0];
		}
	}
});
var mmap=require("./fastmmapmq");
var pf=require("./polfiths");
var datahspeaks=mmap.ConnectMmapSync("spectrareadd","@"+cmdargs);
while(datahspeaks==-1){
	datahspeaks=mmap.ConnectMmapSync("spectrareadd","@"+cmdargs);
	console.log("Reconnecting on hspeaks.");
	pf.sleep(0.1);
}
function inithspeaksprocess(id,idb){
	pf.connect(id+""+cmdargs,idb+""+cmdargs);
	while(true){
		datapeaks=mmap.GetSharedStringSync(datahspeaks);
		if(datapeaks!=""){
			try{
				datapeaks=eval(datapeaks);
				if(datapeaks[3]+""=="True"){
					a=pf.polfit(datapeaks[0],datapeaks[2],datapeaks[1],2);
					if(a<300){
						pf.sleep(0.08);
					}
				}else{
					pf.sleep(0.1);
				}
			}catch(e){console.log(e);}
		}
	}
}
module.exports={inithspeaksprocess};
