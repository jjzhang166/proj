#include <windows.h>
#include <boost/python.hpp>

extern bool Init();
int is_inited = 0;


void AddSelfModulePath(HMODULE hDllHandle)
{
	using namespace boost::python;
	char buffer[MAX_PATH];
	GetModuleFileNameA(hDllHandle, buffer, _countof(buffer));
	auto it = strrchr(buffer, '\\');
	assert(it);
	if (it)
		*it = 0;

	object main_module = import("__main__");
	object main_namespace = main_module.attr("__dict__");
	dict l;
	l["mypath"] = buffer;
	exec(R"(
import sys
def _InitSelfPath(path, sys):
    _new = path.lower()
    for i in sys.path:
        if i.lower() == _new:
            return
    sys.path.append(path)

_InitSelfPath(mypath, sys)
)", main_namespace, l);
}

BOOL __stdcall DllMain(HMODULE hDllHandle, DWORD dwReason, LPVOID lpreserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
	{
		DisableThreadLibraryCalls(hDllHandle);
		is_inited = Py_IsInitialized();
		if (!is_inited)
			Py_Initialize();
		try{
			AddSelfModulePath(hDllHandle);
		}
		catch (...)
		{
			PyErr_Print();
			PyErr_Clear();
		}

		return Init();
	}
	case DLL_PROCESS_DETACH:
	{
		if (!is_inited)
			Py_Finalize();
		break;
	}
	}
	return TRUE;
}
