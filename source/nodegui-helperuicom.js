var mmap=require("./bin_modules/fastmmapmq.node");
var cmdargs="";
process.argv.forEach(function (val, index, array) {
	if(val.indexOf("spectrareadcurrentprocid:")!=-1){
		if(val.indexOf("spectrareadprocid")!=-1){
			cmdargs=val.split("spectrareadcurrentprocid:")[1].split("spectrareadprocid")[0];
		}
	}
});

var globalnodeguihelperuicomifastmmapmqida=mmap.ConnectMmapSync(process.cwd()+"/spectrareadd "+process.cwd()+"/main.js spectrareadcurrentprocid:"+cmdargs,"sfhdfghdbfdb"+cmdargs);
var count=0;
function internalsendcommand(val){
	while(globalnodeguihelperuicomifastmmapmqida==-1){
		globalnodeguihelperuicomifastmmapmqida=mmap.ConnectMmapSync(process.cwd()+"/spectrareadd "+process.cwd()+"/main.js spectrareadcurrentprocid:"+cmdargs,"sfhdfghdbfdb"+cmdargs);
	}
	mmap.WriteSync(globalnodeguihelperuicomifastmmapmqida,"Mainload-"+val);//XXX TODO put on other proccess
	count++;
	if(count>100){
		count=0;
	}
}


function mainload(procid){
	internalsendcommand("load"+procid);
}

function stopacquisition(){
	internalsendcommand("stopacquisition");
}

function readyshowwin(){
	internalsendcommand("readyshowwin");
}


function getselectedvalues(){
	while(globalnodeguihelperuicomifastmmapmqida==-1){
		globalnodeguihelperuicomifastmmapmqida=mmap.ConnectMmapSync(process.cwd()+"/spectrareadd "+process.cwd()+"/main.js spectrareadcurrentprocid:"+cmdargs,"sfhdfghdbfdb"+cmdargs);
	}
	var strint=mmap.GetSharedStringSync(globalnodeguihelperuicomifastmmapmqida);
	while(strint.indexOf("--main-uiproc.pydataseparetor657576--")==-1){
		strint=mmap.GetSharedStringSync(globalnodeguihelperuicomifastmmapmqida);
	}
	return strint.split("--main-uiproc.pydataseparetor657576--")[1];
}

function iswindowclosed(){
	while(globalnodeguihelperuicomifastmmapmqida==-1){
		globalnodeguihelperuicomifastmmapmqida=mmap.ConnectMmapSync(process.cwd()+"/spectrareadd "+process.cwd()+"/main.js spectrareadcurrentprocid:"+cmdargs,"sfhdfghdbfdb"+cmdargs);
	}
	var strint=mmap.GetSharedStringSync(globalnodeguihelperuicomifastmmapmqida);
	while(strint.indexOf("--main-uiproc.pydataseparetor657576--")==-1){
		strint=mmap.GetSharedStringSync(globalnodeguihelperuicomifastmmapmqida);
	}
	//console.log(strint);
	return strint.split("--main-uiproc.pydataseparetor657576--")[2];
}

function getsavecommand(){
	while(globalnodeguihelperuicomifastmmapmqida==-1){
		globalnodeguihelperuicomifastmmapmqida=mmap.ConnectMmapSync(process.cwd()+"/spectrareadd "+process.cwd()+"/main.js spectrareadcurrentprocid:"+cmdargs,"sfhdfghdbfdb"+cmdargs);
	}
	var strint=mmap.GetSharedStringSync(globalnodeguihelperuicomifastmmapmqida);
	while(strint.indexOf("--main-uiproc.pydataseparetor657576--")==-1){
		strint=mmap.GetSharedStringSync(globalnodeguihelperuicomifastmmapmqida);
	}
	//console.log(strint);
	if(strint.split("--main-uiproc.pydataseparetor657576--")[0]=="Falseb"){
		return "False";
	}
	return strint.split("--main-uiproc.pydataseparetor657576--")[0];
}

function upui(){
	internalsendcommand("upui");
}

function clearsavecommand(){
	while(globalnodeguihelperuicomifastmmapmqida==-1){
		globalnodeguihelperuicomifastmmapmqida=mmap.ConnectMmapSync(process.cwd()+"/spectrareadd "+process.cwd()+"/main.js spectrareadcurrentprocid:"+cmdargs,"sfhdfghdbfdb"+cmdargs);
	}
	var strint=mmap.GetSharedStringSync(globalnodeguihelperuicomifastmmapmqida);
	while(strint.indexOf("--main-uiproc.pydataseparetor657576--")==-1){
		strint=mmap.GetSharedStringSync(globalnodeguihelperuicomifastmmapmqida);
	}
	datanew="Falseb--main-uiproc.pydataseparetor657576--"+strint.split("--main-uiproc.pydataseparetor657576--")[1]+"--main-uiproc.pydataseparetor657576--"+strint.split("--main-uiproc.pydataseparetor657576--")[2];
	mmap.WriteSharedStringSync(globalnodeguihelperuicomifastmmapmqida,datanew);
	internalsendcommand("clearsavecommand");
	//console.log("clear");
}

function updateinfotext(freq,temp,extra){
	internalsendcommand("updateinfotext"+freq+"/intspectrareadsepghff"+temp+"/intspectrareadsepghff"+extra+"/intspectrareadsepghff");
}

function updateinfopeaks(peaksdata){
	internalsendcommand("updateinfopeaks"+peaksdata+"spectrareadintterminator");
}

function updategraphspectrumvariable(a,b,aqmode){
	internalsendcommand("updategraphspectrumvariable"+"1 "+a+"/intspectrareadsepghff"+aqmode+" "+b+"/intspectrareadsepghff");
}
module.exports={mainload,stopacquisition,readyshowwin,getselectedvalues,iswindowclosed,getsavecommand,clearsavecommand,updateinfotext,updateinfopeaks,updategraphspectrumvariable,upui};
