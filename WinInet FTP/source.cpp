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
		if (fgetc(pFile) == 10) // ���� ���� ���� Line ����
		{
			nLine++;
		}
	}
	fprintf(pFile, "%s,%.3f,'%s,%.3f,'%s,%.3f\n", filename, double(st*0.001), st_hms, double(et*0.001), et_hms, double((pt)*0.001));
	fclose(pFile);
}

void main() {

	
	const int timeWait = 500;

	HINTERNET hInternet;	//FTP ���� �ڵ�
	HINTERNET hConnection;	//FTP ����
	HINTERNET h_find_file;	//FTP ���� �˻�

	//FTP ���� �ּ�, ID, Password
	//const WCHAR* ftpIp = TEXT("192.168.0.4");
	//const WCHAR* ftpId = TEXT("uav");
	//const WCHAR* ftpPw = TEXT("tilabtilab");

	//
	const WCHAR* ftpIp = TEXT("192.168.0.114");
	const WCHAR* ftpId = TEXT("unti");
	const WCHAR* ftpPw = TEXT("UN2017TI");

	//FTP ���� �ڵ�
	WCHAR* TestName = TEXT("test1");
	hInternet = InternetOpen(TestName, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);

	if (hInternet == NULL)
		return;

	//FTP ����
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
	printf("InternetConnect Success\n");//���� ����

	//if (!FtpSetCurrentDirectory(hConnection, L"/home/Test_lsm")) {
	//	InternetCloseHandle(hConnection);
	//	InternetCloseHandle(hInternet);
	//	return;
	//}
	//printf("FtpSetCurrentDirectory Success\n");//FTP �������� ���� ���丮 ����

	if (!FtpSetCurrentDirectory(hConnection, L"/UN/2017-02-21")) {
		InternetCloseHandle(hConnection);
		InternetCloseHandle(hInternet);
		return;
	}
	printf("FtpSetCurrentDirectory Success\n");//FTP �������� ���� ���丮 ����

	char file_name_img[300][300];	//������ ���丮���� ã�� ������ ���ϸ�
	char file_name_apx[300][300];	//������ ���丮���� ã�� apx ���ϸ�
	int x, y;						//���� ����		
	
	//log ���� ������ ���� �ð� üũ //
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
		//jpg ���� Ž�� �ڵ�
		h_find_file = ::FtpFindFirstFile(hConnection, TEXT("*.jpg"), &find_data, INTERNET_FLAG_RELOAD, 0);
		x = 0;
		if (h_find_file != NULL) {			
			do {				
				if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
				else
				{
					_bstr_t b(find_data.cFileName);//��ȯ�� ��ȯ
					const char* c = b;
					printf("%s \n", c);

					strcpy_s(file_name_img[x], c);//������ ���丮���� ã�� ������ ���ϸ�
					printf("%s \n", file_name_img[x]);
					x++;
				};
			} while (::InternetFindNextFile(h_find_file, &find_data));  // �߰����� ������ �ִ��� �ݺ��ؼ� �˻�																		
			::InternetCloseHandle(h_find_file);// ���� Ž���� ���� �ڵ��� ����
		}
		//txt ���� Ž�� �ڵ�
		y = 0;
		h_find_file = ::FtpFindFirstFile(hConnection, TEXT("*.txt"), &find_data, INTERNET_FLAG_RELOAD, 0);
		if (h_find_file != NULL) {			
			do {				
				if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
				else
				{
					_bstr_t b(find_data.cFileName);//��ȯ�� ��ȯ
					const char* c = b;
					printf("%s \n", c);

					strcpy_s(file_name_apx[y], c);
					printf("%s \n", file_name_apx[y]);
					y++;
				};
			} while (::InternetFindNextFile(h_find_file, &find_data)); // �߰����� ������ �ִ��� �ݺ��ؼ� �˻�																		
			::InternetCloseHandle(h_find_file);// ���� Ž���� ���� �ڵ��� ����
		}
		if (x == 0 && y==0)//������ ���� ��� 0.5�� �ڵ尡 ���� �ٽ� ����
		{
			printf("No File in current Directory \n");
			Sleep(timeWait);
		}
		else
		{
			for (int j = 0; j < y; j++)//������ �������� Ž���� apx file ����ŭ �ٿ�ε�ޱ� ���� �ݺ���
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
				char path_apx[100] = "C://uav_image/apx/";//���� ���� ���
				char Rpath_apx[100] = "C://uav_image/apx/";
				char fname_apx[300];
				char temp[100] = ".temp";
				strcpy_s(fname_apx, file_name_apx[j]);//�Ĺи� ����
				strcat_s(Rpath_apx, file_name_apx[j]);//Rename�� �ϱ� ���ؼ� ������ (��� + ���ϸ�)
				strcat_s(file_name_apx[j], temp);//�����̸�.temp
												 //ExistingName = file_name[i];//���� �̸�
				strcat_s(path_apx, file_name_apx[j]);//���� ���� ��� + ���� �̸� + .temp

													 //���� �ٿ�ε� (���ϸ�(fname) : ���ϸ� + .temp(path))
				if (!FtpGetFileA(hConnection, fname_apx, path_apx, FALSE, NULL, INTERNET_FLAG_TRANSFER_BINARY | INTERNET_FLAG_RELOAD, 0)) {
					InternetCloseHandle(hConnection);
					InternetCloseHandle(hInternet);
					return;
				}
				printf("FtpGetFile Success\n");

				//���ϸ� ���� (���ϸ� + .temp --> ���ϸ�)
				rename(path_apx, Rpath_apx);

				//char Dpath[100] = "/home/Test_lsm/Done/"; //ó���� ������ �̵��� ���
				char Dpath_apx[100] = "/UN/Done/"; //ó���� ������ �̵��� ���
				strcat_s(Dpath_apx, fname_apx); //ó���� ���� �̵� ��� + ���ϸ�

												//ó���� ���� �̵�
				if (!FtpRenameFileA(hConnection, fname_apx, Dpath_apx))
				{
					InternetCloseHandle(hConnection);
					InternetCloseHandle(hInternet);
					return;
				}
				printf("FtpRenameFile Success\n");

				SetCurrentDirectoryA("C://uav_image/apx/");

				char* filename; //���� ���ϸ�
								//char newFilename[200]; //�� ������ �����
								//������ ��ġ, �ڼ�
				double X;
				double Y;
				double Z;
				double heading;
				double roll;
				double pitch;

				//���� ���� �ε�
				filename = Rpath_apx;
				ApxModifier* a = new ApxModifier(filename, true, true);

				//�̺�Ʈ ��, �� ������ �˻� �� ���
				a->interpolateData();

				//�� ���� ����
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
			for (int i = 0; i < x; i++)//���� Ž���� ���� ����ŭ �ٿ�ε�ޱ� ���� �ݺ���
			{
				std::chrono::system_clock::time_point tpDay000 = std::chrono::system_clock::from_time_t(theDay000);

				std::chrono::system_clock::time_point tpStart = std::chrono::system_clock::now();
				int stms = std::chrono::duration_cast<std::chrono::milliseconds>(tpStart - tpDay000).count();
				int sthour = std::chrono::duration_cast<std::chrono::hours>(tpStart - tpDay000).count();
				int stminute = std::chrono::duration_cast<std::chrono::minutes>(tpStart - tpDay000).count();
				int stsecond = std::chrono::duration_cast<std::chrono::seconds>(tpStart - tpDay000).count();
				sprintf_s(sttime_hms, "%d:%d:%.3lf", sthour, int(stminute - sthour * 60), double(stsecond - sthour * 3600 - (stminute - sthour * 60) * 60 + (stms*0.001 - stsecond)));
				printf("%s\n", sttime_hms);

				char path_img[100] = "C://uav_image/";	//���� ���� ���
				char Rpath_img[100] = "C://uav_image/";	//Rename�� ���� ���
				char fname_img[300];
				char temp[100] = ".temp";
				strcpy_s(fname_img, file_name_img[i]);	//�Ĺи� ����
				strcat_s(Rpath_img, file_name_img[i]);	//Rename�� �ϱ� ���ؼ� ������ (��� + ���ϸ�)
				strcat_s(file_name_img[i], temp);		//�����̸�.temp
				strcat_s(path_img, file_name_img[i]);	//���� ���� ��� + ���� �̸�

				//���� �ٿ�ε� (���ϸ� : ���ϸ� + .temp)
				if (!FtpGetFileA(hConnection, fname_img, path_img, FALSE, NULL, INTERNET_FLAG_TRANSFER_BINARY | INTERNET_FLAG_RELOAD, 0)) {
					InternetCloseHandle(hConnection);
					InternetCloseHandle(hInternet);
					return;
				}
				printf("FtpGetFile Success\n");				
				
				//���ϸ� ���� (���ϸ� + .temp --> ���ϸ�)
				rename(path_img, Rpath_img);

				char Dpath_img[100] = "/UN/Done/";	//ó���� ������ �̵��� ���
				strcat_s(Dpath_img, fname_img);		//ó���� ���� �̵� ��� + ���ϸ�

				//ó���� ���� �̵�
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
	// �ڵ�����
	InternetCloseHandle(hConnection);
	InternetCloseHandle(hInternet);
	hConnection = NULL;
	hInternet = NULL;
}
