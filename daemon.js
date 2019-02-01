var fs=require("fs");
var path = require('path');
var cmdargs="";
process.argv.forEach(function (val, index, array) {
	if(val.indexOf("spectrareadcurrentprocid:")!=-1){
		if(val.indexOf("spectrareadprocid")!=-1){
			cmdargs=val.split("spectrareadcurrentprocid:")[1].split("spectrareadprocid")[0];
		}
	}
});
var python = require('python.node');
var time=python.import("time");
var os = python.import('os');
var sys = python.import('sys');
var dirpython = path.dirname(fs.realpathSync(__filename));
sys.path.append(dirpython);
sys.path.append(dirpython+'/spectrometer_modules');
sys.path.append(dirpython+'/Python/lib');
sys.path.append(dirpython+'/Python/lib/python2.7');
sys.path.append(dirpython+'/Python/lib/python2.7/site-packages');
sys.path.append(dirpython+'/Python/lib/python2.7/site-packages/numpy');
var daemonpy=python.import("deviceinterface");
daemonpy.connectm(cmdargs+"");
time.sleep(0.3)
daemonpy.initp();
