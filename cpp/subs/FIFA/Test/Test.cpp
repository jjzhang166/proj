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
#include <Net/JsonCall.h>
#include <Net/PocoMore.h>
#include <hash_set>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/name_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <Common/UsefulClass.h>
#include <Net/IoTimer.h>
#include <boost/uuid/uuid_io.hpp>

#include <Poco/Crypto/Cipher.h>
#include <Poco/Crypto/CipherFactory.h>
#include <Poco/Crypto/RSAKey.h>
#include <Poco/Crypto/CipherKey.h>
#include <Poco/Crypto/X509Certificate.h>
#include <Protocol/P_PriDefines.h>
#include <strstream>
#include <Poco/StreamCopier.h>

using namespace Poco::Crypto;

using namespace std;
using namespace Poco::Crypto;
typedef std::vector<unsigned char> ByteVec;

std::string CheckSafeLpk(){
	std::ifstream stm_lpk("f:\\proj\\cpp\\subs\\FIFA\\CrackData\\AntiCrack\\lpkb.dll", std::ios_base::in | std::ios_base::binary);
	return GenUuidStr(stm_lpk);
}

void GenRsaKey( const std::string& str_pub_file, const std::string& str_pri_file,
			   const std::string& str_private_key ){
	RSAKey gen_rsa(RSAKey::KL_1024, RSAKey::EXP_SMALL);
	std::fstream f_pub(str_pub_file, ios_base::in | ios_base::out | ios_base::trunc /*| ios_base::_Nocreate*/), 
	f_pri(str_pri_file, ios_base::in | ios_base::out | ios_base::trunc);
	gen_rsa.save(&f_pub, &f_pri, str_private_key);
	f_pub.close();
	f_pri.close();
}
std::string CalcRsaKeyUuid( const std::string& rsa_key_fiel )
{
	std::ifstream stm_lpk(rsa_key_fiel, std::ios_base::in | std::ios_base::binary);
	return GenUuidStr(stm_lpk);
}

int main(){
	auto str_tmp = CheckSafeLpk();
	assert(false);
	//auto res = kUG_PeerAcByCard | kUG_PeerCardByAc | kUG_FreezeAc | kUG_PeerUsedCardInfo | kUG_RemoveBind | kUG_PeerAcInfo | kUG_PeerCardInfo | kUG_PeerAllCardInfo | kUG_OpenCard | kUG_GenRegKey;
	/*
	std::string public_stoc = "f:\\tmp\\public_StoC_FI.pem", private_stoc = "f:\\tmp\\private_StoC_FI.pem",
		public_ctos = "f:\\tmp\\public_CtoS_FI.pem", private_ctos = "f:\\tmp\\private_CtoS_FI.pem";

	GenRsaKey(public_stoc, private_stoc, kRSA_PriPassphrase_StoC);
	GenRsaKey(public_ctos, private_ctos, kRSA_PriPassphrase_CtoS);
	auto uuid_public_stc = CalcRsaKeyUuid(public_stoc), uuid_private_ctos = CalcRsaKeyUuid(private_ctos);*/
	/*
	std::ifstream stm_pri("f:\\tmp\\private_StoC.pem");
	std::ifstream stm_pub("f:\\tmp\\public_StoC.pem");
	Crypto_Rsa_Pub cr_pru(stm_pub);
	Crypto_Rsa_Pri cr_pri(stm_pri, kRSA_PriPassphrase_StoC);
	
	auto en_tmp = cr_pru.Encrypt("12345");
	//std::ofstream f_wri("f:\\tmp_en.txt");

	//f_wri.write(en_tmp.c_str(), en_tmp.size());
	std::ifstream cr_rea("f:\\tmp_en.txt");
	en_tmp.clear();
	Poco::StreamCopier::copyToString(cr_rea, en_tmp);
	auto de_tmp = cr_pri.Decrypt(en_tmp);
	*/
	//LoadLibrary(_T("d3d9.dll"));
	//////////////////////////////////////////////////////////////////////////
	//*************json read and write****************************************
	/*using namespace Json;
	std::ifstream ifs("f:\\tmp\\json\\tmp.lua");
	std::ofstream ofs("f:\\tmp\\json\\tmp1.lua", std::ios_base::trunc);
	Reader reader;
	Value root;
	reader.parse(ifs, root, false);
	StyledStreamWriter writer;
	writer.write(ofs, root);*/
	//////////////////////////////////////////////////////////////////////////
	//LoadLibrary(_T("d3d9.dll"));
	system("pause");
	return 0;

}
