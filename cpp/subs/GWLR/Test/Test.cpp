// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WinBase.h>
#include "Test.h"
#include <cassert>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include "FuncObjs/FuncObjInter.h"
#include "Common/Hooker.h"
#include "Common/GObjProxy.h"
#include <algorithm>
#include <boost/locale/encoding.hpp>
#include "Common/JsonMore.h"
#include <sstream>
#include <fstream>
#include "Json/writer.h"
#include <set>
#include "Common/Toolhelp.h"
#include "Common/UsefulFunc.h"
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/thread.hpp>
#include <boost/optional.hpp>
#include <Poco/DateTime.h>
#include <Poco/Timestamp.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/Format.h>
#include <Common/Database.h>
#include <Common/MakeSecret.h>
#include <Script/JsonCall.h>
#include <Socket/PocoMore.h>
#include <hash_set>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/name_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <Common/UsefulClass.h>
#include <Socket/IoTimer.h>
#include <boost/uuid/uuid_io.hpp>

#include <Poco/Crypto/Cipher.h>
#include <Poco/Crypto/CipherFactory.h>
#include <Poco/Crypto/RSAKey.h>
#include <Poco/Crypto/CipherKey.h>
#include <Poco/Crypto/X509Certificate.h>
#include <Protocol/P_PriDefines.h>
#include <strstream>
#include <Poco/StreamCopier.h>
#include <boost/regex.hpp>
#include <Common/CmnImageHlp.h>
#include <Common/CmnDllApi.h>

using namespace boost;
using namespace std;
using namespace Poco::Crypto;
void _1_CopyAndGenLpkUuid();
void _2_GenNewVersion();
void _3_RenameConsoleOfVmpAndGenUuid();
void _4_CopyGameLoginDllAndCopyVersionToDirection();


int main(){
	auto oh = CmnImageHlp::AnalyaisImage(GetModuleHandle(nullptr));
	CmnImageHlp::GetPEImageSize(GetModuleHandle(nullptr));
	LibraryLoader::LoadTheLibrary("d3d9.dll");

	//_1_CopyAndGenLpkUuid();
	//_2_GenNewVersion();
	//_3_RenameConsoleOfVmpAndGenUuid();
	//_4_CopyGameLoginDllAndCopyVersionToDirection();
	std::system("pause");
	return 0;

}

//生成新的rsa密钥文件
/*
int main(){
	std::string public_stoc = "f:\\tmp\\public_StoC_FI.pem", private_stoc = "f:\\tmp\\private_StoC_FI.pem",
		public_ctos = "f:\\tmp\\public_CtoS_FI.pem", private_ctos = "f:\\tmp\\private_CtoS_FI.pem";

	GenNewRsaKeyFile(public_stoc, private_stoc, kRSA_PriPassphrase_StoC);
	GenNewRsaKeyFile(public_ctos, private_ctos, kRSA_PriPassphrase_CtoS);
	auto uuid_public_stc = GenUuidStrOfFile(public_stoc), uuid_private_ctos = GenUuidStrOfFile(private_ctos);
	return 0;
}*/

