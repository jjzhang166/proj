#include "stdafx.h"
#include <WgFacility/CmnQuestHandler.h>
#include <Common/LogOut.h>
#include <boost/scope_exit.hpp>
#include <CmnMix/Cmn_LuaOper.h>
#include <Common/UsefulFunc.h>
#include <CmnMix/Cmn_Exception.h>

//////////////////////////////////////////////////////////////////////////
class CmnExcept_TerminateQuest : public CmnGeneralRunException{
public:
	CmnExcept_TerminateQuest() : CmnGeneralRunException("中止任务"){}

	CMN_EXCEPTION_CLONE
};
//////////////////////////////////////////////////////////////////////////
class CQH_Visitor{
public:
	virtual bool VisitAccept(const CQH_AcceptHandler& handler) = 0;
	virtual bool VisitDo(const CQH_DoHandler& handler) = 0;
	virtual bool VisitDone(const CQH_DoneHandler& handler) = 0;
};
//////////////////////////////////////////////////////////////////////////
CQH_Handler::~CQH_Handler()
{

}

bool CQH_Handler::ParseAsCont(CQH_Handler& cont, const luabind::object& tbl)
{
	using namespace luabind;
	if (!tbl.is_valid())
		return true;
	if (LUA_TTABLE != type(tbl))
	{
		const auto& name = cont.GetName();
		if (name.empty())
		{
			LOG_O(Log_error) << "添加任务处理之参数必须是表类型";
		}
		else
		{
			LOG_O(Log_error) << "添加任务处理之参数必须是表类型：" << name;
		}
		assert(false);
		return false;
	}
	CQH_HandlerPtr handler_tmp;
	object empty_obj;
	for (iterator it(tbl), it_end; it != it_end; ++it)
	{
		object v = *it;
		object key = it.key();
		auto t = type(key);
		if (LUA_TNUMBER == t)
		{
			const auto& node = LuaObjCast(v, handler_tmp);
			if (!node)
			{
				LOG_O(Log_error) << "添加任务处理之表的键的类型不能是数字类型，是不是忘记添加值了？任务名："
					<< cont.GetName() << R"(，要像这样：处理任务("任务名") = {})";
				assert(false);
				continue;
			}
			if (!node->Parse(empty_obj))
			{
				continue;
			}
			cont.Add(node);
		}
		else if (LUA_TUSERDATA == t)
		{
			const auto& node = LuaObjCast(key, handler_tmp);
			if (!node)
			{
				LOG_O(Log_error) << "添加任务处理之键的类型不对，忽略该项的添加，请用 处理任务、做任务处理 等提供的函数创建表的键："
					<< cont.GetName() << R"(，要像这样：处理任务("任务名") = {})";
				assert(false);
				continue;
			}
			if (!node->Parse(v))
			{
				continue;
			}
			cont.Add(node);
		}
		else
		{
			LOG_O(Log_error) << "添加任务处理之键的类型不对，忽略该项的添加，请用 处理任务、提交任务处理 等提供的函数创建键："
				<< cont.GetName() << R"(，要像这样：处理任务("任务名") = {})";
			assert(false);
		}
	}
	return true;
}

const char* CQH_Handler::__GetName__()
{
	return GetName().c_str();
}

bool CQH_Resolver::OnIdle(enHandleFlag flag)
{
	DummySleep(1000);
	return true;
}

CQH_Resolver::~CQH_Resolver()
{
	assert(instance_);
	instance_ = nullptr;
}

void CQH_Resolver::clear()
{
	root_handler_.clear();
}

void CQH_Resolver::AddHandlder(const CQH_HandlerPtr& handler)
{
	root_handler_.Add(handler);
}

bool CQH_Resolver::AcceptOne()
{
	++deep_accept_;
	handle_flag_.set(kHF_Accept);
	BOOST_SCOPE_EXIT_ALL(this){
		--deep_accept_;
		if (0 == deep_accept_)
			handle_flag_.reset(kHF_Accept);
	};
	try{
		if (!OnIdle(kHF_Accept))
		{
			assert(false);
			return false;
		}
		if (!OnUpdateAccept())
		{
			assert(false);
			return false;
		}
		for (auto& v : root_handler_.GetHandlers())
		{
			if (v->CanAccept(nullptr))
			{
				auto name = v->GetName();
				if (!name.empty())
				{
					LOG_O(Log_info) << "开始接任务：" << name;
				}
				if (!v->Accept(nullptr))
				{
					if (!name.empty())
					{
						LOG_O(Log_error) << "接任务失败：" << name;
					}
					return false;
				}
				if (!name.empty())
				{
					LOG_O(Log_info) << "成功接取到任务：" << name;
				}
				OnHandleOne(kHF_Accept, *v);
				return true;
			}
		}
	} DO_CATCH_THE_EXCEPTION(const CmnExcept_TerminateQuest&, {
		if (deep_accept_ > 1 || deep_do_ || deep_done_ || deep_handle_)
			throw;
	})
	return false;
}

