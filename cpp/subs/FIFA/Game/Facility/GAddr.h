#pragma once
/*
@author zhp
@date 2013-12-31
@purpose 存放函数地址或全局变量的地址
*/
#include "Common/MemOper.h"
#include <CD_CommonFiles/CD_Updates.h>
//g_f_s_->stdcall function
//g_f_c_->cdecl function
//g_f_t_->thiscall function
//g_f_a_->其他调用约束的函数
//g_v_->虚函数表
//g_sa_->单字节字符串
//g_sw_->宽字节字符串
namespace GAddr{
	/*
	发送http包
	@1:对象地址
	@2:指向发送内容(ASCII字符串)
	@3:封包有效数据长度
	*/
	//SSL_read-->int 	SSL_read(SSL *ssl,void *buf,int num);
	typedef MemOper<Oper_Cdecl> MO_SslRead;
	//extern MO_SslRead						g_pSSL_read;
	const MO_SslRead								g_pSSL_read(UPDATES_FIFA_REVERSAL_SSL_READ);
	//SSL_write->int SSL_write(SSL *ssl, const void *buf, int num);
	typedef MemOper<Oper_Cdecl> MO_SslWrite;
	//extern MO_SslWrite								g_pSSL_write;
	const MO_SslWrite								g_pSSL_write(UPDATES_FIFA_REVERSAL_SSL_WRITE);
	//////////////////////////////////////////////////////////////////////////
	//SSL_get_error, int SSL_get_error(SSL *ssl, int ret)
	const MemOper<Oper_Cdecl>				g_pSSL_get_error(UPDATES_FIFA_REVERSAL_SSL_GET_ERROR);
	const unsigned int						g_pHttpSendCall_ = UPDATES_FIFA_REVERSAL_HTTP_SEND_CALL;		// 发包CALL
	//hook发包call
	//const MemOper<Oper_Cdecl>				g_pHttpSendCall(g_pHttpSendCall_);
	/*
	用来得到主窗口句柄
	hook Call CreateWindowEx的下一个指令
	*/
	const unsigned int						g_pHookForGainMainWndHandle_ = UPDATES_FIFA_REVERSAL_HOOK_FOR_GAIN_MAIN_WND_HANDLE;	// 窗口句柄
	//malloc of game
	const MemOper<Oper_Cdecl, MemOper_RetrieveAddr> g_pGMalloc(UPDATES_FIFA_REVERSAL_MEMOPER_G_MALLOC);	
	//calloc of game
	const MemOper<Oper_Cdecl, MemOper_RetrieveAddr> g_pGCalloc(UPDATES_FIFA_REVERSAL_MEMOPER_G_CALLOC);
	//free of game
	const MemOper<Oper_Cdecl, MemOper_RetrieveAddr> g_pGFree(UPDATES_FIFA_REVERSAL_MEMOPER_G_FREE);
	//发送键盘、鼠标消息，获取对象地址call
	const MemOper<Oper_Cdecl>					g_pGetMouseKeyEventHandler(UPDATES_FIFA_REVERSAL_MEMOPER_GET_MOUSE_KEY_EVENT_HANDLER);
	//收包call(-4)
	/*const unsigned int							g_pHookRecvCallAddr_ = UPDATES_FIFA_REVERSAL_HOOK_RECV_CALL_ADDR;
	const unsigned int							g_pHookRecvCallAddr1_ = UPDATES_FIFA_REVERSAL_HOOK_RECV_CALL_1;
	const unsigned int							g_pHookRecvCallAddr2_ = UPDATES_FIFA_REVERSAL_HOOK_RECV_CALL_2;
	const unsigned int							g_pHookRecvCallAddr3_ = UPDATES_FIFA_REVERSAL_HOOK_RECV_CALL_3;*/
	const unsigned int							g_pHookRecvCallAddr_ = UPDATES_FIFA_REVERSAL_HOOK_RECV_CALL_ADDR_ZHP;
	//dx GetDeviceState函数调用后的下一条指令的地址
	const unsigned int							g_pHookAfterGetDeviceState_ = UPDATES_FIFA_REVERSAL_HOOK_AFTER_GET_DEVICE_STATE;
	//hook动画特征
	const unsigned int							g_pHookAnimationFeature_ = UPDATES_FIFA_REVERSAL_HOOK_ANIMATION_FEATURE;
	//顶层命令函数
	pt_cdword									g_pHookTopCmdFn = UPDATES_FIFA_REVERSAL_HOOK_TOP_COMMAND_FN;
	//hook并更改创建DX input的标志：后台与共享模式
	const unsigned int							g_pHookDxInputModel = UPDATES_FIFA_REVERSAL_HOOK_DX_INPUT_MODEL;
	//hook渲染线程，sleep，用于降低CPU
	const unsigned int							g_pHookRenderThd_Sleep = UPDATES_FIFA_REVERSAL_STOP_RENDER_AND_SLEEP_HOOK;
	//游戏主线程调用SSL_read，收到一个完整的HTTP包后执行的地方
	const unsigned int							g_pHookAfterGameReadFullHttpPackage = UPDATES_FIFA_REVERSAL_HOOK_AFTER_GAME_READ_FULL_HTTP_PACKAGE;
	//游戏主线程调用SSL_read，执行错误后执行的地方
	//const unsigned int							g_pHookAfterGameReadFullHttpPackage_ByError = 0x00F2A24F;
	//match time hook
	extern int				g_pSetMatchTimeAddr;
	//////////////////////////////////////////////////////////////////////////
	const unsigned int							g_pHookGainSslHandle = UPDATES_FIFA_REVERSAL_SSL_READ_AND_WRITE_SOCKET_ADDR_HOOK;
	extern volatile int							g_hSslHandle;
	//////////////////////////////////////////////////////////////////////////
}