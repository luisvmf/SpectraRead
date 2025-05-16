{
	"target_defaults": {
    "cflags": ["-O3"],
    "default_configuration": "Release",
    "defines": [],
    "include_dirs": [],
    "libraries": []
  },
  "targets": [
    {
      "target_name": "polfit",
      "sources": [ "async.cc" ],
      "include_dirs" : ["<!(../../spectrareadd -e \"require('nan')\")"]
    }
  ],
}
