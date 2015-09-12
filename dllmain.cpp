// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

#include "detours.h"
#pragma comment(lib,"detours.lib")

#include <shellapi.h>

#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>

#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

static int (WSAAPI * TrueWSAStartup)(
	_In_ WORD wVersionRequested,
	_Out_ LPWSADATA lpWSAData
	) = WSAStartup;

int WSAAPI RevisedWSAStartup(
	_In_ WORD wVersionRequested,
	_Out_ LPWSADATA lpWSAData
	)
{
	return NULL;
}

static BOOL (WINAPI * TrueShellExecuteExW)(
	__inout SHELLEXECUTEINFOW *pExecInfo
	)
	= ShellExecuteExW;

BOOL WINAPI RevisedShellExecuteExW(
	__inout SHELLEXECUTEINFOW *pExecInfo
	)
{
	return TRUE;
}



static bool bHijacked1 = false;
static bool bHijacked2 = false;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		DisableThreadLibraryCalls(hModule);

		wchar_t szCurrentPath[MAX_PATH] = { 0 };
		::GetModuleFileName(NULL, szCurrentPath, _countof(szCurrentPath));

		if (wcsstr(szCurrentPath, _T("DrUpdate.exe")))
		{
			bHijacked1 = true;

			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
			DetourAttach(&(PVOID&)TrueWSAStartup, RevisedWSAStartup);
			DetourTransactionCommit();
		}
		else if (wcsstr(szCurrentPath, _T("DrClient.exe")))
		{
			if (1 == ::GetPrivateProfileInt(_T("Hook"), _T("ShellExecuteEx"), 1, _T(".\\FkDrcom.ini")))
			{
				bHijacked2 = true;
				DetourTransactionBegin();
				DetourUpdateThread(GetCurrentThread());
				DetourAttach(&(PVOID&)TrueShellExecuteExW, RevisedShellExecuteExW);
				DetourTransactionCommit();
			}
		}

		break;
	}
	case DLL_THREAD_ATTACH:break;
	case DLL_THREAD_DETACH:break;
	case DLL_PROCESS_DETACH:
	{
		//::MessageBoxW(NULL, _T("ÍË³ö"), _T("W"), MB_OK);
		if (bHijacked1)
		{
			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
			DetourDetach(&(PVOID&)TrueWSAStartup, RevisedWSAStartup);
			DetourTransactionCommit();
		}
		else if (bHijacked2)
		{
			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
			DetourDetach(&(PVOID&)TrueShellExecuteExW, RevisedShellExecuteExW);
			DetourTransactionCommit();
		}
		break;
	}
	}
	return TRUE;
}

