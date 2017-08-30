#include "stdafx.h"
#include <CmnMix/Cmn_GameObjBase.h>
#include <CmnMix/Cmn_Exception.h>
#include <CmnMix/Cmn_UsefulFunc.h>

void GGameObjBase::DebugInfo() const
{

}

bool GGameObjBase::Update()
{
	return GWndExecSync([this](){
		DO_EXCEPTION_TRY;
		return UpdateImpl();
		DO_CATCH_ALL_MSG_BOX_CODE(
			DoUpdateClear();
		return false;);
	});
}

void GGameObjBase::DoUpdateClear()
{

}

void GGameObjMgrBase::DebugInfo() const
{

}

bool GGameObjMgrBase::UpdateAll()
{
	return GWndExecSync([this](){
		DO_EXCEPTION_TRY;
		return UpdateAll();
		DO_CATCH_ALL_MSG_BOX_CODE(
			DoRebuildUpdateClear();
		return false;);
	});
}

bool GGameObjMgrBase::RebuildAll()
{
	return GWndExecSync([this](){
		DO_EXCEPTION_TRY;
		return RebuildAllImpl();
		DO_CATCH_ALL_MSG_BOX_CODE(
			DoRebuildUpdateClear();
		return false;);
	});
}

void GGameObjMgrBase::DoRebuildUpdateClear()
{

}

bool GGameObjMgrBase::UpdateAllImpl()
{
	assert(false);
	return false;
}
