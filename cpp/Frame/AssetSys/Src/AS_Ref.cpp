#include "stdafx.h"
#include "AS_Ref.h"
#include "AS_Sys.h"
#include <array>


AS_FACTORY_REG(AS_Ref)

AS_Ref::AS_Ref() : AS_Node(kNL_EmptyRef)
{

}

AS_Ref::AS_Ref(const AS_Ref& rhs) : AS_Node(rhs), handler_(rhs.handler_), ref_path_(rhs.ref_path_)
{

}

AS_Ref::enAS_NodeCate AS_Ref::GetCate() const
{
	return kMemory;
}

bool AS_Ref::Add(const AS_NodePtr& sub_node, bool replace_existed /*= false*/)
{
	const auto& handler = GetHandler();
	if (!handler)
	{
		//assert(false);
		return false;
	}
	return handler->Add(sub_node, replace_existed);
}

AS_NodePtr AS_Ref::GetHandler() const
{
	if (handler_.expired())
	{
		LockGuard l(mutex_);
		if (ref_path_.empty())
			return nullptr;
		if (handler_.expired())
		{
			const auto& root = GetRoot(false);
			if (!root || root.get() == this)
			{
				assert(false);
				return nullptr;
			}
			const auto& ptr = root->Find(ref_path_, nullptr);
			if (!ptr)
			{
				assert(false);
				return nullptr;
			}
			handler_ = ptr;
		}
	}
	return handler_.lock();
}

bool AS_Ref::Erase(const AS_NodePtr& sub_node)
{
	const auto& handler = GetHandler();
	if (!handler)
	{
		//assert(false);
		return false;
	}
	return handler->Erase(sub_node);
}

AS_NodePtr AS_Ref::Find(const boost::filesystem::path& path, const AS_IFilter* filter, const AS_NodeDiscovery* discovery) const
{
	const auto& handler = GetHandler();
	if (!handler)
	{
		//assert(false);
		return nullptr;
	}
	return handler->Find(path, filter, discovery);
}

AS_NodePtr AS_Ref::FindChild(const boost::filesystem::path& path, const AS_IFilter* filter, const AS_NodeDiscovery* discovery) const
{
	const auto& handler = GetHandler();
	if (!handler)
	{
		//assert(false);
		return nullptr;
	}
	return handler->FindChild(path, filter, discovery);
}

void AS_Ref::SetRefTo(const AS_NodePtr& handler)
{
	assert(handler);
	if (handler.get() == this)
	{
		assert(false);
		return;
	}
	if (GetRoot(false) == handler)
	{
		assert(false);
		return;
	}
	assert(handler_.expired());
	handler_ = handler;
	ref_path_ = handler->GetAbsolutePath(false, true, true).string();
	if (GetPath().empty())
	{
		SetPath(handler->GetPath().filename());
	}
	SetNodeLvl(handler->GetNodeLvl());
}

AS_Ref::enAS_NodeType AS_Ref::GetType() const
{
	return kRef;
}

AS_Node* AS_Ref::Clone() const
{
	return new AS_Ref(*this);
}

AS_NodePtr AS_Ref::FindPosterity(const boost::filesystem::path& path, const AS_IFilter* filter, const AS_NodeDiscovery* discovery) const
{
	const auto& handler = GetHandler();
	if (!handler)
	{
		//assert(false);
		return nullptr;
	}
	return handler->FindPosterity(path, filter, discovery);
}

bool AS_Ref::Serialize(std::istream& istm)
{
	LockGuard l(mutex_);
	if (smode_ == kSM_Sys)
	{
		if (!__super::Serialize(istm))
		{
			assert(false);
			return false;
		}
		if (!ReadRefPath(istm))
			return false;
		return true;
	}
	const auto& handler = GetHandler();
	if (!handler)
	{
		return true;
	}
	return handler->Serialize(istm);
}

bool AS_Ref::Serialize(std::ostream& ostm) const
{
	LockGuard l(mutex_);
	if (smode_ == kSM_Sys)
	{
		if (!__super::Serialize(ostm))
		{
			assert(false);
			return false;
		}
		auto size = (AS_StrSizeT)ref_path_.size();
		ostm.write((char*)&size, sizeof(size));
		ostm.write(ref_path_.c_str(), size);
		return true;
	}
	const auto& handler = GetHandler();
	if (!handler)
	{
		return true;
	}
	return handler->Serialize(ostm);
}

bool AS_Ref::ReadRefPath(std::istream& istm)
{
	AS_StrSizeT name_cnt = 0;
	istm.read((char*)&name_cnt, sizeof(name_cnt));
	if (name_cnt != 0)
	{
		ref_path_.resize(name_cnt);
		istm.read((char*)ref_path_.c_str(), name_cnt);
	}
	return true;
}

std::string AS_Ref::GetString() const
{
	const auto& handler = GetHandler();
	if (!handler)
	{
		//assert(false);
		return std::string();
	}
	return handler->GetString();
}

AS_NodeIteratorPtr AS_Ref::Iterator(bool is_ref_to) const
{
	const auto& handler = GetHandler();
	if (!handler)
	{
		return nullptr;
	}
	if (is_ref_to)
	{
		LockGuard l(mutex_);
		typedef std::array<AS_NodePtr, 1> ArrayT;
		ArrayT ar;
		ar[0] = handler;
		return MakeIterator<AS_NodeIteratorTraits>(ar);
	}
	else
	{
		return handler->Iterator(is_ref_to);
	}
}

const char* AS_Ref::ClsName() const
{
	return BOOST_PP_STRINGIZE(AS_Ref);
}

bool AS_Ref::Refresh(bool is_discory, const AS_NodeDiscovery* discovery)
{
	const auto& handler = GetHandler();
	if (!handler)
	{
		//assert(false);
		return false;
	}
	return handler->Refresh(is_discory, discovery);
}

bool AS_Ref::Empty() const
{
	LockGuard l(mutex_);
	if (handler_.expired())
		return true;
	return false;
}

AS_NodePtr AS_Ref::D2M(const AS_NodeDiscovery* discovery)
{
	LockGuard l(mutex_);
	handler_.reset();
	return shared_from_this();
}

int AS_Ref::DeleteDisks() const
{
	const auto& handler = GetHandler();
	if (!handler)
		return 0;
	return handler->DeleteDisks();
}

int AS_Ref::GetChildCount() const
{
	const auto& handler = GetHandler();
	if (!handler)
		return -2;
	return handler->GetChildCount();
}

void AS_Ref::EraseEmptySubDir()
{
	const auto& handler = GetHandler();
	if (!handler)
	{
		//assert(false);
		return;
	}
	return handler->EraseEmptySubDir();
}

void AS_Ref::clear()
{
	const auto& handler = GetHandler();
	if (!handler)
		return;
	handler->clear();
}