//复制lpka和lpkb，复制lpkb到各个目录中，并计算lpkb的uuid
void _1_CopyAndGenLpkUuid()
{
	std::string file_lpka_src = "F:\\proj\\GWLR\\CrackData\\AntiCrack\\lpka.dll";
	/*std::string file_lpka_dst = "F:\\proj\\GWLR\\Console\\res\\lpka.dll";
	std::cout << "开始复制lpka，源：" << file_lpka_src << "\n目标：" << file_lpka_dst << std::endl;
	if (CopyUnsameFile(file_lpka_src, file_lpka_dst))
	{
		std::cout << "复制lpka成功";
	}
	else
	{
		std::cout << "lpka不需要复制";
	}
	std::cout << endl;*/
	std::string file_lpkb_src = "F:\\proj\\GWLR\\CrackData\\AntiCrack\\lpkb.dll";
	std::string file_lpkb_d_src = "F:\\proj\\GWLR\\CrackData\\AntiCrack\\lpkb_d.dll";
	std::string file_lpk_src = "F:\\proj\\GWLR\\CrackData\\AntiCrack\\lpk.dll";
	std::string file_lpkb_mb_d = "F:\\proj\\GWLR\\D_MB\\",
		file_lpkb_xe_d = "F:\\proj\\GWLR\\D_XE\\",
		file_lpkb_mbp_d = "F:\\proj\\GWLR\\D_MBP\\",
		file_lpkb_mb = "F:\\proj\\GWLR\\R_MB\\",
		file_lpkb_mbp = "F:\\proj\\GWLR\\R_MBP\\",
		file_lpkb_xe = "F:\\proj\\GWLR\\R_XE\\", file_name_lpk = "lpk.dll", file_name_lpkb = "lpkb.dll", file_name_lpka = "lpka.dll";
	std::cout << "开始复制lpkb到以下目录：\n" << /*file_lpkb_mb_d << "\n" << file_lpkb_xe_d << "\n" <<*/ file_lpkb_mb << "\n" << file_lpkb_xe << "\n" << file_lpkb_mbp << std::endl;
	int succeed_cnt = 0;
	//succeed_cnt += !!CopyUnsameFile(file_lpkb_src, file_lpkb_mb_d + file_name_lpkb);
	//succeed_cnt += !!CopyUnsameFile(file_lpkb_src, file_lpkb_xe_d + file_name_lpkb);
	succeed_cnt += !!CopyUnsameFile(file_lpkb_src, file_lpkb_mb + file_name_lpkb);
	succeed_cnt += !!CopyUnsameFile(file_lpkb_src, file_lpkb_xe + file_name_lpkb);
	succeed_cnt += !!CopyUnsameFile(file_lpkb_src, file_lpkb_mbp + file_name_lpkb);
	std::cout << "\n需要复制lpkb的次数为：" << succeed_cnt << "\n开始复制lpk.dll" << std::endl;
	succeed_cnt = 0;
	succeed_cnt += !!CopyUnsameFile(file_lpk_src, file_lpkb_mb_d + file_name_lpk);
	succeed_cnt += !!CopyUnsameFile(file_lpk_src, file_lpkb_xe_d + file_name_lpk);
	succeed_cnt += !!CopyUnsameFile(file_lpk_src, file_lpkb_mb + file_name_lpk);
	succeed_cnt += !!CopyUnsameFile(file_lpk_src, file_lpkb_xe + file_name_lpk);
	succeed_cnt += !!CopyUnsameFile(file_lpk_src, file_lpkb_mbp_d + file_name_lpk);
	succeed_cnt += !!CopyUnsameFile(file_lpk_src, file_lpkb_mbp + file_name_lpk);
	std::cout << "\n需要复制lpk的次数为：" << succeed_cnt << "\n开始复制lpka.dll" << std::endl;
	succeed_cnt = 0;
	succeed_cnt += !!CopyUnsameFile(file_lpka_src, file_lpkb_mb_d + file_name_lpka);
	succeed_cnt += !!CopyUnsameFile(file_lpka_src, file_lpkb_xe_d + file_name_lpka);
	succeed_cnt += !!CopyUnsameFile(file_lpka_src, file_lpkb_mb + file_name_lpka);
	succeed_cnt += !!CopyUnsameFile(file_lpka_src, file_lpkb_xe + file_name_lpka);
	succeed_cnt += !!CopyUnsameFile(file_lpka_src, file_lpkb_mbp_d + file_name_lpka);
	succeed_cnt += !!CopyUnsameFile(file_lpka_src, file_lpkb_mbp + file_name_lpka);
	std::cout << "\n需要复制lpka的次数为：" << succeed_cnt << "\n开始复制lpkb_d.dll" << std::endl;
	succeed_cnt = 0;
	succeed_cnt += !!CopyUnsameFile(file_lpkb_d_src, file_lpkb_mb_d + file_name_lpkb);
	succeed_cnt += !!CopyUnsameFile(file_lpkb_d_src, file_lpkb_xe_d + file_name_lpkb);
	succeed_cnt += !!CopyUnsameFile(file_lpkb_d_src, file_lpkb_mbp_d + file_name_lpkb);
	std::cout << "\n需要复制lpkb_d的次数为：" << succeed_cnt << std::endl;

	std::string file_safe_check = "F:\\proj\\GWLR\\Console\\SafeCheck.h";
	if (Regex_ModifySubStrOfFileByStrTag(file_safe_check, "kUuidStr_SafeCheckLpk", GenUuidStrOfFile(file_lpkb_src)))
		std::cout << "lpkb的uuid修改成功\n";
	else
		std::cout << "lpkb的uuid不需要修改\n";
}
//生成全新的版本号
void _2_GenNewVersion()
{
	int succeed_cnt = 0;
	std::string file_cg_define = "F:\\proj\\GWLR\\Communicate\\CG_Defines.h";
	succeed_cnt += !!Regex_ModifySubStrOfFileByStrTag(file_cg_define, "kStableStr_ConsoleToGame_mb", RandomUniqueStr());
	succeed_cnt += !!Regex_ModifySubStrOfFileByStrTag(file_cg_define, "kStableStr_ConsoleToGame_xe", RandomUniqueStr());
	succeed_cnt += !!Regex_ModifySubStrOfFileByStrTag(file_cg_define, "kStableStr_ConsoleToGame_mbp", RandomUniqueStr());

	std::string file_cgl_define = "F:\\proj\\GWLR\\Communicate\\CGL_Defines.h";
	succeed_cnt += !!Regex_ModifySubStrOfFileByStrTag(file_cgl_define, "kAssistVersionUuidGenerator_mb", RandomUniqueStr());
	succeed_cnt += !!Regex_ModifySubStrOfFileByStrTag(file_cgl_define, "kAssistVersionUuidGenerator_xe", RandomUniqueStr());
	succeed_cnt += !!Regex_ModifySubStrOfFileByStrTag(file_cgl_define, "kAssistVersionUuidGenerator_mbp", RandomUniqueStr());

	std::string file_cl_define = "F:\\proj\\GWLR\\Communicate\\CL_Defines.h";
	succeed_cnt += !!Regex_ModifySubStrOfFileByStrTag(file_cl_define, "kStableStr_ConsoleToLogin_mb", RandomUniqueStr());
	succeed_cnt += !!Regex_ModifySubStrOfFileByStrTag(file_cl_define, "kStableStr_ConsoleToLogin_xe", RandomUniqueStr());
	succeed_cnt += !!Regex_ModifySubStrOfFileByStrTag(file_cl_define, "kStableStr_ConsoleToLogin_mbp", RandomUniqueStr());
	std::cout << "\n" << file_cg_define << "\n" << file_cgl_define << "\n" << file_cl_define << "\n这几个文件版本字符串修改成功次数：" << succeed_cnt << std::endl;

	succeed_cnt = 0;
	std::string file_server_ip_info = "F:\\proj\\GWLR\\Console\\ServerIpInfo.h";
	succeed_cnt += !!Regex_ModifySubStrOfFileByStrTagIncInt(file_server_ip_info, "kConsoleMinVer");
	std::cout << "控制台版本号修改成功次数：" << succeed_cnt << std::endl;
}

