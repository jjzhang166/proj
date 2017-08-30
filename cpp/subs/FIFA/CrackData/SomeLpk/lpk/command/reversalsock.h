#ifndef JFZR_REVERSAL_SOCK_91044C184E5B46508DDB073DA8DEFC42
#define JFZR_REVERSAL_SOCK_91044C184E5B46508DDB073DA8DEFC42

#pragma once
#include "reversalpacket.h"
#include <string>
#include <list>



class CMyLock  
{
public:
	void Lock();
	void UnLock();
	bool IsLock();
	bool CreateEvent(lpkstring strEventName);
	void SetEvent_();
	void ResetEvent_();
	void close()
	{
		CloseHandle(m_hEvent);
	}
private:
	HANDLE	m_hEvent;
	bool	m_bEventFlag;
public:
	CMyLock();
	virtual ~CMyLock();
};

typedef struct
{
	HANDLE			m_hMapFile;
	lpkstring		m_strFileMappingName;
	LPVOID			m_lpFileMappingAddr;
	CMyLock			m_theFileMappingLock;				// 锁
	CMyLock			m_theFileMappingEvent;				// 事件
}SOCKET__,*PSOCKET__;

typedef struct
{
	SOCKET__	m_theDestSocket;		// 我写你的，你写我的
	SOCKET__	m_theScrSocket;			// 我读我的，你读你的
}SOCKET_,*PSOCKET_;

class reversalsock
{
public:
	SOCKET_			m_hsocket;
public:
	reversalsock(void);
	~reversalsock(void);
public:
	bool open(PSOCKET__ ptheSocket,lpkstring strKey);
	bool read(PSOCKET_ ptheSocket, char* pOutBuffer);
	bool send(PSOCKET__ ptheSocket, LPVOID lpBuffer, DWORD dwLen);

	bool bind(lpkstring strServerName,PSOCKET__ ptheSocket);
	bool doconnect();

	bool connect(lpkstring strDestName);
	void close();
};

//using namespace std;


class DBConnectionManager
{
public:
	DBConnectionManager();
	~DBConnectionManager();
public:
	JFZR_PACKET_CONNECT		m_theLpkPacket;
	FIFA_PACKET_CONNECT		m_theFifaLpkPacket;
public:
	virtual void push(LPVOID lpParameter) = 0;
	virtual bool pop(LPVOID lpParameter) = 0;
	virtual void dotask(WORD wType, LPVOID lpParameter) = 0;
};

//extern DBConnectionManager*	ptheDBConnectionManager;


class JFZRDBConnectionManager : public DBConnectionManager
{
public:
	JFZRDBConnectionManager();
	~JFZRDBConnectionManager();
public:
	virtual void push(LPVOID lpParameter);
	virtual bool pop(LPVOID lpParameter);
	virtual void dotask(WORD wType, LPVOID lpParameter);
};

class FIFADBConnectionManager : public DBConnectionManager
{
public:
	FIFADBConnectionManager();
	~FIFADBConnectionManager();
public:
	
public:
	virtual void push(LPVOID lpParameter);
	virtual bool pop(LPVOID lpParameter);
	virtual void dotask(WORD wType, LPVOID lpParameter);
};

extern FIFADBConnectionManager theFIFADBConnectionManager;
#endif