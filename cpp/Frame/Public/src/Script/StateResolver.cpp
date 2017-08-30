#include "stdafx.h"
#include <Script/StateResolver.h>
#include <boost/property_tree/string_path.hpp>
#include <CmnMix/Cmn_LuaOper.h>
#include <Common/UsefulFunc.h>
#include <CmnMix/Cmn_LogOut.h>
#include <luabind/raw_call_policy.hpp>

//////////////////////////////////////////////////////////////////////////
class StateNodePrepare_All : public StateNodeUnion::PreparePolicy{
public:
	StateNodePtr DoPrepare(StateNodeUnion& self_node) override{
		auto& nodes = self_node.GetNodes();
		if (nodes.empty())
		{
			assert(false);
			return nullptr;
		}
		for (auto& v : nodes)
		{
			if (!v->Prepare())
				return nullptr;
		}
		return self_node.shared_from_this();
	}
};

class StateNodePrepare_Any : public StateNodeUnion::PreparePolicy{
public:
	StateNodePtr DoPrepare(StateNodeUnion& self_node) override{
		auto& nodes = self_node.GetNodes();
		if (nodes.empty())
		{
			assert(false);
			return nullptr;
		}
		for (auto& v : nodes)
		{
			if (v->Prepare())
				return self_node.shared_from_this();
		}
		return nullptr;
	}
};

static StateNodePtr _StateNodeDoPrepare_Impl_(StateNode& self_node, const luabind::object& fn_prepare){
	using namespace luabind;
	const auto& this_ptr = self_node.shared_from_this();
	const auto& res = LuaObjCall<object>(fn_prepare, this_ptr);
	auto t = type(res);
	if (LUA_TNIL == t)
		return nullptr;
	else if (LUA_TBOOLEAN == t)
	{
		if (object_cast<bool>(res))
			return this_ptr;
		return nullptr;
	}
	else if (LUA_TSTRING == t)
	{
		auto node = self_node.GetRoot();
		if (!node)
		{
			assert(false);
			return nullptr;
		}
		const auto& path = object_cast<std::string>(res);
		node = node->Find(path);
		assert(node);
		return node;
	}
	else if (LUA_TUSERDATA == t)
	{
		return object_cast<StateNodePtr>(res);
	}
	assert(false);
	return nullptr;
}

class StateNodePrepare_Fn : public StateNodeUnion::PreparePolicy{
public:
	StateNodePrepare_Fn(const luabind::object& fn_prepare) : fn_prepare_(fn_prepare){}
	StateNodePtr DoPrepare(StateNodeUnion& self_node) override{
		return _StateNodeDoPrepare_Impl_(self_node, fn_prepare_);
	}

private:
	luabind::object		fn_prepare_;
};

static bool _StateNodeHandle_Impl_(StateNode& self_node, const luabind::object& fn_handle){
	if (fn_handle.is_valid())
	{
		const auto& this_ptr = self_node.shared_from_this();
		return LuaObjCallByDlt(fn_handle, true, this_ptr);
	}
	return true;
}

class StateNodeHandle_Fn : public StateNodeUnion::HandlePolicy{
public:
	StateNodeHandle_Fn(const luabind::object& fn_handle) : fn_handle_(fn_handle){}
	bool DoHandle(StateNode& self_node) override{
		return _StateNodeHandle_Impl_(self_node, fn_handle_);
	}

private:
	luabind::object		fn_handle_;
};

class StateNodeHandle_SubName : public StateNodeUnion::HandlePolicy{
public:
	StateNodeHandle_SubName(const std::string& handle_node_name) : handle_node_name_(handle_node_name){}
	bool DoHandle(StateNode& self_node) override{
		auto node_cont = dynamic_cast<StateNodeCont*>(&self_node);
		if (!node_cont)
		{
			assert(false);
			return false;
		}
		StateNodePtr node;
		if (impl_.expired())
		{
			if (handle_node_name_.empty())
			{
				auto& nodes = node_cont->GetNodes();
				if (nodes.empty())
				{
					assert(false);
					return false;
				}
				node = nodes[0];
			}
			else
			{
				node = self_node.Find(handle_node_name_);
			}
			if (!node)
			{
				assert(false);
				return false;
			}
			impl_ = node;
		}
		else
		{
			node = impl_.lock();
			if (!node)
			{
				assert(false);
				return false;
			}
		}
		return node->Handle();
	}

private:
	std::string			handle_node_name_;
	StateNodeWeakPtr	impl_;
};

class NextStatePolicy_Path : public LuaStateNode::NextStatePolicy{
public:
	NextStatePolicy_Path(const std::string& next_state_path) : next_state_path_(next_state_path){
		assert(!next_state_path.empty());
	}
	StateNodePtr CalcNextState(StateNode& self_node) override{
		if (!next_state_.expired())
			return next_state_.lock();
		auto node = self_node.GetRoot();
		if (!node)
		{
			assert(false);
			return nullptr;
		}
		node = node->Find(next_state_path_);
		if (!node)
		{
			assert(false);
			return nullptr;
		}
		next_state_ = node;
		return node;
	}

private:
	std::string			next_state_path_;
	StateNodeWeakPtr	next_state_;
};

class NextStatePolicy_SubName : public LuaStateNode::NextStatePolicy{
public:
	NextStatePolicy_SubName(const std::string& next_state_node_name) : next_state_node_name_(next_state_node_name){}
	StateNodePtr CalcNextState(StateNode& self_node) override{
		if (!next_state_.expired())
			return next_state_.lock();
		auto node = self_node.FindChild(next_state_node_name_);
		if (!node)
		{
			assert(false);
			return nullptr;
		}
		next_state_ = node;
		return node;
	}

private:
	std::string			next_state_node_name_;
	StateNodeWeakPtr	next_state_;
};

class NextStatePolicy_Func : public LuaStateNode::NextStatePolicy{
public:
	NextStatePolicy_Func(const luabind::object& fn_next_state) : fn_next_state_(fn_next_state){}
	StateNodePtr CalcNextState(StateNode& self_node) override{
		using namespace luabind;
		auto this_ptr = self_node.shared_from_this();
		const auto& res = LuaObjCall<object>(fn_next_state_, this_ptr);
		auto t = type(res);
		if (LUA_TNIL == t)
			return nullptr;
		else if (LUA_TBOOLEAN == t)
		{
			if (object_cast<bool>(res))
				return this_ptr;
			return nullptr;
		}
		else if (LUA_TSTRING == t)
		{
			auto node = self_node.GetRoot();
			if (!node)
			{
				assert(false);
				return nullptr;
			}
			const auto& path = object_cast<std::string>(res);
			node = node->Find(path);
			assert(node);
			return node;
		}
		else if (LUA_TUSERDATA == t)
		{
			return object_cast<StateNodePtr>(res);
		}
		assert(false);
		return nullptr;
	}

private:
	luabind::object		fn_next_state_;
};

