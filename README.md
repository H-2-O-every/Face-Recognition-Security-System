# Face Recognition Security System for NVIDIA Jetson (Nano) & RaspberryPi using TensorRT
Face recognition with [Google FaceNet](https://arxiv.org/abs/1503.03832)
architecture and retrained model by David Sandberg
([github.com/davidsandberg/facenet](https://github.com/davidsandberg/facenet))
using TensorRT and OpenCV. <br> This project is based on the
implementation of l2norm helper functions which are needed in the output
layer of the FaceNet model. Link to the repo:
[github.com/r7vme/tensorrt_l2norm_helper](https://github.com/r7vme/tensorrt_l2norm_helper). <br>
Moreover, this project uses an adapted version of [PKUZHOU's implementation](https://github.com/PKUZHOU/MTCNN_FaceDetection_TensorRT)
of the mtCNN for face detection. More info below.

This Project's Jetson Nano source code based on the source of nwesem (https://github.com/nwesem/mtcnn_facenet_cpp_tensorRT)


## Hardware
* NVIDIA Jetson Nano
* Raspberry Pi 4
* Raspberry Pi v2 camera 
* Button for Raspberry Pi

## Please read README.md in all of JetsonNano, RaspberryPi directories. Each README file describes the flow of each device.

## Be Careful.
#### We used two Raspberry Pi for this project. If you want to use more Raspberry Pi, there is nothing to do you should. Just be careful Raspberry Pi's ID. ( If you want to know about the Raspberry Pi ID, read the README.md in Raspberry Pi Derectory. )

<br>
