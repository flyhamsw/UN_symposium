#include <stdio.h>
#include <Windows.h>
#include <WinInet.h>
#include <comdef.h>

#pragma comment(lib,"wininet.lib")

void main() {

	const int timeWait = 500;

	HINTERNET hInternet;
	HINTERNET hConnection;
	HINTERNET h_find_file;
	HINTERNET h_find_file2;

	const WCHAR* ftpIp = TEXT("decnas.synology.me");
	const WCHAR* ftpId = TEXT("uav");
	const WCHAR* ftpPw = TEXT("tilabtilab");

	WCHAR* TestName = TEXT("test2");
	hInternet = InternetOpen(TestName, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);

	if (hInternet == NULL)
		return;

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
	printf("InternetConnect Success\n");

	if (!FtpSetCurrentDirectory(hConnection, L"/home/Test_lsm")) {
		InternetCloseHandle(hConnection);
		InternetCloseHandle(hInternet);
		return;
	}
	printf("FtpSetCurrentDirectory Success\n");

	char file_name[300][300];	
	int x;	
	WIN32_FIND_DATA find_data;
	h_find_file = ::FtpFindFirstFile(hConnection, TEXT("*.txt"), &find_data, INTERNET_FLAG_RELOAD, 0);

	for (int k = 0; k < 10000; k++)
	{
		x = 0;		
		if (h_find_file != NULL) {
			// 탐색 경로에 여러개의 파일이 있을수 있기 때문에 반복해서 탐색해야 한다.
			do {
				// find_data.cFileName 에 탐색된 파일의 이름이 저장되어 있다.

				// 속성 정보를 이용해서 파일인지 디렉토리인지 구분한다.
				if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
				else
				{
					_bstr_t b(find_data.cFileName);//반환형 변환
					const char* c = b;
					printf("%s \n", c);

					strcpy_s(file_name[x], c);
					printf("%s \n", file_name[x]);
					x++;
				};
			} while (::InternetFindNextFile(h_find_file, &find_data));  // 추가적인 파일이 있는지 반복해서 검사한다.
																		// 파일 탐색에 사용된 핸들을 정리한다.
			::InternetCloseHandle(h_find_file);
		}
		
		if (x == 0)
		{
			printf("기다려 \n");
			Sleep(timeWait);
		}
		else
		{
			for (int i = 0; i < x; i++)
			{
				char *ExistingName;
				char path[100] = "D://test_ftp/";//파일 받을 경로
				char Rpath[100] = "D://test_ftp/";
				char fname[300];
				char temp[100] = ".temp";
				strcpy_s(fname, file_name[i]);
				strcat_s(Rpath, file_name[i]);
				strcat_s(file_name[i], temp);
				ExistingName = file_name[i];//파일 이름
				strcat_s(path, file_name[i]);

				if (!FtpGetFileA(hConnection, fname, path, FALSE, NULL, INTERNET_FLAG_TRANSFER_BINARY | INTERNET_FLAG_RELOAD, 0)) {
					InternetCloseHandle(hConnection);
					InternetCloseHandle(hInternet);
					return;
				}
				printf("FtpGetFile Success\n");

				rename(path, Rpath);
				char Dpath[100] = "/home/Test_lsm/Done/";
				strcat_s(Dpath, fname);

				if (!FtpRenameFileA(hConnection, fname, Dpath))
				{
					InternetCloseHandle(hConnection);
					InternetCloseHandle(hInternet);
					return;
				}
				printf("FtpRenameFile Success\n");
			}
			
		}
	}

	InternetCloseHandle(hConnection);
	InternetCloseHandle(hInternet);
	hConnection = NULL;
	hInternet = NULL;
}