class NextStatePolicy_Parent : public LuaStateNode::NextStatePolicy{
public:
	StateNodePtr CalcNextState(StateNode& self_node) override{
		return self_node.Parent();
	}
};
//////////////////////////////////////////////////////////////////////////
StateNode::~StateNode()
{

}

bool StateNode::Add(StateNode* sub_node, bool replace_existed)
{
	return Add(StateNodePtr(sub_node), replace_existed);
}

bool StateNode::Add(const StateNodePtr& sub_node, bool replace_existed)
{
	assert(false);
	return false;
}

bool StateNode::Erase(const std::string& path)
{
	auto it = Find(path);
	if (!it)
	{
		assert(false);
		return false;
	}
	if (it.get() == this)
	{
		assert(false);
		return false;
	}
	auto the_parent = it->Parent();
	if (!the_parent)
	{
		assert(false);
		return false;
	}
	return the_parent->Erase(it);
}

bool StateNode::Erase(const StateNodePtr& sub_node)
{
	assert(false);
	return false;
}

StateNodePtr StateNode::Parent() const
{
	if (parent_.expired())
		return nullptr;
	return parent_.lock();
}

bool StateNode::SetParent(const StateNodePtr& parent)
{
	if (!parent)
	{
		assert(false);
		return false;
	}
	if (!parent_.expired())
	{
		assert(false);
		return false;
	}
	parent_ = parent;
	return true;
}

StateNodePtr StateNode::GetRoot() const
{
	auto ptr = shared_from_this();
	while (true)
	{
		const auto& parent = ptr->Parent();
		if (!parent)
			return boost::const_pointer_cast<StateNode>(ptr);
		ptr = parent;
	}

	assert(false);
	return nullptr;
}

void StateNode::SetName(const std::string& name)
{
	if (name.empty())
	{
		assert(false);
		return;
	}
	if (!name_.empty())
	{
		assert(false);
		return;
	}
	name_ = name;
}

const std::string& StateNode::GetName() const
{
	return name_;
}

std::string StateNode::GetAbsolutePath() const
{
	std::vector<std::string> vec;
	auto ptr = shared_from_this();
	auto root = GetRoot();
	while (ptr)
	{
		if (ptr == root)
			break;

		auto& name = ptr->GetName();
		ptr = ptr->Parent();
		vec.push_back(name);
	}

	if (!vec.empty())
		std::reverse(vec.begin(), vec.end());
	std::string res;
	for (auto& v : vec)
	{
		if (!res.empty())
			res.append(".");
		res.append(v);
	}

	return res;
}

void StateNode::RegForLua(lua_State* l)
{
	using namespace luabind;
	module(l)[
		class_<StateNode, StateNodePtr>("StateNode")
		.def("Add", &StateNode::_Add_1_)
		.def("Add", &StateNode::_Add_2_)
		.def("Erase", &StateNode::_Erase_1_)
		.def("Erase", &StateNode::_Erase_2_)
		.def("Find", &StateNode::Find)
		.def("FindChild", &StateNode::FindChild)
		.def("SetName", &StateNode::SetName)
		.def("GetName", &StateNode::GetName)
		.def("GetAbsolutePath", &StateNode::GetAbsolutePath)
		.def("IsSamePath", &StateNode::IsSamePath)
		.def("IsCont", &StateNode::IsCont)
		.def("Prepare", &StateNode::Prepare)
		.def("Handle", &StateNode::Handle)
		.def("NextState", &StateNode::NextState),

		class_<StateResolver>("StateResolver")
		.def(constructor<>())
		.def(constructor<pt_dword>())
		.def("SetRoot", &StateResolver::SetRoot)
		.def("GetRoot", &StateResolver::GetRoot)
		.def("CurState", &StateResolver::CurState)
		.def("Resolve", &StateResolver::Resolve)
		.def("ParseStates", &StateResolver::_ParseStates_1_)
		.def("ParseStates", &StateResolver::_ParseStates_2_)
		.def("CreateLuaState", &StateResolver::_CreateLuaState_1_)
		.def("CreateLuaState", &StateResolver::_CreateLuaState_2_)
		.def("CreateLuaState", &StateResolver::_CreateLuaState_3_)
		.def("CreateLuaState", &StateResolver::_CreateLuaState_4_)
		.def("StateContNode", &StateResolver::StateContNode)
		.def("状态容器结点", &StateResolver::StateContNode)
		.def("StateContNode", &StateResolver::_StateContNode_2_)
		.def("状态容器结点", &StateResolver::_StateContNode_2_)
		.def("StateUnionNode", &StateResolver::StateUnionNode, raw_call(_2))
		.def("状态联合结点", &StateResolver::StateUnionNode, raw_call(_2))
		.def("StateFindPicNode", &StateResolver::StateFindPicNode, raw_call(_2))
		.def("状态找图结点", &StateResolver::StateFindPicNode, raw_call(_2))
	];
}

bool StateNode::_Add_1_(const StateNodePtr& sub_node)
{
	return Add(sub_node);
}

bool StateNode::_Add_2_(const StateNodePtr& sub_node, bool replace_existed)
{
	return Add(sub_node, replace_existed);
}

bool StateNode::_Erase_1_(const StateNodePtr& sub_node)
{
	return Erase(sub_node);
}

bool StateNode::_Erase_2_(const std::string& path)
{
	return Erase(path);
}

StateNodePtr StateNode::Find(const std::string& path) const
{
	assert(false);
	return nullptr;
}

StateNodePtr StateNode::FindChild(const std::string& child_name) const
{
	assert(false);
	return nullptr;
}

StateNodePtr StateNode::Prepare()
{
	return shared_from_this();
}

bool StateNode::Handle()
{
	assert(false);
	return false;
}

StateNodePtr StateNode::NextState()
{
	assert(false);
	return nullptr;
}

bool StateNode::IsCont() const
{
	return false;
}

bool StateNode::IsSamePath(const std::string& path) const
{
	auto node = GetRoot();
	if (!node)
	{
		assert(false);
		return false;
	}
	node = node->Find(path);
	if (!node)
		return false;
	return node.get() == this;
}

bool StateNode::Parse(StateResolver& resolver, const luabind::object& tbl)
{
	assert(false);
	return false;
}

StateNode::StateNode()
{
	next_state_timeout_ = 0;
}

void StateResolver::SetRoot(const StateNodePtr& root)
{
	if (!root)
	{
		assert(false);
		return;
	}
	if (!root->IsCont())
	{
		assert(false);
		return;
	}
	root_ = root;
	cur_state_.reset();
}

const StateNodePtr& StateResolver::GetRoot() const
{
	return root_;
}

const StateNodePtr& StateResolver::CurState() const
{
	return cur_state_;
}

