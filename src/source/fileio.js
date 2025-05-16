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
	var fs = require('fs');
	function savesinglespectrum(data,speclocation){
		fs.writeFileSync(""+speclocation,""+data);
		console.log("save"+speclocation);
	}
	function savesinglepeaks(data,peakslocation){
		fs.writeFileSync(""+peakslocation,""+data);
	}
	function savesinglespectrumpeaks(data,datab,speclocation,peakslocation){
		if(fs.existsSync(""+speclocation+".dat")){
				throw new Error("File "+""+speclocation+".dat"+" already exists.");
				return;
		}
		if(fs.existsSync(""+peakslocation+".dat")){
				throw new Error("File "+""+peakslocation+".dat"+" already exists.");
				return;
		}
		fs.writeFileSync(""+speclocation+".dat",""+data);
		fs.writeFileSync(""+peakslocation+".dat",""+datab);
	}
	//function savemultiplespectra(data,speclocation,spin1,spin2){
	//	console.log("Saving single spectrum on: "+speclocation);
	//}
	//function savemultiplepeaks(data,peakslocation,spin1,spin2){
	//	console.log("Saving single peaks on: "+peakslocation);
	//}
	//function savemultiplespectrapeaks(data,speclocation,peakslocation,spin1,spin2){
	//	console.log("Saving single spectrum on: "+speclocation+" and peaks on: "+peakslocation);
	//}
	module.exports={savesinglespectrum,savesinglepeaks,savesinglespectrumpeaks}
