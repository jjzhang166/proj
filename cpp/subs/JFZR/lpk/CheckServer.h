#pragma once

class CheckServer
{
public:
	bool	m_bRunExeFlag;
	char	m_szReleaseResExeFile[260];

public:
	CheckServer(void);
	~CheckServer(void);

public:
	// 运行检测进程
	bool Check();

	// 恢复执行文件
	bool UnTenioDL(void);

	bool Close();
};

extern CheckServer g_theCheckServer;