int StateResolver::Resolve()
{
	int resolve_cnt = 0;
	if (cur_state_)
		cur_state_ = cur_state_->Prepare();
	if (!cur_state_)
	{
		if (!root_)
		{
			assert(false);
			return resolve_cnt;
		}
		cur_state_ = root_->Prepare();
		if (!cur_state_)
		{
			//assert(false);
			return resolve_cnt;
		}
	}
	last_time_ = DateTimeLocal();
	StateNodePtr the_old_state;
	do{
		auto handle_res = cur_state_->Handle();
		if (!cur_state_)
		{
			assert(false);
			break;
		}
		if (!DoNext())
			break;
		if (!cur_state_)
		{
			assert(false);
			break;
		}
		if (handle_res)
		{
			if (the_old_state != cur_state_)
			{
				++resolve_cnt;
				the_old_state = cur_state_;
			}
			auto old_state = cur_state_;
			cur_state_ = cur_state_->NextState();
			if (cur_state_)
			{
				if (old_state->GetNextStateTimeout())
					continue;
			}
			else
				return resolve_cnt;
		}
		auto old_state = cur_state_;
		cur_state_ = cur_state_->Prepare();
		if (!cur_state_)
		{
			if (!DoNext())
				break;
			auto parent = old_state->Parent();
			if (parent)
			{
				cur_state_ = parent->Prepare();
				if (!cur_state_ && root_ && root_ != parent)
				{
					if (!DoNext())
						break;
					cur_state_ = root_->Prepare();
				}
			}
			else if (root_)
			{
				cur_state_ = root_->Prepare();
			}
		}
	} while (cur_state_);
	return -resolve_cnt;
}

StateResolver::~StateResolver()
{

}

bool StateResolver::DoNext()
{
	DateTimeLocal time_now;
	const auto& time_span = time_now - last_time_;
	auto total_msec = time_span.totalMilliseconds();
	//LOG_O(Log_trace) << "total_msec:" << total_msec;
	const decltype(total_msec) kDiff = 1350;
	if (total_msec >= kDiff)
	{
		//这里sleep下，提供机会处理下其他事情，比如停止脚本等。
		DummySleep(20);
		last_time_ = DateTimeLocal();
		return true;
	}
	DummySleep(kDiff - total_msec);
	last_time_ = DateTimeLocal();
	return true;
}

StateNodePtr StateResolver::CreateLuaState(const std::string& state_name, const luabind::object& fn_prepare, const luabind::object& fn_handle)
{
	if (state_name.empty())
	{
		assert(false);
		return nullptr;
	}
	bool is_valid;
	StateNodePtr node(new LuaStateNode(fn_prepare, fn_handle, false, &is_valid));
	if (is_valid)
		return node;
	node->SetName(state_name);
	assert(false);
	return nullptr;
}

StateNodePtr StateResolver::CreateLuaState(const std::string& state_name, const luabind::object& fn_prepare, const luabind::object& fn_handle, 
	const std::string& next_state_path, pt_dword next_state_timeout)
{
	if (state_name.empty())
	{
		assert(false);
		return nullptr;
	}
	bool is_valid;
	auto ptr = new LuaStateNode(fn_prepare, fn_handle, next_state_path, &is_valid);
	StateNodePtr node(ptr);
	if (!is_valid)
	{
		assert(false);
		return nullptr;
	}
	ptr->SetName(state_name);
	if (next_state_timeout)
		ptr->SetNextStateTimeout(next_state_timeout);
	return node;
}

StateNodePtr StateResolver::CreateLuaState(const std::string& state_name, const luabind::object& fn_prepare, const luabind::object& fn_handle, 
	const luabind::object& fn_next_state, pt_dword next_state_timeout)
{
	if (state_name.empty())
	{
		assert(false);
		return nullptr;
	}
	bool is_valid;
	auto ptr = new LuaStateNode(fn_prepare, fn_handle, fn_next_state, &is_valid);
	StateNodePtr node(ptr);
	if (!is_valid)
	{
		assert(false);
		return nullptr;
	}
	ptr->SetName(state_name);
	if (next_state_timeout)
		ptr->SetNextStateTimeout(next_state_timeout);
	return node;
}

StateNodePtr StateResolver::CreateLuaState(const std::string& state_name, const luabind::object& fn_prepare, const luabind::object& fn_handle,
	pt_dword next_state_timeout)
{
	if (state_name.empty())
	{
		assert(false);
		return nullptr;
	}
	bool is_valid;
	auto ptr = new LuaStateNode(fn_prepare, fn_handle, true, &is_valid);
	StateNodePtr node(ptr);
	if (!is_valid)
	{
		assert(false);
		return nullptr;
	}
	ptr->SetName(state_name);
	if (next_state_timeout)
		ptr->SetNextStateTimeout(next_state_timeout);
	return node;
}

StateNodePtr StateResolver::_CreateLuaState_1_(const std::string& state_name, const luabind::object& fn_prepare, const luabind::object& fn_handle)
{
	return CreateLuaState(state_name, fn_prepare, fn_handle);
}

StateNodePtr StateResolver::_CreateLuaState_2_(const std::string& state_name, const luabind::object& fn_prepare, const luabind::object& fn_handle, const std::string& next_state_path, pt_dword next_state_timeout)
{
	return CreateLuaState(state_name, fn_prepare, fn_handle, next_state_path, next_state_timeout);
}

StateNodePtr StateResolver::_CreateLuaState_3_(const std::string& state_name, const luabind::object& fn_prepare, const luabind::object& fn_handle, const luabind::object& fn_next_state, pt_dword next_state_timeout)
{
	return CreateLuaState(state_name, fn_prepare, fn_handle, fn_next_state, next_state_timeout);
}

StateNodePtr StateResolver::_CreateLuaState_4_(const std::string& state_name, const luabind::object& fn_prepare, const luabind::object& fn_handle, pt_dword next_state_timeout)
{
	return CreateLuaState(state_name, fn_prepare, fn_handle, next_state_timeout);
}

StateNodePtr StateResolver::StateContNode(const std::string& state_name, bool is_private_cont)
{
	if (state_name.empty())
	{
		assert(false);
		return nullptr;
	}
	StateNodePtr node;
	if (is_private_cont)
		node.reset(new StateNodePriCont);
	else
		node.reset(new StateNodeCont);
	node->SetName(state_name);
	return node;
}

StateNodePtr StateResolver::ParseStates(const luabind::object& tbl, const std::string& root_state_name)
{
	using namespace luabind;
	if (!tbl.is_valid() || LUA_TTABLE != type(tbl))
	{
		assert(false);
		return nullptr;
	}
	const auto& root = StateContNode(root_state_name, false);
	if (!root)
	{
		assert(false);
		return root;
	}
	if (!root->Parse(*this, tbl))
	{
		assert(false);
		return nullptr;
	}
	return root_;
}

StateNodePtr StateResolver::ParseStates(const luabind::object& tbl)
{
	using namespace luabind;
	if (!tbl.is_valid() || LUA_TTABLE != type(tbl))
	{
		assert(false);
		return nullptr;
	}
	StateNodePtr root(new StateNodeCont);
	if (!root)
	{
		assert(false);
		return root;
	}
	if (!root->Parse(*this, tbl))
	{
		assert(false);
		return nullptr;
	}
	return root;
}

