#include "stdafx.h"
#include <Script/DmOper.h>
#include <Common/UsefulFunc.h>
#include <luabind/raw_call_policy.hpp>
#include <luabind/raw_policy.hpp>
#include <CmnMix/Cmn_LuaOper.h>
#include <boost/lexical_cast.hpp>
#include <CmnMix/Cmn_LogOut.h>

void DmOper::RegForLua(lua_State* l)
{
	DmSoft::RegForLua(l);
	using namespace luabind;
	module(l)[
		class_<DmOper, DmSoft>("DmOper")
		.def(constructor<>())
		.def("InputBindWndString", &DmOper::InputBindWndString, raw_call(_2))
		.def("MouseDrag", &DmOper::MouseDrag)
		.def("SplitFindPicRes", &DmOper::SplitFindPicRes, raw(_2))
		.def("SplitFindPicRes", &DmOper::SplitFindPicRes_2_, raw(_2))
	];
}

bool DmOper::InputBindWndString(lua_State* l, const std::string& str)
{
	using namespace luabind;
	if (str.empty())
	{
		assert(false);
		return false;
	}
	auto hwnd = GetBindWindow();
	if (!hwnd)
	{
		assert(false);
		return false;
	}
	auto top_n = lua_gettop(l);
	if (top_n < 2)
	{
		assert(false);
		return false;
	}
	int pos_x = -1, pos_y = -1;
	if (top_n >= 4)
	{
		object tmp(from_stack(l, 3));
		pos_x = LuaObjCast(tmp, pos_x);
		if (pos_x >= 0)
		{
			object(from_stack(l, 4)).swap(tmp);
			pos_y = LuaObjCast(tmp, pos_y);
		}
	}
	if (pos_x >= 0 && pos_y >= 0)
	{
		MoveTo(pos_x, pos_y);
		LeftClick();
	}
	if (0 == SendString(hwnd, str))
	{
		assert(false);
		return false;
	}
	return true;
}

bool DmOper::MouseDrag(long pos_x, long pos_y, long new_pos_x, long new_pos_y)
{
	MoveTo(pos_x, pos_y);
	LeftDown();
	DummySleep(50);
	MoveTo(new_pos_x, new_pos_y);
	LeftUp();
	return true;
}

luabind::object DmOper::SplitFindPicRes_2_(lua_State* l, const std::string& str_res)
{
	return SplitFindPicRes(l, str_res, true);
}

luabind::object DmOper::SplitFindPicRes(lua_State* l, const std::string& str_res, bool is_pic_idx)
{
	using namespace luabind;
	object res;
	if (str_res.empty())
		return res;
	static const std::string kSep1("|");
	typedef std::vector<std::string> OutStrT;
	OutStrT out_str;
	SeparateString(str_res, kSep1, out_str);
	if (out_str.empty())
		return res;
	newtable(l).swap(res);
	static const std::string kSep2(",");
	int tbl_idx = 0;
	for (auto& str : out_str)
	{
		OutStrT out_v;
		SeparateString(str, kSep2, out_v);
		if (out_v.empty())
		{
			assert(false);
			continue;
		}
		if (out_v.size() != 3)
		{
			assert(false);
			continue;
		}
		try
		{
			object tbl_item = newtable(l);
			if (is_pic_idx)
				tbl_item[1] = boost::lexical_cast<long>(out_v[0]);
			else
				tbl_item[1] = out_v[0];
			tbl_item[2] = boost::lexical_cast<long>(out_v[1]);
			tbl_item[3] = boost::lexical_cast<long>(out_v[2]);
			++tbl_idx;
			res[tbl_idx] = tbl_item;
		}
		catch (boost::bad_lexical_cast& e)
		{
			LOG_O(Log_debug) << e.what();
			assert(false);
			continue;
		}
	}
	return res;
}
