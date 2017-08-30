#include <windows.h>

extern bool Init();

BOOL __stdcall DllMain(HMODULE hDllHandle, DWORD dwReason, LPVOID lpreserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
	{
		DisableThreadLibraryCalls(hDllHandle);
		return Init();
	}
	case DLL_PROCESS_DETACH:
	{
		break;
	}
	}
	return TRUE;
}
