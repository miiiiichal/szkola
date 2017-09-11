CC=gcc

all: srv client

#
#srv_lib.o: srv_lib.c
#	$(CC) -c -o srv_lib.o srv_lib.c

#srv_lib.so: srv_lib.o
#	$(CC) -shared -o srv_lib.so srv_lib.o

#srv_test.o: srv_test.c
#	gcc -c -o srv_test.o srv_test.c

#srv: srv.o srv_lib.so
#	$(CC) -L/home/mj/priv_workspace/NAUKA/Wstep_do_programowania/lab4 -Wall -o srv srv.o -lsrv_lib
#	$(CC) -o srv srv.o srv_lib.so

srv: 
	$(CC) -o srv_test srv_test.c

client:
	$(CC) -o client client.c
	
	
clean:
	-rm -rf *.o *.so
