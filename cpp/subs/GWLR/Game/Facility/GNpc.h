#pragma once
/*
@author zhp
@date 2014/7/1 22:32
@purpose for npc
*/
#include "GameObj.h"

class GNpc : public GameObjBase{
public:
	GNpc();
	bool Update() override;
	void DebugInfo() const override;
	const std::string& GetCnName() const;

private:
	std::string		ch_name_;
};