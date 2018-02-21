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
		  cmdargs=val;
		});
		var worker=cluster.fork();
		var workerb=cluster.fork();
		var workerc=cluster.fork();
		var python = require('python.node');
		var os = python.import('os');
		var path = require('path');
		var kill=require("./kill.js");
		var spectrabuffer="";
		var sys = python.import('sys');
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
		var colors = python.import('gtkcolors');
		//Get GTK theme colors used to draw the graph.
		function updatecolors(){
			worker.send({colors: colors.getcolors()+""});
		}
		setInterval(function (){
			updatecolors();
		},1000);
		function terminateallworkers(){
			worker.destroy();
			worker.process.kill();
			workerb.destroy();
			workerb.process.kill();
			workerc.destroy();
			workerc.process.kill();
			var filecodeb=""+filecode;
			if(filecodeb!=""){
				if(filecodeb.indexOf("/dev/shm/spectraread-shm")+""!="-1"){
					try{
						fs.unlinkSync(""+filecodeb);
					}catch(e){}
					try{
						fs.unlinkSync(""+filecodeb+"-spectrabuffer1-691727217");
					}catch(e){}
					try{
						fs.unlinkSync(""+filecodeb+"-spectrabuffer2-691727217");
					}catch(e){}
				}
			}
			kill.killallthreads(current_path+"/node-v4.4.2"+"\u0000"+current_path+"/main.js"+"\u0000"+cmdargs+"\u0000","SIGTERM",process.pid);
			kill.killallthreads(current_path+"/node-v4.4.2"+"\u0000"+"main.js"+"\u0000"+cmdargs+"\u0000","SIGTERM",process.pid);
			//kill.killallthreads(current_path+"/node-v4.4.2"+"\u0000"+"main.js"+"\u0000"+cmdargs+"\u0000","SIGKILL",process.pid);
			//kill.killallthreads(current_path+"/node-v4.4.2"+"\u0000"+current_path+"/main.js"+"\u0000"+cmdargs+"\u0000","SIGKILL",process.pid);
			process.exit();
		}
		function startexitprocess(){
			if (fs.existsSync(""+filecode)){
				try{
					fs.appendFileSync(""+filecode,")");
				}catch(e){}
				try{
					fs.appendFileSync(""+filecode,")");
				}catch(e){}
				try{
					fs.appendFileSync(""+filecode,")");
				}catch(e){}
			}
		}
		//Workers send messages to this thread. Send the messages to other workers.
		workerb.on('message', function(msg) {
			if(msg.buffer){
				worker.send({spectra: msg.buffer});
			}
			if(msg.bufferprocess){
				workerc.send({bufferprocess: "ready"});
			}
			if(msg.frequency){
				worker.send({frequency: msg.frequency});
			}
			if(msg.deviceinfo){
				worker.send({deviceinfo: msg.deviceinfo});
			}
			if(msg.filecode){
				worker.send({filecode: msg.filecode});
				workerc.send({filecodeinfo: msg.filecode});
				filecode=""+msg.filecode;
			}
			if(msg.closeready){
				console.log('Device control module closed gracefully');
				terminateallworkers();
			}
			if(msg.stopacquisition){
				worker.send({stopacquisition: msg.stopacquisition});
			}
		});
		worker.on('message', function(msg) {
			if(msg.selectedvalues){
				workerc.send({selectedvalues: msg.selectedvalues});
			}
			if(msg.savefilecommand){
				workerc.send({savefilecommand: msg.savefilecommand});
			}
		});
		worker.on('exit', function(aaa,bbb) {
			console.log('Worker died. Starting close process');
			startexitprocess();
			setTimeout(function (){
				errmessage.showerrormessage("An unexpected exception has happened.\nProgram will close.");
				terminateallworkers();
			},2000);
		});
		workerb.on('exit', function(aaa,bbb) {
			console.log('Worker died. Starting close process');
			startexitprocess();
			setTimeout(function (){
				errmessage.showerrormessage("An unexpected exception has happened.\nProgram will close.");
				terminateallworkers();
			},2000);
		});
		workerc.on('exit', function(aaa,bbb) {
			console.log('Worker died. Starting close process');
			startexitprocess();
			setTimeout(function (){
				errmessage.showerrormessage("An unexpected exception has happened.\nProgram will close.");
				terminateallworkers();
			},2000);
		});
		process.on('exit', function () {
			console.log('About to exit');
			startexitprocess();
			terminateallworkers();
			process.exit();
		});
	}else if (cluster.worker.id === 1) {
		var polfit=require("./polfit.node");
		var processsavedialog=require("./dialoganalysis.js");
		var fileio=require("./fileio.js");
		var python = require('python.node');
		var os = python.import('os');
		var filecode="";
		var fs = require('fs');
		var path = require('path');
		var t0=0;
		//sha1sum is used to verify the integrity of data transfered by the file in /dev/shm.
		var sha1sum = require('sha1');
		var sys = python.import('sys');
		var fs = require('fs');
		//Variables used to stop continuous acquisition data saving when the programed time is reached.
		//In this thread we only control if the program is or is not saving data, the data is actualy saved by workerc.
		//Here we only save data in single mode (single spectrum, peaks or spectrum and peaks) and transfer the spectra received to the graph thread.
		//We don't receive all the spectra acquired here.
		var multiplefilesavefor=0;
		var acquisitionsaving=0;
		var dirpython = path.dirname(fs.realpathSync(__filename));
		//Here we will store the last acquired data for the save function and the UI.
		var currentspectrabuffer="";
		var currrentpeaksbuffer="";
		//---------------------------------------------------------------
		sys.path.append(dirpython);
		sys.path.append(dirpython+'/spectrometer_modules');
		sys.path.append(dirpython+'/Python/lib');
		sys.path.append(dirpython+'/Python/lib/python2.7');
		sys.path.append(dirpython+'/Python/lib/python2.7/site-packages');
		sys.path.append(dirpython+'/Python/lib/python2.7/site-packages/numpy');
		var closewarningok=1;
		var tests = python.import('main-ui');
		var selectedvaluesintsha1sum="";
		function gettime(starttime) {
			if(!starttime){
				return process.hrtime();
			}
			var endtime = process.hrtime(starttime);
			return Math.round((endtime[0]*1000) + (endtime[1]/1000000));
		}
		function getvalues(){
			if(t0==0){
				t0=gettime();
			}
			if(t0!=0){
				if(multiplefilesavefor!=0){
					if(gettime(t0)>multiplefilesavefor*1000){
						//Stop continuous acquisition data saving.
						tests.clearsavecommand();
						process.send({savefilecommand:tests.getsavecommand()});
						multiplefilesavefor=0;
						acquisitionsaving=0;
					}
				}
			}
			selectedvaluesintsha1sum=tests.getselectedvalues();
			if(filecode!=""){
				if(filecode.indexOf("/dev/shm")+""!="-1"){
					if(selectedvaluesintsha1sum+""!=""){
						//Save values to be transfered to python acquisition script in file in /dev/shm with the hash.
						//We can't use process.send here because of the while(true) in workerb that blocks received messages in this way.
						fs.appendFileSync(""+filecode,""+selectedvaluesintsha1sum+"_________"+sha1sum(""+selectedvaluesintsha1sum)+"\n");
						process.send({selectedvalues: ""+selectedvaluesintsha1sum});
					}
				}
			}

			if(tests.iswindowclosed().split(" ")[0]+""=="closed"){
				if(closewarningok==1){
					fs.appendFileSync(""+filecode,")");
					fs.appendFileSync(""+filecode,")");
					fs.appendFileSync(""+filecode,")");
					console.log("Window closed");
					closewarningok=0;
				}
			}
			if(tests.iswindowclosed().split(" ")[1]+""=="False"){
				if(closewarningok==1){
					console.log("Gui process killed");
					fs.appendFileSync(""+filecode,")");
					fs.appendFileSync(""+filecode,")");
					fs.appendFileSync(""+filecode,")");
					closewarningok=0;
				}
			}
			//return format: [file location or false,append1(spectrum filename) or true or false,append2(peaks filename) or true or false,(save every) or false,(save for) or false]
			//remember:::::true==true-->>true
			//             "true"==true-->>false
			//So there will be no problem if the filename is true because the function will return "true" and not true.
			//
			//Here we save single spectrum, peaks and spectrum and peaks.
			var savecomands=processsavedialog.process(tests.getsavecommand());
			if(savecomands[3]==false){
				if(savecomands[4]==false){
					tests.clearsavecommand();
					if(savecomands[1]==true){
						if(savecomands[2]==false){
							fileio.savesinglespectrum(currentspectrabuffer,""+savecomands[0]);
						}
					}
					if(savecomands[2]==true){
						if(savecomands[1]==false){
							fileio.savesinglepeaks(currrentpeaksbuffer,""+savecomands[0]);
						}
					}
					if(savecomands[1]!=false){
						if(savecomands[2]!=false){
							fileio.savesinglespectrumpeaks(currentspectrabuffer,currrentpeaksbuffer,""+savecomands[0]+"/"+savecomands[1],""+savecomands[0]+"/"+savecomands[2])
						}
					}
					tests.clearsavecommand();
				}
			}
			//Here we control continuous acquisition data saving and send the message to workerc to save data.
			if(savecomands[3]!=false){
				if(savecomands[1]!=false){
					if(savecomands[2]==false){
						//save multiple spectra            location          base name           save for              save every
						//console.log("multiple spectra"+savecomands[0]+"/"+savecomands[1]+"  "+savecomands[3]+"   "+savecomands[4]);
						multiplefilesavefor=savecomands[4];
						process.send({savefilecommand:tests.getsavecommand()});
						if(acquisitionsaving==0){
							t0=gettime();
						}
						acquisitionsaving=1;
					}
				}
				if(savecomands[2]!=false){
					if(savecomands[1]==false){
						//save multiple peaks                location         base name           save every           save for
						//console.log("multiple peaks"+savecomands[0]+"/"+savecomands[2]+"  "+savecomands[3]+"   "+savecomands[4]);
						multiplefilesavefor=savecomands[4];
						process.send({savefilecommand:tests.getsavecommand()});
						if(acquisitionsaving==0){
							t0=gettime();
						}
						acquisitionsaving=1;
					}
				}
				if(savecomands[1]!=false){
					if(savecomands[2]!=false){
						//save multiple spectra and peaks           location       peaks base name       location         spectra base name            save every           save for
						//console.log("multiple spectra and peaks"+savecomands[0]+"/"+savecomands[2]+"  "+savecomands[0]+"/"+savecomands[1]+"      "+savecomands[3]+"   "+savecomands[4]);
						multiplefilesavefor=savecomands[4];
						process.send({savefilecommand:tests.getsavecommand()});
						if(acquisitionsaving==0){
							t0=gettime();
						}
						acquisitionsaving=1;
					}
				}

			}

		}
		setInterval(function (){
			getvalues();
		},100);
		var globalfrequpdate=0;
		var globalinfoupdate=[];
		globalinfoupdate[0]="--";
		globalinfoupdate[1]="";
		process.on('message', function(msg) {
			if(msg.colors){
				tests.updatecolorvalues(msg.colors);
			}
			if(msg.spectra){
				polfit.peaks(msg.spectra+"",Number(selectedvaluesintsha1sum.split(";")[0]),Number(selectedvaluesintsha1sum.split(";")[3]),function(err,data){
					tests.updategraphspectrumvariable((data+"").replace("[","").replace("]","").replace(new RegExp(",", 'g'), " "));
					console.log(selectedvaluesintsha1sum);
				});
				currentspectrabuffer=msg.spectra;
			}
			if(msg.frequency){
				globalfrequpdate=msg.frequency;
				tests.updateinfotext(1,globalfrequpdate,""+globalinfoupdate[0],""+globalinfoupdate[1]);
			}
			if(msg.deviceinfo){
				globalinfoupdate=msg.deviceinfo.split("--int-info-seperator--");
				tests.updateinfotext(1,globalfrequpdate,""+globalinfoupdate[0],""+globalinfoupdate[1]);
			}
			if(msg.filecode){
				filecode=""+msg.filecode;
				//console.log(""+filecode);
			}
			if(msg.stopacquisition){
				if(msg.stopacquisition+""=="true"){
					tests.stopacquisition();
				}
			}
		});
	}else if (cluster.worker.id === 2) {
		var python = require('python.node');
		var os = python.import('os');
		var path = require('path');
		var spectrabuffer="";
		var comandexitglobalb="";
		var sys = python.import('sys');
		var fs = require('fs');
		var sha1sum = require('sha1');
		var filecode="";
		var testfilecode="/dev/shm/spectraread-shm-"+Math.round(Math.random()*Math.random()*Math.random()*10000000000)
		while(fs.existsSync(testfilecode)){
			testfilecode="/dev/shm/spectraread-shm-"+Math.round(Math.random()*Math.random()*Math.random()*10000000000)
		}
		filecode=""+testfilecode;
		var path = require('path');
		fs.writeFileSync(""+filecode,"");
		fs.writeFileSync(""+filecode+"-spectrabuffer1-691727217","");
		fs.writeFileSync(""+filecode+"-spectrabuffer2-691727217","");
		process.send({filecode: filecode})
		var t0=0;
		var t1=0;
		var t2=0;
		var nspectra=0;
		var path = require('path');
		var dirpython = path.dirname(fs.realpathSync(__filename));
		sys.path.append(dirpython);
		sys.path.append(dirpython+'/spectrometer_modules');
		sys.path.append(dirpython+'/Python/lib');
		sys.path.append(dirpython+'/Python/lib/python2.7');
		sys.path.append(dirpython+'/Python/lib/python2.7/site-packages');
		sys.path.append(dirpython+'/Python/lib/python2.7/site-packages/numpy');
		var spectrometerio = python.import('imon256.spectrometerio');
		function gettime(starttime) {
			if(!starttime){
				return process.hrtime();
			}
			var endtime = process.hrtime(starttime);
			return Math.round((endtime[0]*1000) + (endtime[1]/1000000));
		}
		var lastspectra="";
		var appendcount=0;
		var appendfile=1;
		var datacommandsglobalfile=""
		function start(){
		while(true){
			try{
				if(t0==0){
					t0=gettime();
				}
				if(t1==0){
					t1=gettime();
				}
				if(t2==0){
					t2=gettime();
				}
				spectrabuffer=spectrometerio.readspectra();
				if(spectrabuffer+""!=""){
				nspectra=nspectra+(spectrabuffer.split("_").length-1);
				if(gettime(t0)>1000){
					try{
						var deviceargs=spectrometerio.getinfo()
						var commands = fs.readFileSync(""+filecode, "utf8");
						datacommandsglobalfile=commands;
						var cmdsplit=commands.split("\n");
						var processingline=cmdsplit[cmdsplit.length-2];
						if(sha1sum(processingline.split("_________")[0])+""==processingline.split("_________")[1]){
							spectrometerio.setcurrentoptions(((processingline.split("_________")[0]).split("----------")[1])+""+((processingline.split("_________")[0]).split("----------")[0]).split(";")[4]);
						}
						fs.writeFileSync(""+filecode,"");
					}catch(e){console.log(e)}
					process.send({frequency: nspectra});
					process.send({deviceinfo: deviceargs[0]+"--int-info-seperator--"+deviceargs[1]});
					process.send({stopacquisition: ""+deviceargs[2]});
					nspectra=0;
					t0=gettime();
				}
				lastspectra=lastspectra+spectrabuffer;
				if(gettime(t1)>100){
					//Here we update the graph 10 times per second
					process.send({buffer: spectrabuffer.split("_")[0]});
					t1=gettime();
				}
				if(gettime(t2)>200){
					var CommandsExit = datacommandsglobalfile;
					if(CommandsExit.indexOf(")")+""!="-1"){
						spectrometerio.close();
						process.send({closeready: "ok"});
					}
					//if(fs.readFileSync(""+filecode+"-spectrabufferappendinginfo-691727217", "utf8")+""=="1"){
					//	fs.writeFileSync(""+filecode+"-spectrabuffer-691727217","");
					//	fs.writeFileSync(""+filecode+"-spectrabufferappendinginfo-691727217","0");
					//	console.log("clear");
					//};
					//Here we send every spectrum received for processing and maybe to be saved to disk...
					//Don't let the file be larger than 200 Mb (It is in RAM memory, we don't want the program crashing the whole session if one thread closes unexpectedly).
					if(fs.statSync(""+filecode+"-spectrabuffer"+appendfile+"-691727217").size/1000000<200){
							//Write the spectra buffer to shared memory device and notify the worker that we finished writting.
							fs.appendFileSync(""+filecode+"-spectrabuffer"+appendfile+"-691727217",""+lastspectra);
							appendcount=appendcount+1;
							if(appendcount>35){
								process.send({bufferprocess: "ready"});
								if(appendfile+""=="1"){
									appendfile=2;
								}else{
									appendfile=1;
								}
							}
							//Clear the buffer so that we can receive new spectra.
							lastspectra="";
					}else{
						if(lastspectra.split("_").length>25000){
							lastspectra=""
							console.log("Can't process spectra: Buffer full\nDropping spectra");
							process.send({bufferprocess: "ready"});
							if(appendfile+""=="1"){
								appendfile=2;
							}else{
								appendfile=1;
							}
						}
					}
					t2=gettime();
				}
				}else{
					if(gettime(t0)>600){
						try{
							var deviceargs=spectrometerio.getinfo()
							process.send({stopacquisition: ""+deviceargs[2]});
							var commands = fs.readFileSync(""+filecode, "utf8");
							comandexitglobalb=commands;
							if(commands+""!=""){
								var cmdsplit=commands.split("\n");
								var processingline=cmdsplit[cmdsplit.length-2];
								if(sha1sum(processingline.split("_________")[0])+""==processingline.split("_________")[1]){
									spectrometerio.setcurrentoptions(((processingline.split("_________")[0]).split("----------")[1])+""+((processingline.split("_________")[0]).split("----------")[0]).split(";")[4]);
								}
								fs.writeFileSync(""+filecode,"");
							}
							process.send({frequency: "0"});
						}catch(e){console.log(e)}
						t0=gettime();
					}
					if(gettime(t0)>100){
						if(comandexitglobalb.indexOf(")")+""!="-1"){
							spectrometerio.close();
							process.send({closeready: "ok"});
						}
						if(fs.readFileSync(""+filecode, "utf8").indexOf(")")+""!="-1"){
							spectrometerio.close();
							process.send({closeready: "ok"});
						}
					}
				}
			}catch(e){console.log(e);}
			
		}
		}
		setTimeout(function (){
			start();
		},200);
	}else{
		var fs = require('fs');
		var processsavedialog=require("./dialoganalysis.js");
		var filecode="";
		var settings=[];
		var readfile=1;
		var saveevery=0;
		var saveskipcount=0;
		var savefilecomand=[];
		fs.writeFileSync(""+filecode+"-spectrabuffer1-691727217","");
		fs.writeFileSync(""+filecode+"-spectrabuffer2-691727217","");
		var savespectrabuffer="";
		function processsave(savefilecurrentdata){
			if(savefilecomand[3]!=false){
				if(savefilecomand[1]!=false){
					if(savefilecomand[2]==false){
						//save multiple spectra            location          base name           save every              save for
						//console.log("multiple spectra"+savecomands[0]+"/"+savecomands[1]+"  "+savecomands[3]+"   "+savecomands[4]);
						saveevery=savefilecomand[3];
						saveskipcount=saveskipcount+1;
						if(saveskipcount-1>=saveevery){
							console.log("Save multiple spectra file on:"+savefilecomand[0]+"/"+savefilecomand[1]);
							saveskipcount=0;
						}
					}
				}
				if(savefilecomand[2]!=false){
					if(savefilecomand[1]==false){
						//save multiple peaks                location         base name           save every           save for
						//console.log("multiple peaks"+savecomands[0]+"/"+savecomands[2]+"  "+savecomands[3]+"   "+savecomands[4]);
						saveevery=savefilecomand[3];
						saveskipcount=saveskipcount+1;
						if(saveskipcount-1>=saveevery){
							console.log("Save multiple peaks file on:"+savefilecomand[0]+"/"+savefilecomand[1]);
							saveskipcount=0;
						}
					}
				}
				if(savefilecomand[1]!=false){
					if(savefilecomand[2]!=false){
						//save multiple spectra and peaks           location       peaks base name       location         spectra base name            save every           save for
						//console.log("multiple spectra and peaks"+savecomands[0]+"/"+savecomands[2]+"  "+savecomands[0]+"/"+savecomands[1]+"      "+savecomands[3]+"   "+savecomands[4]);
						saveevery=savefilecomand[3];
						saveskipcount=saveskipcount+1;
						if(saveskipcount-1>=saveevery){
							console.log("Save multiple peaks and spectra file on:"+savefilecomand[0]+"/"+savefilecomand[1]);
							saveskipcount=0;
						}
					}
				}
			}

		}
		process.on('message', function(msg) {
			if(msg.bufferprocess){
				if(filecode!=""){
					if(filecode.indexOf("/dev/shm")+""!="-1"){
						//console.log("erasing file "+readfile);
						savespectrabuffer=fs.readFileSync(""+filecode+"-spectrabuffer"+readfile+"-691727217","utf8").split("_");
						fs.writeFileSync(""+filecode+"-spectrabuffer"+readfile+"-691727217","");
								if(readfile+""=="1"){
									readfile=2;
								}else{
									readfile=1;
								}
						if(savefilecomand[3]!=false){
							for (i=0; i<savespectrabuffer.length-1; i++){
								processsave(savespectrabuffer[i]);
							}
						}
					}
				}
			}
			if(msg.selectedvalues){
				//console.log("worker3 got selected values"+msg.selectedvalues);
				settings=((msg.selectedvalues).split("----------")[0]).split(";")
			}
			if(msg.filecodeinfo){
				filecode=msg.filecodeinfo;
			}
			if(msg.savefilecommand){
				savefilecomand=processsavedialog.process(msg.savefilecommand);
			}
		});
}
