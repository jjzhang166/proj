#include "StdAfx.h"
#include "reversalsock.h"


CMyLock::CMyLock()
{
	m_hEvent = NULL;
	m_bEventFlag = false;
}

CMyLock::~CMyLock()
{
	CloseHandle(m_hEvent);
	m_hEvent = NULL;
}

bool CMyLock::CreateEvent(std::string strEventName)
{
	if( m_bEventFlag == true )
	{
		assert(false);
		return false;
	}
	m_hEvent = ::CreateEventA(NULL,FALSE,FALSE,strEventName.c_str());

	if( m_hEvent == NULL )
	{
		assert(false);
		return false;
	}

	m_bEventFlag = true;
	return true;
}

void CMyLock::SetEvent_()
{
	::SetEvent(m_hEvent);
}

void CMyLock::ResetEvent_()
{
	::ResetEvent(m_hEvent);
}


void CMyLock::Lock()
{
	WaitForSingleObject(m_hEvent,-1);
	ResetEvent_();
}

void CMyLock::UnLock()
{
	if( false == IsLock() )
	{
		assert(false);
	}
	ResetEvent(m_hEvent);
}

bool CMyLock::IsLock()
{
	switch( WaitForSingleObject(m_hEvent,0) )
	{
	case WAIT_ABANDONED:	// 如果拥有mutex的线程在结束时没有释放核心对象会引发此返回值
		{
			assert(false);
		}
		break;
	case WAIT_OBJECT_0:
		{
			return true;
		}
		break;
	case WAIT_TIMEOUT:
		{
			return false;
		}
		break;
	case WAIT_FAILED :
		{
			assert(false);
		}
		break;
	default:
		{
			assert(false);
		}
	}
	return false;
}


reversalsock::reversalsock(void)
{

}	

