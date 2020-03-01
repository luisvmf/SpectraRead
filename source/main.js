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
		var mmap=require("./bin_modules/fastmmapmq.node");
		var python = require('python.node');
		var os = python.import('os');
		var path = require('path');
		var kill=require("./kill.js");
		var spectrabuffer="";
		var sys = python.import('sys');
		var time = python.import('time');
		var id=mmap.ConnectMmapSync(process.cwd()+"/spectrareads "+cmdargs,""+cmdargs);
		while(id==-1){
			id=mmap.ConnectMmapSync(process.cwd()+"/spectrareads "+cmdargs,""+cmdargs);
			//console.log("Connection failed...Reconnecting");
			time.sleep(0.1);
		}
		var idint=mmap.CreateMmapSync("a"+cmdargs,"rwx------");
		var idintdatabs=mmap.CreateMmapSync("p"+cmdargs,"rwx------");
		var idintdatacs=mmap.CreateMmapSync("I"+cmdargs,"rwx------");
		var idintb=mmap.CreateMmapSync("b"+cmdargs,"rwx------");
		var idintd=mmap.CreateMmapSync("d"+cmdargs,"rwx------");
		var idintcolors=mmap.CreateMmapSync("col"+cmdargs,"rwx------");
		time.sleep(0.05);
		mmap.WriteSync(id,",05load process manager");
		var fs = require('fs');
		var filecode="";
		var t0=0;
		var nspectra=0;
		var path = require('path');
		var current_path=path.dirname(fs.realpathSync(__filename));
		var dirpython = path.dirname(fs.realpathSync(__filename));
		sys.path.append(dirpython);
		sys.path.append(dirpython+'/bin_modules');
		sys.path.append(dirpython+'/spectraread_source');
		sys.path.append(dirpython+'/Python/lib');
		sys.path.append(dirpython+'/Python/lib/python2.7');
		sys.path.append(dirpython+'/Python/lib/python2.7/site-packages');
		//sys.path.append(dirpython+'/Python/lib/python2.7/site-packages/numpy');
		var errmessage = python.import('acquisitiondialog');
		time.sleep(0.05);
		mmap.WriteSync(id,",10load ui manager");
		time.sleep(0.01);
		function terminateallworkers(){
			worker.destroy();
			worker.process.kill();
			workerb.destroy();
			workerb.process.kill();
			kill.killallthreads(current_path+"/spectrareads\u0000"+cmdargs+"\u0000","SIGTERM",process.pid);
			kill.killallthreads(current_path+"/spectrareadd"+"\u0000"+current_path+"/main.js"+"\u0000spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid\u0000spectrareadcmdarggui\u0000","SIGTERM",process.pid);
			kill.killallthreads(current_path+"/spectrareadd"+"\u0000"+current_path+"/main.js"+"\u0000spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid\u0000spectrareadcmdarggui\u0000","SIGTERM",process.pid);
			kill.killallthreads(current_path+"/spectrareadd"+"\u0000"+current_path+"/main.js"+"\u0000spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid\u0000spectrareadcmdarggui\u0000","SIGTERM",process.pid);
			kill.killallthreads(current_path+"/spectrareadd"+"\u0000"+current_path+"/main.js"+"\u0000spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid\u0000spectrareadcmdarggui\u0000","SIGTERM",process.pid);
			kill.killallthreads(current_path+"/spectrareadd"+"\u0000"+current_path+"/peaksworker.js"+"\u0000spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid\u0000spectrareadcmdarggui\u0000specreadprocpeaks:{\u0000spec1\u0000","SIGTERM",process.pid);
			kill.killallthreads(current_path+"/spectrareadd"+"\u0000"+current_path+"/peaksworker.js"+"\u0000spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid\u0000spectrareadcmdarggui\u0000specreadprocpeaks:{\u0000spec1\u0000","SIGTERM",process.pid);
			kill.killallthreads(current_path+"/spectrareadd"+"\u0000"+current_path+"/peaksworker.js"+"\u0000spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid\u0000spectrareadcmdarggui\u0000specreadprocpeaks:par\u0000spec1\u0000","SIGTERM",process.pid);
			kill.killallthreads(current_path+"/spectrareadd"+"\u0000"+current_path+"/peaksworker.js"+"\u0000spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid\u0000spectrareadcmdarggui\u0000specreadprocpeaks:par\u0000spec1\u0000","SIGTERM",process.pid);
			kill.killallthreads(current_path+"/spectrareadd"+"\u0000"+current_path+"/peaksworker.js"+"\u0000spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid\u0000spectrareadcmdarggui\u0000specreadprocpeaks:[\u0000spec1\u0000","SIGTERM",process.pid);
			kill.killallthreads(current_path+"/spectrareadd"+"\u0000"+current_path+"/peaksworker.js"+"\u0000spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid\u0000spectrareadcmdarggui\u0000specreadprocpeaks:[\u0000spec1\u0000","SIGTERM",process.pid);
			kill.killallthreads(current_path+"/spectrareadd"+"\u0000"+current_path+"/peaksworker.js"+"\u0000spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid\u0000spectrareadcmdarggui\u0000specreadprocpeaks:~\u0000spec1\u0000","SIGTERM",process.pid);
			kill.killallthreads(current_path+"/spectrareadd"+"\u0000"+current_path+"/peaksworker.js"+"\u0000spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid\u0000spectrareadcmdarggui\u0000specreadprocpeaks:~\u0000spec1\u0000","SIGTERM",process.pid);
			kill.killallthreads(current_path+"/spectrareadd"+"\u0000"+current_path+"/peaksworker.js"+"\u0000spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid\u0000spectrareadcmdarggui\u0000specreadprocpeaks:{\u0000spec2\u0000","SIGTERM",process.pid);
			kill.killallthreads(current_path+"/spectrareadd"+"\u0000"+current_path+"/peaksworker.js"+"\u0000spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid\u0000spectrareadcmdarggui\u0000specreadprocpeaks:{\u0000spec2\u0000","SIGTERM",process.pid);
			kill.killallthreads(current_path+"/spectrareadd"+"\u0000"+current_path+"/peaksworker.js"+"\u0000spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid\u0000spectrareadcmdarggui\u0000specreadprocpeaks:par\u0000spec2\u0000","SIGTERM",process.pid);
			kill.killallthreads(current_path+"/spectrareadd"+"\u0000"+current_path+"/peaksworker.js"+"\u0000spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid\u0000spectrareadcmdarggui\u0000specreadprocpeaks:par\u0000spec2\u0000","SIGTERM",process.pid);
			kill.killallthreads(current_path+"/spectrareadd"+"\u0000"+current_path+"/peaksworker.js"+"\u0000spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid\u0000spectrareadcmdarggui\u0000specreadprocpeaks:[\u0000spec2\u0000","SIGTERM",process.pid);
			kill.killallthreads(current_path+"/spectrareadd"+"\u0000"+current_path+"/peaksworker.js"+"\u0000spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid\u0000spectrareadcmdarggui\u0000specreadprocpeaks:[\u0000spec2\u0000","SIGTERM",process.pid);
			kill.killallthreads(current_path+"/spectrareadd"+"\u0000"+current_path+"/peaksworker.js"+"\u0000spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid\u0000spectrareadcmdarggui\u0000specreadprocpeaks:~\u0000spec2\u0000","SIGTERM",process.pid);
			kill.killallthreads(current_path+"/spectrareadd"+"\u0000"+current_path+"/peaksworker.js"+"\u0000spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid\u0000spectrareadcmdarggui\u0000specreadprocpeaks:~\u0000spec2\u0000","SIGTERM",process.pid);
			kill.killallthreads(current_path+"/spectrareadd"+"\u0000"+current_path+"/peaksworker.js"+"\u0000spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid\u0000spectrareadcmdarggui\u0000specreadprocpeaks:,\u0000spec1\u0000","SIGTERM",process.pid);
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
			if(msg.errorshow){
				errmessage.showerrormessage(msg.errorshow);
			}
		});
		function exithandler(){
			console.log('Worker died. Starting close process');
			startexitprocess();
			//setTimeout(function (){
				errmessage.showerrormessage("An unexpected exception has happened.\nProgram will close.");
				terminateallworkers();
			//},2000);
		}
		worker.on('exit', function(aaa,bbb) {
			console.log("a");
			exithandler();
		});
		workerb.on('exit', function(aaa,bbb) {
			console.log("b");
			exithandler();
		});
		workergd.on('exit', function(aaa,bbb) {
			console.log("c");
			exithandler();
		});
		workerhsh.on('exit', function(aaa,bbb) {
			console.log("d");
			exithandler();
		});
		//process.on('exit', function () {
		//	console.log('About to exit');
		//	startexitprocess();
		//	terminateallworkers();
			//process.exit();
		//});
		function checkmessage(){
			datamessage=mmap.ReadSync(idintd,0);
			if(datamessage.indexOf("-")!=-1){
				while(datamessage.indexOf("_")==-1){
					datamessage=datamessage+mmap.ReadSync(idintd,0);
				}
				errmessage.showerrormessage(datamessage.split("-")[1].split("_")[0]);
				console.log("showe");
			}
		}
		setInterval(function (){
			checkmessage();
		},350);
	}else if (cluster.worker.id === 1) {
		//Workerb
		require("./nodegui");
	}else if (cluster.worker.id === 2) {
		//Workerc
		var mmap=require("./bin_modules/fastmmapmq.node");
		//var pb=require("./pb");
		var python = require('python.node');
		var polfit=require("./bin_modules/polfit.node");
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
		sys.path.append(dirpython+'/bin_modules');
		sys.path.append(dirpython+'/spectraread_source');
		sys.path.append(dirpython+'/Python/lib');
		sys.path.append(dirpython+'/Python/lib/python2.7');
		sys.path.append(dirpython+'/Python/lib/python2.7/site-packages');
		//sys.path.append(dirpython+'/Python/lib/python2.7/site-packages/numpy');
		var time=python.import("time");
		var errmessage = python.import('acquisitiondialog');
		var datahspeaks=mmap.CreateMmapSync("@"+cmdargs,"rwx------");
		var idintb=mmap.ConnectMmapSync(process.cwd()+"/spectrareadd "+process.cwd()+"/main.js spectrareadcurrentprocid:"+cmdargs,"b"+cmdargs);
		while(idintb==-1){
			idintb=mmap.ConnectMmapSync(process.cwd()+"/spectrareadd "+process.cwd()+"/main.js spectrareadcurrentprocid:"+cmdargs,"b"+cmdargs);
			//console.log("Connection failed on worker 2...Reconnecting");
			time.sleep(0.1);
		}
		var idintdatabs2=mmap.ConnectMmapSync(process.cwd()+"/spectrareadd "+process.cwd()+"/main.js spectrareadcurrentprocid:"+cmdargs,"p"+cmdargs);
		while(idintdatabs2==-1){
			idintdatabs2=mmap.ConnectMmapSync(process.cwd()+"/spectrareadd "+process.cwd()+"/main.js spectrareadcurrentprocid:"+cmdargs,"p"+cmdargs);
			//console.log("Connection failed on worker 2...Reconnecting");
			time.sleep(0.1);
		}
		var datamapaaab=mmap.ConnectMmapSync(process.cwd()+"/spectrareadd "+process.cwd()+"/daemon.js spectrareadcurrentprocid:"+cmdargs,"?"+cmdargs);
		while(datamapaaab==-1){
			datamapaaab=mmap.ConnectMmapSync(process.cwd()+"/spectrareadd "+process.cwd()+"/daemon.js spectrareadcurrentprocid:"+cmdargs,"?"+cmdargs);
			//console.log("Connection failed on worker 2...Reconnecting");
			time.sleep(0.1);
		}
		var iderrmaingui=mmap.ConnectMmapSync(process.cwd()+"/spectrareadd "+process.cwd()+"/main.js spectrareadcurrentprocid:"+cmdargs,"errpd"+cmdargs);
		while(iderrmaingui==-1){
			iderrmaingui=mmap.ConnectMmapSync(process.cwd()+"/spectrareadd "+process.cwd()+"/main.js spectrareadcurrentprocid:"+cmdargs,"errpd"+cmdargs);
			//console.log("Connection failed on worker 3 id errpd...Reconnecting");
			time.sleep(0.1);
		}
		var errboxmmap=mmap.ConnectMmapSync(process.cwd()+"/spectrareadd "+process.cwd()+"/main.js spectrareadcurrentprocid:"+cmdargs,"d"+cmdargs);
		while(errboxmmap==-1){
			errboxmmap=mmap.ConnectMmapSync(process.cwd()+"/spectrareadd "+process.cwd()+"/main.js spectrareadcurrentprocid:"+cmdargs,"d"+cmdargs);
			//console.log("Connection failed on worker 3 id d...Reconnecting");
			time.sleep(0.1);
		}
		var spawn = require("child_process").spawn;
		var currentsavecommands="";
		var aqstop=1;
		var oldsavecommands="";
		var datamainguiproc=[0,0,0];//baseline, risingthreshold, boxcar
		var spectranumbercounter=-1;
		var timefirstsave=time.time()
		var errsavecmd="";
		var showerrmessage=0;
		var iscontmeasactive="False";
		var erronthis=0;
		var saveonthis=0;
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
				if(rawreadcommands==",False"){
					spectranumbercounter=-1;
					errsavecmd="";
					showerrmessage=0;
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
				console.log(e);
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
				if(guistatnew[4]=="True"){
					iscontmeasactive=guistatnew[4];
				}
				if(guistatnew[4]=="False"){
					iscontmeasactive=guistatnew[4];
				}
				if(guistatnew.length!=5){
					guistatnew="";
				}
			}
			}catch(e){
				guistatnew="";
				console.log(e);
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
				mmap.WriteSharedStringSync(datahspeaks,"["+datamainguiproc+",'"+iscontmeasactive+"']--++['"+currentsavecommands[0]+"','"+currentsavecommands[1]+"','"+currentsavecommands[2]+"','"+currentsavecommands[3]+"','"+currentsavecommands[4]+"']");
				if(currentsavecommands!=""){
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
													xsavefinalarr=xdataintpeaks.split(" ");
													//console.log(data);
													ysavefinalarr=((data+"").replace("[","").replace("]","").replace(new RegExp(",", 'g'), " ")).split(" ");
													savestringfinal="";
													jj=0;
													while(jj<xsavefinalarr.length){
														if(jj>=ysavefinalarr.length){
															break;
														}
															savestringfinal=savestringfinal+xsavefinalarr[jj]+"\t"+ysavefinalarr[jj]+"\n";
														jj=jj+1;
													}
													spectranumbercounter=spectranumbercounter+1;
													//console.log(savelocationfinal+"-"+spectranumbercounter.addzeros(4)+".dat");
													if (fs.existsSync(savelocationfinal+"-"+spectranumbercounter.addzeros(4)+".dat")){
															if(errsavecmd==""){
																errsavecmd=savelocationfinal+"-"+(0).addzeros(4)+".dat";
															}
															if(errsavecmd!=savelocationfinal+"-"+(0).addzeros(4)+".dat"){
																errsavecmd="";
																showerrmessage=0;
															}else{
																if(showerrmessage==0){
																	saveonthis=0;
																	console.log("Error. File already exists.");
																	mmap.WriteSharedStringSync(iderrmaingui,"err");
																	mmap.WriteSync(errboxmmap,"-Error saving file: File already exists._");
																	showerrmessage=1;
																}
															}

													}else{
														try{
															fs.writeFileSync(savelocationfinal+"-"+spectranumbercounter.addzeros(4)+".dat", savestringfinal);
															saveonthis=1;
														}catch(e){
															if(currentsavecommands[2]+""=="false"){
																if(erronthis==0){
																	mmap.WriteSharedStringSync(iderrmaingui,"err");
																	mmap.WriteSync(errboxmmap,"-Error on saving file:\n"+e.message+"_");	
																	erronthis=1;
																	saveonthis=0;													
																}
															}
														}
													}
												}
											}
										}
									}
								}
								i=i+1;
							}
						}
					}else{
						if(spectranumbercounter!=-1){
							try{
								spawn("notify-send",["--urgency=normal","--icon="+dirpython+"/../mime-type/icon.svg","SpectraRead acquisition","\nData acquisition is complete."]);
								console.log("Running: notify-send --urgency=normal --icon="+dirpython+"/../mime-type/icon.svg SpectraRead acquisition \nData acquisition is complete.");
							}catch(e){
								console.log(e);
							}
						}
						spectranumbercounter=-1;
						errsavecmd="";
						showerrmessage=0;
						erronthis=0;
					}
				}else{
					if(saveonthis!=0){
						try{
							spawn("notify-send",["--urgency=normal","--icon="+dirpython+"/../mime-type/icon.svg","SpectraRead acquisition","\nData acquisition is complete."]);
							console.log("Running: notify-send --urgency=normal --icon="+dirpython+"/../mime-type/icon.svg SpectraRead acquisition \nData acquisition is complete.");
						}catch(e){
							console.log(e);
						}
					}
					time.sleep(0.1);
					spectranumbercounter=-1;
					errsavecmd="";
					showerrmessage=0;
					erronthis=0;
					saveonthis=0;
				}
			time.sleep(0.01);
			}catch(e){console.log(e);}
			}
	}else if (cluster.worker.id === 3) {
		var mmap=require("./bin_modules/fastmmapmq.node");
		var pf=require("./bin_modules/polfiths.node");
		var fs=require("fs");
		var path=require("path");
		var dirpython = path.dirname(fs.realpathSync(__filename));
		var cmdargs="";
		process.argv.forEach(function (val, index, array) {
			if(val.indexOf("spectrareadcurrentprocid:")!=-1){
				if(val.indexOf("spectrareadprocid")!=-1){
					cmdargs=val.split("spectrareadcurrentprocid:")[1].split("spectrareadprocid")[0];
				}
			}
		});
		var peaksgui=mmap.CreateMmapSync("p&","rwx------");
		var peaksguitot=mmap.CreateMmapSync("pfgs","rwx------");
		var finishloadjghgh=mmap.ConnectMmapSync(process.cwd()+"/spectrareads "+cmdargs,""+cmdargs);
		while(finishloadjghgh==-1){
			finishloadjghgh=mmap.ConnectMmapSync(process.cwd()+"/spectrareads "+cmdargs,""+cmdargs);
			//console.log("Connection failed on worker 3 1...Reconnecting");
			pf.sleep(0.1);
		}
		var infosaveui=mmap.ConnectMmapSync(process.cwd()+"/spectrareadd "+process.cwd()+"/main.js spectrareadcurrentprocid:"+cmdargs,"@"+cmdargs);
		while(infosaveui==-1){
			infosaveui=mmap.ConnectMmapSync(process.cwd()+"/spectrareadd "+process.cwd()+"/main.js spectrareadcurrentprocid:"+cmdargs,"@"+cmdargs);
			//console.log("Connection failed on worker 3...Reconnecting");
			pf.sleep(0.1);
		}
		var id=mmap.ConnectMmapSync(process.cwd()+"/peaksworker.js spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid spectrareadcmdarggui specreadprocpeaks:[","p[0"+cmdargs);
		while(id==-1){
			id=mmap.ConnectMmapSync(process.cwd()+"/peaksworker.js spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid spectrareadcmdarggui specreadprocpeaks:[","p[0"+cmdargs);
			//console.log("Connection failed on worker 3...Reconnecting");
			pf.sleep(0.3);
		}
		var idb=mmap.ConnectMmapSync(process.cwd()+"/peaksworker.js spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid spectrareadcmdarggui specreadprocpeaks:~","p~0"+cmdargs);
		while(idb==-1){
			idb=mmap.ConnectMmapSync(process.cwd()+"/peaksworker.js spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid spectrareadcmdarggui specreadprocpeaks:~","p~0"+cmdargs);
			//console.log("Connection failed on worker 3...Reconnecting");
			pf.sleep(0.1);
		}
		var idc=mmap.ConnectMmapSync(process.cwd()+"/peaksworker.js spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid spectrareadcmdarggui specreadprocpeaks:par","p(0"+cmdargs);
		while(idc==-1){
			idc=mmap.ConnectMmapSync(process.cwd()+"/peaksworker.js spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid spectrareadcmdarggui specreadprocpeaks:par","p(0"+cmdargs);
			//console.log("Connection failed on worker 3...Reconnecting");
			pf.sleep(0.1);
		}
		var idd=mmap.ConnectMmapSync(process.cwd()+"/peaksworker.js spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid spectrareadcmdarggui specreadprocpeaks:{","p{0"+cmdargs);
		while(idd==-1){
			idd=mmap.ConnectMmapSync(process.cwd()+"/peaksworker.js spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid spectrareadcmdarggui specreadprocpeaks:{","p{0"+cmdargs);
			//console.log("Connection failed on worker 3...Reconnecting");
			pf.sleep(0.1);
		}
		var ide=mmap.ConnectMmapSync(process.cwd()+"/peaksworker.js spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid spectrareadcmdarggui specreadprocpeaks:,","p,0"+cmdargs);
		while(ide==-1){
			ide=mmap.ConnectMmapSync(process.cwd()+"/peaksworker.js spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid spectrareadcmdarggui specreadprocpeaks:,","p,0"+cmdargs);
			//console.log("Connection failed on worker 3...Reconnecting");
			pf.sleep(0.1);
		}


		var idf=mmap.ConnectMmapSync(process.cwd()+"/peaksworker.js spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid spectrareadcmdarggui specreadprocpeaks:[","p[1"+cmdargs);
		while(idf==-1){
			idf=mmap.ConnectMmapSync(process.cwd()+"/peaksworker.js spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid spectrareadcmdarggui specreadprocpeaks:[","p[1"+cmdargs);
			//console.log("Connection failed on worker 3...Reconnecting");
			pf.sleep(0.3);
		}
		var idg=mmap.ConnectMmapSync(process.cwd()+"/peaksworker.js spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid spectrareadcmdarggui specreadprocpeaks:par","p(1"+cmdargs);
		while(idg==-1){
			idg=mmap.ConnectMmapSync(process.cwd()+"/peaksworker.js spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid spectrareadcmdarggui specreadprocpeaks:par","p(1"+cmdargs);
			//console.log("Connection failed on worker 3...Reconnecting");
			pf.sleep(0.1);
		}
		var idh=mmap.ConnectMmapSync(process.cwd()+"/peaksworker.js spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid spectrareadcmdarggui specreadprocpeaks:{","p{1"+cmdargs);
		while(idh==-1){
			idh=mmap.ConnectMmapSync(process.cwd()+"/peaksworker.js spectrareadcurrentprocid:"+cmdargs+"spectrareadprocid spectrareadcmdarggui specreadprocpeaks:{","p{1"+cmdargs);
			//console.log("Connection failed on worker 3...Reconnecting");
			pf.sleep(0.1);
		}
		var iderrmaingui=mmap.ConnectMmapSync(process.cwd()+"/spectrareadd "+process.cwd()+"/main.js spectrareadcurrentprocid:"+cmdargs,"errpd"+cmdargs);
		while(iderrmaingui==-1){
			iderrmaingui=mmap.ConnectMmapSync(process.cwd()+"/spectrareadd "+process.cwd()+"/main.js spectrareadcurrentprocid:"+cmdargs,"errpd"+cmdargs);
			//console.log("Connection failed on worker 3 id errpd...Reconnecting");
			pf.sleep(0.1);
		}
		var errboxmmap=mmap.ConnectMmapSync(process.cwd()+"/spectrareadd "+process.cwd()+"/main.js spectrareadcurrentprocid:"+cmdargs,"d"+cmdargs);
		while(errboxmmap==-1){
			errboxmmap=mmap.ConnectMmapSync(process.cwd()+"/spectrareadd "+process.cwd()+"/main.js spectrareadcurrentprocid:"+cmdargs,"d"+cmdargs);
			//console.log("Connection failed on worker 3 id d...Reconnecting");
			pf.sleep(0.1);
		}
		mmap.WriteSharedStringSync(finishloadjghgh,"ok");
		var datapeaks=['undefined','undefined','undefined','undefined','undefined'];
		var firstsave=0;
		var lastsave=new Date().getTime()/1000;
		var notifytext=["",""];
		var spawn = require("child_process").spawn;
		var erronthis=0;
		while(true){
			try{
				pf.sleep(0.1);
				test=mmap.ReadSync(id,0);
				if(test.indexOf("\n")!=-1){
					if(((new Date().getTime()/1000)-lastsave)>0.1){
						mmap.WriteSync(peaksgui,test);
						lastsave=new Date().getTime()/1000;
					}
				}
				test=test+mmap.ReadSync(idb,0);
				test=test+mmap.ReadSync(idc,0);
				test=test+mmap.ReadSync(idd,0);
				test=test+mmap.ReadSync(ide,0);
				test=test+mmap.ReadSync(idf,0);
				test=test+mmap.ReadSync(idg,0);
				test=test+mmap.ReadSync(idh,0);
				if(test.indexOf("\n")!=-1){
					try{
						datapeaks=eval(mmap.GetSharedStringSync(infosaveui).split("--++")[1]);
					}catch(e){console.log(e)}
					if(datapeaks.length!=5){
						datapeaks=["false","false","false","false","false"];
					}
					if(datapeaks[2]!='undefined'){
						if(firstsave==0){
							if(datapeaks[2]!="false"){
								try{
									if(erronthis==0){
										fs.appendFileSync(""+datapeaks[0]+"/"+datapeaks[2]+".dat", " ");
									}
								}catch(e){
									erronthis=1;
									mmap.WriteSharedStringSync(iderrmaingui,"err");
									mmap.WriteSync(errboxmmap,"-Error on saving file:\n"+e.message+"_");
								}
							}
							firstsave=1;
						}
						if(datapeaks[2]!="false"){
							try{
								if(erronthis==0){
									fs.appendFileSync(""+datapeaks[0]+"/"+datapeaks[2]+".dat", test);
								}
							}catch(e){
								erronthis=1;
								mmap.WriteSharedStringSync(iderrmaingui,"err");
								mmap.WriteSync(errboxmmap,"-Error on saving file:\n"+e.message+"_");
							}
						}
						notifytext[0]=datapeaks[0];
						notifytext[1]=datapeaks[2];
					}else{
						if(firstsave!=0){
						if(erronthis==0){
							if(notifytext[1]!="false"){
								try{
									spawn("notify-send",["--urgency=normal","--icon="+dirpython+"/../mime-type/icon.svg","SpectraRead acquisition","\nData acquisition to: "+""+notifytext[0]+"/"+notifytext[1]+".dat is complete."]);
									console.log("Running: notify-send --urgency=normal --icon="+dirpython+"/../mime-type/icon.svg SpectraRead acquisition \nData acquisition to: "+""+notifytext[0]+"/"+notifytext[1]+".dat is complete.");
								}catch(e){
									console.log(e);
								}
							}//else{
							//	try{
							//		spawn("notify-send",["--urgency=normal","--icon="+dirpython+"/../mime-type/icon.svg","SpectraRead acquisition","\nData acquisition is complete."]);
							//		console.log("Running: notify-send --urgency=normal --icon="+dirpython+"/../mime-type/icon.svg SpectraRead acquisition \nData acquisition is complete.");
							//	}catch(e){
							//		console.log(e);
							//	}
							//}
						}
						}
						erronthis=0;
						firstsave=0;
					}
					mmap.WriteSync(peaksguitot,test);
			}
			}catch(e){console.log(e)}
		}
	}else if (cluster.worker.id === 4) {
		//var peakshs=require("./hspeaks.js");
		//peakshs.inithspeaksprocess(":","|");
	}
