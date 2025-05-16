{  
	"target_defaults": {
    "cflags": ["-O3","-fpermissive"],
    "default_configuration": "Release",
    "defines": [],
    "include_dirs": [],
    "libraries": []
  },
 "targets": [ 
   { 
     "target_name": "polfit",
     "sources": [ "pf.cc" ],
     "include_dirs": [ "<!(../../spectrareadd -e \"require('nan')\")" ]
   } 
 ]
}
