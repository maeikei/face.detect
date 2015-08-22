#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <mutex>
using namespace std;
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;
#include "param.hpp"


#define DUMP(x) \
cout << #x << ":=<" << x << ">" << endl;




static atomic<bool> captureFaceVideoBegin(false);
static atomic<bool> captureFaceVideoEnd(true);

static mutex faceDetectMtx;
static vector<cv::Mat> faceDetectQue;

void doDetectFace(void);
int main(int argc, char *argv[])
{
	thread tDetectFace(doDetectFace);
	try
	{
		cv::VideoCapture cap(1);
		if (!cap.isOpened()){
			perror("!cap.isOpened()");
			return -1;
		}
#if 0
		cap.set(CV_CAP_PROP_FRAME_WIDTH,1920);
		cap.set(CV_CAP_PROP_FRAME_HEIGHT,1080);
		DUMP(cap.get(CV_CAP_PROP_FRAME_WIDTH));
		DUMP(cap.get(CV_CAP_PROP_FRAME_HEIGHT));
#endif
		while(true)
		{
			cv::Mat frame,grayFrame,faceFrame;
			cap >> frame;
			{
				lock_guard<mutex> lock(faceDetectMtx);
				if(faceDetectQue.empty())
				{
					faceDetectQue.push_back(frame.clone());
				}
			}
			if(captureFaceVideoBegin)
			{
				static int i = 0;
				string video_path("train.");
				video_path += to_string(i++);
				video_path += ".avi";
				cv::VideoWriter video(video_path,CV_FOURCC('D','I','V','X'),24, frame.size());
				if(false == video.isOpened())
				{
					break;
				}
				while(true)
				{
					cap >> frame;
					video << frame;
					{
						lock_guard<mutex> lock(faceDetectMtx);
						if(faceDetectQue.empty())
						{
							faceDetectQue.push_back(frame.clone());
						}
					}
					if(captureFaceVideoEnd)
					{
						break;
					}
				}
			}
		}
	}
	catch(cv::Exception e)
	{
		DUMP(e.what());
	}
	tDetectFace.join();
	return 0;
}


static const int iConstFaceDispareCounter = 15;

void doDetectFace(void)
{
	cv::CascadeClassifier faceCascade;
	if (!faceCascade.load(cStrFaceCascadeName))
    {
    	std::cerr<<"--(!)Error loading"<<std::endl;
    	exit(-1);
    };
	cv::Mat frame,grayFrame,faceFrame;
	while(true)
	{
		try
		{
			if(faceDetectQue.empty())
			{
				continue;
			}
			frame = faceDetectQue.front();
			cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);
			// Detect faces
			vector<cv::Rect> faces;
		    faceCascade.detectMultiScale(grayFrame, faces,scaleFactorFace,minNeighborsFace,flagsFace,minSizeFace,maxSizeFace);
			auto faceNum = faces.size();
			if(0 == faceNum)
			{
				static int noFaceCounter = 0;
				if(iConstFaceDispareCounter < noFaceCounter++ )
				{
					noFaceCounter = 0;
					if(true == captureFaceVideoBegin)
					{
						captureFaceVideoBegin = false;
					}
					if(false == captureFaceVideoEnd)
					{
						captureFaceVideoEnd = true;
					}
				}
			}
			else if(1 == faceNum)
			{
				if(false == captureFaceVideoBegin)
				{
					captureFaceVideoBegin = true;
				}
				if(true == captureFaceVideoEnd)
				{
					captureFaceVideoEnd = false;
				}
			}
			else
			{
				
			}
			faceDetectQue.clear();
		}
		catch(cv::Exception e)
		{
			DUMP(e.what());
		}
	}
}


