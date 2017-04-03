#pragma once
#include <Windows.h>
#include <stdio.h>
#include <Windows.h>
#include <WinInet.h>
#include <string>

#include <stdio.h>
#include <Windows.h>
#include <WinInet.h>
#include <opencv2\highgui.hpp>

using namespace cv;

Mat A2R_RPY();
Mat R2A_OPK(Mat R_opk);
Mat SystemCalibration(double* EO);