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
		var mmap=require("./fastmmapmq");
		var cmdargs="";
		process.argv.forEach(function (val, index, array) {
			if(val.indexOf("spectrareadcurrentprocid:")!=-1){
				if(val.indexOf("spectrareadprocid")!=-1){
					cmdargs=val.split("spectrareadcurrentprocid:")[1].split("spectrareadprocid")[0];
				}

			}
		});
		var id=mmap.ConnectMmapSync("spectrareads",""+cmdargs);
		var polfit=require("./polfit.node");
		mmap.WriteSync(id,",03load ui modules");
		var processsavedialog=require("./dialoganalysis.js");
		var fileio=require("./fileio.js");
		var python = require('python.node');
		var time=python.import("time");
		while(id==-1){
			id=mmap.ConnectMmapSync("spectrareads",""+cmdargs);
			console.log("Connection failed on nodegui 1...Reconnecting");
			time.sleep(0.1);
		}
		var os = python.import('os');
		time.sleep(0.2);
		var idintc=mmap.ConnectMmapSync("spectrareadd","k"+cmdargs);
		while(idintc==-1){
			idintc=mmap.ConnectMmapSync("spectrareadd","k"+cmdargs);
			console.log("Connection failed nodegui 2...Reconnecting");
			time.sleep(0.1);
		}
		var datamapaaab=mmap.ConnectMmapSync("spectrareadd","?"+cmdargs);
		while(datamapaaab==-1){
			datamapaaab=mmap.ConnectMmapSync("spectrareadd","?"+cmdargs);
			console.log("Connection failed nodegui 2...Reconnecting");
			time.sleep(0.1);
		}
		time.sleep(0.1);
		mmap.WriteSync(id,",03loading file io modules");
		var filecode="";
		var fs = require('fs');
		var path = require('path');
		var t0=0;
		//sha1sum is used to verify the integrity of data transfered by the file in /dev/shm.
		var sha1sum = require('sha1');
		var sys = python.import('sys');
		var fs = require('fs');
		time.sleep(0.1);
		mmap.WriteSync(id,",13loading main ui (01)");
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
		mmap.WriteSync(id,",05loading main ui (02)");
		var tests = python.import('main-ui');
		tests.mainload(cmdargs);
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
			//if(filecode!=""){
				//if(filecode.indexOf("/dev/shm")+""!="-1"){
					if(selectedvaluesintsha1sum+""!=""){
						//Save values to be transfered to python acquisition script in file in /dev/shm with the hash.
						//We can't use process.send here because of the while(true) in workerb that blocks received messages in this way.
						//fs.appendFileSync(""+filecode,""+selectedvaluesintsha1sum+"_________"+sha1sum(""+selectedvaluesintsha1sum)+"\n");
						process.send({selectedvalues: ""+selectedvaluesintsha1sum});
					}
				//}
			//}

			if(tests.iswindowclosed().split(" ")[0]+""=="closed"){
				if(closewarningok==1){
					console.log("Window closed");
					process.send({closegui:true});
					closewarningok=0;
				}
			}
			if(tests.iswindowclosed().split(" ")[1]+""=="False"){
				if(closewarningok==1){
					console.log("Gui process killed");
					process.exit();
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
			if(tests.getsavecommand()=="False"){
				process.send({savefilecommand:"False"});
			}
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
		var globalfrequpdate=0;
		var globalinfoupdate=[];
		globalinfoupdate[0]="--";
		globalinfoupdate[1]="";
		var olddata="";
		var xdataintpeaks="";
		var winopened="";
		var messageb=0
		var uiupdatedlastinteraction=0;
		var lastspecmathuiupdatedlastinteraction="";
		function updateui(){
			if(uiupdatedlastinteraction>-1){
				uiupdatedlastinteraction=uiupdatedlastinteraction-1;
			}
			try{
			if(winopened==""){
				if(id!=-1){
					if(messageb==0){
						time.sleep(0.2);
						if(id!=-1){
							mmap.WriteSync(id,",00waiting daemon (1)");
						}
						messageb=1;
					}
					if(messageb==1){
						time.sleep(0.2);
						if(id!=-1){
							mmap.WriteSync(id,",00waiting daemon (2)");
						}
						messageb=2;
					}
					if(selectedvaluesintsha1sum!=""){
						if(id!=-1){
							mmap.WriteSync(id,",01window load");
						}
						time.sleep(0.2);
						winopened="1";
						tests.readyshowwin();
					}
				}
			}
			if(selectedvaluesintsha1sum!=""){
				dataintupdateui=olddata;
				ueigtghtge=0;
				while(dataintupdateui.indexOf("final")==-1){
				if(idintc!=-1){
					dataintupdateui=dataintupdateui+mmap.GetSharedStringSync(datamapaaab);
				}
					if(ueigtghtge>200){
						dataintupdateui="";
						break;
					}
				ueigtghtge=ueigtghtge+1;
				}
				olddata=(dataintupdateui.split("final")[dataintupdateui.split("final").length-1]);
				dataintupdateui=dataintupdateui.split(";");
				i=0;
				while(i<=(dataintupdateui.length)-1){
					if(i>5){
						break;
					}
					if(dataintupdateui[i].indexOf(":")!=-1){
						if(dataintupdateui[i].indexOf("buffer:")!=-1){
							if(selectedvaluesintsha1sum!=""){
								xdataintpeaks=dataintupdateui[i].split("buffer:")[1].split("?")[0]
					if((/^[0-9 .]*$/.test(dataintupdateui[i].split("buffer:")[1].split("?")[0]+""))==true){
								if(Number(selectedvaluesintsha1sum.split(";")[0])+""!="NaN"){
									if(Number(selectedvaluesintsha1sum.split(";")[3])+""!="NaN"){
										if(Number(selectedvaluesintsha1sum.split(";")[1])+""!="NaN"){
											if(((dataintupdateui[i].split("buffer:")[1].split("?")[1].split("final")[0]+" ").split(" ")).length==((xdataintpeaks+" ").split(" ").length)){
												if((/^[0-9 .]*$/.test(dataintupdateui[i].split("buffer:")[1].split("?")[1].split("final")[0]+""))==true){
												//polfit.process(xdataintpeaks+" ",dataintupdateui[i].split("buffer:")[1].split("?")[1].split("final")[0]+" ",Number(selectedvaluesintsha1sum.split(";")[0]),Number(selectedvaluesintsha1sum.split(";")[3]),0,Number(selectedvaluesintsha1sum.split(";")[1]),function(err,data,datapeaks,l){
													data=polfit.getspecsync(xdataintpeaks+" ",dataintupdateui[i].split("buffer:")[1].split("?")[1].split("final")[0]+" ",Number(selectedvaluesintsha1sum.split(";")[0]),Number(selectedvaluesintsha1sum.split(";")[3]),Number(selectedvaluesintsha1sum.split(";")[1]));
													datapeaks=polfit.getpeakssync();
													tests.updategraphspectrumvariable(xdataintpeaks,(data+"").replace("[","").replace("]","").replace(new RegExp(",", 'g'), " "));
													k=0;
													intvecx=xdataintpeaks.split(" ");
													if(uiupdatedlastinteraction<9){
														if(lastspecmathuiupdatedlastinteraction!=xdataintpeaks+""+(data+"").replace("[","").replace("]","").replace(new RegExp(",", 'g'), " ")){
															uiupdatedlastinteraction=uiupdatedlastinteraction+2;
														}
													}
													lastspecmathuiupdatedlastinteraction=xdataintpeaks+""+(data+"").replace("[","").replace("]","").replace(new RegExp(",", 'g'), " ");
													currentspectrabuffer="";
													while(k<data.length){
														currentspectrabuffer=currentspectrabuffer+intvecx[k]+"\t"+data[k]+"\n";
														k=k+1;
													}
													savebuffer="";
													k=0;
													currrentpeaksbuffer="0\t";
													while(k<datapeaks.length){
														savebuffer=savebuffer+parseFloat(Math.round(Number(datapeaks[k])*100)/100).toFixed(2)+" ";
														currrentpeaksbuffer=currrentpeaksbuffer+"\t"+datapeaks[k];
													k=k+1;
													}
													tests.updateinfopeaks(savebuffer);
												//});
											}
										}
									}
								}
							}}}
					}
					if(idintc!=-1){
					datainfo=mmap.GetSharedStringSync(idintc);
						if(datainfo.indexOf("frequency:")!=-1){
							globalfrequpdate=datainfo.split("frequency:")[1].split(",")[0];
							tests.updateinfotext(globalfrequpdate,""+globalinfoupdate[0],""+globalinfoupdate[1]);
						}
						if(datainfo.indexOf("deviceinfo:")!=-1){
							globalinfoupdate=datainfo.split("deviceinfo:")[1].split(",")[0].split("--int-info-seperator--");
							tests.updateinfotext(globalfrequpdate,""+globalinfoupdate[0],""+globalinfoupdate[1]);
						}
						if(datainfo.indexOf("stopaq:")!=-1){
							mmap.WriteSharedStringSync(idintc,"");
							tests.stopacquisition();
						}
						if(datainfo.indexOf("closeready:")!=-1){
							console.log('Device control module closed gracefully');
							process.exit();
						}
				}
				}
				i=i+1;
				}
			}
		}catch(e){console.log(e);}
		}
		//setInterval(function (){
		//	updateui();
		//},30);
		//setInterval(function (){
		//	getvalues();
		//},50);
		while(true){
			updateui();
			getvalues();
			if(uiupdatedlastinteraction>0){
				time.sleep(0.03);
			}else{
				time.sleep(0.1);
			}
		}
