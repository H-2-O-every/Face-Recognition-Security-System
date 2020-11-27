SERV_SRC = hello_server.c
SERV_OBJ = $(SERV_SRC:.c=.o)
SERV = $(SERV_SRC:.c=)


help:
	@echo "make all"
	@echo "make clean"
	@echo "make server"

$(SERV_OBJ): $(SERV_SRC)
	gcc -c $^

$(SERV): $(SERV_OBJ)
	gcc -o $@ $^

all: $(SERV)

server:
	./$(SERV) 3390

clean:
	rm -rf *.o $(SERV)
