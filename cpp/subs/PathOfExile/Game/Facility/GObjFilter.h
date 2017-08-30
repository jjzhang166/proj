#pragma once
/*
@author zhp
@date 2017/3/24 5:45
@purpose for object filter. 对象过滤策略
*/
#include "GConst.h"
#include "GType.h"

template<typename FnT>
using FilterGameObjLambda = CF_Lambda<FilterGameObjEnsure, FnT>;

template<typename FnT, typename... Args>
FilterGameObjLambda<FnT>* FilterGameObj_MakeLambda(const FnT& fn, Args&... args){
	return new FilterGameObjLambda<FnT>(fn, args...);
}

class FilterGameObjName : public FilterGameObjEnsure{
	bool DoFilt(const GameObjBasePtrT& obj) const override;

public:
	FilterGameObjName(FilterGameObjCont& filter_cont, const std::string& name);

private:
	std::string		name_;
};

class FilterGameObjNotName : public FilterGameObjEnsure{
	bool DoFilt(const GameObjBasePtrT& obj) const override;

public:
	FilterGameObjNotName(FilterGameObjCont& filter_cont, const std::string& name);

private:
	std::string		name_;
};

template<typename ContT>
class FilterGameObjNames : public FilterGameObjEnsure{
	bool DoFilt(const GameObjBasePtrT& obj) const override{
		const auto& the_name = obj->GetCnName();
		return names_.IsIn(the_name);
	}

public:
	FilterGameObjNames(FilterGameObjCont& filter_cont, ContT& names) : FilterGameObjEnsure(filter_cont), names_(names){}

private:
	NameListRef<ContT>		names_;
};

class FilterGameObjId : public FilterGameObjEnsure{
	bool DoFilt(const GameObjBasePtrT& obj) const override;

public:
	FilterGameObjId(FilterGameObjCont& filter_cont, pt_dword obj_id);

private:
	pt_dword		obj_id_;
};

class FilterGameObjNotTheId : public FilterGameObjEnsure{
	bool DoFilt(const GameObjBasePtrT& obj) const override;

public:
	FilterGameObjNotTheId(FilterGameObjCont& filter_cont, pt_dword obj_id);

private:
	pt_dword		obj_id_;
};

class FilterGameObjAlive : public FilterGameObjEnsure{
	bool DoFilt(const GameObjBasePtrT& obj) const override;

public:
	FilterGameObjAlive(FilterGameObjCont& filter_cont);
};

class FilterGameObjDied : public FilterGameObjEnsure{
	bool DoFilt(const GameObjBasePtrT& obj) const override;

public:
	FilterGameObjDied(FilterGameObjCont& filter_cont);
};

class FilterGameObjBlackNameList : public FilterGameObjEnsure{
	bool DoFilt(const GameObjBasePtrT& obj) const override;

public:
	FilterGameObjBlackNameList(FilterGameObjCont& filter_cont, const NameListIdT& black_name_list);

private:
	const NameListIdT&		black_name_list_;
};

class FilterGameObjHittable : public FilterGameObjEnsure{
	bool DoFilt(const GameObjBasePtrT& obj) const override;

public:
	FilterGameObjHittable(FilterGameObjCont& filter_cont);
};

class FilterGameObjNeedVisitable : public FilterGameObjEnsure{
	bool DoFilt(const GameObjBasePtrT& obj) const override;

public:
	FilterGameObjNeedVisitable(FilterGameObjCont& filter_cont);
};

class FilterRememberObjs : public FilterGameObjEnsure{
public:
	typedef std::vector<GameObjBasePtrT> ObjContT;

public:
	FilterRememberObjs(FilterGameObjCont& filter_cont);
	bool DoFilt(const GameObjBasePtrT& obj) const override;
	inline const ObjContT& GetObjs() const{ return objs_; }
	void Add(const GameObjBasePtrT& obj);
	void clear(){ objs_.clear(); }

private:
	ObjContT		objs_;
};

class FilterTransferTown : public FilterGameObjEnsure{
	bool DoFilt(const GameObjBasePtrT& obj) const override;

public:
	FilterTransferTown(FilterGameObjCont& filter_cont);
};

class FilterQuestWorldObj : public FilterGameObjEnsure{
	bool DoFilt(const FittedType& obj) const override;

public:
	FilterQuestWorldObj(FilterGameObjCont& filter_cont);
};

class FilterObjByType : public IFilterGameObj{
	bool DoFilt(const FittedType& obj) const override;

public:
	FilterObjByType(enGameObjType obj_type);

private:
	enGameObjType		obj_type_;
};
