#pragma once
/*
@author zhp
@date 2017/2/5 22:36
@purpose for npc
*/
#include "GameObj.h"

class GNpc : public GameObjBase{
public:
	GNpc();
	bool NeedVisitable() const override;
	void DebugInfo() const override;

private:
	bool UpdateImpl() override;
	bool OpenVisit(GOpenVisitor& visitor, bool is_pre_open, enOpenVisitorRes& open_res) override;
	bool CloseObject() const override;

private:
	stCD_EntityNpc*		entity_npc_;

private:
	bool				is_npc_need_visit_;
};

class GMovement : public GameObjBase{
public:
	GMovement();
	bool IsOpened() const;

private:
	bool UpdateImpl() override;

private:
	bool	is_opened_;
};