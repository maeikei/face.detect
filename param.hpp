#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

static const double scaleFactorFace = 1.1;
static const int minNeighborsFace = 3;
static const int flagsFace = 0;
static const cv::Size minSizeFace(128,128);
static const cv::Size maxSizeFace(512,512);
