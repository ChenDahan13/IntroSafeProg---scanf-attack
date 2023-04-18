all: .so.6 server secret

.so.6: newlibrary.c
	gcc --shared -fPIC newlibrary.c -o .so.6

server: server.c
	gcc server.c -o server

secret: secret.c
	gcc secret.c -o secret

clean:
	rm server .so.6 secret 