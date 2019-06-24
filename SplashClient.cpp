//  ===========================================================================
//  File    SplashClient.cpp
//  Desc    Test stub for the CSplash class
//  ===========================================================================
#include "stdafx.h"
#include "splash.h"
#include <stdlib.h>
#include<iostream>
#include <windows.h>

LPCWSTR mainAppDirectory = L"C:\\Program Files (x86)\\DirectFN Pro11 Price\\";
LPCWSTR mainAppPath = L"C:\\Program Files (x86)\\DirectFN Pro11 Price\\launcher.exe";

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{

	//ShellExecute(NULL, "open", "path\\to\\file.exe", NULL, NULL, SW_SHOWDEFAULT);
    //  =======================================================================
    //  Display the splash screen using the overloaded construcutor
    //  =======================================================================
    //  Launch splash screen
    CSplash splash1(TEXT(".\\dada.bmp"), RGB(255, 255, 255));
    splash1.ShowSplash();

    // your start up code here
    Sleep(5000); //  simulate using a 5 second delay


	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	// set the size of the structures
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	LPWSTR appPath = new WCHAR[500];
	wcscpy_s(appPath, 500, mainAppPath);

	
	 //start the program up
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

	splash1.CloseSplash();
	//splash1.CloseSplash();
	//WaitForSingleObject(pi.hProcess, INFINITE);

	
	// Close process and thread handles. 
	//CloseHandle(pi.hProcess);
	//CloseHandle(pi.hThread);
	//startup(L"C:\Program Files (x86)\DirectFN Pro11 Price\launcher.exe");
	return TRUE;
}

VOID startup(LPCTSTR lpApplicationName)
{
	// additional information
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	// set the size of the structures
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// start the program up
	CreateProcess(lpApplicationName,   // the path
		NULL,        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
	);
	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

