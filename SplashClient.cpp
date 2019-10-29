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

string profile = getenv("USERPROFILE");

string userDataDir = profile + "\\AppData\\Local\\DirectFN";
string tempFilePath = userDataDir + "\\temp.dat";

string shadowInstallerDir = userDataDir + "\\DirectFN_Pro11_Price_4294798646\\shadowInstaller";
string shadowAppPath = shadowInstallerDir + "\\application.exe";
string shadowNodePath = shadowInstallerDir + "\\node.exe";

LPCWSTR nodeCmd;
LPCWSTR nodeWorkingDir;

int APIENTRY WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	ofstream myfile;
	LPWSTR* szArgList;
	int argCount;

	LPCTSTR inputMessage = L"";
	int sleepTime = 1000;

	szArgList = CommandLineToArgvW(GetCommandLine(), &argCount);
	struct stat buffer1;

	/**Check if the shadow entry point exists
	 * in the user directory
	 */
	if ((stat(shadowAppPath.c_str(), &buffer1) == 0)) {
		string command = shadowAppPath;

		if (szArgList != NULL || szArgList[1] != NULL) {
			int argNum = _wtoi(szArgList[1]);
			string arg = to_string(argNum);

			command = command + " " + arg;
		}

		std::wstring stemp = std::wstring(command.begin(), command.end());
		LPCWSTR sw = stemp.c_str();


		std::wstring stemp1 = std::wstring(shadowInstallerDir.begin(), shadowInstallerDir.end());
		LPCWSTR sw1 = stemp1.c_str();

		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		// set the size of the structures
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		LPWSTR parsedCmd = new WCHAR[500];
		wcscpy_s(parsedCmd, 500, sw);

		CreateProcess(NULL,   // the path
			parsedCmd,        // Command line
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			TRUE,          // Set handle inheritance to FALSE
			CREATE_NO_WINDOW,              // No creation flags
			NULL,           // Use parent's environment block
			sw1,           // Use parent's starting directory 
			&si,            // Pointer to STARTUPINFO structure
			&pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
		);
	}
	else {
		if ((stat(shadowNodePath.c_str(), &buffer1) == 0)) {
			std::wstring stemp = std::wstring(shadowInstallerDir.begin(), shadowInstallerDir.end());

			std::wstring stemp1 = std::wstring(shadowNodePath.begin(), shadowNodePath.end());
			LPCWSTR sw1 = stemp1.c_str();

			std::wstring mywstring(stemp1);
			std::wstring concatted_stdstr = mywstring + L" launcher";

			nodeCmd = concatted_stdstr.c_str();
			nodeWorkingDir = stemp.c_str();

			if (szArgList == NULL || szArgList[1] == NULL)
			{
				myfile.open(tempFilePath);
				myfile.close();

				CSplash splash(TEXT(".\\splash.bmp"), RGB(0, 0, 0), inputMessage);
				splash.executeProgram(nodeCmd, nodeWorkingDir);

				LocalFree(szArgList);

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

				Sleep(10000);
				splash.CloseSplash();
			}
			else {
				int messageType = _wtoi(szArgList[1]);

				switch (messageType) {
				case 0:
					inputMessage = L"Application is initializing ...";

					myfile.open(tempFilePath);
					myfile.close();

					break;
				case 1: // Updates
					inputMessage = L"Application is updating. Please Wait ...";

					myfile.open(tempFilePath);
					myfile.close();

					break;

				case 2: // Restarting
					inputMessage = L"Application is Restarting ...";

					myfile.open(tempFilePath);
					myfile.close();

					break;

				case 3: // ReInstall
					inputMessage = L"Something went wrong. Please reinstall the app.";

					myfile.open(tempFilePath);
					myfile.close();

					break;

				default:
					myfile.open(tempFilePath);
					myfile.close();
				}

				CSplash splash(TEXT(".\\splash.bmp"), RGB(0, 0, 0), inputMessage);
				splash.ShowSplash();

				LocalFree(szArgList);

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

				Sleep(10000);
				splash.CloseSplash();
			}

			/*int len;
			int slength = (int)shadowInstallerDir.length() + 1;
			len = MultiByteToWideChar(CP_ACP, 0, shadowInstallerDir.c_str(), slength, 0, 0);
			wchar_t* buf = new wchar_t[len];
			MultiByteToWideChar(CP_ACP, 0, shadowInstallerDir.c_str(), slength, buf, len);
			std::wstring r(buf);
			delete[] buf;

			nodeWorkingDir = r.c_str();

			string nodeCmdString = shadowNodePath + " launcher";
			int len1;
			int slength1 = (int)nodeCmdString.length() + 1;
			len1 = MultiByteToWideChar(CP_ACP, 0, nodeCmdString.c_str(), slength1, 0, 0);
			wchar_t* buf1 = new wchar_t[len1];
			MultiByteToWideChar(CP_ACP, 0, nodeCmdString.c_str(), slength1, buf1, len1);
			std::wstring r1(buf1);
			delete[] buf1;

			const wchar_t *ptr = r1.c_str();*/
		}
		else {
			nodeCmd = L"C:\Program Files\DirectFN Pro11 Price\\node.exe launcher";
			nodeWorkingDir = L"C:\\Program Files\\DirectFN Pro11 Price";

			// No shadow installtion directory
			if (szArgList == NULL || szArgList[1] == NULL)
			{
				myfile.open(tempFilePath);
				myfile.close();

				CSplash splash(TEXT(".\\splash.bmp"), RGB(0, 0, 0), inputMessage);
				splash.executeProgram(nodeCmd, nodeWorkingDir);

				LocalFree(szArgList);

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

				Sleep(10000);
				splash.CloseSplash();
			}
			else {
				int messageType = _wtoi(szArgList[1]);

				switch (messageType) {
				case 0:
					inputMessage = L"Application is initializing ...";

					myfile.open(tempFilePath);
					myfile.close();

					break;
				case 1: // Updates
					inputMessage = L"Application is updating. Please Wait ...";

					myfile.open(tempFilePath);
					myfile.close();

					break;

				case 2: // Restarting
					inputMessage = L"Application is Restarting ...";

					myfile.open(tempFilePath);
					myfile.close();

					break;

				case 3: // ReInstall
					inputMessage = L"Something went wrong. Please reinstall the app.";

					myfile.open(tempFilePath);
					myfile.close();

					break;

				default:
					myfile.open(tempFilePath);
					myfile.close();
				}

				CSplash splash(TEXT(".\\splash.bmp"), RGB(0, 0, 0), inputMessage);
				splash.ShowSplash();

				LocalFree(szArgList);

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

				Sleep(10000);
				splash.CloseSplash();
			}
		}
	}

	//}
	// application.exe in user data is called by the application.exe in the default directory
	//else if (source == 1) {
	//	string shadowNodeInstance = shadowInstallerDir + "//node.exe";

	//	if ((stat(shadowNodeInstance.c_str(), &buffer1) == 0)) {
	//		// Execute the node.exe in user directory
	//		std::wstring stemp = std::wstring(shadowNodeInstance.begin(), shadowNodeInstance.end());

	//		nodeWorkingDir = stemp.c_str();

	//		std::wstring mywstring(nodeWorkingDir);
	//		std::wstring concatted_stdstr = mywstring + L" launcher";
	//		nodeCmd = concatted_stdstr.c_str();
	//	}
	//	else {
	//		// execute the node.exe in defeult directory
	//		nodeCmd = L"C:\\Program Files\\DirectFN Pro11 Price\\node.exe launcher";
	//		nodeWorkingDir = L"C:\\Program Files\\DirectFN Pro11 Price";
	//	}

	//	splash.handleActions();

	//}

	//WaitForSingleObject(pi.hProcess, INFINITE);

	 // Close process and thread handles. 
	//CloseHandle(pi.hProcess);
	//CloseHandle(pi.hThread);
}