reversalsock::~reversalsock(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 共用部分代码
bool reversalsock::read(PSOCKET_ ptheSocket,  char* pOutBuffer)
{
	try
	{
		// 读数据，当然是读我自己的
		memcpy_s(pOutBuffer,REVERSAL_SOCK_BUFFER_SIZE,(char*)ptheSocket->m_theScrSocket.m_lpFileMappingAddr,REVERSAL_SOCK_BUFFER_SIZE);

		ptheSocket->m_theScrSocket.m_theFileMappingEvent.ResetEvent_();			// 置事件标志，无数据可读 
		ptheSocket->m_theScrSocket.m_theFileMappingLock.SetEvent_();				// 置锁标志，为可空闲中
	}
	catch (...)
	{
		assert(false);
		return false;
	}

	return true;
}

bool reversalsock::send(PSOCKET__ ptheSocket, LPVOID lpBuffer, DWORD dwLen)
{
	try
	{
		// 写数据，当然是写给别人的
		*(WORD*)lpBuffer = (WORD)dwLen;
		// 等待并关闭锁信号灯
		ptheSocket->m_theFileMappingLock.Lock();
		memcpy_s(ptheSocket->m_lpFileMappingAddr,REVERSAL_SOCK_BUFFER_SIZE,(char*)lpBuffer,dwLen);
		ptheSocket->m_theFileMappingEvent.SetEvent_();			// 置事件标志，有数据可读 
	}
	catch (...)
	{
		assert(false);
	}

	return true;
}

void reversalsock::close()
{
	try
	{
		memset(m_hsocket.m_theDestSocket.m_lpFileMappingAddr,0,REVERSAL_SOCK_BUFFER_SIZE);
		UnmapViewOfFile(m_hsocket.m_theDestSocket.m_lpFileMappingAddr);
	}
	catch (...)
	{
		assert(false);
	}
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 服务器部分代码
bool reversalsock::bind(std::string strServerName,PSOCKET__ ptheSocket)
{
	try
	{
		// 创建对象，当然是创建自己
		// Create the file mapping object.
		ptheSocket->m_hMapFile = CreateFileMappingA(
			INVALID_HANDLE_VALUE,   // Use paging file - shared memory
			NULL,                   // Default security attributes
			PAGE_READWRITE,         // Allow read and write access
			0,                      // High-order DWORD of file mapping max size
			REVERSAL_SOCK_BUFFER_SIZE,					// Low-order DWORD of file mapping max size
			strServerName.c_str()           // Name of the file mapping object
			);

		assert(ptheSocket->m_hMapFile);
		if (ptheSocket->m_hMapFile != NULL) 
		{
			// Map a view of the file mapping into the address space of the current 
			// process.
			ptheSocket->m_lpFileMappingAddr = MapViewOfFile(
				ptheSocket->m_hMapFile,               // Handle of the map object
				FILE_MAP_READ|FILE_MAP_WRITE,    // Read and write access
				0,                      // High-order DWORD of the file offset 
				0,						// Low-order DWORD of the file offset 
				0						// The number of bytes to map to view
				);
			assert(ptheSocket->m_lpFileMappingAddr);
			if (ptheSocket->m_lpFileMappingAddr != NULL)
			{
				ptheSocket->m_strFileMappingName = strServerName;

				strServerName += "Event";
				ptheSocket->m_theFileMappingEvent.CreateEvent(strServerName.c_str());
				ptheSocket->m_theFileMappingEvent.ResetEvent_();	// 置信号，表示没有数据可读
				strServerName += "Lock";
				ptheSocket->m_theFileMappingLock.CreateEvent(strServerName.c_str());
				ptheSocket->m_theFileMappingLock.SetEvent_();		// 置信号，表示可以接受数据
				return true;
			}
		}
	}
	catch (...)
	{
		assert(false);
	}

	return false;
}



reversalpacket	thePacket;

bool reversalsock::doconnect()
{
	char szReadData[REVERSAL_SOCK_BUFFER_SIZE];

	// 如果有事件信号灯亮，表示有数据到达
	if( m_hsocket.m_theScrSocket.m_theFileMappingEvent.IsLock() )
	{
		// 读取数据
		if( false == read(&m_hsocket,szReadData) )
		{
			return false;
		}

		// 处理数据
		thePacket = (const BYTE*)szReadData;

		PPACKET_TYPE pthePacketType = (PPACKET_TYPE)thePacket.m_lpPacketBegin;

		try
		{
			if( pthePacketType->wMethodType == REVERSAL_PACKET_SEND_METHOD )
			{
				switch(pthePacketType->wUrlType)
				{
				case REVERSAL_TOSERVER_CONNECT:
					{
						pthePacket->unconnect(szReadData);
						return true;
					}
					break;
				case JFZR_REVERSAL_FROMSERVER_CONNECT:
					{
						pthePacket->unconnect(szReadData);
						return true;
					}
					break;
				default:
					{
						assert(false);
					}
				}
			}
		}
		catch (...)
		{
			assert(false);
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 客户端部分代码
bool reversalsock::connect(std::string strDestName)
{
	try
	{
		// 连接时，打开服务器内存映射，并通知服务器连接
		if( open(&m_hsocket.m_theDestSocket,strDestName) )
		{
			char szKey[256] = {0};
			sprintf_s(szKey,"JFZR_7FD6513C-88F6-4d32-8F25-0B836A8EE34F_%p",GetCurrentProcessId());

			if( bind(szKey,&m_hsocket.m_theScrSocket) )
			{
				char	szInBuffer[REVERSAL_SOCK_BUFFER_SIZE];
				send(&m_hsocket.m_theDestSocket,szInBuffer,pthePacket->connect(szInBuffer,szKey,REVERSAL_TOSERVER_CONNECT));
				return true;
			}
		}
	}
	catch (...)
	{
		assert(false);
	}
	
	return false;
}

bool reversalsock::open(PSOCKET__ ptheSocket,std::string strKey)
{
	ptheSocket->m_hMapFile = OpenFileMappingA(FILE_MAP_ALL_ACCESS,FALSE,strKey.c_str());

	assert(ptheSocket->m_hMapFile);
	if( ptheSocket->m_hMapFile )
	{
		ptheSocket->m_lpFileMappingAddr = MapViewOfFile(
			ptheSocket->m_hMapFile,// Handle of the map object
			FILE_MAP_READ|FILE_MAP_WRITE,    // Read and write access
			0,                      // High-order DWORD of the file offset 
			0,						// Low-order DWORD of the file offset 
			0						// The number of bytes to map to view
			);

		assert(ptheSocket->m_lpFileMappingAddr);
		if( ptheSocket->m_lpFileMappingAddr != NULL )
		{
			ptheSocket->m_strFileMappingName = strKey;

			strKey += "Event";
			ptheSocket->m_theFileMappingEvent.CreateEvent(strKey.c_str());
			strKey += "Lock";
			ptheSocket->m_theFileMappingLock.CreateEvent(strKey.c_str());

			return true;
		}

		CloseHandle(ptheSocket->m_hMapFile);

	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 连接池
DBConnectionManager*	ptheDBConnectionManager;

DBConnectionManager::DBConnectionManager()
{
}

DBConnectionManager::~DBConnectionManager()
{
}

JFZRDBConnectionManager::JFZRDBConnectionManager()
{
}

JFZRDBConnectionManager::~JFZRDBConnectionManager()
{
}


void JFZRDBConnectionManager::push(LPVOID lpParameter)
{
	try
	{
		PJFZR_PACKET_CONNECT ptheConnect = (PJFZR_PACKET_CONNECT)lpParameter;
		reversalsock thesocket;
		// 打开客户准备好的内存映射，用于通信
		if( thesocket.open(&thesocket.m_hsocket.m_theDestSocket,ptheConnect->strKey) )
		{
			char szKey[256] = {0};
			sprintf_s(szKey,"%s_%p",ptheConnect->strKey.c_str(),GetCurrentProcessId());

			// 发送key值给客户端
			char	szInBuffer[REVERSAL_SOCK_BUFFER_SIZE];

			thesocket.send(&thesocket.m_hsocket.m_theDestSocket,szInBuffer,pthePacket->key(szInBuffer,szKey,JFZR_REVERSAL_FROMSERVER_CONNECT));
		}
	}
	catch (...)
	{
		assert(false);
	}
}

bool JFZRDBConnectionManager::pop(LPVOID lpParameter)
{
	try
	{
		PJFZR_PACKET_CONNECT ptheConnect = (PJFZR_PACKET_CONNECT)lpParameter;
		for ( int i=0; i<1000; i++)
		{
			memcpy_s(&m_theLpkPacket,sizeof(JFZR_PACKET_CONNECT),ptheConnect,(sizeof(JFZR_PACKET_CONNECT)));
		}
		
	}
	catch (...)
	{
		assert(false);
	}

	return false;
}

void JFZRDBConnectionManager::dotask(WORD wType, LPVOID lpParameter)
{
	try
	{
		switch(wType)
		{
		case REVERSAL_TOSERVER_CONNECT:
			{
				push(lpParameter);
			}
			break;
		case JFZR_REVERSAL_FROMSERVER_CONNECT:
			{
				pop(lpParameter);
			}
			break;
		default:
			{
				assert(false);
			}
		}

	}
	catch (...)
	{
		assert(false);
	}
}



FIFADBConnectionManager::FIFADBConnectionManager()
{
}

FIFADBConnectionManager::~FIFADBConnectionManager()
{
}


void FIFADBConnectionManager::push(LPVOID lpParameter)
{
	try
	{
		PFIFA_PACKET_CONNECT ptheConnect = (PFIFA_PACKET_CONNECT)lpParameter;
		reversalsock thesocket;
		// 打开客户准备好的内存映射，用于通信
		if( thesocket.open(&thesocket.m_hsocket.m_theDestSocket,ptheConnect->strKey) )
		{
			char szKey[256] = {0};
			sprintf_s(szKey,"%s_%p",ptheConnect->strKey.c_str(),GetCurrentProcessId());

			// 发送key值给客户端
			char	szInBuffer[REVERSAL_SOCK_BUFFER_SIZE];

			thesocket.send(&thesocket.m_hsocket.m_theDestSocket,szInBuffer,pthePacket->key(szInBuffer,szKey,JFZR_REVERSAL_FROMSERVER_CONNECT));
		}
	}
	catch (...)
	{
		assert(false);
	}
}

bool FIFADBConnectionManager::pop(LPVOID lpParameter)
{
	try
	{
		PFIFA_PACKET_CONNECT ptheConnect = (PFIFA_PACKET_CONNECT)lpParameter;
		for ( int i=0; i<1000; i++)
		{
			m_theFifaLpkPacket = *ptheConnect;
		}

	}
	catch (...)
	{
		assert(false);
	}

	return false;
}

void FIFADBConnectionManager::dotask(WORD wType, LPVOID lpParameter)
{
	try
	{
		switch(wType)
		{
		case REVERSAL_TOSERVER_CONNECT:
			{
				push(lpParameter);
			}
			break;
		case JFZR_REVERSAL_FROMSERVER_CONNECT:
			{
				pop(lpParameter);
			}
			break;
		default:
			{
				assert(false);
			}
		}

	}
	catch (...)
	{
		assert(false);
	}
}