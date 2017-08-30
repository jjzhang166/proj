#include "stdafx.h"
#include <WgFacility/CmnMapPath.h>

const BW_GridBase* BW_MapPathBase::FindWhiteGrid(const stCmnVecBase& pos) const
{
	for (auto v : grids_)
	{
		if (v->IsWhite() && v->IsIn(pos))
			return v;
	}
	return nullptr;
}

BW_MapPathBase::~BW_MapPathBase()
{
	for (auto v : grids_)
	{
		delete v;
	}
}

const BW_GridBase* BW_MapPathBase::FindNearestWhiteGrid(const stCmnVecBase& pos) const
{
	const BW_GridBase* grid = nullptr;
	double distance;
	for (auto v : grids_)
	{
		if (!v->IsWhite())
			continue;
		auto d = v->Distance(pos);
		if (!grid || d < distance)
		{
			grid = v;
			distance = d;
		}
	}
	return grid;
}

const BW_GridBase* BW_MapPathBase::FindNearestGrid(const stCmnVecBase& pos) const
{
	const BW_GridBase* grid = nullptr;
	double distance;
	for (auto v : grids_)
	{
		auto d = v->Distance(pos);
		if (!grid || d < distance)
		{
			grid = v;
			distance = d;
		}
	}
	return grid;
}

void BW_MapPathBase::ClearGrds()
{
	for (auto v : grids_)
	{
		delete v;
	}
	grids_.clear();
}

bool BW_GridWhite::IsWhite() const
{
	return true;
}

bool BW_GridBase::IsWhite() const
{
	return false;
}
