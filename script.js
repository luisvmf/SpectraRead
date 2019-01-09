		var cmdargs=0;
		var procid=0;
		process.argv.forEach(function (val, index, array) {
			if(val.indexOf("daemon.js")!=-1){
				cmdargs=1;
			}
			if(val.indexOf("specreadproc1")!=-1){
				procid=1;
			}
			if(val.indexOf("specreadproc2")!=-1){
				procid=2;
			}
			if(val.indexOf("specreadproc3")!=-1){
				procid=3;
			}
			if(val.indexOf("specreadproc4")!=-1){
				procid=4;
			}
			if(val.indexOf("specreadproc5")!=-1){
				procid=5;
			}
			if(val.indexOf("specreadproc6")!=-1){
				procid=6;
			}
		});
if(cmdargs==0){
	if(procid==1){
		var a=require("./main.js");
		a.clustermaster();
	}
	if(procid==2){
		var a=require("./main.js");
		a.clustera();
	}
	if(procid==3){
		var a=require("./main.js");
		a.clusterb();
	}
	if(procid==4){
		console.log("d");
	}
	if(procid==5){
		console.log("e");
	}
	if(procid==6){
		console.log("f");
	}

}else{
	require("./daemon.js");
}
