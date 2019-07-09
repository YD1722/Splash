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

using namespace std;

LPCWSTR mainAppDirectory = L"C:\\Program Files (x86)\\DirectFN Pro11 Price\\";
LPCWSTR mainAppPath = L"C:\\Program Files (x86)\\DirectFN Pro11 Price\\launcher.exe";
LPCWSTR tempFilePath = L"C:\\Users\\yashodhah\\AppData\\Local\\OpenFin\\temp.dat"; // TODO: [Yashodha] Make this common

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
	LPCTSTR inputMessage = L"Loading......";
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

		CSplash splash(TEXT(".\\titanium.bmp"), RGB(0, 0, 0), inputMessage);
		splash.ShowSplash();

	
		string path = "C:\\Users\\yashodhah\\AppData\\Local\\OpenFin\\temp.dat";

		while (true) {
			struct stat buffer;
			if (!(stat(path.c_str(), &buffer) == 0)) {
				splash.CloseSplash();
				break;
			}
		}
	}
	else {

	/*	for (int i = 0; i < argCount; i++)
	{
		MessageBox(NULL, szArgList[i], L"Arglist contents", MB_OK);
	}*/
		///inputMessage = szArgList[1];
		int messageType = _wtoi(szArgList[1]);

		switch (messageType) {
		case 1: // Updates
			inputMessage = L"Application is updating....";

			myfile.open(tempFilePath);
			myfile.close();

			break;

		case 2: // Restarting
			inputMessage = L"Application is restarting....";
			
			break;
			
		default:
			myfile.open(tempFilePath);
			myfile.close();
		}

		CSplash splash(TEXT(".\\titanium.bmp"), RGB(0, 0, 0), inputMessage);
		splash.ShowSplash();

		string path = "C:\\Users\\yashodhah\\AppData\\Local\\OpenFin\\temp.dat";

		while (true) { // TODO: [Yashodha] Make this a for loop
			struct stat buffer;
			if (!(stat(path.c_str(), &buffer) == 0)) {
				splash.CloseSplash();
				break;
			}
		}		
	}
	//return 0;

	//szArgList = CommandLineToArgvW(GetCommandLine(), &argCount);

	//if (szArgList == NULL || szArgList[1] == NULL)
	//{
	//	// //start the program up
	//	CreateProcess(NULL,   // the path
	//		appPath,        // Command line
	//		NULL,           // Process handle not inheritable
	//		NULL,           // Thread handle not inheritable
	//		TRUE,          // Set handle inheritance to FALSE
	//		0,              // No creation flags
	//		NULL,           // Use parent's environment block
	//		mainAppDirectory,           // Use parent's starting directory 
	//		&si,            // Pointer to STARTUPINFO structure
	//		&pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
	//	);
	//}
	//else {
	//	inputMessage = szArgList[1];
	//}

	/*for (int i = 0; i < argCount; i++)
	{
		MessageBox(NULL, szArgList[i], L"Arglist contents", MB_OK);
	}*/
	//LPCTSTR 
/*
	LocalFree(szArgList);

    CSplash splash(TEXT(".\\titanium.bmp"), RGB(0, 0, 0), inputMessage);
    splash.ShowSplash();*/

    //Sleep(_wtoi(sleepTime)); 
	//Sleep(sleepTime);


	

	
	//WaitForSingleObject(pi.hProcess, INFINITE);

	
	// Close process and thread handles. 
	//CloseHandle(pi.hProcess);
	//CloseHandle(pi.hThread);
	
}