int RenameConsoleName( const std::string& file_path )
{
	using namespace boost::filesystem;
	path file_name_vmp(file_path + "Console.vmp.exe");
	int succeed_cnt = 0;
	std::string file_console = file_path + "Console.exe";
	if (exists(file_name_vmp))
	{
		boost::system::error_code err;
		remove_all(file_console, err);
		if (err)
		{
			std::cout << err.message() << std::endl;
		}
		else
		{
			rename(file_name_vmp, file_console, err);
			if (err)
			{
				std::cout << err.message() << std::endl;
			}
			else
			{
				++succeed_cnt;
			}
		}
	}
	return succeed_cnt;
}
//重命令VMP后的控制台，并计算console的uuid
void _3_RenameConsoleOfVmpAndGenUuid()
{
	using namespace boost::filesystem;
	std::string file_name_console = "Console.exe";
	const std::string str_path_mb = "F:\\proj\\GWLR\\R_MB\\";
	int succeed_cnt = RenameConsoleName(str_path_mb);

	std::string str_path_xe = "F:\\proj\\GWLR\\R_XE\\";
	succeed_cnt += RenameConsoleName(str_path_xe);

	std::string str_path_mbp = "F:\\proj\\GWLR\\R_MBP\\";
	succeed_cnt += RenameConsoleName(str_path_mbp);

	std::cout << "Console.vmp.exe改为Console.exe,成功次数：" << succeed_cnt << std::endl;

	std::string file_main_lua = "F:\\proj\\GWLR\\Game\\luac\\main.lua";
	succeed_cnt = 0;
	succeed_cnt += !!Regex_ModifySubStrOfFile(file_main_lua, "g_str_MakeMatchSubStrmb.+\".+\"", "g_str_MakeMatchSubStrmb = \"" + GenUuidStrOfFile(str_path_mb + file_name_console) + "\"");
	succeed_cnt += !!Regex_ModifySubStrOfFile(file_main_lua, "g_str_MakeMatchSubStrxe.+\".+\"", "g_str_MakeMatchSubStrxe = \"" + GenUuidStrOfFile(str_path_xe + file_name_console) + "\"");
	succeed_cnt += !!Regex_ModifySubStrOfFile(file_main_lua, "g_str_MakeMatchSubStrpmb.+\".+\"", "g_str_MakeMatchSubStrpmb = \"" + GenUuidStrOfFile(str_path_mbp + file_name_console) + "\"");
	std::cout << "需要写入main.lua的次数：" << succeed_cnt << std::endl;
}

