all: $(OBJS)
	gcc -o server server.c
	gcc -o client client.c
	gcc -o nstest nstest.c
	gcc -o run-suid run-suid.c
	gcc -o enterns enterns.c
	#cp client Runtime/bin/notify_send (build inside container)