bool CQH_Resolver::DoOne()
{
	++deep_do_;
	handle_flag_.set(kHF_Do);
	BOOST_SCOPE_EXIT_ALL(this){
		--deep_do_;
		if (0 == deep_do_)
			handle_flag_.reset(kHF_Do);
	};
	try{
		if (!OnIdle(kHF_Do))
		{
			assert(false);
			return false;
		}
		if (!OnUpdateDo())
		{
			assert(false);
			return false;
		}
		for (auto& v : root_handler_.GetHandlers())
		{
			if (v->CanDo(nullptr))
			{
				auto name = v->GetName();
				if (!name.empty())
				{
					LOG_O(Log_info) << "开始做任务：" << name;
				}
				if (!v->Do(nullptr))
				{
					if (!name.empty())
					{
						LOG_O(Log_error) << "做任务失败：" << name;
					}
					return false;
				}
				if (!name.empty())
				{
					LOG_O(Log_info) << "成功做了任务：" << name;
				}
				OnHandleOne(kHF_Do, *v);
				return true;
			}
		}
	} DO_CATCH_THE_EXCEPTION(const CmnExcept_TerminateQuest&, {
		if (deep_accept_ || deep_do_ > 1 || deep_done_ || deep_handle_)
			throw;
	})
	return false;
}

bool CQH_Resolver::DoneOne()
{
	++deep_done_;
	handle_flag_.set(kHF_Done);
	BOOST_SCOPE_EXIT_ALL(this){
		--deep_done_;
		if (0 == deep_done_)
			handle_flag_.reset(kHF_Done);
	};
	try{
		if (!OnIdle(kHF_Done))
		{
			assert(false);
			return false;
		}
		if (!OnUpdateDone())
		{
			assert(false);
			return false;
		}
		for (auto& v : root_handler_.GetHandlers())
		{
			if (v->CanDone(nullptr))
			{
				auto name = v->GetName();
				if (!name.empty())
				{
					LOG_O(Log_info) << "开始提交完成任务：" << name;
				}
				if (!v->Done(nullptr))
				{
					if (!name.empty())
					{
						LOG_O(Log_error) << "提交完成任务失败：" << name;
					}
					return false;
				}
				if (!name.empty())
				{
					LOG_O(Log_info) << "成功提交完成了任务：" << name;
				}
				OnHandleOne(kHF_Done, *v);
				return true;
			}
		}
	} DO_CATCH_THE_EXCEPTION(const CmnExcept_TerminateQuest&, {
		if (deep_accept_ || deep_do_ || deep_done_ > 1 || deep_handle_)
			throw;
	})
	return false;
}

int CQH_Resolver::HandleAll()
{
	++deep_handle_;
	handle_flag_.set(kHF_Handle);
	BOOST_SCOPE_EXIT_ALL(this){
		--deep_handle_;
		if (0 == deep_handle_)
			handle_flag_.reset(kHF_Handle);
	};
	int cnt;
	int total_cnt = 0;
	int error_cnt = 0;
	try{
		OnHandleAll();
		while (true)
		{
			do
			{
				cnt = 0;
				cnt += AcceptOne();
				cnt += DoOne();
				cnt += DoneOne();
				total_cnt += cnt;
			} while (cnt > 0);
			if (!OnNoneQuest())
			{
				++error_cnt;
				if (error_cnt >= 2)
					break;
			}
			else
				error_cnt = 0;
		}
		LOG_O(Log_warning) << "无法接、做、提交一个任务了，是不是已经完成所有任务了？";
	} DO_CATCH_THE_EXCEPTION(const CmnExcept_TerminateQuest&, {
		if (deep_accept_ || deep_do_ || deep_done_ || deep_handle_ > 1)
			throw;
	})	
	return total_cnt;
}

CQH_Resolver::CQH_Resolver()
{
	deep_accept_ = 0;
	deep_do_ = 0;
	deep_done_ = 0;
	deep_handle_ = 0;
	assert(!instance_);
	instance_ = this;
}

int CQH_Resolver::ParseHandlers(const luabind::object& tbl)
{
	auto old_cnt = root_handler_.GetHandleCount();
	if (!root_handler_.Parse(tbl))
	{
		assert(false);
	}
	auto res = root_handler_.GetHandleCount() - old_cnt;
	LOG_O(Log_info) << "总共添加了[" << res << "]项任务";
	return res;
}

CQH_Resolver* CQH_Resolver::GetInstance()
{
	assert(instance_);
	return instance_;
}

CQH_Resolver& CQH_Resolver::GetMe()
{
	assert(instance_);
	return *instance_;
}

CQH_HandlerPtr CQH_Resolver::MakeHandleQuests(const std::string& quest_name)
{
	if (quest_name.empty())
	{
		LOG_O(Log_error) << "处理任务之任务名不能为空";
		assert(false);
		return nullptr;
	}
	auto resolver = GetInstance();
	if (!resolver)
	{
		assert(false);
		return nullptr;
	}
	CQH_HandlerPtr res(resolver->CreateHandlerQuests(quest_name));
	assert(res);
	return res;
}

CQH_HandlerPtr CQH_Resolver::MakeHandleQuests(const std::string& quest_name, const luabind::object& params)
{
	auto handler = MakeHandleQuests(quest_name);
	if (!handler)
		return nullptr;
	if (!handler->Parse(params))
		return nullptr;
	return handler;
}

