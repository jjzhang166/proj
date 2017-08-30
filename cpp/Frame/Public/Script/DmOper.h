#pragma once
/*
@author zhp
@date 2017/1/17 17:33
@purpose dm operate
*/
#include <dm/dm.h>

class DmOper : public DmSoft{
public:
	static void RegForLua(lua_State* l);
	//@p2:posX.@p3:posY.
	bool InputBindWndString(lua_State* l, const std::string& str);
	bool MouseDrag(long pos_x, long pos_y, long new_pos_x, long new_pos_y);
	luabind::object SplitFindPicRes(lua_State* l, const std::string& str_res, bool is_pic_idx);

private:
	luabind::object SplitFindPicRes_2_(lua_State* l, const std::string& str_res);
};