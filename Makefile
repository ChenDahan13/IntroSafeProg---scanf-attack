all: newlibrary server secret

newlibrary: newlibrary.c
	gcc --shared -fPIC newlibrary.c -o newlibrary 

server: server.c
	gcc server.c -o server

secret: secret.c
	gcc secret.c -o secret

clean:
	rm server newlibrary secret 