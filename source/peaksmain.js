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
			console.log("dffgfd");
			var procb=cluster.fork();
			var python = require('python.node');
			var os = python.import('os');
			var path = require('path');
			var spectrabuffer="";
			var cmdargs="";
			process.argv.forEach(function (val, index, array) {
				if(val.indexOf("peakscurrentprocid:")!=-1){
					if(val.indexOf("peaksprocid")!=-1){
						cmdargs=val.split("peakscurrentprocid:")[1].split("peaksprocid")[0];
					}

				}
			});
			if(cmdargs==""){
					console.log("not started proerly");
					const exec = require("child_process").exec;
					exec('./peaks').unref();
					process.exit();
			}
			var mmap=require("./bin_modules/fastmmapmq.node");
			var sys = python.import('sys');
			var time = python.import('time');
			time.sleep(0.05);
			var id=mmap.ConnectMmapSync("peakss",""+cmdargs);
			while(id==-1){
				id=mmap.ConnectMmapSync("peakss",""+cmdargs);
				console.log("Connection failed...Reconnecting"+cmdargs);
				time.sleep(0.1);
			}
			mmap.WriteSync(id,",10loading libraries 1");
			var sys = python.import('sys');
			var fs = require('fs');
			var path=require("path");
			time.sleep(0.5);
			mmap.WriteSync(id,",10loading libraries 2");
			//var dirpython = path.dirname(fs.realpathSync(__filename));
			//sys.path.append(dirpython);
			//sys.path.append(dirpython+'/peakssource');
			//sys.path.append(dirpython+'/bin_modules');
			//sys.path.append(dirpython+'/Python/lib');
			//sys.path.append(dirpython+'/Python/lib/python2.7');
			//sys.path.append(dirpython+'/Python/lib/python2.7/site-packages');
			//sys.path.append(dirpython+'/Python/lib/python2.7/site-packages/numpy');
			time.sleep(1.1);
			mmap.WriteSync(id,",10loading libraries 3");
			time.sleep(0.2);
			var datasourcesmanager=require("./listpeaksproc");
			var datasources=datasourcesmanager.datasources();
			var connectdatasourcescmdlinepeaks="";
			for(var i=0;i<datasources.length;i++){
				connectdatasourcescmdlinepeaks=connectdatasourcescmdlinepeaks+" "+datasources[i][0];
			}
			console.log(connectdatasourcescmdlinepeaks);
			//const execb = require("child_process").exec;
			os.system("bash -c \"cd peakssource;LD_LIBRARY_PATH='../bin_modules/pygtkdatabox/gtkdatabox/install/lib' ./main"+connectdatasourcescmdlinepeaks+" "+cmdargs+"\"");
			console.log("Peaks util window closed (main terminated)");
			process.exit();
			//var mainmodule=python.import("peaks");
			//mainmodule.mainuiload(""+cmdargs);
		}else{
			//var peaksproccessingmodule=require('./peaksproccesingmodule');
			process.exit();
		}
