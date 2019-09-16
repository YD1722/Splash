//  ===========================================================================
//  File    SplashClient.cpp
//  Desc    Test stub for the CSplash class
//  ===========================================================================
#include "stdafx.h"
#include<iostream>
#include <windows.h>

using namespace std;

LPCWSTR mainAppDirectory = L"C:\\Program Files\\DirectFN Pro11 Price\\";
LPCWSTR mainAppPath = L"C:\\Program Files\\DirectFN Pro11 Price\\runner.exe killStart";

string profile = getenv("USERPROFILE");
string tempFilePath = profile + "\\AppData\\Local\\DirectFN\\temp.dat";

int APIENTRY WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{

	LPWSTR* szArgList;
	int argCount;
	LPCTSTR inputMessage = L"";
	int sleepTime = 1000;
	int splashDisplayLimit = 1200;

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	// set the size of the structures
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	LPWSTR appPath = new WCHAR[500];
	wcscpy_s(appPath, 500, mainAppPath);

	CreateProcess(NULL,   // the path
		appPath,        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		TRUE,          // Set handle inheritance to FALSE
		CREATE_NO_WINDOW,              // No creation flags
		NULL,           // Use parent's environment block
		mainAppDirectory,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
	);

}




