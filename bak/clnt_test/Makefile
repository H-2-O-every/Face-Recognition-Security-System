socket_clnt.o : socket_clnt.cpp
	g++ -c socket_clnt.cpp

main.o : main.cpp
	g++ -c main.cpp

all : main.o socket_clnt.o
	g++ -o socket_clnt socket_clnt.o main.o

clean :
	rm -rf *.o socket_clnt

client :
	./socket_clnt 155.230.14.90 3390
