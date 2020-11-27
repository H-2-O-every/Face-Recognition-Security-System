CLNT_SRC = hello_client.c
CLNT_OBJ = $(CLNT_SRC:.c=.o)
CLNT = $(CLNT_SRC:.c=)

help:
	@echo "make all"
	@echo "make clean"
	@echo "make client"

$(CLNT_OBJ): $(CLNT_SRC)
	gcc -c $^

$(CLNT): $(CLNT_OBJ)
	gcc -o $@ $^

all: $(CLNT)

client:
	./$(CLNT) 127.0.0.1 3390

clean:
	rm -rf *.o $(CLNT)
