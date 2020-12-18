obj-m = gpio_module.o

KDIR:=/lib/modules/$(shell uname -r)/build
PWD:=$(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

cleanmod:
	rm -rf *.o *.ko *.mod.c .led.* Module.symvers modules.order
socket_clnt.o : socket_clnt.cpp
	g++ -c socket_clnt.cpp

main.o : main.cpp
	g++ -c main.cpp

all : main.o socket_clnt.o
	g++ -o socket_clnt socket_clnt.o main.o

cleansock :
	rm -rf *.o socket_clnt

client :
	./socket_clnt 155.230.14.90 3390

