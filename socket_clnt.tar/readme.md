# Face Recognition for Raspberry Pi 4 

## Hardware
* Raspberry Pi 4
* Raspberry Pi v2 camera 
* Button for Raspberry Pi

## Dependencies
imagemagick

## Update
This program now uses linux kernel 5.4.72-v71+. if you want using another version, you should install the kernel version matched.

## Installation
#### 1. Install imagemagick

This package need to use "convert" command. This command is used for resizing an image file to increasing transmission performance.
   
```bash
sudo apt-get install imagemagick
```

#### 2. Kernel build 
```bash
#update
pi@raspberrypi:~ $ sudo apt-get update
pi@raspberrypi:~ $ sudo apt-get upgrade

#Install depandent package
pi@raspberrypi:~ $ sudo apt install git bc bison flex libssl-dev make

#Download raspberry pi kernel source
pi@raspberrypi:~ $ git clone --depth=1 https://github.com/raspberrypi/linux

#kernel configuration
pi@raspberrypi:~ $ cd linux/
#for rpi4
pi@raspberrypi:/~linux $ KERNEL=kernel7l
pi@raspberrypi:/~linux $ make bcm2711_deconfig
#for rpi2,3,3+
KERNEL=kernel7
make bcm2709_deconfig
#for rpi 1, Zero, Zero W
KERNEL=kernel
make bcmrpi_deconfig

#build(It take very long time)
pi@raspberrypi:/~linux $ make -j4 zImage modules dtbs
pi@raspberrypi:/~linux $ sudo make modules_install

#linking kernel source
pi@raspberrypi:~ $ uname -r
5.4.72-v7l+
pi@raspberrypi:~ $ sudo ln -s ~/linux /lib/modules/5.4.72-v7l+/build
```

#### 3. Build module, compile code
First, decompression the .tar file, and go in to the directory.

```bash
#Build gpio_module.c
make modbuild
#compile socket_clnt, gpio
make all
#insert module
make modins
#make node and change mode
make nodmk
```

#### 4. Running
Switch should be linked with gpio pin 12. Please refer to raspberry pi pin map.
```bash
make run
```
The server must be running before client be runned. If not, client process will stop.
