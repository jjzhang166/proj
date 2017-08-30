#pragma once
#include <tchar.h>
#include <boost/thread/lock_types.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/optional.hpp>
#include <ProtocolX/P_DefinesX.h>
#include "../DependSelfLib.h"

//////////////////////////////////////////////////////////////////////////
//****************网络协议*****************************************
const pt_word P_MaxWord = (pt_word)-1;
typedef pt_word P_ObjId;
typedef pt_word P_MsgNT;
typedef pt_word P_Operate;

struct P_FuncObjOper
{
	P_Operate	operate;
	P_MsgNT		msg_num;

	static P_FuncObjOper MakeOper( P_Operate first, P_MsgNT msg );
};

//内部协议
const P_Operate P_Operate_Inner = 1;
//用户消息
const P_Operate P_Operate_UserMsg = 2;

const P_MsgNT		kInnerMsg_SetSecrectKey = -1;
const P_MsgNT		kInnerMsg_JsonCall = -2;
const P_MsgNT		kInnerMsg_CoorTrans = -3;
const P_MsgNT		kInnerMsg_BufCall = -4;
//////////////////////////////////////////////////////////////////////////
typedef unsigned int P_CoorIdT;
static_assert(sizeof(P_CoorIdT) == sizeof(void*), "");
typedef unsigned int P_CoorTypeT;
const P_CoorTypeT kPCT_Any = P_CoorTypeT(-1 - 1000);
const P_CoorIdT kPCI_Ignore = 0;

enum enWayToNextCoor{
	kWTNC_ByType = 1,
	kWTNC_ById,
};
struct P_WayToNextCoor{
	enWayToNextCoor	method_;
	union{
		P_CoorIdT	coor_id_;
		P_CoorTypeT	coor_type_;
	};
};
//////////////////////////////////////////////////////////////////////////
//send or recv buffer max length
const pt_word P_BufferMaxLen = 256 * 2;
//send or recv buffer type
typedef char (P_BufferT)[P_BufferMaxLen];
//port
const pt_word P_Port_Game = 45398;
const pt_word P_Port_Login = 45399;
//协调服务，用于FIFA收金等
const pt_word P_Port_Coordinate = 45400;
//游戏登陆器之辅助登陆
//const pt_word P_Port_LoginAssist = 45500;

//self ip
const char* const kSelfIpStr = "127.0.0.1";

const P_ObjId kFuncObjTemplateId_CustomBase = 4000;
//funcobj模板ID
const P_ObjId kFuncObjTemplateId_Addition = 12;

#define DEF_THE_CUSTOM_TEMPLATE_OBJ_ID_( the_id ) (kFuncObjTemplateId_Addition + kFuncObjTemplateId_CustomBase + the_id)
/**/
const P_ObjId kFuncObjTemplateId_Account = 1 + kFuncObjTemplateId_Addition;
const P_ObjId kFuncObjTemplateId_Game = 2 + kFuncObjTemplateId_Addition;
const P_ObjId kFuncObjTemplateId_Dispatcher = 3 + kFuncObjTemplateId_Addition;
const P_ObjId kFuncObjTemplateId_Inter = 4 + kFuncObjTemplateId_Addition;
const P_ObjId kFuncObjTemplateId_GLua = 5 + kFuncObjTemplateId_Addition;
const P_ObjId KFuncObjTemplateId_JsonEcho = 6 + kFuncObjTemplateId_Addition;
//JsonEchoBase default
const P_ObjId kFuncObjTemplateId_JsonEchoBase = 7 + kFuncObjTemplateId_Addition;
//JsonEchoTrans default
const P_ObjId kFuncObjTemplateId_JsonEchoTrans = 8 + kFuncObjTemplateId_Addition;
const P_ObjId kFuncObjTemplateId_LuaScript = 9 + kFuncObjTemplateId_Addition;
const P_ObjId kFuncObjTemplateId_CoorTrans = 10 + kFuncObjTemplateId_Addition;
const P_ObjId kFuncObjTemplateId_JsonEchoCoorTrans = 11 + kFuncObjTemplateId_Addition;
const P_ObjId kFuncObjTemplateId_BufCallCoorTrans = 12 + kFuncObjTemplateId_Addition;
const P_ObjId kFuncObjTemplateId_Login = 12 + kFuncObjTemplateId_Addition;
const P_ObjId kFuncObjTemplateId_GameOrLogin = 13 + kFuncObjTemplateId_Addition;

#define SAFE_DELETE(x) { if (x) { delete (x); (x) = nullptr; } }(void*)0

#ifdef _AFXDLL
#define GET_RUNTIME_CLASS(T) (assert(T::GetThisClass()), T::GetThisClass())
#else
#define GET_RUNTIME_CLASS(T) (&T::class##T)
#endif

