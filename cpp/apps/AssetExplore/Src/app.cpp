#include "stdafx.h"
#include "app.h"
#include <AssetSys/AS_Sys.h>
#include <wx/cmdline.h>
#include <CmnMix/Cmn_UsefulFunc.h>


CMN_MY_WX_APP_DEFINE(App);

App::App()
{
}

void App::OnInitCmdLine(wxCmdLineParser& parser)
{
	static const wxCmdLineEntryDesc cmdLineDesc[] =
	{
		{ wxCMD_LINE_OPTION, wxLuaT("i"), wxLuaT("in"), wxLuaT("输入要处理的assf文件"),
		wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },

		{ wxCMD_LINE_OPTION, wxLuaT("o"), wxLuaT("out"), wxLuaT("要保存的文件"),
		wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },

		{ wxCMD_LINE_SWITCH, wxLuaT("c"), wxLuaT("console"), wxLuaT("输出到console"),
		wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL },
		
		{ wxCMD_LINE_NONE }
	};

	parser.SetDesc(cmdLineDesc);
}

bool App::OnCmdLineParsed(wxCmdLineParser& parser)
{
	wxString in;
	if (parser.Found(wxT("in"), &in))
	{
		wxString out;
		if (!parser.Found(wxT("out"), &out))
		{
			assert(false);
			return false;
		}
		static const std::string kSep(";");
		SeparateString(in.ToStdString(), kSep, in_files_);
		SeparateString(out.ToStdString(), kSep, out_files_);
		if (in_files_.size() != out_files_.size())
		{
			assert(false);
			return false;
		}
	}
	if (parser.Found(wxT("console")))
	{
		is_log_console_ = true;
	}
	return true;
}

int App::FileCount(bool is_in) const
{
	if (is_in)
		return in_files_.size();
	return out_files_.size();
}

const char* App::GetFileName(bool is_in, int idx) const
{
	if (is_in)
		return in_files_[idx].c_str();
	return out_files_[idx].c_str();
}

MyAppFactory* App::CreateAppFactory()
{
	return new MyAppFactory;
}

bool App::AutoAddCfgFiles(AS_NodePtr& node_root) const
{
	if (!node_root)
	{
		assert(false);
		return false;
	}
	AS_NodePtr lua_node;
	boost::filesystem::path path_lua("lua");
	if (node_root->GetPath() == path_lua)
		lua_node = node_root;
	else
	{
		lua_node = node_root->FindChild(path_lua);
		if (!lua_node)
			return false;
	}

	auto cfg_ame = LuaThrdData::GetRequireLuaFilesName();
	assert(cfg_ame);
	auto file_node = lua_node->FindPosterity(cfg_ame);
	if (!file_node)
		return false;
	Cmn_LuaCfgResolver resolver(lua_node, LuaThrdData::GetMe().lua(), Cmn_LuaCfgResolver::kNormal);
	auto file = resolver.AddFile(cfg_ame, 0);
	if (!file)
	{
		assert(false);
		return false;
	}
	auto the_file = static_cast<Cmn_LuaCfgFileBase_*>(file.get());
	if (!the_file)
	{
		assert(false);
		return false;
	}
	resolver.Resolve(kEmptyStr);
	if (!AS_Sys::GetMe().AddFilesByTbl(the_file->GetTable(), lua_node))
		return false;
	auto parent_node = file_node->Parent(false);
	if (!parent_node)
	{
		assert(false);
		return false;
	}
	if (!parent_node->Erase(file_node))
	{
		assert(false);
		return false;
	}
	lua_node->EraseEmptySubDir();
	return true;
}

AppLuaThrdData::AppLuaThrdData(lua_State* lstate) : LuaThrdData(lstate)
{

}

void AppLuaThrdData::BindClass()
{
	__super::BindClass();
	AS_Sys::RegForLua(lstate_);
	using namespace luabind;
	module(lstate_)
		[
			class_<App, WxLuaApp>("App")
			.def("FileCount", &App::FileCount)
			.def("GetFileName", &App::GetFileName)
			.def("AutoAddCfgFiles", &App::AutoAddCfgFiles),
			def("GetMyApp", &GetMyApp)
		];
}

LuaThrdData* MyAppFactory::CreateAppLuaVm(lua_State* lstate)
{
	return new AppLuaThrdData(lstate);
}