CQH_HandlerPtr CQH_Resolver::MakeHandlePerQuest(const std::string& quest_name)
{
	if (quest_name.empty())
	{
		LOG_O(Log_error) << "处理一个任务之任务名不能为空";
		assert(false);
		return nullptr;
	}
	auto resolver = GetInstance();
	if (!resolver)
	{
		assert(false);
		return nullptr;
	}
	CQH_HandlerPtr res(resolver->CreateHandlerPerQuest(quest_name));
	assert(res);
	return res;
}

CQH_HandlerPtr CQH_Resolver::MakeHandlePerQuest(const std::string& quest_name, const luabind::object& params)
{
	auto handler = MakeHandlePerQuest(quest_name);
	if (!handler)
		return nullptr;
	if (!handler->Parse(params))
		return nullptr;
	return handler;
}

CQH_HandlerPtr CQH_Resolver::MakeAcceptHandler(const luabind::object& fn)
{
	using namespace luabind;
	if (!fn.is_valid() || LUA_TFUNCTION != type(fn))
	{
		LOG_O(Log_error) << "接任务处理之参数必须是函数类型";
		return nullptr;
	}
	auto resolver = GetInstance();
	if (!resolver)
	{
		assert(false);
		return nullptr;
	}
	CQH_HandlerPtr res(resolver->CreateAcceptHandler(fn));
	assert(res);
	return res;
}

CQH_HandlerPtr CQH_Resolver::MakeAcceptHandler(const luabind::object& fn, const luabind::object& params)
{
	auto handler = MakeAcceptHandler(fn);
	if (!handler)
		return nullptr;
	if (!handler->Parse(params))
		return nullptr;
	return handler;
}

CQH_HandlerPtr CQH_Resolver::MakeDoHandler(const luabind::object& fn)
{
	using namespace luabind;
	if (!fn.is_valid() || LUA_TFUNCTION != type(fn))
	{
		LOG_O(Log_error) << "做任务处理之参数必须是函数类型";
		return nullptr;
	}
	auto resolver = GetInstance();
	if (!resolver)
	{
		assert(false);
		return nullptr;
	}
	CQH_HandlerPtr res(resolver->CreateDoHandler(fn));
	assert(res);
	return res;
}

CQH_HandlerPtr CQH_Resolver::MakeDoHandler(const luabind::object& fn, const luabind::object& params)
{
	auto handler = MakeDoHandler(fn);
	if (!handler)
		return nullptr;
	if (!handler->Parse(params))
		return nullptr;
	return handler;
}

CQH_HandlerPtr CQH_Resolver::MakeDoneHandler(const luabind::object& fn)
{
	using namespace luabind;
	if (!fn.is_valid() || LUA_TFUNCTION != type(fn))
	{
		LOG_O(Log_error) << "提交/完成任务处理之参数必须是函数类型";
		return nullptr;
	}
	auto resolver = GetInstance();
	if (!resolver)
	{
		assert(false);
		return nullptr;
	}
	CQH_HandlerPtr res(resolver->CreateDoneHandler(fn));
	assert(res);
	return res;
}

CQH_HandlerPtr CQH_Resolver::MakeDoneHandler(const luabind::object& fn, const luabind::object& params)
{
	auto handler = MakeDoneHandler(fn);
	if (!handler)
		return nullptr;
	if (!handler->Parse(params))
		return nullptr;
	return handler;
}

CQH_Quests* CQH_Resolver::CreateHandlerQuests(const std::string& quest_name)
{
	return new CQH_Quests(*this, quest_name);
}

CQH_PerQuest* CQH_Resolver::CreateHandlerPerQuest(const std::string& quest_name)
{
	return new CQH_PerQuest(quest_name);
}

