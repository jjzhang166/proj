#pragma once
/*
@author zhp
@date 2017/4/2 18:28
@purpose for against tp
*/
#include <CmnMix/Cmn_FileMap.h>
#include <CommonX/CmnX_UsefulClass.h>

struct stAtpDummyBugTrace{
	int				h_safe_check_done_;				//hook安全检查完成,SecureCheck.dll的相对偏移
	int				o_msg_hook_proc_;
	int				edit_pwd_pos_x_;
	int				edit_pwd_pos_y_;
	unsigned long	edit_pwd_gwl_style_;					//窗口风格

	stAtpDummyBugTrace(){
		h_safe_check_done_ = 0;
		o_msg_hook_proc_ = 0;
		edit_pwd_pos_x_ = 0;
		edit_pwd_pos_y_ = 0;
		edit_pwd_gwl_style_ = 0;
	}
};

class Atp_MemFileMap : public MemFileMap, public Singleton<Atp_MemFileMap, Singleton_MakeMe>{
	//type definition
public:
	struct stCommunicate{
		bool			safe_check_done_;			//安全检查
		bool			pwd_input_error_;			//密码输入错误
		bool			ac_freeze_;					//帐号被冻结
		bool			login_succeed_;				//登录成功
		bool			begin_input_pwd_;			//开始输入帐号
		bool			end_input_pwd_;
		bool			input_pwd_has_except_;
		char			pwd_[50];

		stCommunicate();
	};

public:
	Atp_MemFileMap();
	//Read
	const stCommunicate* ReadAsCommu() const;
	//Write
	stCommunicate* WriteAsCommu();
};
