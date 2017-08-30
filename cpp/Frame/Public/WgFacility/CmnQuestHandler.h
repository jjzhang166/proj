#pragma once
/*
@author zhp
@date 2017/3/6 22:12
@purpose for handle quest struct
*/
#include <boost/smart_ptr.hpp>
#include <luabind/luabind.hpp>
#include <luabind/unpack_call.hpp>
#include <vector>
#include <string>
#include <bitset>
#include <boost/filesystem/path.hpp>
#include <ProtocolX/P_DefinesX.h>
#include "../DependSelfLib.h"

//////////////////////////////////////////////////////////////////////////
class CQH_Resolver;
class CQH_Handler;
class CQH_AcceptHandler;
class CQH_DoHandler;
class CQH_DoneHandler;
typedef boost::shared_ptr<CQH_Handler> CQH_HandlerPtr;
class CQH_Visitor;
//////////////////////////////////////////////////////////////////////////
class CQH_Node{
public:
	CQH_Node(const CQH_Node* parent, const CQH_Handler* current);
	CQH_Handler* GetHandler(int lvl) const;
	CQH_Node* GetParentNode() const;
	template<typename T>
	const T* CastTo() const{
		static_assert(sizeof(T) == sizeof(*this), "");
		return static_cast<const T*>(this);
	}

private:
	CQH_Node*		parent_;
	CQH_Handler*	current_;
};
//////////////////////////////////////////////////////////////////////////
class CQH_Handler{
	friend CQH_Resolver;
public:
	virtual ~CQH_Handler();
	virtual bool CanAccept(const CQH_Node* parent_node) const = 0;
	virtual bool Accept(const CQH_Node* parent_node) = 0;
	virtual bool CanDo(const CQH_Node* parent_node) const = 0;
	virtual bool Do(const CQH_Node* parent_node) = 0;
	virtual bool CanDone(const CQH_Node* parent_node) const = 0;
	virtual bool Done(const CQH_Node* parent_node) = 0;

public:
	virtual void Add(const CQH_HandlerPtr& sub_handler) = 0;
	virtual int GetHandleCount() const = 0;
	virtual const std::string& GetName() const = 0;
	virtual bool Parse(const luabind::object& tbl) = 0;
	virtual bool Visit(CQH_Visitor& visitor) const = 0;
	virtual CQH_Handler* FindHandler(const std::string& name) const = 0;

protected:
	static bool ParseAsCont(CQH_Handler& cont, const luabind::object& tbl);

private:
	const char* __GetName__();
};

class CQH_HandlerCont : public CQH_Handler{
private:
	enum enLastTest{
		kLT_Accept,
		kLT_Do,
		kLT_Done,
		kLT_Invalid,
	};

public:
	typedef std::vector<CQH_HandlerPtr> HandlerContT;

public:
	CQH_HandlerCont();

public:
	const HandlerContT& GetHandlers() const;
	void clear();

public:
	bool CanAccept(const CQH_Node* parent_node) const override;
	bool Accept(const CQH_Node* parent_node) override;
	bool CanDo(const CQH_Node* parent_node) const override;
	bool Do(const CQH_Node* parent_node) override;
	bool CanDone(const CQH_Node* parent_node) const override;
	bool Done(const CQH_Node* parent_node) override;

public:
	void Add(const CQH_HandlerPtr& sub_handler) override;
	int GetHandleCount() const override;
	const std::string& GetName() const override;
	bool Parse(const luabind::object& tbl) override;
	bool Visit(CQH_Visitor& visitor) const override;
	CQH_Handler* FindHandler(const std::string& name) const override;

private:
	HandlerContT			handlers_;
	mutable CQH_HandlerPtr	last_valid_handler_;
	mutable enLastTest		last_test_;
};

class CQH_SingleQuests : public CQH_HandlerCont{
public:
	CQH_SingleQuests(const std::string& quest_name);
	const std::string& GetName() const override;

private:
	std::string		quest_name_;
};