void CQH_Resolver::RegForLua(lua_State* l)
{
	assert(l);
	using namespace luabind;
	module(l)[
		class_<CQH_Node>("CQH_Node")
		.def("GetHandler", &CQH_Node::GetHandler)
		.def("处理者", &CQH_Node::GetHandler),

		class_<CQH_Handler, CQH_HandlerPtr>("CQH_Handler")
		.def("GetName", &CQH_Handler::GetName)
		.def_readonly("任务名", &CQH_Handler::__GetName__),

		class_<CQH_HandlerCont, CQH_Handler, CQH_HandlerPtr>("CQH_HandlerCont"),

		class_<CQH_Quests, CQH_HandlerCont, CQH_HandlerPtr>("CQH_Quests"),

		class_<CQH_PerHandler, CQH_Handler, CQH_HandlerPtr>("CQH_PerHandler"),

		class_<CQH_PerQuest, CQH_PerHandler, CQH_HandlerPtr>("CQH_PerQuest"),

		class_<CQH_PerHandlerImpl, CQH_PerHandler, CQH_HandlerPtr>("CQH_PerHandlerImpl"),

		class_<CQH_AcceptHandler, CQH_PerHandlerImpl, CQH_HandlerPtr>("CQH_AcceptHandler"),

		class_<CQH_DoHandler, CQH_PerHandlerImpl, CQH_HandlerPtr>("CQH_DoHandler"),

		class_<CQH_DoneHandler, CQH_PerHandlerImpl, CQH_HandlerPtr>("CQH_DoneHandler"),

		def("ClearQuestHandlers", &ClearQuestHandlers),
		def("清空任务处理", &ClearQuestHandlers),

		def("ParseQuestHandlers", &ParseQuestHandlers),
		def("解析任务处理", &ParseQuestHandlers),

		def("QuestAcceptOne", &QuestAcceptOne),
		def("接一个任务", &QuestAcceptOne),

		def("QuestDoOne", &QuestDoOne),
		def("做一个任务", &QuestDoOne),

		def("QuestDoneOne", &QuestDoneOne),
		def("提交一个任务", &QuestDoneOne),
		def("完成一个任务", &QuestDoneOne),

		def("QuestHandleAll", &QuestHandleAll),
		def("自动任务", &QuestHandleAll),

		def("QuestStop", &QuestStop),
		def("停止任务", &QuestStop),

		def("MakeSingleHandleQuests", &_MakeSingleHandleQuests_1_),
		def("单次处理任务", &_MakeSingleHandleQuests_1_),
		def("MakeSingleHandleQuests", &_MakeSingleHandleQuests_2_),
		def("单次处理任务", &_MakeSingleHandleQuests_2_),

		def("MakeHandleQuests", &_MakeHandleQuests_1_),
		def("处理任务", &_MakeHandleQuests_1_),
		def("MakeHandleQuests", &_MakeHandleQuests_2_),
		def("处理任务", &_MakeHandleQuests_2_),

		def("MakeHandlePerQuest", &_MakeHandlePerQuest_1_),
		def("处理一个任务", &_MakeHandlePerQuest_1_),
		def("MakeHandlePerQuest", &_MakeHandlePerQuest_2_),
		def("处理一个任务", &_MakeHandlePerQuest_2_),

		def("MakeAcceptHandler", &_MakeAcceptHandler_1_),
		def("接任务处理", &_MakeAcceptHandler_1_),
		def("MakeAcceptHandler", &_MakeAcceptHandler_2_),
		def("接任务处理", &_MakeAcceptHandler_2_),

		def("MakeDoHandler", &_MakeDoHandler_1_),
		def("做任务处理", &_MakeDoHandler_1_),
		def("MakeDoHandler", &_MakeDoHandler_2_),
		def("做任务处理", &_MakeDoHandler_2_),

		def("MakeDoneHandler", &_MakeDoneHandler_1_),
		def("提交任务处理", &_MakeDoneHandler_1_),
		def("完成任务处理", &_MakeDoneHandler_1_),
		def("MakeDoneHandler", &_MakeDoneHandler_2_),
		def("提交任务处理", &_MakeDoneHandler_2_),
		def("完成任务处理", &_MakeDoneHandler_2_)
	];
}

void CQH_Resolver::Stop()
{
	LOG_O(Log_info) << "停止处理任务";
	if (deep_accept_ || deep_do_ || deep_done_ || deep_handle_)
		throw CmnExcept_TerminateQuest();
	else
		assert(false);
}

int CQH_Resolver::ParseQuestHandlers(const luabind::object& tbl)
{
	auto resolver = GetInstance();
	if (!resolver)
	{
		assert(false);
		return 0;
	}
	return resolver->ParseHandlers(tbl);
}

void CQH_Resolver::ClearQuestHandlers()
{
	auto resolver = GetInstance();
	if (!resolver)
	{
		assert(false);
		return;
	}
	resolver->clear();
}

bool CQH_Resolver::QuestAcceptOne()
{
	auto resolver = GetInstance();
	if (!resolver)
	{
		assert(false);
		return false;
	}
	return resolver->AcceptOne();
}

bool CQH_Resolver::QuestDoOne()
{
	auto resolver = GetInstance();
	if (!resolver)
	{
		assert(false);
		return false;
	}
	return resolver->DoOne();
}

bool CQH_Resolver::QuestDoneOne()
{
	auto resolver = GetInstance();
	if (!resolver)
	{
		assert(false);
		return false;
	}
	return resolver->DoneOne();

}

int CQH_Resolver::QuestHandleAll()
{
	auto resolver = GetInstance();
	if (!resolver)
	{
		assert(false);
		return 0;
	}
	return resolver->HandleAll();

}

void CQH_Resolver::QuestStop()
{
	auto resolver = GetInstance();
	if (!resolver)
	{
		assert(false);
		return;
	}
	return resolver->Stop();
}

bool CQH_Resolver::OnNoneQuest()
{
	return false;
}

bool CQH_Resolver::ToHandle(const boost::filesystem::path& handler_path, enHandleFlag flag)
{
	CQH_Handler* res = &root_handler_;
	std::vector<CQH_Node> nodes;
	CQH_Node* parent_node = nullptr;
	for (auto& the_name : handler_path)
	{
		res = res->FindHandler(the_name.string());
		if (!res)
			break;
		nodes.push_back(CQH_Node(parent_node, res));
		parent_node = &nodes.back();
	}
	if (!res || res == &root_handler_)
	{
		LOG_O(Log_error) << "没有找到任务处理者，路径：" << handler_path;
		return false;
	}
	assert(parent_node);
	parent_node = parent_node->GetParentNode();
	switch (flag)
	{
	case CQH_Resolver::kHF_Accept:
		return res->Accept(parent_node);
	case CQH_Resolver::kHF_Do:
		return res->Do(parent_node);
	case CQH_Resolver::kHF_Done:
		return res->Done(parent_node);
	}
	assert(false);
	return false;
}