//shared mutex type
typedef boost::shared_mutex SharedMutex;
//read lock
typedef boost::shared_lock<SharedMutex> ReadLock;
//write lock
typedef boost::unique_lock<SharedMutex> WriteLock;

//帐号最大长度
const int kAccountMaxLen = 19;
//帐号最小长度
const int kAccountMinLen = 5;
//密码最大长度
const int kPasswordMaxLen = 19;
//密码最小长度
const int kPasswordMinLen = 5;

//////////////////////////////////////////////////////////////////////////
//*****************************database***********************************
//数据库名最大长度
const pt_dword kDbNameMaxLen = 20;
//项目名最大长度
const pt_dword kProjectNameMaxLen = 20;
//用户名最大长度
const pt_dword kUserNameMaxLen = 20;
//用户名最小长度
const pt_dword kUserNameMinLen = 5;
//用户密码最大长度
const pt_dword kUserPwdMaxLen = 20;
//用户密码最小长度
const pt_dword kUserPwdMinLen = 5;
//机器码的长度
const pt_dword kMachineCodeLen = kUuidStrLen;
//cookie的长度
const pt_dword kCookieStrLen = kUuidStrLenEx;
//留言版消息最大长度
const pt_dword kMsgBoardMsgMaxLen = 150;
//每天最多留言个数
const pt_dword kMsgBoardMaxCnt_EveryDay = 2;
//每个帐号总共最多留言个数
const pt_dword kMsgBoardMaxTotalCnt_OneAc = 20;
//qq最小长度
const pt_dword kQQMinLen = 4;
//qq最大长度
const pt_dword kQQMaxLen = 12;
//ip最大长度
const pt_dword kIPMaxLen = 15;
//卡ID的长度
const pt_dword kCardIdLen = kUuidStrLen;
const pt_dword kCardIdLenEx = kUuidStrLenEx;
//注册码的长度
const pt_dword kRegKeyIdLen = kUuidStrLen;
const pt_dword kRegKeyIdLenEx = kUuidStrLenEx;
//卡密码的长度
const pt_dword kCardPwdLen = kUuidStrLen;
const pt_dword kCardPwdLenEx = kUuidStrLenEx;
static_assert(kCardIdLen == kRegKeyIdLen, "");
static_assert(kCardIdLen == kCardPwdLen, "");
static_assert(kCardIdLenEx == kRegKeyIdLenEx, "");
static_assert(kCardIdLenEx == kCardPwdLenEx, "");
//指定的时间后，验证服务器会断开连接，单位:毫秒
const pt_dword kCheckSvrActiveTime = 1000 * 12;
//tbl_StrData之字符串最大长度
const pt_dword kTblStrDataMaxLen = 200;
//////////////////////////////////////////////////////////////////////////
struct P_StringCheck{
	static bool ValidUserName( pt_csz user_name );
	static bool ValidUserPwd( pt_csz user_pwd );
	static bool ValidMachineCode( pt_csz machine_code );
	static bool ValidMsgBoard( pt_csz msg );
	static bool ValidQQ( pt_csz qq );
	static bool ValidCard( pt_csz str );
	static bool ValidStrData( pt_csz str );
	static bool ValidCookie( pt_csz cookie );
};
//////////////////////////////////////////////////////////////////////////
//用户权限等级
enum enUserGrantLvl{
	kGUL_RestrictBind = 0,
	kUGL_PeerAcByCard,
	kUGL_PeerCardByAc,
	kUGL_PeerAcInfo,
	kUGL_PeerCardInfo,
	kUGL_RemoveBind,
	kUGL_MgrAcState,
	kUGL_PeerUsedCardInfo,
	kUGL_PeerAllCardInfo,
	kUGL_RemoveBindKing,
	kUGL_RollbackPay,
	kUGL_OpenCard,
	kUGL_GenRegKey,
	kUGL_ModifyProjData,
	kUGL_Invalid,
};

//用户的访问权限
typedef unsigned __int64 enUserGrant;
static_assert(sizeof(enUserGrant) * 8 >= kUGL_Invalid, "");

