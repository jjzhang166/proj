#include "stdafx.h"
#include "AtpMiX/AtpLogin.h"

Atp_MemFileMap::stCommunicate::stCommunicate()
{
	safe_check_done_ = false;
	pwd_input_error_ = false;
	ac_freeze_ = false;
	login_succeed_ = false;
	begin_input_pwd_ = false;
	end_input_pwd_ = false;
	input_pwd_has_except_ = false;
	std::memset(pwd_, 0, sizeof(pwd_));
}

Atp_MemFileMap::Atp_MemFileMap() : MemFileMap("9D7884AF14E64AE68E3385A2D7D1854A")
{

}

const Atp_MemFileMap::stCommunicate* Atp_MemFileMap::ReadAsCommu() const
{
	return IndexAs<stCommunicate>(0);
}

Atp_MemFileMap::stCommunicate* Atp_MemFileMap::WriteAsCommu()
{
	return IndexAs<stCommunicate>(0);
}
