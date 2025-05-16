var mmap=require("./bin_modules/fastmmapmq.node");
		var cmdargs="";
		process.argv.forEach(function (val, index, array) {
			if(val.indexOf("spectrareadcurrentprocid:")!=-1){
				if(val.indexOf("spectrareadprocid")!=-1){
					cmdargs=val.split("spectrareadcurrentprocid:")[1].split("spectrareadprocid")[0];
				}

			}
		});
var globalnodeguihelperuicomifastmmapmqida=mmap.ConnectMmapSync(process.cwd()+"/spectrareadd "+process.cwd()+"/main.js spectrareadcurrentprocid:"+cmdargs,"eventrep"+cmdargs);
function sendmessage(messagetype,message){
	while(globalnodeguihelperuicomifastmmapmqida==-1){
		globalnodeguihelperuicomifastmmapmqida=mmap.ConnectMmapSync(process.cwd()+"/spectrareadd "+process.cwd()+"/main.js spectrareadcurrentprocid:"+cmdargs,"eventrep"+cmdargs);
	}
	mmap.WriteSync(globalnodeguihelperuicomifastmmapmqida,"---datasepevent567567---"+messagetype+"---656756datasepintevent---"+message+"---datasepevent567567---");
}


module.exports={sendmessage};
