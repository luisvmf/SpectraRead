all: $(OBJS)
	cd polfit-gui&&./build.sh
	cd polfit-hs&&./build.sh
	cd FastMmapMQ&&./build-node.sh
	cd FastMmapMQ&&./build-python.sh
	cd pygtkdatabox&&./build.sh
clean:
	rm polfit.node
	rm polfiths.node
	rm fastmmapmq.node
	rm fastmmap.so
