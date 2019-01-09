var mmapmq=require("./fastmmapmq.node");
var id=mmapmq.ConnectMmapSync("node","peaks1");
while(true){
	var a=mmapmq.ReadSync(id,0);
	if(a!=""){
		if(a!=" "){
			console.log(a);
		}
	}
}
