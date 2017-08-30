#pragma once
/*
@author zhp
@date 2017/1/11 16:36
@purpose state machine
*/
#include <luabind/luabind.hpp>
#include "../DependSelfLib.h"
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <CmnMix/Cmn_PocoMore.h>
#include "DmOper.h"
//////////////////////////////////////////////////////////////////////////
/*
StateRefNode：暂时未用到。
StateNodeCont：状态结点的容器，通常只被当作父节点。
StateNodePriCont：私有容器结点。
LuaStateNode：控制状态节点的具体行为，核心类。
StateNodeUnion：虽然StateNodeUnion是继承自StateNodeCont的，但它不属于容器，它被当作一个整体的存在。
	其fn_prepare的满足条件由子节点决定的：1.子节点的fn_prepare全都满足；2.子节点的fn_prepare任意一个满足。
	其fn_handle的任务可由StateNodeUnion自己来完成，也可由指定的子节点来完成。
	其NextState的计算任务可由StateNodeUnion自己来完成，也可由指定的子节点来完成。
----------------------------------------------------
lua配置格式：
{
	[StateContNode("容器结点1")] = {
		{ "LuaStateNode结点1", fn_prepare, fn_handle },
		{ "LuaStateNode结点2", fn_prepare, fn_handle, fn_next_state },
		[StateUnionNode("状态联合结点1")] = {
			{ "LuaStateNode结点1", fn_prepare, fn_handle },
			{ "LuaStateNode结点2", fn_prepare, fn_handle, "容器结点1.状态联合结点2" },
		},
		[状态联合结点("状态联合结点2")] = {
			{ "LuaStateNode结点1", fn_prepare, fn_handle },
		},
	},
	[状态容器结点("容器结点2")] = {
		{ "LuaStateNode结点2", fn_prepare, fn_handle, "容器结点1.LuaStateNode结点1" },
	},
	[StateFindPicNode("状态找图结点")] = {
	},
}
----------------------------------------------------
StateContNode/状态容器结点，以下是参数信息：
@p1->state_name，字符串类型。不能为空。
@p2->is_private_cont,是否私有窗口。可为空，默认为false。
StateUnionNode/状态容器结点，以下是参数信息：
@p1->state_name，字符串类型。不能为空。
@p2->is_all，布尔类型，true表示子结点的Prepare全部都满足才满足，false表示任意子结点满足就会满足；
	 fn_prepare，函数类型。为空表示true。
@p3->handle_node_name，字符串类型，表示交由该子节点去处理；
	 fn_handle，函数类型。为空表示handle_node_name，值为第一个结点的名字。
@p4->next_state_node_name,字符串类型，表示下一状态的判断由该子节点去决定；
	 next_state_path，字符串类型，表示下一状态的路径，为了与next_state_node_name区分，next_state_path的第一个字符必须是空格字符；
	 next_state_is_parent，布尔类型，	true表示下一状态是父结点，false表示下一状态为空；
	 fn_next_state，函数类型。为空表示true。
@p5->next_state_timeout，数字类型；
	 也可以是布尔类型，true表示def_next_state_timeout，false表示0。只有@p4是next_state_is_parent、fn_next_state、next_state_path,@p5才有效。为空默认为0。
StateFindPicNode/状态找图结点，以下是参数信息：
@p1->state_name，字符串类型。不能为空。
@p2->dm，大漠对象。
	也可以是函数类型，用来获得dm对象。不能为空。
@p3->fn_handle_find_pic，函数类型；
	 { fn_handle_find_pic1, fn_handle_find_pic2 }，多个处理函数。可为空，表示默认处理成功。
@p4->next_state_path，字符串类型，表示下一状态的路径；
	 next_state_is_parent，布尔类型，	true表示下一状态是StateFindPicNode的父结点，false表示下一状态为StateFindPicNode结点；
	 fn_next_state，函数类型。为空表示next_state_is_parent=false。
@p5->next_state_timeout，数字类型；
	 也可以是布尔类型，true表示def_next_state_timeout，false表示0。为空默认为0。
----------------------------------------------------
StateFindPicNode子结点格式，{...}，以下是参数信息：
@p1->pic_name，字符串类型。
	 { pic_name1, pic_name2, ... },表类型，表示需要同时找到这几个图片(fn_handle_find_pic的pic_name参数为pic_name1)。不可为空。
@p2->fn_handle_idx，数字类型；
	 fn_handle_find_pic，函数类型。可为空，默认为0.
@p3->next_state_path，字符串类型，表示下一状态的路径；
	 next_state_is_parent，布尔类型，	true表示下一状态是StateFindPicNode的父结点，false表示下一状态为空；
	 fn_next_state，函数类型。为空表示交由StateFindPicNode处理。
fn_handle_find_pic的参数是(StateNode, pic_name,pos_x, pos_y)，其返回值与fn_handle相同。
----------------------------------------------------
LuaStateNode结点：
state_name和fn_prepare不可为空，fn_handle可为空，表示不需要做任何事，并且返回true。
{state_name,fn_prepare,fn_handle}->CreateLuaState(state_name,lua_fn_prepare,lua_fn_handle,0);//NextState是父节点，直接返回父节点，没有延迟。
{state_name,fn_prepare,fn_handle,true}->CreateLuaState(state_name,lua_fn_prepare,lua_fn_handle,def_next_state_timeout_);//NextState是父节点，延迟返回父节点，延迟是def_next_state_timeout_。
{state_name,fn_prepare,fn_handle,false}->CreateLuaState(state_name,lua_fn_prepare,lua_fn_handle);//NextState为空，通常表示循环结束（任务处理完了）。
{state_name,fn_prepare,fn_handle,next_state_timeout}->CreateLuaState(state_name, lua_fn_prepare, lua_fn_handle, next_state_timeout);//NextState是父节点，延迟返回父节点，延迟是next_state_timeout。
{state_name,fn_prepare,fn_handle,next_state_path}->CreateLuaState(state_name,lua_fn_prepare,lua_fn_handle,next_state_path,def_next_state_timeout_);
{state_name,fn_prepare,fn_handle,next_state_path,next_state_timeout}->CreateLuaState(state_name,lua_fn_prepare,lua_fn_handle,next_state_path,next_state_timeout);
{state_name,fn_prepare,fn_handle,fn_next_state}->CreateLuaState(state_name,lua_fn_prepare,lua_fn_handle,fn_next_state,def_next_state_timeout_);
{state_name,fn_prepare,fn_handle,fn_next_state,next_state_timeout}->CreateLuaState(state_name,lua_fn_prepare,lua_fn_handle,fn_next_state,next_state_timeout);
----------------------------------------------------
state_name：状态结点的名字，通常用来当前状态定位到下一状态，定位方式类似：root_node_name.next_node_name.the_node_name，也可以省掉根节点的名字：next_node_name.the_node_name。
fn_prepare：用于判断当前游戏环境是否符合该状态，
	返回LUA_TNIL表示不符合当前状态；
	返回true表示当前状态符合；
	返回字符串表示节点的路径，该节点符合当前游戏环境；
	返回StateNodePtr表示，该节点符合当前游戏环境。
fn_handle：处理当前游戏状态，默认返回true，表示该状态已处理完成，可以进行定位下一状态了；返回false，表示没有处理成功，则再次进入fn_prepare的流程，循环直到fn_handle返回真。
NextState的计算方式有四种：
	1.返回空，表示当前状态后，没有下一状态了；
	2.返回父节点；
	3.返回下一个状态结点的路径；
	4.通过lua函数计算得到下一状态结点。
next_state_timeout：当得到下一状态后，若next_state_timeout不为0，则间隔判断该状态的fn_prepare是否已满足。
next_state_path：字符串是结点路径，来定位到下一个结点。
fn_next_state：该lua函数可计算得到下一状态，返回值有4种：
	1.返回空表示没有下一状态了；
	2.返回true表示下一状态还是当前状态，返回false表示没有下一状态了；
	3.返回字符串，表示下一状态的路径；
	4.直接返回StateNodePtr对象，该对象便是下一状态了。
----------------------------------------------------
*/
//////////////////////////////////////////////////////////////////////////