void CQH_Resolver::OnHandleOne(enHandleFlag flag, const CQH_Handler& handler)
{

}

CQH_HandlerPtr CQH_Resolver::_MakeHandleQuests_1_(const std::string& quest_name)
{
	return MakeHandleQuests(quest_name);
}

CQH_HandlerPtr CQH_Resolver::_MakeHandleQuests_2_(const std::string& quest_name, const luabind::object& params)
{
	return MakeHandleQuests(quest_name, params);
}

CQH_HandlerPtr CQH_Resolver::_MakeHandlePerQuest_1_(const std::string& quest_name)
{
	return MakeHandlePerQuest(quest_name);
}

CQH_HandlerPtr CQH_Resolver::_MakeHandlePerQuest_2_(const std::string& quest_name, const luabind::object& params)
{
	return MakeHandlePerQuest(quest_name, params);
}

CQH_HandlerPtr CQH_Resolver::_MakeAcceptHandler_1_(const luabind::object& fn)
{
	return MakeAcceptHandler(fn);
}

CQH_HandlerPtr CQH_Resolver::_MakeAcceptHandler_2_(const luabind::object& fn, const luabind::object& params)
{
	return MakeAcceptHandler(fn, params);
}

CQH_HandlerPtr CQH_Resolver::_MakeDoHandler_1_(const luabind::object& fn)
{
	return MakeDoHandler(fn);
}

CQH_HandlerPtr CQH_Resolver::_MakeDoHandler_2_(const luabind::object& fn, const luabind::object& params)
{
	return MakeDoHandler(fn, params);
}

CQH_HandlerPtr CQH_Resolver::_MakeDoneHandler_1_(const luabind::object& fn)
{
	return MakeDoneHandler(fn);
}

CQH_HandlerPtr CQH_Resolver::_MakeDoneHandler_2_(const luabind::object& fn, const luabind::object& params)
{
	return MakeDoneHandler(fn, params);
}

CQH_HandlerPtr CQH_Resolver::MakeSingleHandleQuests(const std::string& quest_name)
{
	if (quest_name.empty())
	{
		LOG_O(Log_error) << "单次处理任务之任务名不能为空";
		assert(false);
		return nullptr;
	}
	auto resolver = GetInstance();
	if (!resolver)
	{
		assert(false);
		return nullptr;
	}
	CQH_HandlerPtr res(resolver->CreateSingleHandlerQuests(quest_name));
	assert(res);
	return res;
}

CQH_HandlerPtr CQH_Resolver::MakeSingleHandleQuests(const std::string& quest_name, const luabind::object& params)
{
	auto handler = MakeSingleHandleQuests(quest_name);
	if (!handler)
		return nullptr;
	if (!handler->Parse(params))
		return nullptr;
	return handler;
}

CQH_SingleQuests* CQH_Resolver::CreateSingleHandlerQuests(const std::string& quest_name)
{
	return new CQH_SingleQuests(quest_name);
}

CQH_HandlerPtr CQH_Resolver::_MakeSingleHandleQuests_1_(const std::string& quest_name)
{
	return MakeSingleHandleQuests(quest_name);
}

CQH_HandlerPtr CQH_Resolver::_MakeSingleHandleQuests_2_(const std::string& quest_name, const luabind::object& params)
{
	return MakeSingleHandleQuests(quest_name, params);
}

void CQH_Resolver::OnHandleAll()
{
}

CQH_Resolver* CQH_Resolver::instance_ = nullptr;

const CQH_HandlerCont::HandlerContT& CQH_HandlerCont::GetHandlers() const
{
	return handlers_;
}

void CQH_HandlerCont::clear()
{
	handlers_.clear();
}

void CQH_HandlerCont::Add(const CQH_HandlerPtr& sub_handler)
{
	if (!sub_handler)
	{
		assert(false);
		return;
	}
	if (sub_handler.get() == this)
	{
		assert(false);
		return;
	}
	const auto& name = sub_handler->GetName();
	if (!name.empty())
	{
		for (auto& v : handlers_)
		{
			if (v == sub_handler)
			{
				assert(false);
				return;
			}
			if (v->GetName() == name)
			{
				LOG_O(Log_error) << "添加任务处理失败，已经存在同名任务了：" << name;
				return;
			}
		}
	}
	else
	{
		for (auto& v : handlers_)
		{
			if (v == sub_handler)
			{
				assert(false);
				return;
			}
		}
	}
	handlers_.push_back(sub_handler);
}

int CQH_HandlerCont::GetHandleCount() const
{
	int cnt = 0;
	for (auto& v : handlers_)
	{
		++cnt;
		cnt += v->GetHandleCount();
	}
	return cnt;
}

CQH_HandlerCont::CQH_HandlerCont()
{
	last_test_ = kLT_Invalid;
}

const std::string& CQH_HandlerCont::GetName() const
{
	return kEmptyStr;
}

bool CQH_HandlerCont::Parse(const luabind::object& tbl)
{
	return ParseAsCont(*this, tbl);
}

