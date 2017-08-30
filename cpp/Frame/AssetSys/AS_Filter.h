#pragma once
/*
@author zhp
@date 2016/11/4 15:57
@purpose Asset System Filter
*/

class AS_Node;
class AS_IFilter{
public:
	virtual ~AS_IFilter(){}
	//返回真表示未被过滤
	virtual bool Do(const AS_Node& node) const = 0;
};

class AS_FilterExt : public AS_IFilter{
public:
	AS_FilterExt(const char* ext);
	bool Do(const AS_Node& node) const override;

private:
	const char*		ext_;
};