class CQH_Quests : public CQH_SingleQuests{
public:
	CQH_Quests(CQH_Resolver& resolver, const std::string& quest_name);
	bool Accept(const CQH_Node* parent_node) override;
	bool Do(const CQH_Node* parent_node) override;
	bool Done(const CQH_Node* parent_node) override;

private:
	CQH_Resolver&	resolver_;
};

class CQH_PerHandler : public CQH_Handler{
public:
	bool CanAccept(const CQH_Node* parent_node) const override;
	bool Accept(const CQH_Node* parent_node) override;
	bool CanDo(const CQH_Node* parent_node) const override;
	bool Do(const CQH_Node* parent_node) override;
	bool CanDone(const CQH_Node* parent_node) const override;
	bool Done(const CQH_Node* parent_node) override;

public:
	void Add(const CQH_HandlerPtr& sub_handler) override;
	int GetHandleCount() const override;
	const std::string& GetName() const override;
	bool Parse(const luabind::object& tbl) override;
	CQH_Handler* FindHandler(const std::string& name) const override;
};

class CQH_PerQuest : public CQH_PerHandler{
public:
	CQH_PerQuest(const std::string& quest_name);
	bool CanAccept(const CQH_Node* parent_node) const override;
	bool Accept(const CQH_Node* parent_node) override;
	bool CanDo(const CQH_Node* parent_node) const override;
	bool Do(const CQH_Node* parent_node) override;
	bool CanDone(const CQH_Node* parent_node) const override;
	bool Done(const CQH_Node* parent_node) override;

public:
	void Add(const CQH_HandlerPtr& sub_handler) override;
	const std::string& GetName() const override;
	bool Parse(const luabind::object& tbl) override;
	bool Visit(CQH_Visitor& visitor) const override;
	CQH_Handler* FindHandler(const std::string& name) const override;

private:
	std::string			quest_name_;
	CQH_HandlerPtr		handler_accept_;
	CQH_HandlerPtr		handler_do_;
	CQH_HandlerPtr		handler_done_;
};

class CQH_PerHandlerImpl : public CQH_PerHandler{
protected:
	CQH_PerHandlerImpl(const luabind::object& fn);
	bool Parse(const luabind::object& params) override;
	virtual bool CallLua(const CQH_Node* parent_node) const;

protected:
	luabind::object		fn_impl_;
	luabind::object		fn_params_;
	bool				param_direct_;
};

class CQH_AcceptHandler : public CQH_PerHandlerImpl{
public:
	CQH_AcceptHandler(const luabind::object& fn);
	bool CanAccept(const CQH_Node* parent_node) const override;
	bool Accept(const CQH_Node* parent_node) override;
	bool Visit(CQH_Visitor& visitor) const override;
};

class CQH_DoHandler : public CQH_PerHandlerImpl{
public:
	CQH_DoHandler(const luabind::object& fn);
	bool CanDo(const CQH_Node* parent_node) const override;
	bool Do(const CQH_Node* parent_node) override;
	bool Visit(CQH_Visitor& visitor) const override;
};

class CQH_DoneHandler : public CQH_PerHandlerImpl{
public:
	CQH_DoneHandler(const luabind::object& fn);
	bool CanDone(const CQH_Node* parent_node) const override;
	bool Done(const CQH_Node* parent_node) override;
	bool Visit(CQH_Visitor& visitor) const override;
};

