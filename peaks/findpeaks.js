var fs = require('fs');

function derivada(vetorinternoderivadax,vetorinternoderivada){
	var contador1=0;
	var vetorretornaderivada=[];
	while(contador1<aux1.length-1){
		vetorretornaderivada[contador1]=(Number(vetorinternoderivada[contador1+1])-Number(vetorinternoderivada[contador1]))/(Number(vetorinternoderivadax[contador1+1])-Number(vetorinternoderivadax[contador1]));
		contador1=contador1+1;
	}
	vetorretornaderivada[contador1-1]=vetorretornaderivada[contador1-2];
	return vetorretornaderivada;
}
function boxcar1(vetorinternoboxcar){
	var contador1=1;
	while(contador1<vetorinternoboxcar.length-1){
		vetorinternoboxcar[contador1]=(Number(vetorinternoboxcar[contador1-1])+Number(vetorinternoboxcar[contador1])+Number(vetorinternoboxcar[contador1+1]))/3;
		contador1=contador1+1;
	}
	return vetorinternoboxcar;
}

function boxcar(vintboxcar,nintboxcar){
	var cnt_boxcar=0;
	while(cnt_boxcar<nintboxcar){
		vintboxcar=boxcar1(vintboxcar);
		cnt_boxcar=cnt_boxcar+1;
	}
	return vintboxcar;
}

function split_t(vetorinternosplit,posicaointernasplit){
	var contador1=0;
	var vetorretorna=[];
	while(contador1<aux1.length-1){
		vetorretorna[contador1]=(vetorinternosplit[contador1]).split("\t")[posicaointernasplit];
		contador1=contador1+1;
	}
	return vetorretorna;
}

function readasciispectrumfile(filename,encoding){
	var a=fs.readFileSync(""+filename, ""+encoding);
	if(a.indexOf("\r\n")+""!="-1"){
		var aux1=a.split("\r\n");
	}else{
		var aux1=a.split("\n");
	}
	return aux1;
}

function saveasciispectrumfile(file,vetorx,vetory){
	var cnt=0;
	var stringgrafico="";
	while(cnt<vetory.length){
		stringgrafico=stringgrafico+vetorx[cnt]+"\t"+vetory[cnt]+"\n";
		cnt=cnt+1;
	}
	fs.writeFileSync(""+file, ""+stringgrafico+"");
}


function findpeaks(vetorx,vetory,threshold){
	var vetorderivadaint=derivada(vetorx,vetory);
	var cnt=0;
	var cntb=0;
	var peaksint=[];
	var peaks=[];
	var inpeakregion=0;
	while(cnt<vetorderivadaint.length){
		if(vetorderivadaint[cnt]<0){
			if(vetorderivadaint[cnt+1]>0){
			peaksint[cntb]=vetorx[cnt];
			cntb=cntb+1;
			inpeakregion=1;
			//console.log("Inside peak region::.."+peaksint[Math.round((peaksint.length-1))]);
			//console.log("v1:"+vetorderivadaint[cnt]+" v2:"+vetorderivadaint[cnt+1]);
		}
		}
		if(vetorderivadaint[cnt]>0){
			if(vetorderivadaint[cnt+1]<0){
			peaksint[cntb]=vetorx[cnt];
			cntb=cntb+1;
			inpeakregion=1;
			//console.log("Inside peak region::.."+peaksint[Math.round((peaksint.length-1))]);
			//console.log("v1:"+vetorderivadaint[cnt]+" v2:"+vetorderivadaint[cnt+1]);
		}
		}
	//	else{
	//		if(inpeakregion==1){
	//			peaks[peaks.length]=peaksint[Math.round((peaksint.length-1)/2)];
	//		}
	//		inpeakregion=0;
	//		peaksint=[];
	//		cntb=0;
	//	}
		cnt=cnt+1;
	}
	return peaksint;
}
module.exports={derivada,boxcar,split_t,readasciispectrumfile,saveasciispectrumfile,findpeaks};