StateNodePtr StateResolver::_ParseStates_1_(const luabind::object& tbl)
{
	return ParseStates(tbl);
}

StateNodePtr StateResolver::_ParseStates_2_(const luabind::object& tbl, const std::string& root_state_name)
{
	return ParseStates(tbl, root_state_name);
}

StateResolver::StateResolver()
{
	def_next_state_timeout_ = 1000 * 60;
}

StateResolver::StateResolver(pt_dword def_next_state_timeout)
{
	def_next_state_timeout_ = def_next_state_timeout;
}

StateNodePtr StateResolver::StateUnionNode(lua_State* l, const std::string& state_name)
{
	//assert(false);
	using namespace luabind;
	if (state_name.empty())
	{
		assert(false);
		return nullptr;
	}
	auto top_n = lua_gettop(l);
	if (top_n < 2)
	{
		assert(false);
		return nullptr;
	}
	auto ptr = new StateNodeUnion;
	if (!ptr)
	{
		assert(false);
		return nullptr;
	}
	StateNodePtr node(ptr);
	ptr->SetName(state_name);
	//////////////////////////////////////////////////////////////////////////
	object tmp;
	int t;
	bool use_p5 = true;
	if (top_n >= 5)
	{
		object(from_stack(l, 5)).swap(tmp);
		t = type(tmp);
		if (LUA_TNIL == t)
		{
			if (!ptr->SetNextStateInfo(true))
				return nullptr;
		}
		else if (LUA_TSTRING == t)
		{
			const auto& str_tmp = object_cast<std::string>(tmp);
			if (str_tmp.size() < 1)
			{
				assert(false);
				return nullptr;
			}
			if (str_tmp[0] == ' ')
			{
				if (!ptr->SetNextStatePath(str_tmp))
					return nullptr;
			}
			else
			{
				if (!ptr->SetNextStateInfo(str_tmp))
					return nullptr;
				use_p5 = false;
			}
		}
		else if (LUA_TBOOLEAN == t)
		{
			if (!ptr->SetNextStateInfo(object_cast<bool>(tmp)))
				return nullptr;
		}
		else if (LUA_TFUNCTION == t)
		{
			if (!ptr->SetNextStateInfo(tmp))
				return nullptr;
		}
		else
		{
			assert(false);
			return nullptr;
		}
	}
	else
	{
		if (!ptr->SetNextStateInfo(true))
			return nullptr;
	}
	if (use_p5)
	{
		if (top_n >= 6)
		{
			object(from_stack(l, 6)).swap(tmp);
			t = type(tmp);
			if (LUA_TNUMBER == t)
			{
				ptr->SetNextStateTimeout(object_cast<pt_dword>(tmp));
			}
			else if (LUA_TBOOLEAN == t)
			{
				if (object_cast<bool>(tmp))
					ptr->SetNextStateTimeout(def_next_state_timeout_);
			}
			else if (LUA_TNIL != t)
			{
				assert(false);
				return nullptr;
			}
		}
	}
	if (top_n >= 4)
	{
		object(from_stack(l, 4)).swap(tmp);
		t = type(tmp);
		if (LUA_TNIL == t)
		{
			if (!ptr->SetHandleInfo(kEmptyStr))
				return nullptr;
		}
		else if (LUA_TSTRING == t)
		{
			if (!ptr->SetHandleInfo(object_cast<std::string>(tmp)))
				return nullptr;
		}
		else if (LUA_TFUNCTION == t)
		{
			if (!ptr->SetHandleInfo(tmp))
				return nullptr;
		}
		else
		{
			assert(false);
			return nullptr;
		}
	}
	else
	{
		if (!ptr->SetHandleInfo(kEmptyStr))
			return nullptr;
	}
	if (top_n >= 3)
	{
		object(from_stack(l, 3)).swap(tmp);
		t = type(tmp);
		if (LUA_TNIL == t)
		{
			if (!ptr->SetPrepareInfo(true))
				return nullptr;
		}
		else if (LUA_TBOOLEAN == t)
		{
			if (!ptr->SetPrepareInfo(object_cast<bool>(tmp)))
				return nullptr;
		}
		else if (LUA_TFUNCTION == t)
		{
			if (!ptr->SetPrepareInfo(tmp))
				return nullptr;
		}
		else
		{
			assert(false);
			return nullptr;
		}
	}
	else
	{
		if (!ptr->SetPrepareInfo(true))
			return nullptr;
	}
	//////////////////////////////////////////////////////////////////////////
	return node;
}

bool StateNodeCont::Add(const StateNodePtr& sub_node, bool replace_existed)
{
	if (!sub_node)
	{
		assert(false);
		return false;
	}
	auto& the_name = sub_node->GetName();
	if (the_name.empty())
	{
		assert(false);
		return false;
	}
	auto it = nodes_.begin();
	for (; it != nodes_.end(); ++it)
	{
		auto& v = *it;
		if (v->GetName() == the_name)
			break;
	}
	if (it != nodes_.end())
	{
		if (!replace_existed)
			return false;
		if (!Erase(*it))
			return false;
	}
	if (!sub_node->SetParent(shared_from_this()))
	{
		assert(false);
		return false;
	}
	nodes_.push_back(sub_node);
	return true;
}

bool StateNodeCont::Erase(const StateNodePtr& sub_node)
{
	if (!sub_node)
	{
		assert(false);
		return false;
	}
	auto& the_name = sub_node->GetName();
	if (the_name.empty())
	{
		assert(false);
		return false;
	}
	auto it = nodes_.begin();
	for (; it != nodes_.end(); ++it)
	{
		auto& v = *it;
		if (v->GetName() == the_name)
			break;
	}
	if (it != nodes_.end())
	{
		nodes_.erase(it);
		return true;
	}
	assert(false);
	return false;
}

StateNodePtr StateNodeCont::Find(const std::string& path) const
{
	typedef boost::property_tree::path_of<std::string>::type path_type;
	path_type p(path);
	auto ptr = boost::const_pointer_cast<StateNode>(shared_from_this());
	while (ptr && !p.empty())
	{
		const auto& fragment = p.reduce();
		ptr = FindChild(fragment);
	}
	return ptr;
}

StateNodePtr StateNodeCont::FindChild(const std::string& child_name) const
{
	if (child_name.empty())
	{
		assert(false);
		return nullptr;
	}
	for (auto& v : nodes_)
	{
		if (v->GetName() == child_name)
			return v;
	}
	return nullptr;
}

StateNodeCont::StateNodeCont()
{
}

StateNodePtr StateNodeCont::Prepare()
{
	for (auto& v : nodes_)
	{
		if (v->IsCont())
			continue;
		const auto& res = v->Prepare();
		if (res)
			return res;
	}
	for (auto& v : nodes_)
	{
		if (!v->IsCont())
			continue;
		const auto& res = v->Prepare();
		if (res)
			return res;
	}
	return nullptr;
}

