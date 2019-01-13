var pf=require("./polfit");
pf.connect();
while(true){
	//void polfit(order,boxcar,risingthreshold,timerun)
	pf.polfit(0,0,0.66,1);
}
