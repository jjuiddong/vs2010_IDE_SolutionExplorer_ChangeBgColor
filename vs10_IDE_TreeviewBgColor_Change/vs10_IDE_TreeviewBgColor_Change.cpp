// vs10_IDE_TreeviewBgColor_Change.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include <windows.h>
#include "psapi.h"
#include "shlwapi.h"
#include "commctrl.h"
#include <iostream>

#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "Psapi.lib") 

COLORREF clr = RGB(220,220,220);

BOOL CALLBACK wenum( HWND hwnd, LPARAM lParam)
{
	const UINT cb = 261;
	static wchar_t    name[] = L"SysTreeView32",
		tmp[cb] = {0};
	if( ::GetClassNameW( hwnd, tmp, 260 ) && 0 == _wcsicmp( name, tmp ) )
	{
		::SendMessageW( hwnd, TVM_SETBKCOLOR, 0, (LPARAM)clr );
	}

	return TRUE;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) 
{
	DWORD dwThreadId  = 0, dwProcessId = 0;
	//HINSTANCE  hInstance;
	static wchar_t derVS[]     = L"devenv.exe";
	wchar_t  name[_MAX_PATH]   = {0}, *exe = 0;

	HANDLE hProcess;
	if (!hwnd)  return TRUE;     // Not a window
	if (!::IsWindowVisible(hwnd)) return TRUE;       // Not visible

	if (!SendMessage(hwnd, WM_GETTEXT, sizeof(name), (LPARAM)name))
		return TRUE;      // No window title
	dwThreadId = GetWindowThreadProcessId(hwnd, &dwProcessId);
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
	if( !GetModuleFileNameEx(hProcess, 0, name, sizeof(name))) goto exit;

	exe = ::PathFindFileNameW( name );
	if( (void*)exe == (void*)name ) goto exit; // mhm? maybe not exit?

	if( _wcsicmp( derVS, exe ) ) goto exit;

	EnumChildWindows( hwnd, wenum, (LPARAM)hProcess );

exit:
	CloseHandle(hProcess);
//	int res = GetLastError();
//	return res;
	return true;
}

int wmain(int argc, wchar_t * argv[]) 
{
	if( argc >= 4 )
	{
		wchar_t *end = 0;
		long r = wcstol( argv[1], &end, 0 );
		long g = wcstol( argv[2], &end, 0 );
		long b = wcstol( argv[3], &end, 0 );

		clr = RGB(r,g,b);
		//long l = wcstol( argv[2], &end, 16 );
		//clr = (DWORD)l;
	}
	::EnumWindows(EnumWindowsProc, NULL);
	return 0;
}