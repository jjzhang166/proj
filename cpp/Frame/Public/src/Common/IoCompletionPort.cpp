#include "stdafx.h"
#include <Common/IoCompletionPort.h>

CIOCP::CIOCP(int nMaxConcurrency) { 
	m_hIOCP = NULL; 
	if (nMaxConcurrency != -1)
		(void) Create(nMaxConcurrency);
}

CIOCP::~CIOCP() { 
	if (m_hIOCP != NULL) 
		chVERIFY(CloseHandle(m_hIOCP)); 
}


BOOL CIOCP::Close() {
	BOOL bResult = CloseHandle(m_hIOCP);
	m_hIOCP = NULL;
	return(bResult);
}

BOOL CIOCP::Create(int nMaxConcurrency ) {
	m_hIOCP = CreateIoCompletionPort(
		INVALID_HANDLE_VALUE, NULL, 0, nMaxConcurrency);
	chASSERT(m_hIOCP != NULL);
	return(m_hIOCP != NULL);
}

BOOL CIOCP::AssociateDevice(HANDLE hDevice, ULONG_PTR CompKey) {
	BOOL fOk = (CreateIoCompletionPort(hDevice, m_hIOCP, CompKey, 0) 
		== m_hIOCP);
	chASSERT(fOk);
	return(fOk);
}

BOOL CIOCP::AssociateSocket(SOCKET hSocket, ULONG_PTR CompKey) {
	return(AssociateDevice((HANDLE) hSocket, CompKey));
}

BOOL CIOCP::PostStatus(ULONG_PTR CompKey, DWORD dwNumBytes, 
					   OVERLAPPED* po) 
{
	BOOL fOk = PostQueuedCompletionStatus(m_hIOCP, dwNumBytes, CompKey, po);
	chASSERT(fOk);
	return(fOk);
}

BOOL CIOCP::GetStatus(ULONG_PTR* pCompKey, PDWORD pdwNumBytes,
					  OVERLAPPED** ppo, DWORD dwMilliseconds ) 
{
	return(GetQueuedCompletionStatus(m_hIOCP, pdwNumBytes, 
		pCompKey, ppo, dwMilliseconds));
}