bool StateNodeCont::IsCont() const
{
	return true;
}

const StateNodeCont::NodeCont& StateNodeCont::GetNodes() const
{
	return nodes_;
}

StateNodePtr StateResolver::ParseCreateLuaState(const luabind::object& tbl)
{
	using namespace luabind;
	if (LUA_TTABLE != type(tbl))
	{
		assert(false);
		return nullptr;
	}
	object lua_state_name = tbl[1];
	auto t = type(lua_state_name);
	if (LUA_TSTRING != t)
	{
		assert(false);
		return nullptr;
	}
	object lua_fn_prepare = tbl[2];
	t = type(lua_fn_prepare);
	if (LUA_TFUNCTION != t)
	{
		assert(false);
		return nullptr;
	}
	object lua_fn_handle = tbl[3];
	t = type(lua_fn_handle);
	if (LUA_TFUNCTION != t && LUA_TNIL != t)
	{
		assert(false);
		return nullptr;
	}
	const auto& state_name = object_cast<std::string>(lua_state_name);
	object lua_p4 = tbl[4];
	t = type(lua_p4);
	if (LUA_TNIL == t)
		return CreateLuaState(state_name, lua_fn_prepare, lua_fn_handle, 0);
	else if (LUA_TBOOLEAN == t)
	{
		if (object_cast<bool>(lua_p4))
			return CreateLuaState(state_name, lua_fn_prepare, lua_fn_handle, def_next_state_timeout_);
		else
			return CreateLuaState(state_name, lua_fn_prepare, lua_fn_handle);
	}
	else if (LUA_TNUMBER == t)
		return CreateLuaState(state_name, lua_fn_prepare, lua_fn_handle, object_cast<pt_dword>(lua_p4));
	else if (LUA_TSTRING == t)
	{
		return CreateLuaState(state_name, lua_fn_prepare, lua_fn_handle, object_cast<std::string>(lua_p4), LuaObjCast(tbl[5], def_next_state_timeout_));
	}
	else
		return CreateLuaState(state_name, lua_fn_prepare, lua_fn_handle, lua_p4, LuaObjCast(tbl[5], def_next_state_timeout_));
}

