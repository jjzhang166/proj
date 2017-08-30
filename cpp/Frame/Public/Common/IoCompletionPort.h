/******************************************************************************
Module:  IOCP.h
Notices: Copyright (c) 2007 Jeffrey Richter & Christophe Nasarre
Purpose: This class wraps an I/O Completion Port.
         See Appendix B.
******************************************************************************/


#pragma once   // Include this header file once per compilation unit


///////////////////////////////////////////////////////////////////////////////


#include "CmnHdr.h"              // See Appendix A.
#include "../DependSelfLib.h"

///////////////////////////////////////////////////////////////////////////////


class CIOCP {
public:
   CIOCP(int nMaxConcurrency = -1);

   ~CIOCP();
   BOOL Close();
   BOOL Create(int nMaxConcurrency = 0);
   BOOL AssociateDevice(HANDLE hDevice, ULONG_PTR CompKey);
   BOOL AssociateSocket(SOCKET hSocket, ULONG_PTR CompKey);
   BOOL PostStatus(ULONG_PTR CompKey, DWORD dwNumBytes = 0, 
      OVERLAPPED* po = NULL);
   BOOL GetStatus(ULONG_PTR* pCompKey, PDWORD pdwNumBytes,
      OVERLAPPED** ppo, DWORD dwMilliseconds = INFINITE);
private:
   HANDLE m_hIOCP;
};


///////////////////////////////// End of File /////////////////////////////////
