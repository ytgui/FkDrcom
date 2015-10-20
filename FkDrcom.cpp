// FkDrcom.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "FkDrcom.h"

int FuckDrcom(void)
{
	return 0;
}

int WSAAPI RevisedWSAStartup(
	_In_ WORD wVersionRequested,
	_Out_ LPWSADATA lpWSAData
	)
{
	return NULL;
}

BOOL WINAPI RevisedShellExecuteExW(
	__inout SHELLEXECUTEINFOW *pExecInfo
	)
{
	return TRUE;
}

LSTATUS APIENTRY RevisedRegOpenKeyExA(
	__in HKEY hKey,
	__in_opt LPCSTR lpSubKey,
	__in_opt DWORD ulOptions,
	__in REGSAM samDesired,
	__out PHKEY phkResult
	)
{
	if (HKEY_LOCAL_MACHINE == hKey)
	{
		if (strstr(lpSubKey, "4D36E972-E325-11CE-BFC1-08002bE10318"))//MAC
		{
			phkResult = NULL;
			::ExitThread(0);
			return ERROR_ACCESS_DENIED;
		}
	}
	return TrueRegOpenKeyExA(hKey, lpSubKey, ulOptions, samDesired, phkResult);
}

BOOL WINAPI RevisedProcess32Next(
	HANDLE hSnapshot,
	LPPROCESSENTRY32 lppe
	)
{
	::ExitThread(0);
	return FALSE;
}