class StateResolver;
class StateNode;
typedef boost::shared_ptr<StateNode> StateNodePtr;
typedef boost::weak_ptr<StateNode> StateNodeWeakPtr;
class StateNode : public boost::enable_shared_from_this<StateNode>{
public:
	StateNode();
	virtual ~StateNode();
	static void RegForLua(lua_State* l);
	bool Add(StateNode* sub_node, bool replace_existed = false);
	virtual bool Add(const StateNodePtr& sub_node, bool replace_existed = false);
	virtual bool Erase(const StateNodePtr& sub_node);
	bool Erase(const std::string& path);
	virtual StateNodePtr Find(const std::string& path) const;
	//只查询子节点
	virtual StateNodePtr FindChild(const std::string& child_name) const;
	StateNodePtr Parent() const;
	bool SetParent(const StateNodePtr& parent);
	StateNodePtr GetRoot() const;
	template<typename T>
	T* As(){
		return dynamic_cast<T*>(this);
	}
	void SetName(const std::string& name);
	const std::string& GetName() const;
	std::string GetAbsolutePath() const;
	bool IsSamePath(const std::string& path) const;
	virtual bool IsCont() const;
	virtual bool Parse(StateResolver& resolver, const luabind::object& tbl);
	void SetNextStateTimeout(pt_dword next_state_timeout);
	pt_dword GetNextStateTimeout() const;

public:
	virtual StateNodePtr Prepare();
	virtual bool Handle();
	virtual StateNodePtr NextState();

private:
	bool _Add_1_(const StateNodePtr& sub_node);
	bool _Add_2_(const StateNodePtr& sub_node, bool replace_existed);
	bool _Erase_1_(const StateNodePtr& sub_node);
	bool _Erase_2_(const std::string& path);

private:
	StateNodeWeakPtr		parent_;
	std::string				name_;

protected:
	pt_dword				next_state_timeout_;
};

