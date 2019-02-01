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
var cmdargs="";
process.argv.forEach(function (val, index, array) {
	if(val.indexOf("spectrareadcurrentprocid:")!=-1){
		if(val.indexOf("spectrareadprocid")!=-1){
			cmdargs=val.split("spectrareadcurrentprocid:")[1].split("spectrareadprocid")[0];
		}
	}
});
var mmap=require("./fastmmapmq");
var pf=require("./polfiths");
function inithspeaksprocess(id,idb,createconnect,sleeptime){
	var datahspeaksb=mmap.ConnectMmapSync(process.cwd()+"/spectrareadd "+process.cwd()+"/main.js spectrareadcurrentprocid:"+cmdargs,"@"+cmdargs);
	while(datahspeaksb==-1){
		datahspeaksb=mmap.ConnectMmapSync(process.cwd()+"/spectrareadd "+process.cwd()+"/main.js spectrareadcurrentprocid:"+cmdargs,"@"+cmdargs);
		console.log("Reconnecting on hspeaks.");
		pf.sleep(0.1);
	}
	pf.sleep(sleeptime);
	pf.connect(id+""+cmdargs,idb+""+cmdargs,createconnect);
	while(true){
		datapeaks=mmap.GetSharedStringSync(datahspeaksb);
		if(datapeaks.indexOf(",")!=-1){
			try{
				datapeaks=eval(datapeaks.split("--++")[0]);
				if(datapeaks[3]+""=="True"){
					a=pf.polfit(datapeaks[0],datapeaks[2],datapeaks[1],1);
					if(a<300){
						pf.sleep(0.02);
					}
				}else{
					pf.sleep(0.1);
				}
			}catch(e){console.log(e);}
		}else{
			pf.sleep(0.1);
		}
	}
}
module.exports={inithspeaksprocess};
