#pragma once
#include <string>
#include <fstream>
#include "splash.h"

using namespace std;
class ProcessHandler
{
public:

	LPCWSTR stringToLPCWSTR(string str);

	LPCWSTR concatLPCWSTR(LPCWSTR str1, LPCWSTR str2);

	void executeProgram(LPCWSTR command, LPCWSTR workingDir, bool isShowSPlash);

	LPCTSTR getInputMessage(int arg);

	void showSplash(LPCTSTR inputMessage);

private:
	int subscribeForSplashClose(CSplash splash);

};

