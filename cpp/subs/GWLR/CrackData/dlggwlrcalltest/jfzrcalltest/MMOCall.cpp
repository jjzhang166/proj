#include "StdAfx.h"
#include "MMOCall.h"
#include "MMOClientTraverse.h"
#include "public/UsefulFns.h"

MMOCall::MMOCall(void)
{
}

MMOCall::~MMOCall(void)
{
}


//id ²ÎÊý£¬Èç£º MMOCLIENT_REVERSAL_SKILL_ID_MOUSE_LEFT
bool MMOCall::useskill(int nSkillId)		
{
	MMOActor theActor;
	theActor.ReadActor();
	

	if( theActor.m_dwActorObjAddr != 0 )
	{
		return UseSkill(nSkillId, *(void**)(theActor.m_dwActorObjAddr+MMOCLIENT_REVERSAL_SKILL_OFFSET));
	}
	return false;
}