//复制注入dll
void _4_CopyGameLoginDllAndCopyVersionToDirection()
{
	using namespace boost::filesystem;
	using namespace boost::system;
	std::string path_mb_src = "\\\\SERVERM\\r_mb\\";
	std::string path_xe_src = "\\\\SERVERM\\r_xe\\";
	std::string path_mbp_src = "\\\\SERVERM\\r_mbp\\";
	std::string path_mb_dst = "F:\\proj\\GWLR\\R_MB\\";
	std::string path_xe_dst = "F:\\proj\\GWLR\\R_XE\\";
	std::string path_mbp_dst = "F:\\proj\\GWLR\\R_MBP\\";
	std::string file_name_game = "Game.dll";
	std::string file_name_login = "Login.dll";
	std::string file_name_console = "Console.exe";
	std::string file_name_lpkb = "lpkb.dll";
	std::string file_name_lpk = "lpk.dll";
	std::string file_name_lpka = "lpka.dll";
	std::string file_name_skin = "SkinPPWTL.dll";
	std::string path_mb_for_pub = "F:\\tmp\\GW_Ver\\mb\\";
	std::string path_mbp_for_pub = "F:\\tmp\\GW_Ver\\mbp\\";
	std::string path_xe_for_pub = "F:\\tmp\\GW_Ver\\xe\\";
	std::cout << "开始从服务器上复制文件\n";
	int succeed_cnt = 0;
	if (CopyUnsameFile(path_mb_src + file_name_game, path_mb_dst + file_name_game))
	{
		std::cout << "从服务器复制了文件，源：" << path_mb_src + file_name_game << " 目标：" << path_mb_dst + file_name_game << std::endl;
		++succeed_cnt;
	}
	else
	{
		std::cout << "从服务器不需要复制该文件，源：" << path_mb_src + file_name_game << " 目标：" << path_mb_dst + file_name_game << std::endl;
	}
	if (CopyUnsameFile(path_mb_src + file_name_login, path_mb_dst + file_name_login))
	{
		std::cout << "从服务器复制了文件，源：" << path_mb_src + file_name_login << " 目标：" << path_mb_dst + file_name_login << std::endl;
		++succeed_cnt;
	}
	else
	{
		std::cout << "从服务器不需要复制该文件，源：" << path_mb_src + file_name_login << " 目标：" << path_mb_dst + file_name_login << std::endl;
	}
	if (CopyUnsameFile(path_mbp_src + file_name_game, path_mbp_dst + file_name_game))
	{
		std::cout << "从服务器复制了文件，源：" << path_mbp_src + file_name_game << " 目标：" << path_mbp_dst + file_name_game << std::endl;
		++succeed_cnt;
	}
	else
	{
		std::cout << "从服务器不需要复制该文件，源：" << path_mbp_src + file_name_game << " 目标：" << path_mbp_dst + file_name_game << std::endl;
	}
	if (CopyUnsameFile(path_mbp_src + file_name_login, path_mbp_dst + file_name_login))
	{
		std::cout << "从服务器复制了文件，源：" << path_mbp_src + file_name_login << " 目标：" << path_mbp_dst + file_name_login << std::endl;
		++succeed_cnt;
	}
	else
	{
		std::cout << "从服务器不需要复制该文件，源：" << path_mbp_src + file_name_login << " 目标：" << path_mbp_dst + file_name_login << std::endl;
	}
	if (CopyUnsameFile(path_xe_src + file_name_game, path_xe_dst + file_name_game))
	{
		std::cout << "从服务器复制了文件，源：" << path_xe_src + file_name_game << " 目标：" << path_xe_dst + file_name_game << std::endl;
		++succeed_cnt;
	}
	else
	{
		std::cout << "从服务器不需要复制该文件，源：" << path_xe_src + file_name_game << " 目标：" << path_xe_dst + file_name_game << std::endl;
	}
	if (CopyUnsameFile(path_xe_src + file_name_login, path_xe_dst + file_name_login))
	{
		std::cout << "从服务器复制了文件，源：" << path_xe_src + file_name_login << " 目标：" << path_xe_dst + file_name_login << std::endl;
		++succeed_cnt;
	}
	else
	{
		std::cout << "从服务器不需要复制该文件，源：" << path_xe_src + file_name_login << " 目标：" << path_xe_dst + file_name_login << std::endl;
	}

	std::cout << "从服务器上成功更新到的次数：" << succeed_cnt << std::endl;
	succeed_cnt = 0;

	std::cout << "开始复制到临时发布文件夹" << std::endl;
	succeed_cnt += CopyUnsameFile(path_mb_dst + file_name_game, path_mb_for_pub + file_name_game);
	succeed_cnt += CopyUnsameFile(path_mb_dst + file_name_login, path_mb_for_pub + file_name_login);
	succeed_cnt += CopyUnsameFile(path_mb_dst + file_name_console, path_mb_for_pub + file_name_console);
	succeed_cnt += CopyUnsameFile(path_mb_dst + file_name_lpkb, path_mb_for_pub + file_name_lpkb);
	succeed_cnt += CopyUnsameFile(path_mb_dst + file_name_lpk, path_mb_for_pub + file_name_lpk);
	succeed_cnt += CopyUnsameFile(path_mb_dst + file_name_lpka, path_mb_for_pub + file_name_lpka);

	succeed_cnt += CopyUnsameFile(path_xe_dst + file_name_game, path_xe_for_pub + file_name_game);
	succeed_cnt += CopyUnsameFile(path_xe_dst + file_name_login, path_xe_for_pub + file_name_login);
	succeed_cnt += CopyUnsameFile(path_xe_dst + file_name_console, path_xe_for_pub + file_name_console);
	succeed_cnt += CopyUnsameFile(path_xe_dst + file_name_lpkb, path_xe_for_pub + file_name_lpkb);
	succeed_cnt += CopyUnsameFile(path_xe_dst + file_name_skin, path_xe_for_pub + file_name_skin);
	succeed_cnt += CopyUnsameFile(path_xe_dst + file_name_lpk, path_xe_for_pub + file_name_lpk);
	succeed_cnt += CopyUnsameFile(path_xe_dst + file_name_lpka, path_xe_for_pub + file_name_lpka);

	succeed_cnt += CopyUnsameFile(path_mbp_dst + file_name_game, path_mbp_for_pub + file_name_game);
	succeed_cnt += CopyUnsameFile(path_mbp_dst + file_name_login, path_mbp_for_pub + file_name_login);
	succeed_cnt += CopyUnsameFile(path_mbp_dst + file_name_console, path_mbp_for_pub + file_name_console);
	succeed_cnt += CopyUnsameFile(path_mbp_dst + file_name_lpkb, path_mbp_for_pub + file_name_lpkb);
	succeed_cnt += CopyUnsameFile(path_mbp_dst + file_name_skin, path_mbp_for_pub + file_name_skin);
	succeed_cnt += CopyUnsameFile(path_mbp_dst + file_name_lpk, path_mbp_for_pub + file_name_lpk);
	succeed_cnt += CopyUnsameFile(path_mbp_dst + file_name_lpka, path_mbp_for_pub + file_name_lpka);

	std::cout << "复制到临时发布文件夹的个数：" << succeed_cnt << std::endl;

	std::string script_src = "F:\\proj\\GWLR\\Game\\script_c_";
	std::string script_dst_mb = "F:\\tmp\\GW_Ver\\mb\\script_c";
	std::string script_dst_mbp = "F:\\tmp\\GW_Ver\\mbp\\script_c";
	std::string script_dst_xe = "F:\\tmp\\GW_Ver\\xe\\script_c";

	boost::system::error_code err;
	remove_all(script_dst_mb, err);
	if (err)
	{
		std::cout << err.message() << '\n';
	}
	err.clear();
	remove_all(script_dst_xe, err);
	if (err)
	{
		std::cout << err.message() << '\n';
	}
	err.clear();
	remove_all(script_dst_mbp, err);
	if (err)
	{
		std::cout << err.message() << '\n';
	}
	std::function<bool(const boost::filesystem::directory_iterator&)> but_func = []( const boost::filesystem::directory_iterator& it ){
		if ( _stricmp(it->path().filename().string().c_str(), ".svn") == 0 )
			return true;
		return false;
	};
	CopyAllFilesOfDirectory(script_src, script_dst_mb, &but_func);
	CopyAllFilesOfDirectory(script_src, script_dst_xe, &but_func);
	CopyAllFilesOfDirectory(script_src, script_dst_mbp, &but_func);

	std::cout << "所有文件复制完成" << std::endl;
}


/*
int main()
{
	using namespace std;
	auto module_file_path = GetModulePath(nullptr);
	std::string the_path = CT2CA(module_file_path);
	std::ofstream fstm(the_path + "\\TotalStagePosInfo.lua", ios_base::out | ios_base::trunc);
	fstm << "local stage_pos_mgr;\nlocal pos_info;\nlocal room_info;\n\n";

	const std::string file_path = the_path + "\\StagePosInfo\\";
	if ( !DoExtensionFileItem(file_path, ".lua", [&fstm]( const boost::filesystem::directory_iterator& it ){
		const auto& path_file = it->path().string();
		std::cout << "处理文件：" << path_file << "\n";
		std::ifstream the_if(path_file, ios_base::in);
		std::string str_tmp;
		Poco::StreamCopier::copyToString(the_if, str_tmp);
		fstm << str_tmp << std::endl;
		the_if.close();
	}) )
	{
		std::cout << "出现了一些错误";
		return 0;
	}
	fstm << std::endl;
	std::cout << "处理完成" << std::endl;
	std::system("pause");
	return 0;
}*/