class StateRefNode : public StateNode{
public:
	StateRefNode(const std::string& node_path);
	bool Add(const StateNodePtr& sub_node, bool replace_existed) override;
	bool Erase(const StateNodePtr& sub_node) override;
	StateNodePtr Find(const std::string& path) const override;
	StateNodePtr FindChild(const std::string& child_name) const override;
	bool IsCont() const override;

public:
	StateNodePtr Prepare() override;
	bool Handle() override;
	StateNodePtr NextState() override;

private:
	StateNodePtr GetImpl() const;

private:
	std::string					node_path_;
	mutable	StateNodeWeakPtr	impl_;
};

class StateNodeCont : public StateNode{
	typedef std::vector<StateNodePtr> NodeCont;

public:
	StateNodeCont();
	bool Add(const StateNodePtr& sub_node, bool replace_existed) override;
	bool Erase(const StateNodePtr& sub_node) override;
	StateNodePtr Find(const std::string& path) const override;
	StateNodePtr FindChild(const std::string& child_name) const override;
	bool IsCont() const override;
	const NodeCont& GetNodes() const;
	bool Parse(StateResolver& resolver, const luabind::object& tbl) override;

public:
	StateNodePtr Prepare() override;

protected:
	bool ParseSubNode(StateResolver& resolver, const luabind::object& key, const luabind::object& tbl);

private:
	NodeCont		nodes_;
};

class StateNodePriCont : public StateNodeCont{
public:
	StateNodePtr Prepare() override;
};

class LuaStateNode : public StateNode{
private:
	LuaStateNode(const luabind::object& fn_prepare, const luabind::object& fn_handle, bool* out_valid);

public:
	class NextStatePolicy{
	public:
		virtual ~NextStatePolicy();
		virtual StateNodePtr CalcNextState(StateNode& self_node) = 0;
	};

public:
	LuaStateNode(const luabind::object& fn_prepare, const luabind::object& fn_handle, bool next_state_is_parent, bool* out_valid);
	LuaStateNode(const luabind::object& fn_prepare, const luabind::object& fn_handle, const std::string& next_state_path, bool* out_valid);
	LuaStateNode(const luabind::object& fn_prepare, const luabind::object& fn_handle, const luabind::object& fn_next_state, bool* out_valid);
	~LuaStateNode();

public:
	StateNodePtr Prepare() override;
	bool Handle() override;
	StateNodePtr NextState() override;

private:
	luabind::object		fn_prepare_;
	luabind::object		fn_handle_;
	NextStatePolicy*	next_state_policy_;
};

class StateNodeUnion : public StateNodeCont{
public:
	class PreparePolicy{
	public:
		virtual ~PreparePolicy();
		virtual StateNodePtr DoPrepare(StateNodeUnion& self_node) = 0;
	};
	class HandlePolicy{
	public:
		virtual ~HandlePolicy();
		virtual bool DoHandle(StateNode& self_node) = 0;
	};
	typedef LuaStateNode::NextStatePolicy NextStatePolicy;

public:
	StateNodeUnion();
	~StateNodeUnion();
	bool SetPrepareInfo(bool is_all);
	bool SetPrepareInfo(const luabind::object& fn_prepare);
	bool SetHandleInfo(const luabind::object& fn_handle);
	bool SetHandleInfo(const std::string& handle_node_name);
	bool SetNextStateInfo(const std::string& next_state_node_name);
	bool SetNextStateInfo(bool next_state_is_parent);
	bool SetNextStateInfo(const luabind::object& fn_next_state);
	bool SetNextStatePath(const std::string& next_state_path);
	bool IsCont() const override;

public:
	StateNodePtr Prepare() override;
	bool Handle() override;
	StateNodePtr NextState() override;

private:
	PreparePolicy*		prepare_policy_;
	HandlePolicy*		handle_policy_;
	NextStatePolicy*	next_state_policy_;
};

