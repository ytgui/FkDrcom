// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "FkDrcom.h"


static bool bHijacked1 = false;
static bool bHijacked2 = false;

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		//::MessageBoxW(NULL, _T("½øÈë"), _T("W"), MB_OK);
		DisableThreadLibraryCalls(hModule);

		wchar_t szCurrentPath[MAX_PATH] = { 0 };
		::GetModuleFileName(NULL, szCurrentPath, _countof(szCurrentPath));

		if (wcsstr(szCurrentPath, _T("DrUpdate.exe")))
		{
			//::MessageBoxW(NULL, _T("DrUpdate.exe"), _T("W"), MB_OK);
			bHijacked1 = true;

			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
			DetourAttach(&(PVOID&)TrueWSAStartup, RevisedWSAStartup);
			DetourTransactionCommit();
		}
		else if (wcsstr(szCurrentPath, _T("DrClient.exe")))
		{
			//::MessageBoxW(NULL, _T("DrClient.exe"), _T("W"), MB_OK);
			bHijacked2 = true;
			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
			if (1 == ::GetPrivateProfileInt(_T("Hook"), _T("ShellExecuteEx"), 1, _T(".\\FkDrcom.ini")))
			{
				DetourAttach(&(PVOID&)TrueShellExecuteExW, RevisedShellExecuteExW);
			}
			DetourAttach(&(PVOID&)TrueProcess32Next, RevisedProcess32Next);
			DetourTransactionCommit();
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
			if (1 == ::GetPrivateProfileInt(_T("Hook"), _T("ShellExecuteEx"), 1, _T(".\\FkDrcom.ini")))
			{
				DetourDetach(&(PVOID&)TrueShellExecuteExW, RevisedShellExecuteExW);
			}
			DetourDetach(&(PVOID&)TrueProcess32Next, RevisedProcess32Next);
			DetourTransactionCommit();
		}
		break;
	}
	}
	return TRUE;
}

