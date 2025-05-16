const fs = require('fs');
const path = require('path');
function killallthreads(programcmdline,signal,currentpid){
	var killed=true;
	while(killed==true){
		killed=false;
		if(signal==undefined){
			signal="SIGTERM";
		}
		function getDirectories(path){
				return fs.readdirSync(path).filter(function (file) {
					try{
						return fs.statSync(path+'/'+file).isDirectory();
					}catch(e){}
				});
		}
		var listdirs=getDirectories("/proc");
		var listpids=[];
		var j=0;
		for (i=0; i<listdirs.length; i++){
			if(Number(listdirs[i])+""==""+listdirs[i]){
				listpids[j]=listdirs[i];
				j=j+1;
			}
		}
		for (i=0; i<listpids.length; i++){
			try{
				var cmdline=fs.readFileSync("/proc/"+listpids[i]+"/cmdline", "utf8");
				if(cmdline+""==""+programcmdline){
					if(listpids[i]+""!=""+currentpid){
						killed=true;
						process.kill(listpids[i],signal);
					}
				}
			}catch(e){}
		}
	}
}
module.exports={killallthreads};