const enUserGrant kUG_No = 0;											//无权限
const enUserGrant kUG_RestrictBind = 1 << kGUL_RestrictBind;			//限制解绑帐号
const enUserGrant kUG_PeerAcByCard = 1 << kUGL_PeerAcByCard;			//通过卡号和卡密查询帐号信息
const enUserGrant kUG_PeerCardByAc = 1 << kUGL_PeerCardByAc;			//通过帐号和密码查询卡号信息
const enUserGrant kUG_PeerAcInfo = 1 << kUGL_PeerAcInfo;				//查看帐户信息
const enUserGrant kUG_PeerCardInfo = 1 << kUGL_PeerCardInfo;			//查看卡号信息
const enUserGrant kUG_RemoveBind = 1 << kUGL_RemoveBind;				//解绑权限（普通）
const enUserGrant kUG_MgrAcState = 1 << kUGL_MgrAcState;				//管理帐号状态，冻结、封号、解冻、解封
const enUserGrant kUG_PeerUsedCardInfo = 1 << kUGL_PeerUsedCardInfo;	//查看使用过的卡号信息
const enUserGrant kUG_PeerAllCardInfo = 1 << kUGL_PeerAllCardInfo;		//查看所有卡号信息
const enUserGrant kUG_RemoveBindKing = 1 << kUGL_RemoveBindKing;		//解绑权限（也可解绑限制解绑帐号）
const enUserGrant kUG_RollbackPay = 1 << kUGL_RollbackPay;				//支付回滚（卡依旧是使用过的，但是帐号的时间会被减去）
const enUserGrant kUG_OpenCard = 1 << kUGL_OpenCard;					//开卡权限
const enUserGrant kUG_GenRegKey = 1 << kUGL_GenRegKey;					//生成注册码的权限
const enUserGrant kUG_ModifyProjData = 1 << kUGL_ModifyProjData;		//修改项目数据权限
//代理的权限
const enUserGrant kUG_TheProxyPerson = kUG_RestrictBind | kUG_PeerAcByCard | kUG_PeerCardByAc | kUG_PeerAcInfo | kUG_PeerCardInfo | kUG_RemoveBind | kUG_MgrAcState | kUG_PeerUsedCardInfo;
//没有充值回滚权限
const enUserGrant kUG_KingLower = kUG_PeerAcByCard | kUG_PeerCardByAc | kUG_PeerAcInfo | kUG_PeerCardInfo | kUG_RemoveBind | kUG_MgrAcState | kUG_PeerUsedCardInfo | kUG_PeerAllCardInfo | kUG_RemoveBindKing | kUG_OpenCard | kUG_GenRegKey;
//所有权限
const enUserGrant kUG_TheKingMax = kUG_KingLower | kUG_RollbackPay | kUG_ModifyProjData;
//////////////////////////////////////////////////////////////////////////
enum class enAcState{
	kAS_Normal = 0,
	kAS_Freeze,				//冻结，无截止日期，时间不会流逝
	kAS_Ban,				//封号，有截止日期，时间会流逝
	kAS_Invalid,
};
//////////////////////////////////////////////////////////////////////////
/*
//腾讯登陆器的安全检查的事件对象的名字，目前未使用
const char* const kLogin_SafeCheckEventName = "0CFFBDDA408A41C2A693A0BB76E2451D";
*/

//验证控制端唯一实例事件对象
//#ifdef __PROJ_FIFA_MAIN_
//uuid of key
const char* const kRSA_UuidPrivate_CtoS = "8e395278-7fe6-592d-9f8a-3cc6e7a3ee98";
const char* const kRSA_UuidPublic_StoC = "a49b0d72-8668-5dda-8c1e-88ef08a470a5";

extern const std::string kStrPlaceholder;
//extern const std::wstring kWStrPlaceholder;

//math
//////////////////////////////////////////////////////////////////////////
const double kPi = 3.141592653589793238462643383279502884197169399375105820974944592307816;
//////////////////////////////////////////////////////////////////////////
const TCHAR* const kQxAqDllName = _T("_kolavb_.dll");
//登陆等待时间
const int kToServerWaitTime = 1000 * 60;
//////////////////////////////////////////////////////////////////////////
const char* const kLoginLocalInfo_FileMapNameS = "51A2002A-1B8E-4E81-AABE-52C86D4AB5A0";

enum enCrackCaptchaMode{
	kCCMode_DaMaTu,
	kCCMode_Local,
};

const char* const kGameDllVersion = "A434BD281F23460EB3B87D98F3F351F8";
const char* const kLoginDllVersion = "B6C866C183F34676BB0FD6B148D1DA23";
struct stAcSerInfo{
	template<class Archive>
	void serialize(Archive & ar){
		ar & ac_
			& pwd_
			& area_
			& ser_;
	}

	std::string		ac_;
	std::string		pwd_;
	std::string		area_;
	std::string		ser_;
};

struct stAsSerSessionInfo : public stAcSerInfo{
	std::string		login_process_info_;

	template<class Archive>
	void serialize(Archive & ar){
		__super::serialize(ar);
		ar & login_process_info_;
	}
};
//////////////////////////////////////////////////////////////////////////
//**************游戏相关
//创建角色结果
enum enCmnCreateRoleRes{
	kCmnCRR_NameErr,
	kCmnCRR_Error,
	kCmnCRR_Succeed,
};
//////////////////////////////////////////////////////////////////////////