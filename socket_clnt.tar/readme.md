실행 방법
1.  tar파일을 압축 해제. 디렉터리명은 반드시 socket_clnt로, 위치는 /home/pi/socket_clnt 로 지정할 것. 부팅시 자동으로 모듈등록과 노드 생성이 이뤄지는데 이때 절대경로를 사용하기 때문에 반드시 지킬 것.
    socket_clnt 디렉터리에서
2.  make : 모듈 빌드
3.  make modins : 모듈 탑재
4.  make nodmk : 노드 생성, 권한 변경 //2~4번은 이미 모듈 빌드가 끝나있다면 할 필요 없음. 부팅 시 자동으로 모듈 등록과 노드 생성이 이뤄짐.
5.  make all : client 코드 컴파일
6.  make gpio : gpio.c 코드 컴파일
7.  ./gpio : gpio 프로그램 실행 => 백그라운드로 전환하거나, 다른 터미널 열고 이 디렉터리로 들어올 것.
8.  make client : client 프로그램 실행. 클라이언트 실행 전에 반드시 서버를 실행시켜서 accept준비를 마칠 것.
9.  버튼(gpio 12 pin) 입력 시 인터럽트 발생. gpio 프로세스는 이 인터럽트를 감지하고 IRflag라는 파일에 '1'을 작성. 그리고 3초간 sleep. 후에 다시 '0'으로 수정.
10. socket_clnt 프로세스는 IRflag라는 파일을 반복해서 fopen, fscanf, fclose를 실행. 읽어들인 값이 '1'인 경우 버튼 입력을 인식, IRflag파일을 '0'으로 수정. 캡쳐와 이미지 전송을 수행.
%%%%9번과 10번 항목은 main 코드에서 file_operations의 멤버인 read, write함수를 인식하지 못하는 문제를 해결하지 못했기 때문에, 버튼 인식 프로세스와 소켓/캡쳐 프로세스를 분리하고, 그 통신을 파일 입출력을 통해 수행하기 위한 부분임.
11. 이미지 파일을 전송한 클라이언트는 retVal이라는 값을 서버로부터 전송받기를 기다림. 1이면 등록된 사람, 0이면 얼굴을 인식하지 못했거나 등록되지 않은 사람.
12. 이후 9.~11.을 무한루프로 진행.

# Face Recognition for Raspberry Pi 4 

## Hardware
* Raspberry Pi 4
* Raspberry Pi v2 camera 

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

#### 3. Build module
First, you 