bool CQH_HandlerCont::CanAccept(const CQH_Node* parent_node) const
{
	CQH_Node cur_node(parent_node, this);
	last_test_ = kLT_Accept;
	last_valid_handler_.reset();
	for (auto& v : handlers_)
	{
		if (v->CanAccept(&cur_node))
		{
			last_valid_handler_ = v;
			return true;
		}
	}
	return false;
}

bool CQH_HandlerCont::Accept(const CQH_Node* parent_node)
{
	BOOST_SCOPE_EXIT_ALL(this){
		last_valid_handler_.reset();
		last_test_ = kLT_Invalid;
	};
	CQH_Node cur_node(parent_node, this);
	if (kLT_Accept == last_test_ && last_valid_handler_)
		goto LABEL_HEAR;
	if (!CQH_HandlerCont::CanAccept(parent_node))
		return false;
	if (!last_valid_handler_)
	{
		assert(false);
		return false;
	}
LABEL_HEAR:
	if (last_valid_handler_->Accept(&cur_node))
	{
		const auto& name = last_valid_handler_->GetName();
		if (!name.empty())
		{
			LOG_O(Log_info) << "成功接取了该子任务：" << name;
		}
		return true;
	}
	return false;
}

bool CQH_HandlerCont::CanDo(const CQH_Node* parent_node) const
{
	CQH_Node cur_node(parent_node, this);
	last_test_ = kLT_Do;
	last_valid_handler_.reset();
	for (auto& v : handlers_)
	{
		if (v->CanDo(&cur_node))
		{
			last_valid_handler_ = v;
			return true;
		}
	}
	return false;
}

bool CQH_HandlerCont::Do(const CQH_Node* parent_node)
{
	BOOST_SCOPE_EXIT_ALL(this){
		last_valid_handler_.reset();
		last_test_ = kLT_Invalid;
	};
	CQH_Node cur_node(parent_node, this);
	if (kLT_Do == last_test_ && last_valid_handler_)
		goto LABEL_HEAR;
	if (!CQH_HandlerCont::CanDo(parent_node))
		return false;
	if (!last_valid_handler_)
	{
		assert(false);
		return false;
	}
LABEL_HEAR:
	if (last_valid_handler_->Do(&cur_node))
	{
		const auto& name = last_valid_handler_->GetName();
		if (!name.empty())
		{
			LOG_O(Log_info) << "成功做了该子任务：" << name;
		}
		return true;
	}
	return false;
}

bool CQH_HandlerCont::CanDone(const CQH_Node* parent_node) const
{
	CQH_Node cur_node(parent_node, this);
	last_test_ = kLT_Done;
	last_valid_handler_.reset();
	for (auto& v : handlers_)
	{
		if (v->CanDone(&cur_node))
		{
			last_valid_handler_ = v;
			return true;
		}
	}
	return false;
}

bool CQH_HandlerCont::Done(const CQH_Node* parent_node)
{
	CQH_Node cur_node(parent_node, this);
	BOOST_SCOPE_EXIT_ALL(this){
		last_valid_handler_.reset();
		last_test_ = kLT_Invalid;
	};
	if (kLT_Done == last_test_ && last_valid_handler_)
		goto LABEL_HEAR;
	if (!CQH_HandlerCont::CanDone(parent_node))
		return false;
	if (!last_valid_handler_)
	{
		assert(false);
		return false;
	}
LABEL_HEAR:
	if (last_valid_handler_->Done(&cur_node))
	{
		const auto& name = last_valid_handler_->GetName();
		if (!name.empty())
		{
			LOG_O(Log_info) << "成功提交/完成了该子任务：" << name;
		}
		return true;
	}	
	return false;
}

bool CQH_HandlerCont::Visit(CQH_Visitor& visitor) const
{
	assert(false);
	return false;
}

CQH_Handler* CQH_HandlerCont::FindHandler(const std::string& name) const
{
	for (auto& v : handlers_)
	{
		if (v->GetName() == name)
			return const_cast<CQH_Handler*>(v.get());
	}
	return nullptr;
}

bool CQH_PerHandler::CanAccept(const CQH_Node* parent_node) const
{
	//assert(false);
	return false;
}

bool CQH_PerHandler::Accept(const CQH_Node* parent_node)
{
	assert(false);
	return false;
}

bool CQH_PerHandler::CanDo(const CQH_Node* parent_node) const
{
	//assert(false);
	return false;
}

bool CQH_PerHandler::Do(const CQH_Node* parent_node)
{
	assert(false);
	return false;
}

bool CQH_PerHandler::CanDone(const CQH_Node* parent_node) const
{
	//assert(false);
	return false;
}

bool CQH_PerHandler::Done(const CQH_Node* parent_node)
{
	assert(false);
	return false;
}

void CQH_PerHandler::Add(const CQH_HandlerPtr& sub_handler)
{
	assert(false);
}

int CQH_PerHandler::GetHandleCount() const
{
	return 0;
}

const std::string& CQH_PerHandler::GetName() const
{
	return kEmptyStr;
}

bool CQH_PerHandler::Parse(const luabind::object& tbl)
{
	assert(false);
	return false;
}

