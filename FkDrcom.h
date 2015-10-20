#include "stdafx.h"

extern "C" __declspec(dllexport) int FuckDrcom(void);

static int (WSAAPI * TrueWSAStartup)(
	_In_ WORD wVersionRequested,
	_Out_ LPWSADATA lpWSAData
	) = WSAStartup;

extern int WSAAPI RevisedWSAStartup(
	_In_ WORD wVersionRequested,
	_Out_ LPWSADATA lpWSAData
	);

static BOOL(WINAPI * TrueShellExecuteExW)(
	__inout SHELLEXECUTEINFOW *pExecInfo
	)
	= ShellExecuteExW;

extern BOOL WINAPI RevisedShellExecuteExW(
	__inout SHELLEXECUTEINFOW *pExecInfo
	);

static LSTATUS(APIENTRY * TrueRegOpenKeyExA)(
	__in HKEY hKey,
	__in_opt LPCSTR lpSubKey,
	__in_opt DWORD ulOptions,
	__in REGSAM samDesired,
	__out PHKEY phkResult
	) = RegOpenKeyExA;

extern LSTATUS APIENTRY RevisedRegOpenKeyExA(
	__in HKEY hKey,
	__in_opt LPCSTR lpSubKey,
	__in_opt DWORD ulOptions,
	__in REGSAM samDesired,
	__out PHKEY phkResult
	);

static BOOL(WINAPI * TrueProcess32Next)(
HANDLE hSnapshot,
LPPROCESSENTRY32 lppe
) = Process32Next;

extern BOOL WINAPI RevisedProcess32Next(
HANDLE hSnapshot,
LPPROCESSENTRY32 lppe
);