class StateNodeFindPic : public StateNodeCont{
public:
	typedef StateNodeUnion::HandlePolicy HandlePolicy;
	typedef LuaStateNode::NextStatePolicy NextStatePolicy;
	typedef boost::shared_ptr<HandlePolicy> HandlePolicyPtr;
	typedef boost::shared_ptr<NextStatePolicy> NextStatePolicyPtr;
	typedef std::vector<std::string> PicNameContT;
	struct stPicItem{
		PicNameContT		pic_names_;
		luabind::object		fn_handle_find_pic_;
		NextStatePolicyPtr	next_state_policy_;
		int					fn_handle_idx_;
		bool				next_state_nil_;
		stPicItem();
	};
	typedef std::vector<stPicItem> PicContT;
	typedef std::vector<luabind::object> ObjContT;

public:
	StateNodeFindPic();
	~StateNodeFindPic();
	bool SetDmObj(const luabind::object& dm);
	bool SetHandleInfo(const luabind::object& handle);
	bool SetNextStateInfo(const std::string& next_state_path);
	bool SetNextStateInfo(bool next_state_is_parent);
	bool SetNextStateInfo(const luabind::object& fn_next_state);
	bool Parse(StateResolver& resolver, const luabind::object& tbl) override;

public:
	StateNodePtr Prepare() override;
	bool Handle() override;
	StateNodePtr NextState() override;

private:
	const std::string& GetPicNameMix();
	bool ParsePicItem(stPicItem& item, const luabind::object& tbl);
	DmOper& GetDm() const;

private:
	PicContT			pic_items_;
	std::string			pic_name_mix_;
	ObjContT			fn_handles_;
	luabind::object		dm_holder_;
	mutable DmOper*		dm_;
	NextStatePolicy*	next_state_policy_;
	const stPicItem*	last_pic_item_;
	long				last_pos_x_;
	long				last_pos_y_;
};

class StateResolver{
	friend class StateNode;
	friend class StateNodeCont;
public:
	StateResolver();
	StateResolver(pt_dword def_next_state_timeout);
	virtual ~StateResolver();
	void SetRoot(const StateNodePtr& root);
	const StateNodePtr& GetRoot() const;
	const StateNodePtr& CurState() const;
	int Resolve();
	StateNodePtr ParseStates(const luabind::object& tbl);
	StateNodePtr ParseStates(const luabind::object& tbl, const std::string& root_state_name);
	StateNodePtr CreateLuaState(const std::string& state_name, const luabind::object& fn_prepare, const luabind::object& fn_handle);
	StateNodePtr CreateLuaState(const std::string& state_name, const luabind::object& fn_prepare, const luabind::object& fn_handle, pt_dword next_state_timeout);
	StateNodePtr CreateLuaState(const std::string& state_name, const luabind::object& fn_prepare, const luabind::object& fn_handle, 
		const std::string& next_state_path, pt_dword next_state_timeout);
	StateNodePtr CreateLuaState(const std::string& state_name, const luabind::object& fn_prepare, const luabind::object& fn_handle, 
		const luabind::object& fn_next_state, pt_dword next_state_timeout);
	StateNodePtr StateContNode(const std::string& state_name, bool is_private_cont);
	StateNodePtr StateUnionNode(lua_State* l, const std::string& state_name);
	StateNodePtr StateFindPicNode(lua_State* l, const std::string& state_name, const luabind::object& fn_find_pic);

protected:
	virtual bool DoNext();

private:
	StateNodePtr _StateContNode_2_(const std::string& state_name);
	StateNodePtr _ParseStates_1_(const luabind::object& tbl);
	StateNodePtr _ParseStates_2_(const luabind::object& tbl, const std::string& root_state_name);
	StateNodePtr _CreateLuaState_1_(const std::string& state_name, const luabind::object& fn_prepare, const luabind::object& fn_handle);
	StateNodePtr _CreateLuaState_2_(const std::string& state_name, const luabind::object& fn_prepare, const luabind::object& fn_handle, 
		const std::string& next_state_path, pt_dword next_state_timeout);
	StateNodePtr _CreateLuaState_3_(const std::string& state_name, const luabind::object& fn_prepare, const luabind::object& fn_handle,
		const luabind::object& fn_next_state, pt_dword next_state_timeout);
	StateNodePtr _CreateLuaState_4_(const std::string& state_name, const luabind::object& fn_prepare, const luabind::object& fn_handle, 
		pt_dword next_state_timeout);
	StateNodePtr ParseCreateLuaState(const luabind::object& tbl);

private:
	StateNodePtr	root_;
	StateNodePtr	cur_state_;
	DateTimeLocal	last_time_;
	pt_dword		def_next_state_timeout_;
};