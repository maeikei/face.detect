#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

static const double scaleFactorFace = 1.1;
static const int minNeighborsFace = 3;
static const int flagsFace = 0;
static const cv::Size minSizeFace(128,128);
static const cv::Size maxSizeFace(512,512);
static const int iConstPad = 40;
#if __APPLE__
const static string cStrFaceCascadeName =
"/usr/local/share/OpenCV/haarcascades/haarcascade_frontalface_alt.xml";
#else
const static string cStrFaceCascadeName =
"/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml";
#endif
