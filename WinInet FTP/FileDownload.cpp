#include <stdio.h>
#include <Windows.h>
#include <WinInet.h>
#pragma comment(lib,"wininet.lib")

void FileDownload() {


	HINTERNET hInternet;
	HINTERNET hConnection;

	const WCHAR* ftpIp = TEXT("decnas.synology.me");
	const WCHAR* ftpId = TEXT("uav");
	const WCHAR* ftpPw = TEXT("tilabtilab");

	WCHAR* fileName = TEXT("DSC00059.JPG");
	WCHAR* filePath = TEXT("./DSC00059.JPG");

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

	if (!FtpSetCurrentDirectory(hConnection, L"/home")) {
		InternetCloseHandle(hConnection);
		InternetCloseHandle(hInternet);
		return;
	}
	printf("FtpSetCurrentDirectory Success\n");

	//if (!FtpPutFile(hConnection, filePath, fileName, INTERNET_FLAG_TRANSFER_BINARY | INTERNET_FLAG_RELOAD, NULL))
	//{
	//	InternetCloseHandle(hConnection);
	//	InternetCloseHandle(hInternet);
	//	return;
	//}
	//printf("FtpPutFile Success\n");

	if (!FtpGetFile(hConnection, fileName, filePath, FALSE, NULL, FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD, NULL)) {
		InternetCloseHandle(hConnection);
		InternetCloseHandle(hInternet);
		return;
	}
	printf("FtpGetFile Success\n");

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