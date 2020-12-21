#include "thread.h"

thread_data::thread_data(){

	rasp_id = 0;
	serv_sock = clnt_sock = 0;
	clnt_addr_size = 0;
	port = 0;
	videoFrameWidth = videoFrameHeight = 0;
}


void thread_dataInit(thread_data_t *data, int rasp_id, int videoFrameWidth, int videoFrameHeight, FaceNetClassifier &faceNet){
	data->rasp_id = rasp_id;
	data->videoFrameWidth = videoFrameWidth;
	data->videoFrameHeight = videoFrameHeight;
	data->port = PORT + rasp_id;
	data->faceNet = faceNet;
}

void* thread_running(void *argu){

	thread_data_t *data = (thread_data_t *)argu;
	mtcnn mtCNN(data->videoFrameHeight, data->videoFrameWidth);	

	printf("%d RaspberryPi Receive\n", data->rasp_id);

	printf("Socket Init\n");
	socketInit(&(data->serv_sock), &(data->sck_addr), data->port);
	printf("Socket Bind and Listen\n");
	bind_and_listen(data->serv_sock, data->sck_addr);
	printf("Port = %d\n", data->port);
	printf("Socket Accepting...\n");

	client_socket_process( &(data->clnt_sock), data->serv_sock, &(data->clnt_addr_size), &(data->clnt_addr) );
	cv::Mat frame;
	std::vector<struct Bbox> outputBbox;
	outputBbox.reserve(5);

	while(1){
		socket_process(data->clnt_sock,data->serv_sock, "../receive_imgs" , data->framefilename);

		printf("-------------------------------\n");
			
		std::string filenamestring(data->framefilename);
		printf("Receive Image loadInputImage\n");	
		loadInputImage(filenamestring, frame, data->videoFrameWidth, data->videoFrameHeight);

		if (frame.empty()){
			std::cout << "Empty frame! Exiting...\n Try restarting nvargus-daemon by "  << std::endl;
			//break;
			continue;
		}


		int returnValue = 0;
		char web_name[STRING_LENGTH];
		char sz_data[STRING_LENGTH];
		printf("Find Face\n");
		outputBbox = mtCNN.findFace(frame);

		data->faceNet.forward(frame, outputBbox);
		printf("Feature Matching\n");
		(data->faceNet.featureMatching(frame, web_name));
		data->faceNet.resetVariables();
		
		//strcpy(web_name, name.c_str());
		webdata(web_name, data->framefilename, sz_data);
		if( !(strcmp(web_name, "New Person") == 0 || strcmp(web_name, "Not Recognized") == 0) ){
			webupdate(sz_data);
			returnValue = 1;
		}
		write(data->clnt_sock, &returnValue, sizeof(int));		

				
		//pthread_mutex_lock(&mutex_lock);
		cv::imshow("VideoSource", frame);
		//nbFrames++;
		//pthread_mutex_unlock(&mutex_lock);
		
		char keyboard = cv::waitKey(0);
		if (keyboard == 'q' || keyboard == 27){
			printf("Stop the RaspberryPi %d\n", data->rasp_id);
			outputBbox.clear();
			frame.release();

			cv::destroyAllWindows();
			break;
		}
		else if(keyboard == 'a') {
			printf("We will add this person\n");
			outputBbox = mtCNN.findFace(frame);
			printf("Add the face\n");
			//cv::imshow("We will add this person", frame);
			data->faceNet.addNewFace(frame, outputBbox);
			outputBbox.clear();
			frame.release();
			cv::destroyAllWindows();
			continue;
		}
		else if(keyboard == 'n'){
			cv::destroyAllWindows();
			outputBbox.clear();
			frame.release();
			continue;
		}
		else{
			cv::destroyAllWindows();
			outputBbox.clear();
			frame.release();
			break;
		}
	}
	close(data->clnt_sock);
	close(data->serv_sock);
	return NULL;
}