template<typename NodeT, typename BaseT>
class CQH_HandlerNodeT : public BaseT{
	static_assert(std::is_same<NodeT, CQH_Node>::value == false, "");
public:
	typedef NodeT NodeType;

public:
	template<typename... Args>
	CQH_HandlerNodeT(Args&... args) : BaseT(args...){}

protected:
	virtual bool CanAccept(const NodeType* parent_node) const{
		return __super::CanAccept(parent_node);
	}
	virtual bool Accept(const NodeType* parent_node){
		return __super::Accept(parent_node);
	}
	virtual bool CanDo(const NodeType* parent_node) const{
		return __super::CanDo(parent_node);
	}
	virtual bool Do(const NodeType* parent_node){
		return __super::Do(parent_node);
	}
	virtual bool CanDone(const NodeType* parent_node) const{
		return __super::CanDone(parent_node);
	}
	virtual bool Done(const NodeType* parent_node){
		return __super::Done(parent_node);
	}

private:
	bool CanAccept(const CQH_Node* parent_node) const override final{
		return CanAccept(parent_node->CastTo<NodeType>());
	}
	bool Accept(const CQH_Node* parent_node) override final{
		return Accept(parent_node->CastTo<NodeType>());
	}
	bool CanDo(const CQH_Node* parent_node) const override final{
		return CanDo(parent_node->CastTo<NodeType>());
	}
	bool Do(const CQH_Node* parent_node) override final{
		return Do(parent_node->CastTo<NodeType>());
	}
	bool CanDone(const CQH_Node* parent_node) const override final{
		return CanDone(parent_node->CastTo<NodeType>());
	}
	bool Done(const CQH_Node* parent_node) override final{
		return Done(parent_node->CastTo<NodeType>());
	}
};

template<typename NodeT, typename BaseT>
class CQH_TryHandlerNodeT : public CQH_HandlerNodeT<NodeT, BaseT>{
public:
	typedef CQH_HandlerNodeT<NodeT, BaseT> HandlerNodeType;
	typedef CQH_TryHandlerNodeT<NodeT, BaseT> TryHandlerNodeType;

public:
	template<typename... Args>
	CQH_TryHandlerNodeT(const Args&... args) : CQH_HandlerNodeT(args...){}

protected:
	virtual bool Try(const NodeT* parent_node) const = 0;
	bool CanAccept(const NodeT* parent_node) const override{
		return Try(parent_node);
	}
	bool CanDo(const NodeT* parent_node) const override{
		return Try(parent_node);
	}
	bool CanDone(const NodeT* parent_node) const override{
		return Try(parent_node);
	}
};

template<typename NodeT, typename BaseT>
class CQH_HandlerImplNodeT : public CQH_HandlerNodeT<NodeT, BaseT>{
public:
	template<typename... Args>
	CQH_HandlerImplNodeT(const Args&... args) : CQH_HandlerNodeT(args...){}

protected:
	bool CallLua(const NodeType* parent_node) const{
		CQH_Node cur_node(parent_node, this);
		if (param_direct_)
			return LuaObjCallByDlt(fn_impl_, true, (NodeType*)cur_node.CastTo<NodeType>(), fn_params_);
		return LuaObjCallByDlt(fn_impl_, true, (NodeType*)cur_node.CastTo<NodeType>(), luabind::unpack_call(fn_params_));
	}

private:
	bool CallLua(const CQH_Node* parent_node) const override final{
		return CallLua(parent_node->CastTo<NodeType>());
	}
};

