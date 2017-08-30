#pragma once
/*
@author zhp
@date 2014/6/2 10:44
@purpose for login helper address
*/

class tagHookSafeCheckDone{};
const unsigned int		g_o_HookSafeCheckDone(0x2531C);		//hook安全检查完成,SecureCheck.dll的相对偏移
extern void*			g_pCheckLoginFailed;
//const int				g_o_login_succeed = 0xA24B;