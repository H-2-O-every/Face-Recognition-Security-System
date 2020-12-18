#include <stdio.h>
#include <pthread.h>
#include <iostream>
#include <string>
#include <chrono>
#include <NvInfer.h>
#include <NvInferPlugin.h>
#include <l2norm_helper.h>
#include <opencv2/highgui.hpp>
#include <stdlib.h>
#include "mtcnn.h"
#include "faceNet.h"
#include "network.h"
#include "socket.h"
#include "web.h"

using namespace nvinfer1;
using namespace nvuffparser;


#define MAX_CLIENT 2

//pthread_mutex_t mutex_lock;

typedef struct thread_data{
		thread_data();
		thread_data(int) = delete;
		int rasp_id;
		int serv_sock;
		int clnt_sock;
		struct sockaddr_in sck_addr, clnt_addr;
		socklen_t clnt_addr_size;
		char framefilename[MAX_FILENAME*2];
		int port;
		int videoFrameWidth;
		int videoFrameHeight;
		FaceNetClassifier faceNet = FaceNetClassifier(Logger(), DataType::kHALF, "../facenetModels/facenet.uff", "../facenetModels/facenet.engine", 1, true, 1., 5, 640, 480);
}thread_data_t;

void thread_dataInit(thread_data_t *data, int rasp_id, int videoFrameWidth, int videoFrameHeight, FaceNetClassifier facenet);
void* thread_running(void *argu);


