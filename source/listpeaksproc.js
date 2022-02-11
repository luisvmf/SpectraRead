		//MIT License
		//
		//Copyright (c) 2022 Luís Victor Müller Fabris
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
		var fs = require('fs');
		var path = require('path');
		var fastmmapmq=require("./bin_modules/fastmmapmq.node");
		var updateevery=3; //Update data sources every 3 seconds.
		var connectedsourcesarray=[];
		var mapsidarray=[];
		var started=0;
		var lastupdate=new Date().getTime();
		var spawn=require('child_process').spawn;
		var configdata=JSON.parse(fs.readFileSync(path.dirname(fs.realpathSync(__filename))+"/../spectraread.conf","utf8").replace(/(\/\*([\s\S]*?)\*\/)|(\/\/(.*)$)/gm, '')).PeaksMonitorDataSources;
		String.prototype.replaceAll=function(search, replace){
				return this.split(search).join(replace);
		};
		function startdaemons(){
			for(var index=0;index<configdata.length;index++){
				sourceid=configdata[index];
				if(sourceid.daemon==true){
					spawn(""+sourceid.location,{
						detached:true
					});
				}
			}
		}
		function listallthreads(programcmdline){
				var retarray=[];
				var pidsretarray=[];
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
						if(cmdline.replaceAll("\u0000"," ").indexOf(""+programcmdline)>-1){
							//console.log(cmdline);
							retarray.push(cmdline.replaceAll("\u0000"," ").slice(0, cmdline.replaceAll("\u0000"," ").length-1));
							pidsretarray.push(listpids[i]);
						}
					}catch(e){}
				}
				return [retarray,pidsretarray];
		}
		function adddatasources(connectedarray,mapsid,connectmmap){
				for(var index2=0;index2<configdata.length;index2++){
					sourceid=configdata[index2];
					var intvarhj=listallthreads(sourceid.location);
					dataproc=intvarhj[0];
					for(var index3=0;index3<dataproc.length;index3++){
						if(sourceid.type=="fastmmapmq"){
							var push=1;
					 		for(var index=0;index<connectedarray.length;index++){
								var intidvarhghh="";
								try{
									intidvarhghh=dataproc[index3].split("spectrareadcurrentprocid:")[1].split("spectrareadprocid")[0];
								}catch(e){}
								if(connectedarray[index]+""==""+[sourceid.name+""+intidvarhghh+" "+sourceid.hastimestamp+" '"+dataproc[index3]+"' "+sourceid.key,dataproc[index3]]){
									push=0;
								}
							}
							if(push==1){
								var intidvarhghh="";
								try{
									intidvarhghh=dataproc[index3].split("spectrareadcurrentprocid:")[1].split("spectrareadprocid")[0];
								}catch(e){}
								connectedarray.push([sourceid.name+""+intidvarhghh+" "+sourceid.hastimestamp+" '"+dataproc[index3]+"' "+sourceid.key,dataproc[index3],intvarhj[1][index3]]);
								if(connectmmap==0){
									mapsid.push(-1);
								}else{
									//console.log(dataproc[index3]+","+sourceid.key);
									mapsid.push(fastmmapmq.ConnectMmapSync(dataproc[index3],sourceid.key));
									if(mapsid[mapsid.length-1]==-1){
										connectedarray.splice(connectedarray.length-1,1);
										mapsid.splice(mapsid.length-1,1);
									}
								}
							}
						}
					}
				}
		}
		function removedcloseddatasources(connectedarray,mapsid){
			tempmapsidarray=[];
			tempconnectedarray=[];
			adddatasources(tempconnectedarray,tempmapsidarray,0);
			for(var index=0;index<connectedarray.length;index++){
				var thelookprogram=connectedarray[index];
				var removethissource=1;
				for(var indexb=0;indexb<tempconnectedarray.length;indexb++){
					if(tempconnectedarray[indexb]+""==thelookprogram+""){
						removethissource=0;
					}
				}
				if(removethissource==1){
					connectedarray.splice(index,1);
					mapsid.splice(index,1);
				}
			}
		}
		function includes(a, obj) {
			for(var i = 0; i < a.length; i++){
				if(a[i][2]==obj[2]){
				    return true;
				}
			}
			return false;
		}
		function datasources(){
			if(started==0){
				startdaemons();
				adddatasources(connectedsourcesarray,mapsidarray,1);
				removedcloseddatasources(connectedsourcesarray,mapsidarray);
			}
			started=1;
			if(Math.abs((new Date().getTime())-lastupdate)>updateevery*1000){
				adddatasources(connectedsourcesarray,mapsidarray,1);
				removedcloseddatasources(connectedsourcesarray,mapsidarray);
				lastupdate=new Date().getTime();
			}
			var retarray=[];
			for(var index=0;index<connectedsourcesarray.length;index++){
				if(!includes(retarray,[connectedsourcesarray[index][0],mapsidarray[index],connectedsourcesarray[index][1],connectedsourcesarray[index][2]])){
					retarray.push([connectedsourcesarray[index][0],mapsidarray[index],connectedsourcesarray[index][1],connectedsourcesarray[index][2]]);
				}
			}
			return retarray;
		}
		module.exports={datasources,fastmmapmq};
