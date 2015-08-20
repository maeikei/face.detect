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
#include <opencv2/ocl/ocl.hpp>
using namespace cv;
#include "param.hpp"


#define DUMP(x) \
cout << #x << ":=<" << x << ">" << endl;



const static string cStrFaceCascadeName = 
 "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml";
static int faceSize = 256;
static bool faceGray = false;
static int constSkipFrontFaceCounter = 24*5;
static int constTotalFaceCounter = 24*60;



void faceCutFrame(const cv::CascadeClassifier &classifier,const string &path);

class FaceFrameCutter
{
public:
	FaceFrameCutter(const string &haar,const string &videoPath,const string &output);
	void init();
	void cut();
private:
	void cut(const string &video);
private:
	string haar_;
	string inputVideos_;
	string outputPitures_;
	cv::ocl::OclCascadeClassifier faceCascade_;
	ofstream train_;
	ofstream val_;
	int label_ = 0;
	int frameCounter_ = 0;
};


int main(int argc, char *argv[])
{
	FaceFrameCutter cutter(cStrFaceCascadeName,"data","output");
	cutter.init();
	cutter.cut();
	return 0;
}



FaceFrameCutter::FaceFrameCutter(const string &haar,const string &input,const string &output)
:haar_(haar)
,inputVideos_(input)
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
	{
		string cmd("mkdir -p ");
		cmd += outputPitures_;
		system( cmd.c_str());
	}
	{
		string cmd("mkdir -p ");
		cmd += outputPitures_;
		cmd += "/train";
		system( cmd.c_str());
	}
	{
		string cmd("mkdir -p ");
		cmd += outputPitures_;
		cmd += "/val";
		system( cmd.c_str());
	}
	{
		string path(outputPitures_);
		path += "/train.txt";
		train_.open(path,ofstream::out);
	}
	{
		string path(outputPitures_);
		path += "/val.txt";
		val_.open(path,ofstream::out);
	}
}


#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
namespace fs = boost::filesystem;

void FaceFrameCutter::cut()
{
	const fs::path path(inputVideos_);
	BOOST_FOREACH(const fs::path& p, std::make_pair(fs::recursive_directory_iterator(path),fs::recursive_directory_iterator()))
	{
		if (!fs::is_directory(p))
		{
			DUMP(p.extension().string());
			if(".avi" == p.extension().string() )
			{
				DUMP(p);
				frameCounter_ = 0;
				cut(p.string());
				label_++;
			}
		}
	}
	train_.close();
	val_.close();
}

const static cv::Rect constZeroFace(0,0,0,0);

bool isNearFace(const cv::Rect &a,const cv::Rect &b)
{
	auto distance = pow(a.x -  b.x + (a.width - b.width)/2 ,2);
	distance += pow(a.y -  b.y + (a.height - b.height)/2 ,2);
	if(distance > 25)
	{
		return false;
	}
	return true;
}
void FaceFrameCutter::cut(const string &video)
{
	const fs::path path(video);
	try
	{
		cv::VideoCapture cap(video);
		cv::Rect prevFace(0,0,0,0);
		while(true)
//		for(int iFrame =0 ;iFrame < 120;iFrame++)
		{
			cv::Mat frame,grayFrame;
			cap >> frame;
			if(frame.empty())
			{
				break;
			}
			cv::ocl::oclMat oclFrame;
			oclFrame.upload(frame);
			//cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);
			// Detect faces
			vector<cv::Rect> faces;
		    faceCascade_.detectMultiScale(oclFrame, faces,scaleFactorFace,minNeighborsFace,flagsFace,minSizeFace,maxSizeFace);
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
				
				std::ostringstream sout;
				sout << std::setfill('0') << std::setw(6) << ++frameCounter_;
				
				if(constSkipFrontFaceCounter > frameCounter_)
				{
					cout << "skip first face" << endl;
					continue;
				}
				if(constTotalFaceCounter < frameCounter_)
				{
					cout << "finnish face cut" << endl;
					break;
				}
				if(frameCounter_%24)
				{
					image_path += "/train/";
					train_ << path.stem().string() << "." << sout.str() << ".png" << " " << label_ << endl;
				}
				else
				{
					image_path += "/val/";
					val_ << path.stem().string() << "." << sout.str() << ".png" << " " << label_ << endl;
				}
				
				image_path += path.stem().string();
				image_path += ".";
				image_path += sout.str();
				image_path += ".png";
	
				DUMP(image_path);
				cv::imwrite(image_path, dstFrame);
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


