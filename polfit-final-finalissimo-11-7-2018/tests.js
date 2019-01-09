var polfit=require("./polfit.node");
var fs=require("fs");
var i=0;
var j=0;
	//                 x,                     y,          baseline,boxcar,id,risingthreshold
	polfit.process("1 2 3 4 5 6 7 8 ","4 4 9 29 25 9 4 4 ",1,0,i,0.5,function(err,spec,peaks,l){
		k=0;
		//polfit.returned(Math.random());
		savebuffer="";
		while(k<spec.length){
			savebuffer=savebuffer+spec[k]+"\n";
		k=k+1;
		}
		//fs.writeFile("test"+k+".dat",savebuffer,function(err){});
		console.log(","+savebuffer+",");
		console.log(peaks);
	});