CQH_Handler* CQH_PerHandler::FindHandler(const std::string& name) const
{
	assert(false);
	return nullptr;
}

bool CQH_PerQuest::Visit(CQH_Visitor& visitor) const
{
	assert(false);
	return false;
}

void CQH_PerQuest::Add(const CQH_HandlerPtr& sub_handler)
{
	if (!sub_handler)
	{
		assert(false);
		return;
	}
	if (sub_handler.get() == this)
	{
		assert(false);
		return;
	}
	class Visitor : public CQH_Visitor{
	public:
		Visitor(CQH_PerQuest& handler, const CQH_HandlerPtr& the_handler) : handler_(handler), the_handler_(the_handler){}
		bool VisitAccept(const CQH_AcceptHandler& handler) override{
			if (handler_.handler_accept_)
			{
				LOG_O(Log_error) << "已经添加过接任务处理了：" << handler_.GetName();
				assert(false);
				return false;
			}
			handler_.handler_accept_ = the_handler_;
			return true;
		}
		bool VisitDo(const CQH_DoHandler& handler){
			if (handler_.handler_do_)
			{
				LOG_O(Log_error) << "已经添加过做任务处理了：" << handler_.GetName();
				assert(false);
				return false;
			}
			handler_.handler_do_ = the_handler_;
			return true;
		}
		bool VisitDone(const CQH_DoneHandler& handler){
			if (handler_.handler_done_)
			{
				LOG_O(Log_error) << "已经添加过提交任务处理了：" << handler_.GetName();
				assert(false);
				return false;
			}
			handler_.handler_done_ = the_handler_;
			return true;
		}

		CQH_PerQuest&			handler_;
		const CQH_HandlerPtr&	the_handler_;
	};
	Visitor visitor(*this, sub_handler);
	sub_handler->Visit(visitor);
}

CQH_PerQuest::CQH_PerQuest(const std::string& quest_name) : quest_name_(quest_name)
{

}

const std::string& CQH_PerQuest::GetName() const
{
	return quest_name_;
}

bool CQH_PerQuest::CanAccept(const CQH_Node* parent_node) const
{
	if (!handler_accept_)
		return false;
	CQH_Node cur_node(parent_node, this);
	return handler_accept_->CanAccept(&cur_node);
}

bool CQH_PerQuest::Accept(const CQH_Node* parent_node)
{
	if (!handler_accept_)
		return false;
	auto& name = GetName();
	LOG_O(Log_info) << "任务处理之开始接子任务：" << name;
	CQH_Node cur_node(parent_node, this);
	auto res = handler_accept_->Accept(&cur_node);
	if (res)
		LOG_O(Log_info) << "任务处理之子任务接成功：" << name;
	else
		LOG_O(Log_info) << "任务处理之子任务接失败：" << name;
	return res;
}

bool CQH_PerQuest::CanDo(const CQH_Node* parent_node) const
{
	if (!handler_do_)
		return false;
	CQH_Node cur_node(parent_node, this);
	return handler_do_->CanDo(&cur_node);
}

bool CQH_PerQuest::Do(const CQH_Node* parent_node)
{
	if (!handler_do_)
		return false;
	auto& name = GetName();
	LOG_O(Log_info) << "任务处理之开始做子任务：" << name;
	CQH_Node cur_node(parent_node, this);
	auto res = handler_do_->Do(&cur_node);
	if (res)
		LOG_O(Log_info) << "任务处理之子任务做成功：" << name;
	else
		LOG_O(Log_info) << "任务处理之子任务做失败：" << name;
	return res;
}

bool CQH_PerQuest::CanDone(const CQH_Node* parent_node) const
{
	if (!handler_done_)
		return false;
	CQH_Node cur_node(parent_node, this);
	return handler_done_->CanDone(&cur_node);
}

bool CQH_PerQuest::Done(const CQH_Node* parent_node)
{
	if (!handler_done_)
		return false;
	auto& name = GetName();
	LOG_O(Log_info) << "任务处理之开始完成/提交子任务：" << name;
	CQH_Node cur_node(parent_node, this);
	auto res = handler_done_->Done(&cur_node);
	if (res)
		LOG_O(Log_info) << "任务处理之子任务完成/提交成功：" << name;
	else
		LOG_O(Log_info) << "任务处理之子任务完成/提交失败：" << name;
	return res;
}

bool CQH_PerQuest::Parse(const luabind::object& tbl)
{
	if (!ParseAsCont(*this, tbl))
		return false;
	if (handler_accept_ || handler_do_ || handler_done_)
		return true;
	LOG_O(Log_error) << "任务处理之接、做、提交的任务处理都没有，解析失败：" << GetName();
	return false;
}

CQH_Handler* CQH_PerQuest::FindHandler(const std::string& name) const
{
	if (handler_accept_ && name == handler_accept_->GetName())
		return const_cast<CQH_Handler*>(handler_accept_.get());
	if (handler_do_ && name == handler_do_->GetName())
		return const_cast<CQH_Handler*>(handler_do_.get());
	if (handler_done_ && name == handler_done_->GetName())
		return const_cast<CQH_Handler*>(handler_done_.get());
	return nullptr;
}

