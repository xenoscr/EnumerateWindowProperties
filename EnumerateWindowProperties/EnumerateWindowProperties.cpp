// EnumerateWindowProperties.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cstdlib>
#include <stdbool.h>

using namespace std;

#define BUFFER 4096

std::stringstream windowInfo;

void hexDump(char *desc, void *addr, int len);

/*
Convert strings to wide strings
*/
std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

/*
Gather the properties for a window process
*/

BOOL CALLBACK WinPropProc(HWND hwndSubclass, LPTSTR lpszString, HANDLE hData, ULONG_PTR dwData)
{
	ATOM atmText;
	atmText = GlobalFindAtom(lpszString);
	string strText;
	static int nProp = 1;
	HANDLE propHandle;

	char szBuf[MAX_PATH] = { 0 };
	GlobalGetAtomNameA(atmText, szBuf, _countof(szBuf));
	strText = szBuf;

	if (strcmp(szBuf, "UxSubclassInfo") == 0)
	{
		cout << windowInfo.str();
		cout << nProp++ << " : " << strText << "\n";
		std::wstring stemp = s2ws(strText);
		propHandle = GetProp(hwndSubclass, (LPCWSTR)stemp.c_str());
		cout << "Parent: " << hwndSubclass << " Property: " << propHandle << "\n";
		hexDump("None", &propHandle, 128);
	}
	return TRUE;
}

/*
Enumerate Child Windows
*/

BOOL CALLBACK EnumChildProc(HWND hWnd, LPARAM lParam)
{
	EnumPropsEx(hWnd, WinPropProc, NULL);
	return TRUE; /* Return TRUE or the enumeration will stop. */
}

/*
Enumerate Windows Processes
*/
BOOL CALLBACK enumWindowsProc(__in HWND hWnd, __in LPARAM lParam)
{
	int length = ::GetWindowTextLength(hWnd);
	if (length == 0) return TRUE;

	TCHAR* buffer;
	buffer = new TCHAR[length + 1];
	memset(buffer, 0, (length + 1) * sizeof(TCHAR));

	GetWindowText(hWnd, buffer, length + 1);
	tstring windowTitle = tstring(buffer);
	delete[] buffer;

	windowInfo.str("");
	std::string sWindowTitle(windowTitle.begin(), windowTitle.end());
	windowInfo << hWnd << ": " << sWindowTitle << "\n";

	EnumPropsEx(hWnd, WinPropProc, 0);
	EnumChildWindows(hWnd, EnumChildProc, 0);

	return TRUE;
}

/*
Dump memory
*/
void hexDump(char *desc, void *addr, int len)
{
	int i;
	unsigned char buff[17];
	unsigned char *pc = (unsigned char*)addr;

	// Output description if given.
	if (desc != NULL)
		printf("%s\n", desc);

	// Process every byte in the data.
	for (i = 0; i < len; i++)
	{
		if ((i % 16) == 0)
		{
			if (i != 0)
				printf(" %s\n", buff);

			// Output the offset.
			printf(" %04x ", i);
		}
		// Now the hex code for the specific character.
		printf(" %02x", pc[i]);

		// And store a printable ASCII character for later.
		if ((pc[i] < 0x20) || (pc[i] > 0x7e))
		{
			buff[i % 16] = '.';
		}
		else
		{
			buff[i % 16] = pc[i];
		}

		buff[(i % 16) + 1] = '\0';
	}
	// Pad out last line if not exactly 16 characters.
	while ((i % 16) != 0)
	{
		printf("    ");
		i++;
	}
	// And print the final ASCII bit.
	printf("  %s\n", buff);
}


int main()
{
	std::wstring s2ws(const std::string& s);

	HWND hWnd;
	int result = 0;

	wcout << TEXT("Enumerating Windows...") << endl;
	BOOL enumeratingWindowsSucceeded = ::EnumWindows(enumWindowsProc, NULL);
	cin.get();
    return 0;
}

