#include "stdafx.h"
#include "LLH_Defines.h"

LLH_MemFileMap::stCommunicate::stCommunicate()
{
	safe_check_done_ = false;
	pwd_input_error_ = false;
	ac_freeze_ = false;
	login_succeed_ = false;
}

LLH_MemFileMap::LLH_MemFileMap() : MemFileMap("DA1F0FE7-EA9E-45D9-BD85-407B653190D1")
{

}

const LLH_MemFileMap::stCommunicate* LLH_MemFileMap::ReadAsCommu() const
{
	return IndexAs<stCommunicate>(0);
}

LLH_MemFileMap::stCommunicate* LLH_MemFileMap::WriteAsCommu()
{
	return IndexAs<stCommunicate>(0);
}
