#include "stdafx.h"
#include <Common/WaitChainTraversal.h>
#include <Common/CmnDllApi.h>

CWCT::CWCT() {

	_hOLE32DLL = NULL;

	// Open the WCT session in synchronous mode
	_hWCTSession = OpenThreadWaitChainSession(0, NULL);
	if (_hWCTSession == NULL)
		return;

	// It is also possible to detect locks on COM calls
	InitCOM();   
}


void CWCT::InitCOM() {

	PCOGETCALLSTATE       CallStateCallback;
	PCOGETACTIVATIONSTATE ActivationStateCallback;

	// Load OLE32.DLL into the process to be able to get the address 
	// of the two COM functions required by RegisterWaitChainCOMCallback
	_hOLE32DLL = LibraryLoader::LoadTheLibrary(TEXT("OLE32.DLL"));

	CallStateCallback = (PCOGETCALLSTATE)
		LibFuncLoader::GetTheProcAddress(_hOLE32DLL, "CoGetCallState");
	ActivationStateCallback = (PCOGETACTIVATIONSTATE)
		LibFuncLoader::GetTheProcAddress(_hOLE32DLL, "CoGetActivationState");

	// Pass these COM helper functions to WCT
	RegisterWaitChainCOMCallback(CallStateCallback, ActivationStateCallback);
}


CWCT::~CWCT() {

	// Don't forget to unload OLE32.DLL
	if (_hOLE32DLL != NULL) {
		FreeLibrary(_hOLE32DLL);
	}

	// Don't forget to close the WCT session
	if (_hWCTSession != NULL) {
		CloseThreadWaitChainSession(_hWCTSession);
	}
}


void CWCT::ParseThreads(DWORD PID) {

	_PID = PID;

	// List all threads in the given process
	CToolhelp th(TH32CS_SNAPTHREAD, PID);
	THREADENTRY32 te = { sizeof(te) };
	BOOL fOk = th.ThreadFirst(&te);
	for (; fOk; fOk = th.ThreadNext(&te)) {
		// Only parse threads of the given process
		if (te.th32OwnerProcessID == PID) {
			ParseThread(te.th32ThreadID);
		}
	}
}


void CWCT::ParseThread(DWORD TID) {

	WAITCHAIN_NODE_INFO  chain[WCT_MAX_NODE_COUNT];
	DWORD                dwNodesInChain;
	BOOL                 bDeadlock;

	dwNodesInChain = WCT_MAX_NODE_COUNT;
	DWORD cnt = WCT_MAX_NODE_COUNT;
	// Get the chain for the current thread
	if (!GetThreadWaitChain(_hWCTSession, NULL, WCTP_GETINFO_ALL_FLAGS,
		TID, &dwNodesInChain, chain, &bDeadlock)) {

			_dwNodeCount = 0;
			OnThread(TID, FALSE, 0);
			return;
	}

	// Start the chain processing for the current thread
	_dwNodeCount = std::min(dwNodesInChain, cnt);
	OnThread(TID, bDeadlock, dwNodesInChain);

	// For each node in the chain, call the virtual method with details
	for (
		DWORD current = 0; 
		current < std::min(dwNodesInChain, cnt); 
	current++
		) {
			OnChainNodeInfo(TID, current, chain[current]);
	}
}


DWORD CWCT::GetNodesInChain() { 

	return(_dwNodeCount); 
}


DWORD CWCT::GetPID() {

	return(_PID);
}


void CWCT::OnThread(DWORD TID, BOOL bDeadlock, DWORD nodeCount) {

	// Virtual method to be implemented by derived classes
}


void CWCT::OnChainNodeInfo(DWORD rootTID, DWORD currentNode, 
						   WAITCHAIN_NODE_INFO nodeInfo) {

							   // Virtual method to be implemented by derived classes
}



//////////////////////////////// End of File //////////////////////////////////
