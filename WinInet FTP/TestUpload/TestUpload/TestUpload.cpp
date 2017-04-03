#include <stdio.h>
#include <Windows.h>
#include <WinInet.h>
#include <comdef.h>

#pragma comment(lib,"wininet.lib")

void main() {

	SetCurrentDirectoryA("C://asdf/");

	char file_name[300][300];
	char file_name2[300][300];
	int x,y = 0;
	WIN32_FIND_DATA find_data;
	HANDLE find_file;
	find_file = ::FindFirstFile(TEXT("*.txt"), &find_data);
	if (find_file != NULL) {
		// 탐색 경로에 여러개의 파일이 있을수 있기 때문에 반복해서 탐색해야 한다.
		do {
			// find_data.cFileName 에 탐색된 파일의 이름이 저장되어 있다.

			// 속성 정보를 이용해서 파일인지 디렉토리인지 구분한다.
			if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) "탐색된 대상이 디렉토리(폴더)인 경우";
			else
			{
				_bstr_t b(find_data.cFileName);//반환형 변환
				const char* c = b;
				printf("%s \n", c);

				strcpy_s(file_name[x], c);
				printf("%s \n", file_name[x]);
				x++;
			}"탐색된 대상이 파일인 경우";

		} while (::FindNextFile(find_file, &find_data));  // 추가적인 파일이 있는지 반복해서 검사한다.
														  // 파일 탐색에 사용된 핸들을 정리한다.
		::FindClose(find_file);
	}

	find_file = ::FindFirstFile(TEXT("*.jpg"), &find_data);
	if (find_file != NULL) {
		// 탐색 경로에 여러개의 파일이 있을수 있기 때문에 반복해서 탐색해야 한다.
		do {
			// find_data.cFileName 에 탐색된 파일의 이름이 저장되어 있다.

			// 속성 정보를 이용해서 파일인지 디렉토리인지 구분한다.
			if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) "탐색된 대상이 디렉토리(폴더)인 경우";
			else
			{
				_bstr_t b(find_data.cFileName);//반환형 변환
				const char* c = b;
				printf("%s \n", c);

				strcpy_s(file_name2[y], c);
				printf("%s \n", file_name2[y]);
				y++;
			}"탐색된 대상이 파일인 경우";

		} while (::FindNextFile(find_file, &find_data));  // 추가적인 파일이 있는지 반복해서 검사한다.
														  // 파일 탐색에 사용된 핸들을 정리한다.
		::FindClose(find_file);
	}

	HINTERNET hInternet;
	HINTERNET hConnection;
	HINTERNET h_find_file;

	const WCHAR* ftpIp = TEXT("decnas.synology.me");
	const WCHAR* ftpId = TEXT("uav");
	const WCHAR* ftpPw = TEXT("tilabtilab");

	//WCHAR* fileName1 = TEXT("2016-07-14_115221.705855.jpg");
	//WCHAR* fileName2 = TEXT("2016-05-14_115221.log");
	//WCHAR* filePath = TEXT("C:\\2016-05-14_115221.705855.jpg");

	WCHAR* TestName = TEXT("Test");
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
	//	system("pause");

	if (!FtpSetCurrentDirectory(hConnection, L"/home/Test_lsm")) {
	InternetCloseHandle(hConnection);
	InternetCloseHandle(hInternet);
	return;
	}
	printf("FtpSetCurrentDirectory Success\n");

	//char file_name[300][300];
	//int x = 0;
	//WIN32_FIND_DATA find_data;
	//h_find_file = ::FtpFindFirstFile(hConnection, TEXT("*.*"), &find_data, INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE, 0);
	//if (h_find_file != NULL) {
	//	// 탐색 경로에 여러개의 파일이 있을수 있기 때문에 반복해서 탐색해야 한다.
	//	do {
	//		// find_data.cFileName 에 탐색된 파일의 이름이 저장되어 있다.

	//		// 속성 정보를 이용해서 파일인지 디렉토리인지 구분한다.
	//		if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) "탐색된 대상이 디렉토리(폴더)인 경우";
	//		else
	//		{
	//			_bstr_t b(find_data.cFileName);//반환형 변환
	//			const char* c = b;
	//			printf("%s \n", c);

	//			strcpy_s(file_name[x], c);
	//			printf("%s \n", file_name[x]);
	//			x++;
	//		}"탐색된 대상이 파일인 경우";

	//	} while (::InternetFindNextFile(h_find_file, &find_data));  // 추가적인 파일이 있는지 반복해서 검사한다.
	//																// 파일 탐색에 사용된 핸들을 정리한다.
	//	::InternetCloseHandle(h_find_file);
	//}

	/*char *ExistingName, *fileName1, *filePath;
	char path[100] = "./";
	ExistingName = file_name[1];
	strcat_s(path, file_name[1]);

	if (!FtpGetFileA(hConnection, ExistingName, path, FALSE, NULL, FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD, NULL)) {
	InternetCloseHandle(hConnection);
	InternetCloseHandle(hInternet);
	return;
	}
	printf("FtpGetFile Success\n");*/

	//for (int i = 0; i < x; i++)
	//{
	//	char *ExistingName, *fileName1, *filePath;
	//	char *NewName;
	//	char path[100] = "./";
	//	char Dpath[100] = "/home/Test_lsm/Done/";
	//	ExistingName = file_name[i];
	//	strcat_s(path, file_name[i]);
	//	strcat_s(Dpath, file_name[i]);

	//	if (!FtpGetFileA(hConnection, ExistingName, path, FALSE, NULL, FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD, NULL)) {
	//		InternetCloseHandle(hConnection);
	//		InternetCloseHandle(hInternet);
	//		return;
	//	}
	//	printf("FtpGetFile Success\n");

	//	/*if (!FtpRenameFileA(hConnection, ExistingName, Dpath))
	//	{
	//	InternetCloseHandle(hConnection);
	//	InternetCloseHandle(hInternet);
	//	return;
	//	}
	//	printf("FtpRenameFile Success\n");*/
	//}

	for (int i = 0; i < x; i++)
	{
		char fname[300];
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

		char *ExistingName, *fileName1, *filePath;
		char *NewName;
		//char path[100] = "/home/Test_lsm/";
		char Dpath[100] = "C:\\asdf/";
		ExistingName = file_name[i];
		//strcat_s(path, file_name[i]);
		strcat_s(Dpath, file_name[i]);

		/*if (!FtpGetFileA(hConnection, ExistingName, path, FALSE, NULL, FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD, NULL)) {
		InternetCloseHandle(hConnection);
		InternetCloseHandle(hInternet);
		return;
		}
		printf("FtpGetFile Success\n");*/

		if (!FtpPutFileA(hConnection, Dpath, ExistingName, INTERNET_FLAG_TRANSFER_BINARY | INTERNET_FLAG_RELOAD, 0))
		{
			InternetCloseHandle(hConnection);
			InternetCloseHandle(hInternet);
			return;
		}
		printf("FtpPutFile Success\n");
	}

	/*if (!FtpPutFile(hConnection, filePath, fileName2, INTERNET_FLAG_TRANSFER_BINARY | INTERNET_FLAG_RELOAD, NULL))
	{
	InternetCloseHandle(hConnection);
	InternetCloseHandle(hInternet);
	return;
	}
	printf("FtpPutFile Success\n");*/

	/*if (!FtpDeleteFile(hConnection, fileName)) {
	InternetCloseHandle(hConnection);
	InternetCloseHandle(hInternet);
	return;
	}
	printf("FtpDeleteFile Success\n");*/

	InternetCloseHandle(hConnection);
	InternetCloseHandle(hInternet);
	hConnection = NULL;
	hInternet = NULL;
}