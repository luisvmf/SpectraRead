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
function inithspeaksprocess(id,idb,createconnect,sleeptime){
	var datahspeaksb=mmap.ConnectMmapSync("main.js","@"+cmdargs);
	while(datahspeaksb==-1){
		datahspeaksb=mmap.ConnectMmapSync("main.js","@"+cmdargs);
		console.log("Reconnecting on hspeaks.");
		pf.sleep(0.1);
	}
	pf.sleep(sleeptime);
	pf.connect(id+""+cmdargs,idb+""+cmdargs,createconnect);
	while(true){
		datapeaks=mmap.GetSharedStringSync(datahspeaksb);
		if(datapeaks.indexOf(",")!=-1){
			try{
				datapeaks=eval(datapeaks);
				if(datapeaks[3]+""=="True"){
					a=pf.polfit(datapeaks[0],datapeaks[2],datapeaks[1],1);
					if(a<300){
						pf.sleep(0.02);
					}
				}else{
					pf.sleep(0.1);
				}
			}catch(e){console.log(e);}
		}else{
			pf.sleep(0.1);
		}
	}
}
module.exports={inithspeaksprocess};
