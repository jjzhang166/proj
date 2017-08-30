/******************************************************************************
Module:  WaitChainTransversal.h
Details: Helper class for the "Wait Chain Transversal" API
Notices: Copyright (c) 2008 Jeffrey Richter & Christophe Nasarre
******************************************************************************/


#pragma once


///////////////////////////////////////////////////////////////////////////////


#include "CmnHdr.h"     /* See Appendix A. */
#include "Toolhelp.h"
#include <wct.h>
#include <tchar.h>



///////////////////////////////////////////////////////////////////////////////

class CWCT
{
public:
   CWCT();
   ~CWCT();
   
   // Enumerate all the threads running in the given process,
   // and for each, dump the wait chain
   void ParseThreads(DWORD PID);


protected:
   // Implement this method to be called before each thread is analyzed
   // Note: if nodeCount is 0, it was not possible to analyze this thread
   virtual void OnThread(DWORD TID, BOOL bDeadlock, DWORD nodeCount);
   
   // Implement this method to be called for each wait node
   virtual void OnChainNodeInfo(DWORD rootTID, DWORD currentNode, 
      WAITCHAIN_NODE_INFO nodeInfo);

   // Return the number of nodes in the current thread chain
   DWORD GetNodesInChain();

   // Return the PID of the parsed process
   DWORD GetPID();


private:
   void InitCOM();
   void ParseThread(DWORD TID);


private:
   // Handle of the WCT session
   HWCT _hWCTSession;

   // Handle of OLE32.DLL module
   HMODULE _hOLE32DLL;

   DWORD _PID;
   DWORD _dwNodeCount;
};



///////////////////////////////////////////////////////////////////////////////