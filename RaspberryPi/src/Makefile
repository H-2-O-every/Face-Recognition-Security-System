obj-m = gpio_module.o

KDIR:=/lib/modules/$(shell uname -r)/build
PWD:=$(shell pwd)

IP:=155.230.14.90
PORT:=3390

modbuild:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

socket_clnt.o : socket_clnt.cpp
	g++ -c socket_clnt.cpp

main.o : main.cpp
	g++ -c main.cpp

all : main.o socket_clnt.o
	g++ -o socket_clnt socket_clnt.o main.o
	gcc -o gpio gpio.c

modins :
	sudo insmod gpio_module.ko

nodmk :
	sudo mknod /dev/LED_IRQ c 200 0
	sudo chmod 666 /dev/LED_IRQ


run :
	./gpio&
	./socket_clnt $(IP) $(PORT) 

cleansock :
	rm -rf *.o socket_clnt gpio

cleanmod:
	rm -rf *.o *.ko *.mod.c .led.* Module.symvers modules.order *.mod




