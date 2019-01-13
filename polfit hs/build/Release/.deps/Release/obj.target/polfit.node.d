cmd_Release/obj.target/polfit.node := g++ -shared -pthread -rdynamic -m64  -Wl,-soname=polfit.node -o Release/obj.target/polfit.node -Wl,--start-group Release/obj.target/polfit/pf.o -Wl,--end-group 
