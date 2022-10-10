var mmap=require("./bin_modules/fastmmapmq.node");
var globalnodeguihelperuicomifastmmapmqida=mmap.ConnectMmapSync("spectrareadd","sfhdfghdbfdb");//XXX TODO put proccess id here.

function internalsendcommand(val){
	while(globalnodeguihelperuicomifastmmapmqida==-1){
		globalnodeguihelperuicomifastmmapmqida=mmap.ConnectMmapSync("spectrareadd","sfhdfghdbfdb");//XXX TODO put proccess id here.
	}
	mmap.WriteSync(globalnodeguihelperuicomifastmmapmqida,"Mainload-"+val);//XXX TODO put on other proccess

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
		globalnodeguihelperuicomifastmmapmqida=mmap.ConnectMmapSync("spectrareadd","sfhdfghdbfdb");//XXX TODO put proccess id here.
	}
	var strint=mmap.GetSharedStringSync(globalnodeguihelperuicomifastmmapmqida);
	while(strint.indexOf("--main-uiproc.pydataseparetor657576--")==-1){
		strint=mmap.GetSharedStringSync(globalnodeguihelperuicomifastmmapmqida);
	}
	return strint.split("--main-uiproc.pydataseparetor657576--")[1];
}

function iswindowclosed(){
	while(globalnodeguihelperuicomifastmmapmqida==-1){
		globalnodeguihelperuicomifastmmapmqida=mmap.ConnectMmapSync("spectrareadd","sfhdfghdbfdb");//XXX TODO put proccess id here.
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
		globalnodeguihelperuicomifastmmapmqida=mmap.ConnectMmapSync("spectrareadd","sfhdfghdbfdb");//XXX TODO put proccess id here.
	}
	var strint=mmap.GetSharedStringSync(globalnodeguihelperuicomifastmmapmqida);
	while(strint.indexOf("--main-uiproc.pydataseparetor657576--")==-1){
		strint=mmap.GetSharedStringSync(globalnodeguihelperuicomifastmmapmqida);
	}
	//console.log(strint);
	return strint.split("--main-uiproc.pydataseparetor657576--")[0];
}


function clearsavecommand(){
	internalsendcommand("clearsavecommand");
}

function updateinfotext(freq,temp,extra){
	internalsendcommand("updateinfotext"+freq+"/intspectrareadsepghff"+temp+"/intspectrareadsepghff"+extra+"/intspectrareadsepghff");
}

function updateinfopeaks(peaksdata){
	internalsendcommand("updateinfopeaks"+peaksdata+"spectrareadintterminator");
}

function updategraphspectrumvariable(a,b){
	internalsendcommand("updategraphspectrumvariable"+a+"/intspectrareadsepghff"+b+"/intspectrareadsepghff");
}
module.exports={mainload,stopacquisition,readyshowwin,getselectedvalues,iswindowclosed,getsavecommand,clearsavecommand,updateinfotext,updateinfopeaks,updategraphspectrumvariable};
