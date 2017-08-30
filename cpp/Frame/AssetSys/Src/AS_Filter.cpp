#include "stdafx.h"
#include "AS_Filter.h"
#include "AS_Node.h"

AS_FilterExt::AS_FilterExt(const char* ext)
{
	ext_ = ext;
}

bool AS_FilterExt::Do(const AS_Node& node) const
{
	auto& path = node.GetPath();
	if (ext_)
	{
		if (!path.has_extension())
			return false;
	}
	else
	{
		if (path.has_extension())
			return false;
		return true;
	}
	return path.extension().string() == ext_;
}
