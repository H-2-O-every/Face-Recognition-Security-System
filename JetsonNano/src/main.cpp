#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include <NvInfer.h>
#include <NvInferPlugin.h>
#include <l2norm_helper.h>
#include <opencv2/highgui.hpp>
#include <stdlib.h>
#include "faceNet.h"
#include "network.h"
#include "mtcnn.h"
#include "socket.h"
#include "thread.h"


// Uncomment to print timings in milliseconds
// #define LOG_TIMES

using namespace nvinfer1;
using namespace nvuffparser;



int main()
{
	//pthread_mutex_init(&mutex_lock, NULL);
    Logger gLogger = Logger();
    // Register default TRT plugins (e.g. LRelu_TRT)
    if (!initLibNvInferPlugins(&gLogger, "")) { return 1; }

    // USER DEFINED VALUES
    const string uffFile="../facenetModels/facenet.uff";
    const string engineFile="../facenetModels/facenet.engine";
    DataType dtype = DataType::kHALF;
    //DataType dtype = DataType::kFLOAT;
    bool serializeEngine = true;
    int batchSize = 1;
    int nbFrames = 0;
    int videoFrameWidth = 640;
    int videoFrameHeight = 480;
    int maxFacesPerScene = 5;
    float knownPersonThreshold = 1.;

	
    // init facenet
    FaceNetClassifier faceNet = FaceNetClassifier(gLogger, dtype, uffFile, engineFile, batchSize, serializeEngine, knownPersonThreshold, maxFacesPerScene, videoFrameWidth, videoFrameHeight);
    


    
	// init mtCNN
    mtcnn mtCNN(videoFrameHeight, videoFrameWidth);

    //init Bbox and allocate memory for "maxFacesPerScene" faces per scene
    std::vector<struct Bbox> outputBbox;
    outputBbox.reserve(maxFacesPerScene);

    // get embeddings of known faces
    std::vector<struct Paths> paths;
    cv::Mat image;
    getFilePaths("../imgs", paths); // common
    for(int i=0; i < paths.size(); i++) {
        loadInputImage(paths[i].absPath, image, videoFrameWidth, videoFrameHeight); // common / image = imread(inputFilePath.c_str());
		outputBbox = mtCNN.findFace(image);
        std::size_t index = paths[i].fileName.find_last_of(".");
        std::string rawName = paths[i].fileName.substr(0,index);
        faceNet.forwardAddFace(image, outputBbox, rawName);
        faceNet.resetVariables();
    }
    outputBbox.clear();
	


	thread_data_t t_data[MAX_CLIENT];
	pthread_t nano_thread[MAX_CLIENT];
	if(nano_thread == NULL){
		printf("Nano Thread Null\n");
	}
	// thread data Initialize.
	for(int i=0; i<MAX_CLIENT; i++){
		thread_dataInit(&(t_data[i]), i, videoFrameWidth, videoFrameHeight, faceNet);
	}

	printf("Thread Running Start\n");
	for(int i=0; i<MAX_CLIENT; i++){
		pthread_create(&nano_thread[i], NULL, thread_running,(void*)&(t_data[i]));
	}
	for(int i=0; i<MAX_CLIENT; i++){
		pthread_join(nano_thread[i], NULL);
	}

    cv::destroyAllWindows();
	return 0;
}
