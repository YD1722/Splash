#pragma once
#include <string>
#include <fstream>
#include "splash.h"

using namespace std;
class ProcessHandler
{
public:

	LPCWSTR stringToLPCWSTR(string str);

	wstring concatLPCWSTR(LPCWSTR str1, LPCWSTR str2);

	void executeProgram(LPCWSTR command, LPCWSTR workingDir, LPCWSTR inputMessage);
	void executeProgram(LPCWSTR command, LPCWSTR workingDir);

	LPCTSTR getInputMessage(int arg);

	int showSplash(LPCTSTR inputMessage);

};

