#include "stdafx.h"
#include <CmnMix/Cmn_EnsureCleanup.h>

 CEnsureReleaseRegion::CEnsureReleaseRegion(PVOID pv) : m_pv(pv) { }

  CEnsureReleaseRegion::~CEnsureReleaseRegion() { Cleanup(); }

  PVOID CEnsureReleaseRegion::operator=(PVOID pv) { 
	  Cleanup(); 
	  m_pv = pv; 
	  return(m_pv); 
  }
  CEnsureReleaseRegion::operator PVOID() { return(m_pv); }
  void CEnsureReleaseRegion::Cleanup() { 
	  if (m_pv != NULL) { 
		  VirtualFree(m_pv, 0, MEM_RELEASE); 
		  m_pv = NULL; 
	  } 
  }

  CEnsureHeapFree::CEnsureHeapFree(PVOID pv , HANDLE hHeap ) 
	  : m_pv(pv), m_hHeap(hHeap) { }
 CEnsureHeapFree::~CEnsureHeapFree() { Cleanup(); }

  PVOID CEnsureHeapFree::operator=(PVOID pv) { 
	  Cleanup(); 
	  m_pv = pv; 
	  return(m_pv); 
  }
  CEnsureHeapFree::operator PVOID() { return(m_pv); }
  void CEnsureHeapFree::Cleanup() { 
	  if (m_pv != NULL) { 
		  HeapFree(m_hHeap, 0, m_pv); 
		  m_pv = NULL; 
	  } 
  }