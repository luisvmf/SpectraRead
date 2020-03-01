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
		var datasourcesmanager=require("./listpeaksproc");
		fastmmapmq=datasourcesmanager.fastmmapmq;
		var pf=require("./bin_modules/polfiths.node");
		peaksipcint=fastmmapmq.CreateMmapSync("peaksipc","rwx------")
		var aaa=0;
		var j=0;
		var timectn=0;
		var kk=0;
		var stm=0;
		var datasendfinal="";
		while(true){
			datasources=datasourcesmanager.datasources();
			var data="";
			datasendfinal="";
			predatavarvb="";
			for(var i=0;i<datasources.length;i++){
				try{
					predatavarv="";
					datarawreadmmap=fastmmapmq.ReadSync(datasources[i][1],0).split("\n");
					stm=stm+1;
					datarawreadmmap.splice(datarawreadmmap.length-1,1);
					for(var l=0;l<datarawreadmmap.length;l++){
						predatavarvb="";
						datarawreadmmapb=datarawreadmmap[l];
						datarawreadmmapb=datarawreadmmapb.split(" ");
						datarawreadmmapb.splice(0,1);
						timestamp=datarawreadmmapb[0];
						datarawreadmmapb.splice(0,1);
						if(timestamp!=""){
							if(timestamp!=" "){
								if(Number(timestamp)>1000.0){
									predatavarv=predatavarv.slice(0, predatavarv.length-1);
									predatavarv=predatavarv+"|"+datasources[i][0]+""+i+":";
									predatavarvb=predatavarvb.slice(0, predatavarvb.length-1);
									predatavarvb=predatavarvb+"|"+datasources[i][0]+""+i+":";
									for(var k=0;k<datarawreadmmapb.length;k++){
										datarawreadmmapc=datarawreadmmapb[k];
										if(datarawreadmmapc!=""){
											if(datarawreadmmapc!=" "){
												predatavarv=predatavarv+""+datarawreadmmapc+";"+timestamp+" ";
												predatavarvb=predatavarvb+""+datarawreadmmapc+";"+timestamp+" ";
											}
										}
									}
								}
							}
						}
					}
					predatavarv=predatavarv.slice(0, predatavarv.length-1);
					predatavarv=predatavarv+",";
					predatavarvb=predatavarvb.slice(0, predatavarvb.length-1);
					predatavarvb=predatavarvb+",";
					if(predatavarv!=","){
						//console.log(predatavarv);
						fastmmapmq.WriteSync(peaksipcint,predatavarv)
						datasendfinal=datasendfinal+predatavarvb;
					}
				}catch(e){console.log(e);}
			}
			pf.sleep(0.003);
			if(datasendfinal!=""){
				//console.log(datasendfinal);
				fastmmapmq.WriteSharedStringSync(peaksipcint,datasendfinal)
			}
		}
