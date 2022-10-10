{
  "targets": [
    {
      "target_name": "binding",
      "sources": [
        "src/binding.cc",
        "src/utils.cc",
        "src/py_object_wrapper.cc"
      ],
      "conditions": [
        ['OS=="mac"', {
            "xcode_settings": {
              "OTHER_CFLAGS": [
                "-Wno-error=unused-command-line-argument",
                "<!(python-config --cflags)"
              ],
              "OTHER_LDFLAGS": [
                "<!(python-config --ldflags)"
              ]
            }
        },
        'OS=="win"',{ # win
            'include_dirs':[
                "C:\\python27\\include"
            ],
            'link_settings':{
                'libraries':[
                    "C:\\python27\\libs\\python27.lib"
                ]
            }
        },
        { # linux
          "cflags": [
            "-I../../../Python/include/python2.7 -I../../../Python/include/python2.7 -fno-strict-aliasing -g -O2 -DNDEBUG -g -fwrapv -O3 -Wall -Wstrict-prototypes"
          ],
          "libraries": [
            "-L/home/luisvmf/Desktop/spectraread/SpectraRead/source/Python/lib/libpython2.7.so.1.0 -lpthread -ldl  -lutil -lm"
          ]
        }]
      ]
    }
  ]
}
