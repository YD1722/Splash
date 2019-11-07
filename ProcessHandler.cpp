#include "stdafx.h"
#include <string>
#include <fstream>

#include "ProcessHandler.h"
#include "splash.h"

using namespace std;

string profile1 = getenv("USERPROFILE");
string tempFilePath1 = profile1 + "\\AppData\\Local\\DirectFN\\temp.dat";

LPCWSTR ProcessHandler::stringToLPCWSTR(string str ) {
	std::wstring stemp = std::wstring(str.begin(), str.end());
	return stemp.c_str();
}

LPCWSTR ProcessHandler::concatLPCWSTR(LPCWSTR str1, LPCWSTR str2) {
	std::wstring mywstring(str1);
	std::wstring concatted_stdstr = mywstring + str2;
	return concatted_stdstr.c_str();
}

void ProcessHandler::executeProgram(LPCWSTR command, LPCWSTR workingDir, bool isShowSPlash) {
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	LPWSTR parsedCmd = new WCHAR[500];
	wcscpy_s(parsedCmd, 500, command);

	// Start the child process.
	if (!CreateProcess(NULL,   // the path
		parsedCmd,        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		TRUE,          // Set handle inheritance to FALSE
		CREATE_NO_WINDOW,              // No creation flags
		NULL,           // Use parent's environment block
		workingDir,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi)) // Pointer to PROCESS_INFORMATION
	{
		puts("Error");
		return;
	}

	if (isShowSPlash) {
		showSplash(L"");
	}

	// Wait until child process exits.
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
};

LPCTSTR ProcessHandler::getInputMessage(int messageType) {
	LPCTSTR inputMessage = L"";

	switch (messageType) {
	case 0:
		inputMessage = L"Application is initializing ...";

		break;
	case 1: // Updates
		inputMessage = L"Application is updating. Please Wait ...";

		break;

	case 2: // Restarting
		inputMessage = L"Application is Restarting ...";

		break;

	case 3: // ReInstall
		inputMessage = L"Something went wrong. Please reinstall the app.";

		break;
	}

	return inputMessage;
};

void ProcessHandler::showSplash(LPCTSTR inputMessage) {
	std::ofstream myFile;

	myFile.open(tempFilePath1);
	myFile.close();

	CSplash splash(TEXT(".\\splash.bmp"), RGB(0, 0, 0), inputMessage);
	splash.ShowSplash();

	subscribeForSplashClose(splash);
};

int ProcessHandler::subscribeForSplashClose(CSplash splash) {
	for (int a = 0; a < 1200; a = a + 1) {
		struct stat buffer;
		if (!(stat(tempFilePath1.c_str(), &buffer) == 0)) {
			splash.CloseSplash();
			break;
			return 0;
		}
		Sleep(50);
	}

	if (remove(tempFilePath1.c_str()) != 0)
		return 0;
	else
		return 1;

	splash.CloseSplash();
}




