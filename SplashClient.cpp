#include "stdafx.h"
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

#include "splash.h"
#include "ProcessHandler.h"
#include <direct.h>

#define GetCurrentDir _getcwd

using namespace std;


LPCTSTR inputMessage = L"";

LPCWSTR nodeCmd;
LPCWSTR nodeWorkingDir;

int APIENTRY WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	ProcessHandler processHandler;

	LPWSTR* szArgList;
	int argCount;
	szArgList = CommandLineToArgvW(GetCommandLine(), &argCount);
	struct stat buffer1;

	string profile= getenv("USERPROFILE");
	string wd = profile + "\\AppData\\Local\\DirectFN\\DirectFN_Pro11_Price_QA_4294798646\\assets\\shadowInstaller\\app";

	/*char buff[MAX_PATH];
	getcwd(buff, MAX_PATH);
	std::string wd(buff);*/

	std::wstring parsed_wd = std::wstring(wd.begin(), wd.end());
	nodeWorkingDir = parsed_wd.c_str();

	if (szArgList == NULL || szArgList[1] == NULL)
	{
		std::wstring mywstring(nodeWorkingDir);
		std::wstring concatted_stdstr = mywstring + L"\\node.exe launcher";
		nodeCmd = concatted_stdstr.c_str();

		processHandler.executeProgram(nodeCmd, nodeWorkingDir, L"");
	}
	else {
		int messageType = _wtoi(szArgList[1]);

		if (messageType == 0) {
			std::wstring mywstring(nodeWorkingDir);
			std::wstring concatted_stdstr = mywstring + L"\\node.exe launcher";
			nodeCmd = concatted_stdstr.c_str();

			inputMessage = L"Application is initializing ...";
			processHandler.executeProgram(nodeCmd, nodeWorkingDir, inputMessage);
		}
		else if (messageType == 10) {
			std::wstring mywstring(nodeWorkingDir);
			std::wstring concatted_stdstr = mywstring + L"\\node.exe launchAfterUpdate";
			nodeCmd = concatted_stdstr.c_str();

			processHandler.executeProgram(nodeCmd, nodeWorkingDir);
		}else if (messageType == 11) {
			std::wstring mywstring(nodeWorkingDir);
			std::wstring concatted_stdstr = mywstring + L"\\node.exe silentLaunchAfterUpdate";
			nodeCmd = concatted_stdstr.c_str();

			processHandler.executeProgram(nodeCmd, nodeWorkingDir);
		}
		else {
			inputMessage = processHandler.getInputMessage(messageType);
			processHandler.showSplash(inputMessage);
		}
	}
}