class CQH_Resolver{
	friend class CQH_Quests;

public:
	enum enHandleFlag{
		kHF_Accept = 0,
		kHF_Do,
		kHF_Done,
		kHF_Handle,
		kHF_Max,
	};

public:
	CQH_Resolver();
	CQH_Resolver(const CQH_Resolver&) = delete;
	CQH_Resolver& operator = (const CQH_Resolver&) = delete;
	virtual ~CQH_Resolver();
	CMN_CAST_TO
	static void RegForLua(lua_State* l);
	static CQH_Resolver* GetInstance();
	static CQH_Resolver& GetMe();
	static void ClearQuestHandlers();
	static int ParseQuestHandlers(const luabind::object& tbl);
	static bool QuestAcceptOne();
	static bool QuestDoOne();
	static bool QuestDoneOne();
	static int QuestHandleAll();
	static void QuestStop();
	static CQH_HandlerPtr MakeSingleHandleQuests(const std::string& quest_name);
	static CQH_HandlerPtr MakeSingleHandleQuests(const std::string& quest_name, const luabind::object& params);
	static CQH_HandlerPtr MakeHandleQuests(const std::string& quest_name);
	static CQH_HandlerPtr MakeHandleQuests(const std::string& quest_name, const luabind::object& params);
	static CQH_HandlerPtr MakeHandlePerQuest(const std::string& quest_name);
	static CQH_HandlerPtr MakeHandlePerQuest(const std::string& quest_name, const luabind::object& params);
	static CQH_HandlerPtr MakeAcceptHandler(const luabind::object& fn);
	static CQH_HandlerPtr MakeAcceptHandler(const luabind::object& fn, const luabind::object& params);
	static CQH_HandlerPtr MakeDoHandler(const luabind::object& fn);
	static CQH_HandlerPtr MakeDoHandler(const luabind::object& fn, const luabind::object& params);
	static CQH_HandlerPtr MakeDoneHandler(const luabind::object& fn);
	static CQH_HandlerPtr MakeDoneHandler(const luabind::object& fn, const luabind::object& params);

public:
	bool AcceptOne();
	bool DoOne();
	bool DoneOne();
	virtual int HandleAll();
	void Stop();
	bool ToHandle(const boost::filesystem::path& handler_path, enHandleFlag flag);

public:
	void AddHandlder(const CQH_HandlerPtr& handler);
	int ParseHandlers(const luabind::object& tbl);
	void clear();

protected:
	virtual bool OnUpdateAccept() = 0;
	virtual bool OnUpdateDo() = 0;
	virtual bool OnUpdateDone() = 0;
	virtual void OnHandleOne(enHandleFlag flag, const CQH_Handler& handler);
	virtual void OnHandleAll();
	virtual bool OnIdle(enHandleFlag flag);
	virtual bool OnNoneQuest();
	virtual CQH_SingleQuests* CreateSingleHandlerQuests(const std::string& quest_name);
	virtual CQH_Quests* CreateHandlerQuests(const std::string& quest_name);
	virtual CQH_PerQuest* CreateHandlerPerQuest(const std::string& quest_name);
	virtual CQH_AcceptHandler* CreateAcceptHandler(const luabind::object& fn) = 0;
	virtual CQH_DoHandler* CreateDoHandler(const luabind::object& fn) = 0;
	virtual CQH_DoneHandler* CreateDoneHandler(const luabind::object& fn) = 0;

private:
	static CQH_HandlerPtr _MakeSingleHandleQuests_1_(const std::string& quest_name);
	static CQH_HandlerPtr _MakeSingleHandleQuests_2_(const std::string& quest_name, const luabind::object& params);
	static CQH_HandlerPtr _MakeHandleQuests_1_(const std::string& quest_name);
	static CQH_HandlerPtr _MakeHandleQuests_2_(const std::string& quest_name, const luabind::object& params);
	static CQH_HandlerPtr _MakeHandlePerQuest_1_(const std::string& quest_name);
	static CQH_HandlerPtr _MakeHandlePerQuest_2_(const std::string& quest_name, const luabind::object& params);
	static CQH_HandlerPtr _MakeAcceptHandler_1_(const luabind::object& fn);
	static CQH_HandlerPtr _MakeAcceptHandler_2_(const luabind::object& fn, const luabind::object& params);
	static CQH_HandlerPtr _MakeDoHandler_1_(const luabind::object& fn);
	static CQH_HandlerPtr _MakeDoHandler_2_(const luabind::object& fn, const luabind::object& params);
	static CQH_HandlerPtr _MakeDoneHandler_1_(const luabind::object& fn);
	static CQH_HandlerPtr _MakeDoneHandler_2_(const luabind::object& fn, const luabind::object& params);

private:
	CQH_HandlerCont			root_handler_;
	std::bitset<kHF_Max>	handle_flag_;
	int						deep_accept_;
	int						deep_do_;
	int						deep_done_;
	int						deep_handle_;

private:
	static CQH_Resolver*	instance_;
};
