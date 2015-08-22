#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <map>
using namespace std;
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;
#include "param.hpp"


#define DUMP(x) \
cout << #x << ":=<" << x << ">" << endl;





class FaceFrameCutter
{
public:
	FaceFrameCutter(const string &haar,const string &output);
	void init();
	void cut();
private:
	void cut(int video);
private:
	string haar_;
	string outputPitures_;
	cv::CascadeClassifier faceCascade_;
	ofstream train_;
	ofstream val_;
	int label_ = 0;
	int frameCounter_ = 0;
};


int main(int argc, char *argv[])
{
	FaceFrameCutter cutter(cStrFaceCascadeName,"face.classify.png");
	cutter.init();
	cutter.cut();
	return 0;
}



FaceFrameCutter::FaceFrameCutter(const string &haar,const string &output)
:haar_(haar)
,outputPitures_(output)
{
	
}
void FaceFrameCutter::init()
{
	if (!faceCascade_.load(haar_))
    {
    	std::cerr<<"--(!)Error loading"<<std::endl;
    	exit(-1);
    };
	{
		string cmd("rm -rf ");
		cmd += outputPitures_;
		system( cmd.c_str());
	}
}


#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
namespace fs = boost::filesystem;

void FaceFrameCutter::cut()
{
	cut(0);
}

void FaceFrameCutter::cut(int video)
{
	try
	{
		cv::VideoCapture cap(video);
		while(true)
		{
			cv::Mat frame,grayFrame;
			cap >> frame;
			DUMP(frame.channels());
			cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);
			// Detect faces
			vector<cv::Rect> faces;
		    faceCascade_.detectMultiScale(grayFrame, faces,scaleFactorFace,minNeighborsFace,flagsFace,minSizeFace,maxSizeFace);
			auto faceNum = faces.size();
			if(1 == faceNum)
			{
				auto face = faces.front();
				cv::Point pt1(face.x, face.y);
		        cv::Point pt2((face.x + face.height), (face.y + face.width));
				cv::Mat faceFrame(frame,face);
				cv::Mat dstFrame(256,256,faceFrame.type());
				cv::resize(faceFrame,dstFrame,cv::Size(256,256));
				string image_path(outputPitures_);
				DUMP(outputPitures_);
				cv::imwrite(outputPitures_, dstFrame);
				break;
			}
			else
			{
				
			}
		}
	}
	catch(cv::Exception e)
	{
		DUMP(e.what());
	}
}