bool CQH_AcceptHandler::Visit(CQH_Visitor& visitor) const
{
	return visitor.VisitAccept(*this);
}

CQH_AcceptHandler::CQH_AcceptHandler(const luabind::object& fn) : CQH_PerHandlerImpl(fn)
{
}

bool CQH_AcceptHandler::Accept(const CQH_Node* parent_node)
{
	return CallLua(parent_node);
}

bool CQH_AcceptHandler::CanAccept(const CQH_Node* parent_node) const
{
	assert(false);
	return false;
}

bool CQH_DoHandler::Visit(CQH_Visitor& visitor) const
{
	return visitor.VisitDo(*this);
}

CQH_DoHandler::CQH_DoHandler(const luabind::object& fn) : CQH_PerHandlerImpl(fn)
{
}

bool CQH_DoHandler::Do(const CQH_Node* parent_node)
{
	return CallLua(parent_node);
}

bool CQH_DoHandler::CanDo(const CQH_Node* parent_node) const
{
	assert(false);
	return false;
}

bool CQH_DoneHandler::Visit(CQH_Visitor& visitor) const
{
	return visitor.VisitDone(*this);
}

CQH_DoneHandler::CQH_DoneHandler(const luabind::object& fn) : CQH_PerHandlerImpl(fn)
{
}

bool CQH_DoneHandler::Done(const CQH_Node* parent_node)
{
	return CallLua(parent_node);
}

bool CQH_DoneHandler::CanDone(const CQH_Node* parent_node) const
{
	assert(false);
	return false;
}

CQH_Quests::CQH_Quests(CQH_Resolver& resolver, const std::string& quest_name)
	: CQH_SingleQuests(quest_name), resolver_(resolver)
{
}

const std::string& CQH_SingleQuests::GetName() const
{
	return quest_name_;
}

bool CQH_Quests::Accept(const CQH_Node* parent_node)
{
	int cnt = 0;
	do 
	{
		if (!__super::Accept(parent_node))
			break;
		++cnt;

		if (!resolver_.OnIdle(CQH_Resolver::kHF_Accept))
		{
			assert(false);
			break;
		}
		if (!resolver_.OnUpdateAccept())
		{
			assert(false);
			break;
		}
		if (!CanAccept(parent_node))
			break;
		resolver_.OnHandleOne(CQH_Resolver::kHF_Accept, *this);
	} while (true);
	return cnt > 0;
}

bool CQH_Quests::Do(const CQH_Node* parent_node)
{
	int cnt = 0;
	do
	{
		if (!__super::Do(parent_node))
			break;
		++cnt;

		if (!resolver_.OnIdle(CQH_Resolver::kHF_Do))
		{
			assert(false);
			break;
		}
		if (!resolver_.OnUpdateDo())
		{
			assert(false);
			break;
		}
		if (!CanDo(parent_node))
			break;
		resolver_.OnHandleOne(CQH_Resolver::kHF_Do, *this);
	} while (true);
	return cnt > 0;
}

bool CQH_Quests::Done(const CQH_Node* parent_node)
{
	int cnt = 0;
	do
	{
		if (!__super::Done(parent_node))
			break;
		++cnt;

		if (!resolver_.OnIdle(CQH_Resolver::kHF_Done))
		{
			assert(false);
			break;
		}
		if (!resolver_.OnUpdateDone())
		{
			assert(false);
			break;
		}
		if (!CanDone(parent_node))
			break;
		resolver_.OnHandleOne(CQH_Resolver::kHF_Done, *this);
	} while (true);
	return cnt > 0;
}

CQH_PerHandlerImpl::CQH_PerHandlerImpl(const luabind::object& fn) : fn_impl_(fn)
{
	using namespace luabind;
	assert(LUA_TFUNCTION == type(fn));
	param_direct_ = true;
}

bool CQH_PerHandlerImpl::Parse(const luabind::object& params)
{
	using namespace luabind;
	if (!params.is_valid() || LUA_TTABLE != type(params))
		param_direct_ = true;
	else
		param_direct_ = false;
	fn_params_ = params;
	return true;
}

bool CQH_PerHandlerImpl::CallLua(const CQH_Node* parent_node) const
{
	CQH_Node cur_node(parent_node, this);
	if (param_direct_)
		return LuaObjCallByDlt(fn_impl_, true, &cur_node, fn_params_);
	return LuaObjCallByDlt(fn_impl_, true, &cur_node, luabind::unpack_call(fn_params_));
}

CQH_Node::CQH_Node(const CQH_Node* parent, const CQH_Handler* current)
{
	assert(current);
	parent_ = const_cast<CQH_Node*>(parent);
	current_ = const_cast<CQH_Handler*>(current);
}

CQH_Handler* CQH_Node::GetHandler(int lvl) const
{
	if (lvl <= 0)
		return current_;
	auto cur_node = this;
	do 
	{
		if (!cur_node->parent_)
			break;
		cur_node = cur_node->parent_;
		--lvl;
	} while (lvl > 0);
	assert(cur_node->current_);
	return cur_node->current_;
}

CQH_Node* CQH_Node::GetParentNode() const
{
	return parent_;
}

CQH_SingleQuests::CQH_SingleQuests(const std::string& quest_name)
	: quest_name_(quest_name)
{
}
