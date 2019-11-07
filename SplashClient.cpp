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

using namespace std;

string profile = getenv("USERPROFILE");
string userDataDir = profile + "\\AppData\\Local\\DirectFN";
string tempFilePath = userDataDir + "\\temp.dat";
string shadowInstallerDir = userDataDir + "\\DirectFN_Pro11_Price_4294798646\\shadowInstaller";
string shadowAppPath = shadowInstallerDir + "\\application.exe";
string shadowNodePath = shadowInstallerDir + "\\node.exe";
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

	/**Check if the shadow entry point exists
	 * in the user directory
	 */
	if ((stat(shadowAppPath.c_str(), &buffer1) == 0)) {
		string command = shadowAppPath;

		/** Check if any argument is passsed and recreate 
		 *  the command accordingly
		 */
		if (szArgList != NULL && szArgList[1] != NULL)
		{
			int argNum = _wtoi(szArgList[1]);
			string arg = to_string(argNum);

			command = command + " " + arg;
		}

		nodeCmd = processHandler.stringToLPCWSTR(command);
		nodeWorkingDir = processHandler.stringToLPCWSTR(shadowInstallerDir);

		processHandler.executeProgram(nodeCmd, nodeWorkingDir,false);
	}
	else {
		if ((stat(shadowNodePath.c_str(), &buffer1) == 0)) {
			nodeWorkingDir = processHandler.stringToLPCWSTR(shadowInstallerDir);

			if (szArgList == NULL || szArgList[1] == NULL)
			{
				nodeCmd = processHandler.concatLPCWSTR(processHandler.stringToLPCWSTR(shadowNodePath), L" launcher");
				processHandler.executeProgram(nodeCmd, nodeWorkingDir,true);
			}
			else {
				int messageType = _wtoi(szArgList[1]);

				if (messageType == 10) {
					nodeCmd = processHandler.concatLPCWSTR(processHandler.stringToLPCWSTR(shadowNodePath), L" launchAfterUpdate");

					processHandler.executeProgram(nodeCmd, nodeWorkingDir,false);
				}
				else {
					inputMessage = processHandler.getInputMessage(messageType);
					processHandler.showSplash(inputMessage);
				}
			}
		}
		else {
			nodeWorkingDir = L"C:\\Program Files\\DirectFN Pro11 Price";

			if (szArgList == NULL || szArgList[1] == NULL)
			{
				nodeCmd = L"C:\\Program Files\\DirectFN Pro11 Price\\node.exe launcher";

				processHandler.executeProgram(nodeCmd, nodeWorkingDir, true);
			}
			else {
				int messageType = _wtoi(szArgList[1]);

				if (messageType == 10) {
					nodeCmd = L"C:\\Program Files\\DirectFN Pro11 Price\\node.exe launchAfterUpdate";

					processHandler.executeProgram(nodeCmd, nodeWorkingDir,false);
				}
				else {
					inputMessage = processHandler.getInputMessage(messageType);
					processHandler.showSplash(inputMessage);
				}
				//LocalFree(szArgList);
			}
		}
	}
}