StateNodePtr StateResolver::StateFindPicNode(lua_State* l, const std::string& state_name, const luabind::object& dm)
{
	using namespace luabind;
	if (state_name.empty())
	{
		assert(false);
		return nullptr;
	}
	auto top_n = lua_gettop(l);
	if (top_n < 3)
	{
		assert(false);
		return nullptr;
	}
	auto ptr = new StateNodeFindPic;
	if (!ptr)
	{
		assert(false);
		return nullptr;
	}
	StateNodePtr node(ptr);
	//////////////////////////////////////////////////////////////////////////
	if (!ptr->SetDmObj(dm))
	{
		assert(false);
		return nullptr;
	}
	object tmp;
	if (top_n >= 4)
	{
		object(from_stack(l, 4)).swap(tmp);
		if (!ptr->SetHandleInfo(tmp))
		{
			assert(false);
			return nullptr;
		}
	}
	else
	{
		if (!ptr->SetHandleInfo(tmp))
		{
			assert(false);
			return nullptr;
		}
	}
	int t;
	if (top_n >= 5)
	{
		object(from_stack(l, 5)).swap(tmp);
		t = type(tmp);
		if (LUA_TNIL == t)
		{
			if (!ptr->SetNextStateInfo(true))
				return nullptr;
		}
		else if (LUA_TSTRING == t)
		{
			if (!ptr->SetNextStateInfo(object_cast<std::string>(tmp)))
				return nullptr;
		}
		else if (LUA_TBOOLEAN == t)
		{
			if (!ptr->SetNextStateInfo(object_cast<bool>(tmp)))
				return nullptr;
		}
		else if (LUA_TFUNCTION == t)
		{
			if (!ptr->SetNextStateInfo(tmp))
				return nullptr;
		}
		else
		{
			assert(false);
			return nullptr;
		}
	}
	else
	{
		if (!ptr->SetNextStateInfo(false))
			return nullptr;
	}
	if (top_n >= 6)
	{
		object(from_stack(l, 6)).swap(tmp);
		t = type(tmp);
		if (LUA_TNUMBER == t)
		{
			ptr->SetNextStateTimeout(object_cast<pt_dword>(tmp));
		}
		else if (LUA_TBOOLEAN == t)
		{
			if (object_cast<bool>(tmp))
				ptr->SetNextStateTimeout(def_next_state_timeout_);
		}
		else if (LUA_TNIL != t)
		{
			assert(false);
			return nullptr;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	ptr->SetName(state_name);
	return node;
}

StateNodePtr StateResolver::_StateContNode_2_(const std::string& state_name)
{
	return StateContNode(state_name, false);
}

bool StateNodeCont::Parse(StateResolver& resolver, const luabind::object& tbl)
{
	using namespace luabind;
	if (!tbl.is_valid() || LUA_TTABLE != type(tbl))
	{
		assert(false);
		return false;
	}
	for (iterator it(tbl), it_end; it != it_end; ++it)
	{
		object v = *it;
		object key = it.key();
		auto t = type(key);
		if (LUA_TNUMBER == t)
		{
			const auto& node = resolver.ParseCreateLuaState(v);
			if (!node)
			{
				assert(false);
				continue;
			}
			Add(node, false);
		}
		else if (LUA_TUSERDATA == t)
		{
			if (!ParseSubNode(resolver, key, v))
			{
				assert(false);
				return false;
			}
		}
		else
			assert(false);
	}
	return true;
}

bool StateNodeCont::ParseSubNode(StateResolver& resolver, const luabind::object& key, const luabind::object& tbl)
{
	using namespace luabind;
	if (LUA_TTABLE != type(tbl))
	{
		assert(false);
		return false;
	}
	StateNodePtr tmp;
	const auto& node = LuaObjCast(key, tmp);
	if (!node)
	{
		assert(false);
		return false;
	}
	Add(node, false);
	if (!node->Parse(resolver, tbl))
		return false;
	return true;
}

StateRefNode::StateRefNode(const std::string& node_path) : node_path_(node_path)
{
	assert(!node_path.empty());
}

StateNodePtr StateRefNode::GetImpl() const
{
	if (impl_.expired())
	{
		const auto& root = GetRoot();
		if (!root)
		{
			assert(false);
			return nullptr;
		}
		const auto& node = root->Find(node_path_);
		if (!node)
		{
			assert(false);
			return nullptr;
		}
		impl_ = node;
		return node;
	}
	return impl_.lock();
}

bool StateRefNode::Add(const StateNodePtr& sub_node, bool replace_existed)
{
	const auto& impl = GetImpl();
	if (impl)
		return impl->Add(sub_node, replace_existed);
	assert(false);
	return false;
}

bool StateRefNode::Erase(const StateNodePtr& sub_node)
{
	const auto& impl = GetImpl();
	if (impl)
		return impl->Erase(sub_node);
	assert(false);
	return false;
}

StateNodePtr StateRefNode::Find(const std::string& path) const
{
	const auto& impl = GetImpl();
	if (impl)
		return impl->Find(path);
	assert(false);
	return nullptr;
}

StateNodePtr StateRefNode::FindChild(const std::string& child_name) const
{
	const auto& impl = GetImpl();
	if (impl)
		return impl->FindChild(child_name);
	assert(false);
	return nullptr;
}

StateNodePtr StateRefNode::Prepare()
{
	const auto& impl = GetImpl();
	if (impl)
		return impl->Prepare();
	assert(false);
	return nullptr;
}

bool StateRefNode::Handle()
{
	const auto& impl = GetImpl();
	if (impl)
		return impl->Handle();
	assert(false);
	return false;
}

StateNodePtr StateRefNode::NextState()
{
	const auto& impl = GetImpl();
	if (impl)
		return impl->NextState();
	assert(false);
	return nullptr;
}

bool StateRefNode::IsCont() const
{
	const auto& impl = GetImpl();
	if (impl)
		return impl->IsCont();
	assert(false);
	return false;
}

LuaStateNode::LuaStateNode(const luabind::object& fn_prepare, const luabind::object& fn_handle, const std::string& next_state_path, bool* out_valid)
	: LuaStateNode(fn_prepare, fn_handle, out_valid)
{
	using namespace luabind;
	if (out_valid)
	{
		if (!*out_valid)
			return;
		*out_valid = false;
	}
	next_state_policy_ = new NextStatePolicy_Path(next_state_path);
	if (out_valid)
		*out_valid = true;
}

LuaStateNode::LuaStateNode(const luabind::object& fn_prepare, const luabind::object& fn_handle, bool next_state_is_parent, bool* out_valid)
	: LuaStateNode(fn_prepare, fn_handle, out_valid)
{
	using namespace luabind;
	if (out_valid)
	{
		if (!*out_valid)
			return;
		*out_valid = false;
	}
	if (next_state_is_parent)
		next_state_policy_ = new NextStatePolicy_Parent;
	if (out_valid)
		*out_valid = true;
}

LuaStateNode::LuaStateNode(const luabind::object& fn_prepare, const luabind::object& fn_handle, const luabind::object& fn_next_state, bool* out_valid)
	: LuaStateNode(fn_prepare, fn_handle, out_valid)
{
	using namespace luabind;
	if (out_valid)
	{
		if (!*out_valid)
			return;
		*out_valid = false;
	}
	if (LUA_TFUNCTION != type(fn_next_state))
	{
		assert(false);
		return;
	}
	next_state_policy_ = new NextStatePolicy_Func(fn_next_state);
	if (out_valid)
		*out_valid = true;
}

LuaStateNode::LuaStateNode(const luabind::object& fn_prepare, const luabind::object& fn_handle, bool* out_valid)
{
	using namespace luabind;
	next_state_policy_ = nullptr;
	if (out_valid)
		*out_valid = false;
	if (LUA_TFUNCTION != type(fn_prepare))
	{
		assert(false);
		return;
	}
	auto t = type(fn_handle);
	if (LUA_TFUNCTION == t)
		fn_handle_ = fn_handle;
	else if (LUA_TNIL != t)
	{
		assert(false);
		return;
	}
	fn_prepare_ = fn_prepare;
	if (out_valid)
		*out_valid = true;
}

StateNodePtr LuaStateNode::Prepare()
{
	return _StateNodeDoPrepare_Impl_(*this, fn_prepare_);
}

bool LuaStateNode::Handle()
{
	return _StateNodeHandle_Impl_(*this, fn_handle_);
}

static StateNodePtr _StateNodeNextState_Impl_(StateNodePtr res, pt_dword next_state_timeout){
	if (!TimerDo(1000, next_state_timeout, [&res](){
		const auto& the_res = res->Prepare();
		if (the_res)
		{
			res = the_res;
			return true;
		}
		return false;
	}))
	{
		assert(false);
		return res;
	}
	return res;
}
static StateNodePtr _StateNodeNextState_Impl_(StateNode& self_node, LuaStateNode::NextStatePolicy* next_state_policy){
	if (!next_state_policy)
		return nullptr;
	auto res = next_state_policy->CalcNextState(self_node);
	if (!res)
		return res;
	auto next_state_timeout = self_node.GetNextStateTimeout();
	if (!next_state_timeout)
		return res;
	return _StateNodeNextState_Impl_(res, next_state_timeout);
}

StateNodePtr LuaStateNode::NextState()
{
	return _StateNodeNextState_Impl_(*this, next_state_policy_);
}

LuaStateNode::~LuaStateNode()
{
	if (next_state_policy_)
		delete next_state_policy_;
}

void StateNode::SetNextStateTimeout(pt_dword next_state_timeout)
{
	if (next_state_timeout_)
	{
		assert(false);
		return;
	}
	next_state_timeout_ = next_state_timeout;
}

pt_dword StateNode::GetNextStateTimeout() const
{
	return next_state_timeout_;
}

LuaStateNode::NextStatePolicy::~NextStatePolicy()
{

}

StateNodeUnion::StateNodeUnion()
{
	prepare_policy_ = nullptr;
	handle_policy_ = nullptr;
	next_state_policy_ = nullptr;
}

StateNodeUnion::~StateNodeUnion()
{
	delete prepare_policy_;
	delete handle_policy_;
	delete next_state_policy_;
}

StateNodePtr StateNodeUnion::Prepare()
{
	if (!prepare_policy_)
	{
		assert(false);
		return nullptr;
	}
	return prepare_policy_->DoPrepare(*this);
}

bool StateNodeUnion::Handle()
{
	if (!handle_policy_)
	{
		assert(false);
		return false;
	}
	return handle_policy_->DoHandle(*this);
}

StateNodePtr StateNodeUnion::NextState()
{
	return _StateNodeNextState_Impl_(*this, next_state_policy_);
}

bool StateNodeUnion::SetPrepareInfo(bool is_all)
{
	if (prepare_policy_)
	{
		assert(false);
		return false;
	}
	if (is_all)
		prepare_policy_ = new StateNodePrepare_All;
	else
		prepare_policy_ = new StateNodePrepare_Any;
	if (!prepare_policy_)
	{
		assert(false);
		return false;
	}
	return true;
}

bool StateNodeUnion::SetPrepareInfo(const luabind::object& fn_prepare)
{
	using namespace luabind;
	if (prepare_policy_)
	{
		assert(false);
		return false;
	}
	if (LUA_TFUNCTION != type(fn_prepare))
	{
		assert(false);
		return false;
	}
	prepare_policy_ = new StateNodePrepare_Fn(fn_prepare);
	if (!prepare_policy_)
	{
		assert(false);
		return false;
	}
	return true;
}

bool StateNodeUnion::SetHandleInfo(const luabind::object& fn_handle)
{
	using namespace luabind;
	if (handle_policy_)
	{
		assert(false);
		return false;
	}
	if (LUA_TFUNCTION != type(fn_handle))
	{
		assert(false);
		return false;
	}
	handle_policy_ = new StateNodeHandle_Fn(fn_handle);
	if (!handle_policy_)
	{
		assert(false);
		return false;
	}
	return true;
}

bool StateNodeUnion::SetHandleInfo(const std::string& handle_node_name)
{
	if (handle_policy_)
	{
		assert(false);
		return false;
	}
	handle_policy_ = new StateNodeHandle_SubName(handle_node_name);
	if (!handle_policy_)
	{
		assert(false);
		return false;
	}
	return true;
}

bool StateNodeUnion::SetNextStateInfo(const std::string& next_state_node_name)
{
	if (next_state_node_name.empty())
	{
		assert(false);
		return false;
	}
	if (next_state_policy_)
	{
		assert(false);
		return false;
	}
	next_state_policy_ = new NextStatePolicy_SubName(next_state_node_name);
	assert(next_state_policy_);
	return !!next_state_policy_;
}

bool StateNodeUnion::SetNextStateInfo(bool next_state_is_parent)
{
	if (next_state_policy_)
	{
		assert(false);
		return false;
	}
	if (!next_state_is_parent)
		return true;
	next_state_policy_ = new NextStatePolicy_Parent;
	assert(next_state_policy_);
	return !!next_state_policy_;
}

bool StateNodeUnion::SetNextStateInfo(const luabind::object& fn_next_state)
{
	using namespace luabind;
	if (next_state_policy_)
	{
		assert(false);
		return false;
	}
	if (LUA_TFUNCTION != type(fn_next_state))
	{
		assert(false);
		return false;
	}
	next_state_policy_ = new NextStatePolicy_Func(fn_next_state);
	assert(next_state_policy_);
	return !!next_state_policy_;
}

bool StateNodeUnion::SetNextStatePath(const std::string& next_state_path)
{
	auto size = next_state_path.size();
	if (size < 2)
	{
		assert(false);
		return false;
	}
	if (next_state_path[0] != ' ')
	{
		assert(false);
		return false;
	}
	if (next_state_policy_)
	{
		assert(false);
		return false;
	}
	auto ptr = next_state_path.c_str();
	std::string path(ptr + 1, size - 1);
	next_state_policy_ = new NextStatePolicy_Path(path);
	assert(next_state_policy_);
	return !!next_state_policy_;
}

bool StateNodeUnion::IsCont() const
{
	return false;
}

StateNodeUnion::PreparePolicy::~PreparePolicy()
{

}

StateNodeUnion::HandlePolicy::~HandlePolicy()
{

}

StateNodeFindPic::StateNodeFindPic()
{
	next_state_policy_ = nullptr;
	last_pic_item_ = nullptr;
	dm_ = nullptr;
	last_pos_x_ = -1;
	last_pos_y_ = -1;
}

bool StateNodeFindPic::Parse(StateResolver& resolver, const luabind::object& tbl)
{
	using namespace luabind;
	if (LUA_TTABLE != type(tbl))
	{
		assert(false);
		return false;
	}
	for (iterator it(tbl), it_end; it != it_end; ++it)
	{
		object v = *it;
		object key = it.key();
		auto t = type(key);
		if (LUA_TNUMBER == t)
		{
			if (LUA_TTABLE != type(v))
			{
				assert(false);
				continue;
			}
			stPicItem item;
			if (!ParsePicItem(item, v))
			{
				assert(false);
				continue;
			}
			pic_items_.push_back(item);
		}
		else if (LUA_TUSERDATA == t)
		{
			if (!ParseSubNode(resolver, key, v))
			{
				assert(false);
				return false;
			}
		}
		else
			assert(false);
	}
	return true;
}

StateNodePtr StateNodeFindPic::Prepare()
{
	using namespace luabind;
	last_pic_item_ = nullptr;
	auto& dm = GetDm();
	last_pos_x_ = -1;
	last_pos_y_ = -1;
	auto res_idx = dm.FindPic(0, 0, 2000, 2000, GetPicNameMix(), kEmptyStr, 0.9, 0, &last_pos_x_, &last_pos_y_);
	if (res_idx < 0 || last_pos_x_ < 0 || last_pos_y_ < 0)
		return __super::Prepare();
	int idx = -1;
	int pic_name_idx;
	for (auto& v : pic_items_)
	{
		pic_name_idx = -1;
		for (auto& name : v.pic_names_)
		{
			++idx;
			++pic_name_idx;
			if (idx == res_idx)
			{
				last_pic_item_ = &v;
				if (v.pic_names_.size() == 1)
				{
					LOG_O(Log_trace) << "找到了图片：" << last_pic_item_->pic_names_[0] << "\tposX:" << last_pos_x_ << "\tposY:" << last_pos_y_;
					return shared_from_this();
				}
				if (pic_name_idx != 0)
				{
					last_pic_item_ = nullptr;
					return nullptr;
				}
				goto LABEL_HEAR;
			}
		}
	}
LABEL_HEAR:
	if (!last_pic_item_)
	{
		assert(false);
		return nullptr;
	}
	std::string pic_name_mix;
	auto size = (int)last_pic_item_->pic_names_.size();
	for (int i = 0; i < size; ++i)
	{
		if (i == pic_name_idx)
			continue;
		pic_name_mix += last_pic_item_->pic_names_[i];
		pic_name_mix.append(1, '|');
	}
	if (pic_name_mix.empty())
	{
		assert(false);
		last_pic_item_ = nullptr;
		return nullptr;
	}
	pic_name_mix.pop_back();
	const auto& res = dm.FindPicEx(0, 0, 2000, 2000, pic_name_mix, kEmptyStr, 0.9, 0);
	if (res.empty())
	{
		last_pic_item_ = nullptr;
		return nullptr;
	}
	int res_cnt = 0;
	for (auto c : res)
	{
		if (c == '|')
			++res_cnt;
	}
	++res_cnt;
	--size;
	assert(size >= res_cnt);
	if (size == res_cnt)
	{
		LOG_O(Log_trace) << "找到了多个图片，第一个图片：" << last_pic_item_->pic_names_[0] << "\tposX:" << last_pos_x_ << "\tposY:" << last_pos_y_;
		return shared_from_this();
	}
	last_pic_item_ = nullptr;
	return nullptr;
}

bool StateNodeFindPic::Handle()
{
	if (!last_pic_item_)
	{
		assert(false);
		return false;
	}
	if (last_pic_item_->fn_handle_find_pic_.is_valid())
	{
		assert(last_pos_x_ >= 0);
		return LuaObjCallByDlt(last_pic_item_->fn_handle_find_pic_, true, shared_from_this(), last_pic_item_->pic_names_[0], last_pos_x_, last_pos_y_);
	}
	if (fn_handles_.empty())
		return true;
	int handle_idx = last_pic_item_->fn_handle_idx_ % fn_handles_.size();
	if (handle_idx < 0)
	{
		assert(false);
		return false;
	}
	const auto& fn_handle = fn_handles_[handle_idx];
	assert(last_pos_x_ >= 0);
	return LuaObjCallByDlt(fn_handle, true, shared_from_this(), last_pic_item_->pic_names_[0], last_pos_x_, last_pos_y_);
}

StateNodePtr StateNodeFindPic::NextState()
{
	if (!last_pic_item_)
	{
		assert(false);
		return nullptr;
	}
	if (last_pic_item_->next_state_policy_)
		return last_pic_item_->next_state_policy_->CalcNextState(*this);
	if (last_pic_item_->next_state_nil_)
		return nullptr;
	if (next_state_policy_)
		return next_state_policy_->CalcNextState(*this);
	const auto& this_ptr = shared_from_this();
	auto next_state_timeout = GetNextStateTimeout();
	if (!next_state_timeout)
		return this_ptr;
	return _StateNodeNextState_Impl_(this_ptr, next_state_timeout);
}

bool StateNodeFindPic::SetDmObj(const luabind::object& dm)
{
	using namespace luabind;
	if (LUA_TFUNCTION == type(dm))
	{
		dm_holder_ = dm;
		return true;
	}
	dm_ = LuaObjCast(dm, dm_);
	if (!dm_)
	{
		assert(false);
		return false;
	}
	dm_holder_ = dm;
	return true;
}

bool StateNodeFindPic::SetHandleInfo(const luabind::object& handle)
{
	using namespace luabind;
	if (!fn_handles_.empty())
	{
		assert(false);
		return false;
	}
	bool is_nil = !handle.is_valid();
	int t;
	if (!is_nil)
		t = type(handle);
	if (is_nil)
	{
		return true;
	}
	else if (LUA_TFUNCTION == t)
	{
		fn_handles_.push_back(handle);
	}
	else if (LUA_TTABLE == t)
	{
		for (iterator it(handle), it_end; it != it_end; ++it)
		{
			object v = *it;
			if (LUA_TFUNCTION != type(v))
			{
				assert(false);
				return false;
			}
			fn_handles_.push_back(v);
		}
	}
	else
	{
		assert(false);
		return false;
	}
	return true;
}

bool StateNodeFindPic::SetNextStateInfo(const std::string& next_state_path)
{
	if (next_state_path.empty())
	{
		assert(false);
		return false;
	}
	if (next_state_policy_)
	{
		assert(false);
		return false;
	}
	next_state_policy_ = new NextStatePolicy_Path(next_state_path);
	assert(next_state_policy_);
	return !!next_state_policy_;
}

bool StateNodeFindPic::SetNextStateInfo(bool next_state_is_parent)
{
	if (next_state_policy_)
	{
		assert(false);
		return false;
	}
	if (!next_state_is_parent)
		return true;
	next_state_policy_ = new NextStatePolicy_Parent;
	assert(next_state_policy_);
	return !!next_state_policy_;
}

bool StateNodeFindPic::SetNextStateInfo(const luabind::object& fn_next_state)
{
	using namespace luabind;
	if (next_state_policy_)
	{
		assert(false);
		return false;
	}
	if (LUA_TFUNCTION != type(fn_next_state))
	{
		assert(false);
		return false;
	}
	next_state_policy_ = new NextStatePolicy_Func(fn_next_state);
	assert(next_state_policy_);
	return !!next_state_policy_;
}

StateNodeFindPic::~StateNodeFindPic()
{
	delete next_state_policy_;
}

bool StateNodeFindPic::ParsePicItem(stPicItem& item, const luabind::object& tbl)
{
	using namespace luabind;
	object tmp = tbl[1];
	auto t = type(tmp);
	if (LUA_TSTRING == t)
	{
		item.pic_names_.push_back(object_cast<const char*>(tmp));
		if (item.pic_names_.back().empty())
		{
			assert(false);
			return false;
		}
	}
	else if (LUA_TTABLE == t)
	{
		for (iterator it(tmp), it_end; it != it_end; ++it)
		{
			object v = *it;
			if (LUA_TSTRING != type(v))
			{
				assert(false);
				return false;
			}
			item.pic_names_.push_back(object_cast<const char*>(v));
			if (item.pic_names_.back().empty())
			{
				assert(false);
				return false;
			}
		}
	}
	else
	{
		assert(false);
		return false;
	}
	if (item.pic_names_.empty())
	{
		assert(false);
		return false;
	}
	tmp = tbl[2];
	t = type(tmp);
	if (LUA_TNUMBER == t)
	{
		item.fn_handle_idx_ = object_cast<int>(tmp);
		if (item.fn_handle_idx_ < 0)
		{
			assert(false);
			return false;
		}
		if (item.fn_handle_idx_ >= (int)fn_handles_.size())
		{
			assert(false);
			return false;
		}
	}
	else if (LUA_TFUNCTION == t)
	{
		item.fn_handle_find_pic_ = tmp;
	}
	else if (LUA_TNIL != t)
	{
		assert(false);
		return false;
	}
	tmp = tbl[3];
	t = type(tmp);
	if (LUA_TSTRING == t)
	{
		item.next_state_policy_.reset(new NextStatePolicy_Path(object_cast<std::string>(tmp)));
		if (!item.next_state_policy_)
		{
			assert(false);
			return false;
		}
	}
	else if (LUA_TBOOLEAN == t)
	{
		if (object_cast<bool>(tmp))
		{
			item.next_state_policy_.reset(new NextStatePolicy_Parent);
			if (!item.next_state_policy_)
			{
				assert(false);
				return false;
			}
		}
		else
		{
			item.next_state_nil_ = true;
		}
	}
	else if (LUA_TFUNCTION == t)
	{
		item.next_state_policy_.reset(new NextStatePolicy_Func(tmp));
		if (!item.next_state_policy_)
		{
			assert(false);
			return false;
		}
	}
	else if (LUA_TNIL != t)
	{
		assert(false);
		return false;
	}
	return true;
}

const std::string& StateNodeFindPic::GetPicNameMix()
{
	if (!pic_name_mix_.empty())
		return pic_name_mix_;
	for (auto& v : pic_items_)
	{
		for (auto& s : v.pic_names_)
		{
			pic_name_mix_ += s;
			pic_name_mix_.append(1, '|');
		}
	}
	if (!pic_name_mix_.empty())
		pic_name_mix_.pop_back();
	return pic_name_mix_;
}

DmOper& StateNodeFindPic::GetDm() const
{
	if (dm_)
		return *dm_;
	decltype(dm_) tmp = nullptr;
	dm_ = LuaObjCallByDlt(dm_holder_, tmp);
	assert(dm_);
	return *dm_;
}

StateNodeFindPic::stPicItem::stPicItem()
{
	fn_handle_idx_ = 0;
	next_state_nil_ = false;
}

StateNodePtr StateNodePriCont::Prepare()
{
	return nullptr;
}
