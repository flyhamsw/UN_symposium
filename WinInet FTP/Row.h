#pragma once

using namespace std;

//GPS Data
class RowGPGGA
{
public:
	char* time;
	char* lat;
	char* latHeading;
	char* lng;
	char* lngHeading;
	char* gpsQuality;
	char* numOfSV;
	char* HDOP;
	char* alt;
	char* altUnit;
	char* heightWGS84;
	char* heightWGS84Unit;
	char* DGPS;
	char* checksum;
	RowGPGGA(char*, char*, char*, char*, char*, char*, char*, char*, char*, char*, char*, char*, char*, char*);
};

//INS Data
class RowPASHR
{
public:
	char* time;
	char* heading;
	char* headingTrue;
	char* roll;
	char* pitch;
	char* heave;
	char* rollAccuracy;
	char* pitchAccuracy;
	char* headingAccuracy;
	char* aidingStatus;
	char* IMUStatus;
	RowPASHR(char*, char*, char*, char*, char*, char*, char*, char*, char*, char*, char*);
};

//GPS Heading Data
class RowGPHDT
{
public:
	char* heading;
	char* headingTrue;
	char* checksum;
	RowGPHDT(char* txt_heading, char* txt_headingTrue, char* txt_checksum);
};

//Event Data
class RowPTNL
{
public:
	char* type;
	char* time;
	char* field4;
	char* field5;
	char* field6;
	char* field7;
	char* field8;
	RowPTNL(char*, char*, char*, char*, char*, char*, char*);
};

//Matched Data
class Row
{
public:
	RowGPGGA* rowGPGGA;
	RowPASHR* rowPASHR;
	RowGPHDT* rowGPHDT;
	Row(RowGPGGA*, RowPASHR*, RowGPHDT*);
};

//Interpolated Data
class RowInterpolated
{
public:
	double X;
	double Y;
	double Z;
	double heading;
	double roll;
	double pitch;
	RowInterpolated(double, double, double, double, double, double);
};
