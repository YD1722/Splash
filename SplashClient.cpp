//  ===========================================================================
//  File    SplashClient.cpp
//  Desc    Test stub for the CSplash class
//  ===========================================================================
#include "stdafx.h"
#include "splash.h"
#include <stdlib.h>
#include<iostream>
#include <windows.h>
#include <string>
#include <shellapi.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <cstring>
#include <cstdlib>

using namespace std;

LPCWSTR mainAppDirectory = L"C:\\Program Files\\DirectFN Pro11 Price\\";
LPCWSTR mainAppPath = L"C:\\Program Files\\DirectFN Pro11 Price\\launcher.exe";

string profile = getenv("USERPROFILE");
string tempFilePath = profile+ "\\AppData\\Local\\OpenFin\\temp.dat"; 

int APIENTRY WinMain(
					 HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{

	ifstream infile;
	fstream fileStream;
	ofstream myfile;
	int status;
	string line;
		
	LPWSTR* szArgList;
	int argCount;
	LPCTSTR inputMessage = L"Loading ...";
	int sleepTime =1000;

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	// set the size of the structures
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	LPWSTR appPath = new WCHAR[500];
	wcscpy_s(appPath, 500, mainAppPath);

	szArgList = CommandLineToArgvW(GetCommandLine(), &argCount);

	if (szArgList == NULL || szArgList[1] == NULL)
	{

		myfile.open(tempFilePath);
		myfile.close();

		CreateProcess(NULL,   // the path
			appPath,        // Command line
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			TRUE,          // Set handle inheritance to FALSE
			0,              // No creation flags
			NULL,           // Use parent's environment block
			mainAppDirectory,           // Use parent's starting directory 
			&si,            // Pointer to STARTUPINFO structure
			&pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
		);


		LocalFree(szArgList);

		CSplash splash(TEXT(".\\splash.bmp"), RGB(0, 0, 0), inputMessage);
		splash.ShowSplash();

		for (int a = 0; a < 1200; a = a + 1) {
			struct stat buffer;
			if (!(stat(tempFilePath.c_str(), &buffer) == 0)) {
				splash.CloseSplash();
				break;
				return 0;
			}
			Sleep(50);
		}
		if (remove(tempFilePath.c_str()) != 0)
			return 0;
		else
			return 1;
		splash.CloseSplash();
	}
	else {	
		int messageType = _wtoi(szArgList[1]);

		switch (messageType) {
		case 0:
			inputMessage = L"Application is initializing ...";

			myfile.open(tempFilePath);
			myfile.close();

			CreateProcess(NULL,   // the path
				appPath,        // Command line
				NULL,           // Process handle not inheritable
				NULL,           // Thread handle not inheritable
				TRUE,          // Set handle inheritance to FALSE
				0,              // No creation flags
				NULL,           // Use parent's environment block
				mainAppDirectory,           // Use parent's starting directory 
				&si,            // Pointer to STARTUPINFO structure
				&pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
			);

			LocalFree(szArgList);

			break;
		case 1: // Updates
			inputMessage = L"Application is updating ...";

			myfile.open(tempFilePath);
			myfile.close();

			break;
			
		default:
			myfile.open(tempFilePath);
			myfile.close();
		}

		CSplash splash(TEXT(".\\splash.bmp"), RGB(0, 0, 0), inputMessage);
		splash.ShowSplash();

		for (int a = 0; a < 1200; a = a + 1) {
			struct stat buffer;
			if (!(stat(tempFilePath.c_str(), &buffer) == 0)) {
				splash.CloseSplash();
				break;
				return 0;
			}
			Sleep(50);
		}

		if (remove(tempFilePath.c_str()) != 0)
			return 0;
		else
			return 1;
		splash.CloseSplash();
	}
	
	
	//WaitForSingleObject(pi.hProcess, INFINITE);
	
	// Close process and thread handles. 
	//CloseHandle(pi.hProcess);
	//CloseHandle(pi.hThread);
}


