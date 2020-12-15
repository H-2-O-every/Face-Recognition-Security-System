all: web.o main.o
	g++ -o web web.o main.o
	
web.o: web.cpp
	g++ -c web.cpp

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm *.o web
