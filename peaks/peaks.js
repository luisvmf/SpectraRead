var findpeaks=require("./findpeaks");
function removebaseline(ydata,order){
	return polfitremovebaseline(ydata+"",order)
}
var derivada=findpeaks.derivada;
var boxcar=findpeaks.boxcar;
var split_t=findpeaks.split_t;
var readasciispectrumfile=findpeaks.readasciispectrumfile;
var saveasciispectrumfile=findpeaks.saveasciispectrumfile;
var find_peaks=findpeaks.findpeaks;
module.exports={removebaseline,derivada,boxcar,split_t,readasciispectrumfile,saveasciispectrumfile};
