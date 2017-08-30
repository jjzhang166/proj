#pragma once
/*
@author zhp
@date 2014/7/9 22:58
@purpose for unknown, for exampleµÙ¬‰ŒÔ∆∑
*/
#include "GameObj.h"
#include "GClass.h"

class GItemTemplate;
class GUnknown : public GameObjBase{
public:
	GUnknown();
	bool Update();
	bool IsGold() const;

private:
	const GItemTemplate*			item_template_;
};