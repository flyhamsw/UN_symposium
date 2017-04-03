#include <stdio.h>
#include <Windows.h>
#include <WinInet.h>
#include <comdef.h>

#include <chrono>
#include <iostream>
#include <ctime>
#include <opencv2\highgui.hpp>

using namespace cv;

#pragma comment(lib,"wininet.lib")


void LogFile(char* filename, int st, char* st_hms, int et, char* et_hms, int pt)
{
	FILE* pFile = NULL;
	fopen_s(&pFile, "C:\\Log_file/TimeUpload.csv", "a+");
	fseek(pFile, 0, SEEK_SET);

	int nLine = 1;
	while (!feof(pFile))
	{
		if (fgetc(pFile) == 10) // 라인 끝에 도착 Line 증가
		{
			nLine++;
		}
	}
	fprintf(pFile, "%s,%.3f,'%s,%.3f,'%s,%.3f\n", filename, double(st*0.001), st_hms, double(et*0.001), et_hms, double((pt)*0.001));
	fclose(pFile);
}

void main() {

	char file_name[300][300];	
	const int timeWait = 500;
	int x;

	HINTERNET hInternet;
	HINTERNET hConnection;
	HANDLE find_file;

	/*const WCHAR* ftpIp = TEXT("211.106.171.49");
	const WCHAR* ftpId = TEXT("uosimage");
	const WCHAR* ftpPw = TEXT("gaia3dftp");*/

	/*const WCHAR* ftpIp = TEXT("192.168.0.11");
	const WCHAR* ftpId = TEXT("uosimage");
	const WCHAR* ftpPw = TEXT("gaia3dftp");*/
	
	const WCHAR* ftpIp = TEXT("lgs.mago3d.com");
	const WCHAR* ftpId = TEXT("ftpuser");
	const WCHAR* ftpPw = TEXT("ftpuser");

	WCHAR* TestName = TEXT("UOS");
	hInternet = InternetOpen(TestName, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);

	if (hInternet == NULL)
		return;

	hConnection = InternetConnect(hInternet,
		ftpIp,
		21,
		ftpId,
		ftpPw,
		INTERNET_SERVICE_FTP,
		0,
		0);

	if (hConnection == NULL) {
		InternetCloseHandle(hInternet);
		printf("InternetConnect Fail\n");
		return;
	}
	printf("InternetConnect Success\n");	

	if (!FtpSetCurrentDirectory(hConnection, L"/temp")) {
		InternetCloseHandle(hConnection);
		InternetCloseHandle(hInternet);
		return;
	}
	printf("FtpSetCurrentDirectory Success\n");

	char sttime_hms[30], edtime_hms[30];
	std::time_t tNow = std::time(NULL);
	std::tm tmHMS;
	localtime_s(&tmHMS, &tNow);
	tmHMS.tm_hour = 0;
	tmHMS.tm_min = 0;
	tmHMS.tm_sec = 0;

	std::time_t theDay000 = std::mktime(&tmHMS);

	for (int k = 0; k < 10000; k++)
	{
		SetCurrentDirectoryA("C://uav_image/Result/Temp");
		WIN32_FIND_DATA find_data;
		x = 0;		
		find_file = ::FindFirstFile(TEXT("*.png"), &find_data);
		if (find_file != INVALID_HANDLE_VALUE) {			
			do {				
				if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY); // "탐색된 대상이 디렉토리(폴더)인 경우"
				else
				{
					_bstr_t b(find_data.cFileName);//반환형 변환
					const char* c = b;
					printf("%s \n", c);

					strcpy_s(file_name[x], c);
					printf("%s \n", file_name[x]);
					x++;
				};	// 탐색된 대상이 파일인 경우

			} while (::FindNextFile(find_file, &find_data));  // 추가적인 파일이 있는지 반복해서 검사																		
			::FindClose(find_file);// 파일 탐색에 사용된 핸들을 정리한다.
		}
		find_file = ::FindFirstFile(TEXT("*.pgw"), &find_data);
		if (find_file != INVALID_HANDLE_VALUE) {			
			do {
				if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY); // "탐색된 대상이 디렉토리(폴더)인 경우"
				else
				{
					_bstr_t b(find_data.cFileName);//반환형 변환
					const char* c = b;
					printf("%s \n", c);

					strcpy_s(file_name[x], c);
					printf("%s \n", file_name[x]);
					x++;
				};	// 탐색된 대상이 파일인 경우
			} while (::FindNextFile(find_file, &find_data));  // 추가적인 파일이 있는지 반복해서 검사															  
			::FindClose(find_file); // 파일 탐색에 사용된 핸들을 정리한다.
		}
		char fname[300];
		if (x == 0)
		{
			printf("No File in current Directory\n");
			Sleep(timeWait);
		}
		else {
			for (int i = 0; i < x; i++)
			{				
				std::chrono::system_clock::time_point tpDay000 = std::chrono::system_clock::from_time_t(theDay000);

				std::chrono::system_clock::time_point tpStart = std::chrono::system_clock::now();
				int stms = std::chrono::duration_cast<std::chrono::milliseconds>(tpStart - tpDay000).count();
				int sthour = std::chrono::duration_cast<std::chrono::hours>(tpStart - tpDay000).count();
				int stminute = std::chrono::duration_cast<std::chrono::minutes>(tpStart - tpDay000).count();
				int stsecond = std::chrono::duration_cast<std::chrono::seconds>(tpStart - tpDay000).count();
				sprintf_s(sttime_hms, "%d:%d:%.3lf",sthour,int(stminute-sthour*60), double(stsecond-sthour*3600-(stminute-sthour*60)*60+(stms*0.001-stsecond)));				
				printf("%s\n", sttime_hms);

				char path[100] = "C:\\uav_image\\Result\\Temp/";
				char Rpath[100] = "/Result/";
				char temp[100] = ".temp";
				
				strcpy_s(fname, file_name[i]);
				strcat_s(Rpath, file_name[i]);
				strcat_s(path, file_name[i]);
				strcat_s(file_name[i], temp);

				//const char *vis_img1 = Opath.c_str();

				
				//cvDestroyWindow(vis_img1);
				if (!FtpPutFileA(hConnection, path, file_name[i], INTERNET_FLAG_TRANSFER_BINARY | INTERNET_FLAG_RELOAD, 0))
				{
					printf("Put: %s\n", file_name[i]);
					system("pause");
					InternetCloseHandle(hConnection);
					InternetCloseHandle(hInternet);
					return;
				}
				printf("FtpPutFile Success\n");

				if (!FtpRenameFileA(hConnection, file_name[i], Rpath))
				{
					printf("Re\n");
					system("pause");
					InternetCloseHandle(hConnection);
					InternetCloseHandle(hInternet);
					return;
				}
				printf("FtpRenameFile Success\n");

				char Dpath[200] = "C://uav_image/Result/Temp/Done/";
				strcat_s(Dpath, fname);
				if (-1 == rename(fname, Dpath))
				{
					printf("re\n");
					system("pause");
				}
				printf("Rename is done\n");				

				std::chrono::system_clock::time_point tpEnd = std::chrono::system_clock::now();

				int edms = std::chrono::duration_cast<std::chrono::milliseconds>(tpEnd - tpDay000).count();
				int edhour = std::chrono::duration_cast<std::chrono::hours>(tpEnd - tpDay000).count();
				int edminute = std::chrono::duration_cast<std::chrono::minutes>(tpEnd - tpDay000).count();
				int edsecond = std::chrono::duration_cast<std::chrono::seconds>(tpEnd - tpDay000).count();
				int processtime = std::chrono::duration_cast<std::chrono::milliseconds>(tpEnd - tpStart).count();
				sprintf_s(edtime_hms, "%d:%d:%.3lf", edhour, int(edminute - edhour * 60), double(edsecond - edhour * 3600 - (edminute - edhour * 60) * 60 + (edms*0.001 - edsecond)));
				printf("%s\n", edtime_hms);
				LogFile(fname, stms, sttime_hms, edms, edtime_hms, processtime);
			}			
		}		
	}

	InternetCloseHandle(hConnection);
	InternetCloseHandle(hInternet);
	hConnection = NULL;
	hInternet = NULL;
}

