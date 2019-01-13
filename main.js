	//MIT License
	//
	//Copyright (c) 2018 Luís Victor Müller Fabris
	//
	//Permission is hereby granted, free of charge, to any person obtaining a copy
	//of this software and associated documentation files (the "Software"), to deal
	//in the Software without restriction, including without limitation the rights
	//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	//copies of the Software, and to permit persons to whom the Software is
	//furnished to do so, subject to the following conditions:
	//
	//The above copyright notice and this permission notice shall be included in all
	//copies or substantial portions of the Software.
	//
	//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	//SOFTWARE.
	const cluster = require('cluster');
	if (cluster.isMaster) {
		//Get random number passed as argument (this is used to kill all threads when the main window is closed but to avoid
		//killing another instance of the program running).
		var cmdargs="";
		process.argv.forEach(function (val, index, array) {
			if(val.indexOf("spectrareadcurrentprocid:")!=-1){
				if(val.indexOf("spectrareadprocid")!=-1){
					cmdargs=val.split("spectrareadcurrentprocid:")[1].split("spectrareadprocid")[0];
				}

			}
		});
		var worker=cluster.fork();
		var workerb=cluster.fork();
		var workergd=cluster.fork();
		var workerhsh=cluster.fork();
		var workersgagf=cluster.fork();
		var mmap=require("./fastmmapmq");
		var python = require('python.node');
		var os = python.import('os');
		var path = require('path');
		var kill=require("./kill.js");
		var spectrabuffer="";
		var sys = python.import('sys');
		var time = python.import('time');
		var id=mmap.ConnectMmapSync("spectrareads",""+cmdargs);
		while(id==-1){
			id=mmap.ConnectMmapSync("spectrareads",""+cmdargs);
			console.log("Connection failed...Reconnecting");
			time.sleep(0.1);
		}
		var idint=mmap.CreateMmapSync("a"+cmdargs,"rwx------");
		var idintdatabs=mmap.CreateMmapSync("p"+cmdargs,"rwx------");
		var idintdatacs=mmap.CreateMmapSync("I"+cmdargs,"rwx------");
		var idintb=mmap.CreateMmapSync("b"+cmdargs,"rwx------");
		var idintd=mmap.CreateMmapSync("d"+cmdargs,"rwx------");
		var idintcolors=mmap.CreateMmapSync("col"+cmdargs,"rwx------");
		time.sleep(0.1);
		mmap.WriteSync(id,",05load process manager");
		var fs = require('fs');
		var filecode="";
		var t0=0;
		var nspectra=0;
		var path = require('path');
		var current_path=path.dirname(fs.realpathSync(__filename));
		var dirpython = path.dirname(fs.realpathSync(__filename));
		sys.path.append(dirpython);
		sys.path.append(dirpython+'/spectrometer_modules');
		sys.path.append(dirpython+'/Python/lib');
		sys.path.append(dirpython+'/Python/lib/python2.7');
		sys.path.append(dirpython+'/Python/lib/python2.7/site-packages');
		sys.path.append(dirpython+'/Python/lib/python2.7/site-packages/numpy');
		var errmessage = python.import('acquisitiondialog');
		time.sleep(0.1);
		mmap.WriteSync(id,",10load ui manager");
		time.sleep(0.1);
		function terminateallworkers(){
			worker.destroy();
			worker.process.kill();
			workerb.destroy();
			workerb.process.kill();
			kill.killallthreads(current_path+"/spectrareads\u0000"+cmdargs+"\u0000","SIGKILL",process.pid);
			kill.killallthreads(current_path+"/spectrareadd"+"\u0000"+current_path+"/main.js"+"\u0000spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid\u0000spectrareadcmdarggui\u0000","SIGTERM",process.pid);
			kill.killallthreads(current_path+"/spectrareadd"+"\u0000"+current_path+"/main.js"+"\u0000spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid\u0000spectrareadcmdarggui\u0000","SIGKILL",process.pid);
			kill.killallthreads(current_path+"/spectrareadd"+"\u0000"+current_path+"/main.js"+"\u0000spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid\u0000spectrareadcmdarggui\u0000","SIGTERM",process.pid);
			kill.killallthreads(current_path+"/spectrareadd"+"\u0000"+current_path+"/main.js"+"\u0000spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid\u0000spectrareadcmdarggui\u0000","SIGKILL",process.pid);
			kill.killallthreads(current_path+"/spectrareadd"+"\u0000"+current_path+"/daemon.js"+"\u0000spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid\u0000spectrareadcmdargdaemon\u0000","SIGTERM",process.pid);
            mmap.WriteSync(id,"\2");
			process.exit();
		}
		function startexitprocess(){
		}
		time.sleep(0.1);
		//Workers send messages to this thread. Send the messages to other workers.
		worker.on('message', function(msg) {
			if(msg.selectedvalues){
				mmap.WriteSharedStringSync(idint,","+msg.selectedvalues);
				mmap.WriteSharedStringSync(idintdatabs,","+msg.selectedvalues);
				mmap.WriteSharedStringSync(idintdatacs,","+msg.selectedvalues);
			}
			if(msg.savefilecommand){
				mmap.WriteSharedStringSync(idintb,","+msg.savefilecommand);
			}
			if(msg.closegui){
				console.log('Gui closed gracefully');
				terminateallworkers();
			}
		});
		worker.on('exit', function(aaa,bbb) {
			console.log('Worker 1 died. Starting close process');
			startexitprocess();
			//setTimeout(function (){
				errmessage.showerrormessage("An unexpected exception has happened.\nProgram will close.");
				terminateallworkers();
			//},2000);
		});
		workerb.on('exit', function(aaa,bbb) {
			console.log('Worker 2 died. Starting close process');
			startexitprocess();
			//setTimeout(function (){
				errmessage.showerrormessage("An unexpected exception has happened.\nProgram will close.");
				terminateallworkers();
			//},2000);
		});
		process.on('exit', function () {
			console.log('About to exit');
			startexitprocess();
			terminateallworkers();
			//process.exit();
		});
		function checkmessage(){
			datamessage=mmap.ReadSync(idintd,0);	
			if(datamessage.indexOf("-")!=-1){
				while(datamessage.indexOf("_")==-1){
					datamessage=datamessage+mmap.ReadSync(idintd,0);
				}
				errmessage.showerrormessage(datamessage.split("-")[1].split("_")[0]);
			}
		}
		setInterval(function (){
			checkmessage();
		},150);
	}else if (cluster.worker.id === 1) {
		//Workerb
		require("./nodegui");
	}else if (cluster.worker.id === 2) {
		//Workerc
		var mmap=require("./fastmmapmq");
		//var pb=require("./pb");
		var python = require('python.node');
		var polfit=require("./polfit.node");
		var dialoganalysis=require("./dialoganalysis");
		var cmdargs="";
		process.argv.forEach(function (val, index, array) {
			if(val.indexOf("spectrareadcurrentprocid:")!=-1){
				if(val.indexOf("spectrareadprocid")!=-1){
					cmdargs=val.split("spectrareadcurrentprocid:")[1].split("spectrareadprocid")[0];
				}

			}
		});
		var sys = python.import('sys');
		var fs = require('fs');
		var path=require("path");
		var dirpython = path.dirname(fs.realpathSync(__filename));
		sys.path.append(dirpython);
		sys.path.append(dirpython+'/spectrometer_modules');
		sys.path.append(dirpython+'/Python/lib');
		sys.path.append(dirpython+'/Python/lib/python2.7');
		sys.path.append(dirpython+'/Python/lib/python2.7/site-packages');
		sys.path.append(dirpython+'/Python/lib/python2.7/site-packages/numpy');
		var time=python.import("time");
		var idintb=mmap.ConnectMmapSync("spectrareadd","b"+cmdargs);
		while(idintb==-1){
			idintb=mmap.ConnectMmapSync("spectrareadd","b"+cmdargs);
			time.sleep(0.1);
		}
		var idintdatabs2=mmap.ConnectMmapSync("spectrareadd","p"+cmdargs);
		while(idintdatabs2==-1){
			idintdatabs2=mmap.ConnectMmapSync("spectrareadd","p"+cmdargs);
			time.sleep(0.1);
		}
		var datamapaaab=mmap.ConnectMmapSync("spectrareadd","?"+cmdargs);
		while(datamapaaab==-1){
			datamapaaab=mmap.ConnectMmapSync("spectrareadd","?"+cmdargs);
			console.log("Connection failed...Reconnecting");
			time.sleep(0.1);
		}
		var currentsavecommands="";
		var aqstop=1;
		var oldsavecommands="";
		var datamainguiproc=[0,0,0];//baseline, risingthreshold, boxcar
		var spectranumbercounter=0;
		var timefirstsave=time.time()
		Number.prototype.addzeros=function(numlength){
			var strnum=this+"";
			while (strnum.length<(numlength||2)){
				strnum="0"+strnum;
			}
			return strnum;
		}
		while(true){
			aqstop=1;
			try{
				var rawreadcommands="";
				if(idintb!=-1){
					rawreadcommands=mmap.GetSharedStringSync(idintb);
				}
				rawreadcommands=rawreadcommands.split(",");
				var i=0;
				while(i<rawreadcommands.length){
					if(rawreadcommands[i].indexOf("-")!=-1){
						if(rawreadcommands[i]!="-"){
							rawreadcommands=rawreadcommands[i];
							aqstop=0;
						}
					}
					i=i+1;
				}
				if(rawreadcommands!=['']){
					if(rawreadcommands!=undefined){
						if(rawreadcommands.indexOf("-")!=-1){
							currentsavecommands=(dialoganalysis.process(rawreadcommands));
						}
					}
				}
			}catch(e){
				if(aqstop==0){
					currentsavecommands=oldsavecommands;
				}else{
					currentsavecommands="";
				}
			}
			if(aqstop==1){
				currentsavecommands="";
			}
			oldsavecommands=currentsavecommands;
			if(idintdatabs2!=-1){
				guistatnew=mmap.GetSharedStringSync(idintdatabs2);
			}
			try{
			if(guistatnew.indexOf(",")!=-1){
				guistatnew=guistatnew.split(",");
				guistatnew=guistatnew[guistatnew.length-1];
				guistatnew=guistatnew.split("----------");
				if(guistatnew.length==2){
					guistatnew=guistatnew[0];
				}else{
					guistatnew="";
				}
			}else{
				guistatnew="";
			}
			if(guistatnew!=""){
				guistatnew=guistatnew.split(";");
				if(guistatnew.length!=5){
					guistatnew="";
				}
			}
			}catch(e){
				guistatnew="";
			}
			if(guistatnew!=""){
				if(guistatnew.length==5){
					tempvar=NaN;
					if(guistatnew[0]!=""){
						tempvar=Number(guistatnew[0]);
					}
					if(tempvar!=NaN){
						datamainguiproc[0]=tempvar;
					}
					tempvar=NaN;
					if(guistatnew[1]!=""){
						tempvar=Number(guistatnew[1]);
					}
					if(tempvar!=NaN){
						datamainguiproc[1]=tempvar;
					}
					tempvar=NaN;
					if(guistatnew[3]!=""){
						tempvar=Number(guistatnew[3]);
					}
					if(tempvar!=NaN){
						datamainguiproc[2]=tempvar;
					}
				}
			}
			try{
				if(currentsavecommands!=""){
					//console.log(datamainguiproc[0]+","+datamainguiproc[1]+","+datamainguiproc[2]);
					//console.log(currentsavecommands);
					if(currentsavecommands[1]!=false){
						savelocationfinal=currentsavecommands[0]+"/"+currentsavecommands[1];
						if((time.time()-timefirstsave)>currentsavecommands[3]/1000){
							timefirstsave=time.time();
							datasavespec=mmap.GetSharedStringSync(datamapaaab);
							dataintupdateui=datasavespec.split(";");
							i=0;
							while(i<=(dataintupdateui.length)-1){
								if(i>5){
									break;
								}
								if(dataintupdateui[i].indexOf(":")!=-1){
									if(dataintupdateui[i].indexOf("buffer:")!=-1){
										xdataintpeaks=dataintupdateui[i].split("buffer:")[1].split("?")[0]
										if((/^[0-9 .]*$/.test(dataintupdateui[i].split("buffer:")[1].split("?")[0]+""))==true){
											if(((dataintupdateui[i].split("buffer:")[1].split("?")[1].split("final")[0]+" ").split(" ")).length==((xdataintpeaks+" ").split(" ").length)){
												if((/^[0-9 .]*$/.test(dataintupdateui[i].split("buffer:")[1].split("?")[1].split("final")[0]+""))==true){
													data=polfit.getspecsync(xdataintpeaks+" ",dataintupdateui[i].split("buffer:")[1].split("?")[1].split("final")[0]+" ",Number(datamainguiproc[0]),Number(datamainguiproc[2]),Number(datamainguiproc[1]));
													console.log("save: "+savelocationfinal+"-"+spectranumbercounter.addzeros(4)+".dat "+xdataintpeaks+" "+(data+"").replace("[","").replace("]","").replace(new RegExp(",", 'g'), " "));
													spectranumbercounter=spectranumbercounter+1;
												}
											}
										}
									}
								}
								i=i+1;
							}
						}
					}else{
						spectranumbercounter=0;
					}
					//pb(datamainguiproc[0],datamainguiproc[1],datamainguiproc[2],1,1,0,0.5,"spec","peaks");
				}else{
					time.sleep(0.1);
				}
			}catch(e){console.log(e);}
			}
	}else if (cluster.worker.id === 3) {
		//Workerc
		//require("./dataprocess2");
	}else if (cluster.worker.id === 4) {
		//Workerc
		//require("./dataprocess3");
	}else if (cluster.worker.id === 5) {
		//Workerc
		//require("./dataprocess4");
	}
