#include "stdafx.h"
#include "GNpc.h"
#include <Common/MemOper.h>

GNpc::GNpc() : GameObjBase(kGOT_Npc)
{

}

bool GNpc::Update()
{
	assert(false);
	return true;
}

void GNpc::DebugInfo() const
{
	
}

const std::string& GNpc::GetCnName() const
{
	return ch_name_;
}
