#pragma once

#define LPK_CHECK_EVENT			"FFDFFF9A-5EDC-40a5-96E4-C5395B2CFFE7"
#define LPK_CHEKC_FILEMAPPING	"E750A151-1227-4c88-92BF-F0EEFCFD2A69"
#define LPK_CHECK_VIEWOFFILE	"{0A46BE53-0E77-49eb-A4BC-7AAEDF68118A}{3C7BB159-A43D-4822-9B40-33CAD4D3BDC6}"
#define LPK_CHEKC_FILEMAPPING1	"97B57C9B-79FF-4b29-BD18-9C3E3E3AF7A0"
#define LPK_CHECK_VIEWOFFILE1	"5E4DC4A9-91F5-4f7c-BF89-AA28A83413D6"

class CheckLpk
{
private:
	HANDLE	m_hEvent;
	HANDLE	m_hMapFile;
	PVOID	m_pView;
public:
	CheckLpk(void);
	~CheckLpk(void);
public:
	// 创建事件对象	参数一：要被创建事件对象的名称
	bool LpkCreateEvent(const char* pszObjEvent);
	void LpkCloseEvent();

	// 打开事件对象 参数一：已经被创建事件对象的名称
	bool LpkOpenEvent(const char* pszObjEvent);

	// 创建内存共享对象 参数一：要被创建共享内存对象的名称
	bool LpkCreateFileMapping(const char* pszFileMappingObj);

	// 写入数据到共享内存 参数一：要被写入的数据
	bool LpkFlushViewOfFile( const char* pszData, int str_len );

	// 打开内存共享对象 参数一：已经被创建的内存共享对象的名称 参数二：读取数据的参数
	bool LpkOpenFileMapping(const char* pszFileMapping, OUT char* pszOutData);
	PVOID LpkOpenConsoleFileMapping(const char* pszFileMapping);

	// 异或加密
	bool xor(const char* pszData, char* pszOutData);

};

extern CheckLpk	g_theCheckLpk;;


///////////////////////////////////////////////////////////////////////////////////////
// 函数功能：	开启LPK验证接口
// 参数：		预留接口，暂时无用,传 NULL 即可
// 返回值：		true 表示成功， false 表示失败
///////////////////////////////////////////////////////////////////////////////////////
extern bool OpenCheckLpk(LPVOID lpParameter);
