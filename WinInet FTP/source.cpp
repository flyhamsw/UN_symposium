#include <stdio.h>
#include <Windows.h>
#include <WinInet.h>
#include <comdef.h>

#include <chrono>
#include <iostream>
#include <ctime>

#include "stdafx.h"
#include "ApxModifier.h"
#include <string.h>
#include <iostream>
#include <math.h>
#include <string>

#pragma comment(lib,"wininet.lib")

void LogFile(char* filename, int st, char* st_hms, int et, char* et_hms, int pt)
{
	FILE* pFile = NULL;
	fopen_s(&pFile, "C:\\Log_file/TimeDownload.csv", "a+");
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

	
	const int timeWait = 500;

	HINTERNET hInternet;	//FTP 접속 핸들
	HINTERNET hConnection;	//FTP 접속
	HINTERNET h_find_file;	//FTP 파일 검색

	//FTP 접속 주소, ID, Password
	//const WCHAR* ftpIp = TEXT("192.168.0.4");
	//const WCHAR* ftpId = TEXT("uav");
	//const WCHAR* ftpPw = TEXT("tilabtilab");

	//
	const WCHAR* ftpIp = TEXT("192.168.0.114");
	const WCHAR* ftpId = TEXT("unti");
	const WCHAR* ftpPw = TEXT("UN2017TI");

	//FTP 접속 핸들
	WCHAR* TestName = TEXT("test1");
	hInternet = InternetOpen(TestName, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);

	if (hInternet == NULL)
		return;

	//FTP 접속
	hConnection = InternetConnect(hInternet,
		ftpIp,
		INTERNET_DEFAULT_FTP_PORT,
		ftpId,
		ftpPw,
		INTERNET_SERVICE_FTP,
		0, 0);

	if (hConnection == NULL) {
		InternetCloseHandle(hInternet);
		printf("InternetConnect Fail\n");
		return;
	}
	printf("InternetConnect Success\n");//접속 성공

	//if (!FtpSetCurrentDirectory(hConnection, L"/home/Test_lsm")) {
	//	InternetCloseHandle(hConnection);
	//	InternetCloseHandle(hInternet);
	//	return;
	//}
	//printf("FtpSetCurrentDirectory Success\n");//FTP 서버에서 현재 디렉토리 설정

	if (!FtpSetCurrentDirectory(hConnection, L"/UN/2017-02-21")) {
		InternetCloseHandle(hConnection);
		InternetCloseHandle(hInternet);
		return;
	}
	printf("FtpSetCurrentDirectory Success\n");//FTP 서버에서 현재 디렉토리 설정

	char file_name_img[300][300];	//설정된 디렉토리에서 찾을 영상의 파일명
	char file_name_apx[300][300];	//설정된 디렉토리에서 찾을 apx 파일명
	int x, y;						//파일 개수		
	
	//log 파일 생성을 위한 시간 체크 //
	char sttime_hms[30], edtime_hms[30];
	std::time_t tNow = std::time(NULL);
	std::tm tmHMS;
	localtime_s(&tmHMS, &tNow);
	tmHMS.tm_hour = 0;
	tmHMS.tm_min = 0;
	tmHMS.tm_sec = 0;

	std::time_t theDay000 = std::mktime(&tmHMS);
	////////////////////////////////////

	for (int k = 0; k < 10000; k++)
	{
		WIN32_FIND_DATA find_data;
		//jpg 파일 탐색 핸들
		h_find_file = ::FtpFindFirstFile(hConnection, TEXT("*.jpg"), &find_data, INTERNET_FLAG_RELOAD, 0);
		x = 0;
		if (h_find_file != NULL) {			
			do {				
				if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
				else
				{
					_bstr_t b(find_data.cFileName);//반환형 변환
					const char* c = b;
					printf("%s \n", c);

					strcpy_s(file_name_img[x], c);//설정된 디렉토리에서 찾은 파일의 파일명
					printf("%s \n", file_name_img[x]);
					x++;
				};
			} while (::InternetFindNextFile(h_find_file, &find_data));  // 추가적인 파일이 있는지 반복해서 검사																		
			::InternetCloseHandle(h_find_file);// 파일 탐색에 사용된 핸들을 정리
		}
		//txt 파일 탐색 핸들
		y = 0;
		h_find_file = ::FtpFindFirstFile(hConnection, TEXT("*.txt"), &find_data, INTERNET_FLAG_RELOAD, 0);
		if (h_find_file != NULL) {			
			do {				
				if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
				else
				{
					_bstr_t b(find_data.cFileName);//반환형 변환
					const char* c = b;
					printf("%s \n", c);

					strcpy_s(file_name_apx[y], c);
					printf("%s \n", file_name_apx[y]);
					y++;
				};
			} while (::InternetFindNextFile(h_find_file, &find_data)); // 추가적인 파일이 있는지 반복해서 검사																		
			::InternetCloseHandle(h_find_file);// 파일 탐색에 사용된 핸들을 정리
		}
		if (x == 0 && y==0)//파일이 없는 경우 0.5간 코드가 쉬고 다시 실행
		{
			printf("No File in current Directory \n");
			Sleep(timeWait);
		}
		else
		{
			for (int j = 0; j < y; j++)//정해진 폴더에서 탐색한 apx file 수만큼 다운로드받기 위한 반복문
			{
				std::chrono::system_clock::time_point tpDay000 = std::chrono::system_clock::from_time_t(theDay000);

				std::chrono::system_clock::time_point tpStart = std::chrono::system_clock::now();
				int stms = std::chrono::duration_cast<std::chrono::milliseconds>(tpStart - tpDay000).count();
				int sthour = std::chrono::duration_cast<std::chrono::hours>(tpStart - tpDay000).count();
				int stminute = std::chrono::duration_cast<std::chrono::minutes>(tpStart - tpDay000).count();
				int stsecond = std::chrono::duration_cast<std::chrono::seconds>(tpStart - tpDay000).count();
				sprintf_s(sttime_hms, "%d:%d:%.3lf", sthour, int(stminute - sthour * 60), double(stsecond - sthour * 3600 - (stminute - sthour * 60) * 60 + (stms*0.001 - stsecond)));
				printf("%s\n", sttime_hms);

				//char *ExistingName;
				char path_apx[100] = "C://uav_image/apx/";//파일 받을 경로
				char Rpath_apx[100] = "C://uav_image/apx/";
				char fname_apx[300];
				char temp[100] = ".temp";
				strcpy_s(fname_apx, file_name_apx[j]);//파밀명 저장
				strcat_s(Rpath_apx, file_name_apx[j]);//Rename을 하기 위해서 설정한 (경로 + 파일명)
				strcat_s(file_name_apx[j], temp);//파일이름.temp
												 //ExistingName = file_name[i];//파일 이름
				strcat_s(path_apx, file_name_apx[j]);//현재 파일 경로 + 파일 이름 + .temp

													 //파일 다운로드 (파일명(fname) : 파일명 + .temp(path))
				if (!FtpGetFileA(hConnection, fname_apx, path_apx, FALSE, NULL, INTERNET_FLAG_TRANSFER_BINARY | INTERNET_FLAG_RELOAD, 0)) {
					InternetCloseHandle(hConnection);
					InternetCloseHandle(hInternet);
					return;
				}
				printf("FtpGetFile Success\n");

				//파일명 변경 (파일명 + .temp --> 파일명)
				rename(path_apx, Rpath_apx);

				//char Dpath[100] = "/home/Test_lsm/Done/"; //처리된 파일을 이동할 경로
				char Dpath_apx[100] = "/UN/Done/"; //처리된 파일을 이동할 경로
				strcat_s(Dpath_apx, fname_apx); //처리된 파일 이동 경로 + 파일명

												//처리된 파일 이동
				if (!FtpRenameFileA(hConnection, fname_apx, Dpath_apx))
				{
					InternetCloseHandle(hConnection);
					InternetCloseHandle(hInternet);
					return;
				}
				printf("FtpRenameFile Success\n");

				SetCurrentDirectoryA("C://uav_image/apx/");

				char* filename; //기존 파일명
								//char newFilename[200]; //새 파일의 상대경로
								//내삽합 위치, 자세
				double X;
				double Y;
				double Z;
				double heading;
				double roll;
				double pitch;

				//기존 파일 로드
				filename = Rpath_apx;
				ApxModifier* a = new ApxModifier(filename, true, true);

				//이벤트 전, 후 데이터 검색 및 계산
				a->interpolateData();

				//새 파일 생성
				string filename_new(filename);
				string filename_apx_new(fname_apx);
				filename_new.erase(filename_new.end() - 8, filename_new.end() - 4);
				filename_apx_new.erase(filename_apx_new.end() - 8, filename_apx_new.end() - 4);
				char cstr[MAX_PATH];
				char cstr2[MAX_PATH];
				char fileN[MAX_PATH];
				strcpy_s(cstr, filename_new.c_str());
				strcpy_s(cstr2, filename_apx_new.c_str());

				strcpy_s(fileN, cstr);
				strcat_s(cstr, temp);

				char newFilename[200] = "C://uav_image/";
				char newFilename2[200] = "C://uav_image/";
				strcat_s(newFilename, cstr);
				strcat_s(newFilename2, cstr2);
				a->writeNewFile(cstr);

				rename(cstr, newFilename2);

				std::chrono::system_clock::time_point tpEnd = std::chrono::system_clock::now();

				int edms = std::chrono::duration_cast<std::chrono::milliseconds>(tpEnd - tpDay000).count();
				int edhour = std::chrono::duration_cast<std::chrono::hours>(tpEnd - tpDay000).count();
				int edminute = std::chrono::duration_cast<std::chrono::minutes>(tpEnd - tpDay000).count();
				int edsecond = std::chrono::duration_cast<std::chrono::seconds>(tpEnd - tpDay000).count();
				int processtime = std::chrono::duration_cast<std::chrono::milliseconds>(tpEnd - tpStart).count();
				sprintf_s(edtime_hms, "%d:%d:%.3lf", edhour, int(edminute - edhour * 60), double(edsecond - edhour * 3600 - (edminute - edhour * 60) * 60 + (edms*0.001 - edsecond)));
				printf("%s\n", edtime_hms);
				LogFile(fname_apx, stms, sttime_hms, edms, edtime_hms, processtime);
			}
			for (int i = 0; i < x; i++)//파일 탐색한 영상 수만큼 다운로드받기 위한 반복문
			{
				std::chrono::system_clock::time_point tpDay000 = std::chrono::system_clock::from_time_t(theDay000);

				std::chrono::system_clock::time_point tpStart = std::chrono::system_clock::now();
				int stms = std::chrono::duration_cast<std::chrono::milliseconds>(tpStart - tpDay000).count();
				int sthour = std::chrono::duration_cast<std::chrono::hours>(tpStart - tpDay000).count();
				int stminute = std::chrono::duration_cast<std::chrono::minutes>(tpStart - tpDay000).count();
				int stsecond = std::chrono::duration_cast<std::chrono::seconds>(tpStart - tpDay000).count();
				sprintf_s(sttime_hms, "%d:%d:%.3lf", sthour, int(stminute - sthour * 60), double(stsecond - sthour * 3600 - (stminute - sthour * 60) * 60 + (stms*0.001 - stsecond)));
				printf("%s\n", sttime_hms);

				char path_img[100] = "C://uav_image/";	//파일 받을 경로
				char Rpath_img[100] = "C://uav_image/";	//Rename한 파일 경로
				char fname_img[300];
				char temp[100] = ".temp";
				strcpy_s(fname_img, file_name_img[i]);	//파밀명 저장
				strcat_s(Rpath_img, file_name_img[i]);	//Rename을 하기 위해서 설정한 (경로 + 파일명)
				strcat_s(file_name_img[i], temp);		//파일이름.temp
				strcat_s(path_img, file_name_img[i]);	//현재 파일 경로 + 파일 이름

				//파일 다운로드 (파일명 : 파일명 + .temp)
				if (!FtpGetFileA(hConnection, fname_img, path_img, FALSE, NULL, INTERNET_FLAG_TRANSFER_BINARY | INTERNET_FLAG_RELOAD, 0)) {
					InternetCloseHandle(hConnection);
					InternetCloseHandle(hInternet);
					return;
				}
				printf("FtpGetFile Success\n");				
				
				//파일명 변경 (파일명 + .temp --> 파일명)
				rename(path_img, Rpath_img);

				char Dpath_img[100] = "/UN/Done/";	//처리된 파일을 이동할 경로
				strcat_s(Dpath_img, fname_img);		//처리된 파일 이동 경로 + 파일명

				//처리된 파일 이동
				if (!FtpRenameFileA(hConnection, fname_img, Dpath_img))
				{
					InternetCloseHandle(hConnection);
					InternetCloseHandle(hInternet);
					return;
				}
				printf("FtpRenameFile Success\n");

				std::chrono::system_clock::time_point tpEnd = std::chrono::system_clock::now();

				int edms = std::chrono::duration_cast<std::chrono::milliseconds>(tpEnd - tpDay000).count();
				int edhour = std::chrono::duration_cast<std::chrono::hours>(tpEnd - tpDay000).count();
				int edminute = std::chrono::duration_cast<std::chrono::minutes>(tpEnd - tpDay000).count();
				int edsecond = std::chrono::duration_cast<std::chrono::seconds>(tpEnd - tpDay000).count();
				int processtime = std::chrono::duration_cast<std::chrono::milliseconds>(tpEnd - tpStart).count();
				sprintf_s(edtime_hms, "%d:%d:%.3lf", edhour, int(edminute - edhour * 60), double(edsecond - edhour * 3600 - (edminute - edhour * 60) * 60 + (edms*0.001 - edsecond)));
				printf("%s\n", edtime_hms);
				LogFile(fname_img, stms, sttime_hms, edms, edtime_hms, processtime);
			}

		}
	}
	// 핸들정리
	InternetCloseHandle(hConnection);
	InternetCloseHandle(hInternet);
	hConnection = NULL;
	hInternet = NULL;
}
