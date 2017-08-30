#include "stdafx.h"
#include "AS_Dir.h"
#include "AS_Sys.h"
#include <boost/filesystem/operations.hpp>
#include "AS_File.h"
#include <boost/scope_exit.hpp>
#include "AS_Ref.h"


AS_FACTORY_REG(AS_DiskDir)
AS_FACTORY_REG(AS_MemoryDir)
//AS_Factory::RegHelper<AS_Root> _reg_as_root_;		//已转移到AS_Sys::SetMe中。
static AS_NodeDiscovery discover_dlt_;

AS_Node* AS_NodeDiscovery::DiscoverDir() const
{
	return new AS_DiskDir;
}

AS_Node* AS_NodeDiscovery::DiscoverFile() const
{
	return new AS_DiskFile;
}

AS_DirBase::enAS_NodeType AS_DirBase::GetType() const
{
	return kDir;
}

bool AS_DirBase::Add(const AS_NodePtr& sub_node, bool replace_existed)
{
	LockGuard l(mutex_);
	if (!sub_node)
	{
		assert(false);
		return false;
	}
	assert(!sub_node->GetPath().empty());
	auto it = nodes_.begin();
	auto& the_path = sub_node->GetPath();
	const auto& the_path_filename = the_path.filename();
	for (; it != nodes_.end(); ++it)
	{
		auto& v = *it;
		auto& path = v->GetPath();
		if (path == the_path || path.filename() == the_path_filename)
			break;
	}
	if (it != nodes_.end())
	{
		if (!replace_existed)
		{
			if (sub_node->GetCate() == (*it)->GetCate())
				return true;
			else
				return false;
		}
		if (!Erase(*it))
			return false;
	}
	if (!sub_node->SetParent(shared_from_this()))
	{
		assert(false);
		return false;
	}
	//说明ref还未设置handler(SetRefHandler)
	//assert(sub_node->GetNodeLvl() != kNL_EmptyRef);
	nodes_.insert(sub_node);
	return true;
}

bool AS_DirBase::Erase(const AS_NodePtr& sub_node)
{
	LockGuard l(mutex_);
	auto it = nodes_.find(sub_node);
	if (it != nodes_.end())
	{
		nodes_.erase(it);
		return true;
	}
	assert(false);
	return false;
}

AS_NodePtr AS_DirBase::Find(const boost::filesystem::path& path, const AS_IFilter* filter, const AS_NodeDiscovery* discovery) const
{
	if (path.empty())
		return boost::const_pointer_cast<AS_Node>(shared_from_this());
	boost::filesystem::path pa(path);
	RectifyPath(pa);
	auto self_abs_path = GetAbsolutePath(true, true);
	RectifyPath(self_abs_path);
	auto abs_path = self_abs_path / pa.parent_path();
	RectifyPath(abs_path);
	AS_NodePtr the_node;
	if (abs_path == self_abs_path)
	{
		the_node = boost::const_pointer_cast<AS_Node>(shared_from_this());
	}
	else
	{
		if (!self_abs_path.empty())
		{
			self_abs_path += "/";
			self_abs_path.make_preferred();
		}
		const auto& abs_path_native = abs_path.native();
		auto pos = abs_path_native.find(self_abs_path.native());
		if (pos != abs_path_native.npos)
		{
			the_node = boost::const_pointer_cast<AS_Node>(shared_from_this());
			abs_path = boost::filesystem::path(abs_path.c_str() + pos + self_abs_path.native().size());
			if (!abs_path.empty())
			{
				the_node = FindSub_(the_node, abs_path, filter, false, discovery);
			}
			else
			{
				assert(false);
			}
		}
		else
		{
			auto root = GetRoot(true);
			if (!root)
			{
				assert(false);
				return nullptr;
			}
			the_node = FindSub_(root, abs_path, filter, true, discovery);
		}
		
	}
	if (!the_node)
	{
		//assert(false);
		return nullptr;
	}
	return the_node->FindChild(pa.filename(), filter, discovery);
}

AS_NodePtr AS_DirBase::FindSub_(const AS_NodePtr& the_node, const boost::filesystem::path& sub,
	const AS_IFilter* filter, bool include_self, const AS_NodeDiscovery* discovery)
{
	auto node = the_node;
	assert(node);
	auto it = sub.begin();
	auto end = sub.end();
	if (include_self && it != end)
	{
		auto& path = node->GetPath();
		auto& v = *it;
		if (path == v || path.filename() == v)
			++it;
	}
	for (; it != end; ++it)
	{
		node = node->FindChild(*it, filter, discovery);
		if (!node)
			return nullptr;
	}
	return node;
}

AS_NodePtr AS_DirBase::FindChild(const boost::filesystem::path& path, const AS_IFilter* filter, const AS_NodeDiscovery* discovery) const
{
	LockGuard l(mutex_);
	for (auto& v : nodes_)
	{
		auto& the_path = v->GetPath();
		if (the_path == path || the_path.filename() == path)
		{
			if (!filter)
				return v;
			if (filter->Do(*v))
				return v;
		}
	}
	return nullptr;
}

AS_NodePtr AS_DirBase::FindPosterity(const boost::filesystem::path& path, const AS_IFilter* filter, const AS_NodeDiscovery* discovery) const
{
	LockGuard l(mutex_);
	for (auto& v : nodes_)
	{
		auto& the_path = v->GetPath();
		if (the_path == path || the_path.filename() == path)
		{
			if (!filter)
				return v;
			if (filter->Do(*v))
				return v;
		}
		AS_NodePtr res = v->FindPosterity(path, filter, discovery);
		if (res)
			return res;
	}
	return nullptr;
}

AS_DirBase::AS_DirBase(const AS_DirBase& rhs) : AS_Node(rhs)
{
	LockGuard l(rhs.mutex_);
	for (auto& v : rhs.nodes_)
	{
		nodes_.insert(v);
	}
}

AS_DirBase::AS_DirBase(enAS_NodeLvl node_lvl) : AS_Node(node_lvl)
{

}

bool AS_DirBase::Serialize(std::istream& istm)
{
	LockGuard l(mutex_);
	if (!__super::Serialize(istm))
		return false;
	AS_DirSizeT cnt = 0;
	istm.read((char*)&cnt, sizeof(cnt));
	auto& factory = AS_Factory::GetMe();
	while (cnt > 0)
	{
		AS_NodePtr ptr(factory.CreateWithSerialize(istm));
		if (!ptr)
		{
			assert(false);
			return false;
		}
		if (!Add(ptr, false))
		{
			assert(false);
			return false;
		}
		--cnt;
	}
	return true;
}

bool AS_DirBase::Serialize(std::ostream& ostm) const
{
	LockGuard l(mutex_);
	if (!__super::Serialize(ostm))
		return false;
	auto cnt = (AS_DirSizeT)nodes_.size();
	ostm.write((char*)&cnt, sizeof(cnt));
	auto& factory = AS_Factory::GetMe();
	for (auto& v : nodes_)
	{
		if (!factory.WriteToSerialize(v, ostm))
		{
			assert(false);
			return false;
		}
	}
	return true;
}

std::string AS_DirBase::GetString() const
{
	return std::string();
}

AS_NodeIteratorPtr AS_DirBase::Iterator(bool is_ref_to) const
{
	LockGuard l(mutex_);
	return MakeIterator<AS_NodeIteratorTraits>(nodes_);
}

bool AS_DirBase::Refresh(bool is_discory, const AS_NodeDiscovery* discovery)
{
	SetT tmp;
	LockGuard l(mutex_);
	nodes_.swap(tmp);
	for (auto& v : tmp)
	{
		nodes_.insert(v);
	}
	return true;
}

bool AS_DirBase::Empty() const
{
	LockGuard l(mutex_);
	return nodes_.empty();
}

AS_NodePtr AS_DirBase::D2M(const AS_NodeDiscovery* discovery)
{
	LockGuard l(mutex_);
	if (!d2m_)
		return shared_from_this();
	AS_NodePtr cpy(D2MImpl());
	assert(cpy);
	CopyFrom(*cpy, *this);
	for (auto& v : nodes_)
	{
		const auto& tmp = v->D2M(discovery);
		if (tmp)
			cpy->Add(tmp, false);
	}
	cpy->Refresh(false, discovery);
	return cpy;
}

int AS_DirBase::GetChildCount() const
{
	LockGuard l(mutex_);
	return nodes_.size();
}

void AS_DirBase::EraseEmptySubDir()
{
	std::vector<AS_NodePtr> rubbish;
	LockGuard l(mutex_);
	for (auto& v : nodes_)
	{
		assert(v);
		v->EraseEmptySubDir();
		if (0 == v->GetChildCount())
			rubbish.push_back(v);			
	}
	for (auto& v : rubbish)
	{
		Erase(v);
	}
}

void AS_DirBase::clear()
{
	LockGuard l(mutex_);
	nodes_.clear();
}

AS_Root::AS_Root()
{
	SetPath("root");
}

AS_Root::AS_Root(const AS_Root& rhs) : AS_MemoryDir(rhs), real_sys_dir_(rhs.real_sys_dir_), sys_dir_(rhs.sys_dir_), pri_dir_(rhs.pri_dir_)
{
	//root只能有一个在用，rhs已失效
	if (sys_dir_)
		Add(sys_dir_);
	if (pri_dir_)
		Add(pri_dir_);
}

AS_Node* AS_Root::Clone() const
{
	return new AS_Root(*this);
}

AS_NodePtr AS_Root::GetPriDir() const
{
	if (pri_dir_)
		return pri_dir_;
	LockGuard l(mutex_);
	if (pri_dir_)
		return pri_dir_;
	pri_dir_.reset(new AS_MemoryDir);
	pri_dir_->SetPath(AS_kPriDir);
	pri_dir_->SetParent(boost::const_pointer_cast<AS_Node>(shared_from_this()));
	pri_dir_->SetNodeLvl(kNL_PriDir);
	nodes_.insert(pri_dir_);
	return pri_dir_;
}

AS_NodePtr AS_Root::GetSysDir() const
{
	assert(sys_dir_);
	return sys_dir_;
}

bool AS_Root::Serialize(std::istream& istm)
{
	LockGuard l(mutex_);
	if (!__super::Serialize(istm))
		return false;
	bool has_pri_dir = false;
	istm.read((char*)&has_pri_dir, sizeof(has_pri_dir));
	if (has_pri_dir)
	{
		auto& factory = AS_Factory::GetMe();
		auto ptr = factory.CreateWithSerialize(istm);
		if (!ptr)
		{
			assert(false);
			return false;
		}
		ptr->SetParent(shared_from_this());
		pri_dir_ = ptr;
		nodes_.insert(pri_dir_);
	}
	return true;
}

bool AS_Root::Serialize(std::ostream& ostm) const
{
	LockGuard l(mutex_);
	if (sys_dir_)
		nodes_.erase(sys_dir_);
	if (pri_dir_)
		nodes_.erase(pri_dir_);
	if (!__super::Serialize(ostm))
		return false;
	if (sys_dir_)
		nodes_.insert(sys_dir_);
	if (pri_dir_)
		nodes_.insert(pri_dir_);

	bool has_pri_dir = !!pri_dir_;
	if (has_pri_dir)
		has_pri_dir = !pri_dir_->Empty();
	ostm.write((char*)&has_pri_dir, sizeof(has_pri_dir));
	if (has_pri_dir)
	{
		auto& factory = AS_Factory::GetMe();
		if (!factory.WriteToSerialize(pri_dir_, ostm))
		{
			assert(false);
			return false;
		}
	}
	return true;
}

void AS_Root::SetSysDir(const std::string& sys_dir)
{
	if (sys_dir.empty())
	{
		assert(false);
		return;
	}
	if (real_sys_dir_)
	{
		assert(false);
		return;
	}
	CreateSysDir_(sys_dir_, real_sys_dir_, sys_dir);
	Add(sys_dir_);
}

const char* AS_Root::ClsName() const
{
	return BOOST_PP_STRINGIZE(AS_Root);
}

AS_DirBase* AS_Root::D2MImpl() const
{
	return new AS_Root;
}

AS_NodePtr AS_Root::D2M(const AS_NodeDiscovery* discovery)
{
	LockGuard l(mutex_);
	if (!d2m_)
		return __super::D2M(discovery);

	if (sys_dir_)
		nodes_.erase(sys_dir_);
	if (pri_dir_)
		nodes_.erase(pri_dir_);
	auto res = __super::D2M(discovery);
	BOOST_SCOPE_EXIT_ALL(this){
		if (sys_dir_)
			nodes_.insert(sys_dir_);
		if (pri_dir_)
			nodes_.insert(pri_dir_);
	};
	if (!sys_dir_ && !pri_dir_)
		return res;
	assert(res);
	auto cpy_root = res->As<AS_Root>();
	if (!cpy_root)
	{
		assert(false);
		return res;
	}
	if (sys_dir_)
	{
		assert(real_sys_dir_);
		cpy_root->real_sys_dir_ = real_sys_dir_;
		cpy_root->sys_dir_ = sys_dir_;
		cpy_root->Add(cpy_root->sys_dir_);
	}
	if (pri_dir_)
	{
		cpy_root->pri_dir_ = pri_dir_->D2M(discovery);
		if (cpy_root->pri_dir_)
		{
			cpy_root->pri_dir_->SetNodeLvl(kNL_PriDir);
			cpy_root->Add(cpy_root->pri_dir_);
		}
	}	
	return res;
}

void AS_Root::CreateSysDir_(AS_NodePtr& node_sys, AS_NodePtr& node_real, const boost::filesystem::path& sys_dir)
{
	node_real.reset(new AS_DiskDir);
	node_real->SetPath(sys_dir);
	//node_sys必须是AS_Ref类型，不能随意更改，否则去看下GetAbsolutePath的skip_when_parent_is_ref参数
	node_sys.reset(new AS_Ref);
	node_real->SetParent(node_sys);
	node_sys->SetPath(AS_kSysDir);
	node_sys->SetRefTo(node_real);
	node_sys->SetNodeLvl(kNL_SysDir);
	node_sys->SetD2M(false);
	node_real->SetD2M(false);
}

AS_DiskDir::enAS_NodeCate AS_DiskDir::GetCate() const
{
	return kDisk;
}

AS_DiskDir::AS_DiskDir() : AS_DirBase(kNL_DirDisk)
{

}

AS_DiskDir::AS_DiskDir(const AS_DiskDir& rhs) : AS_DirBase(rhs)
{

}

AS_Node* AS_DiskDir::Clone() const
{
	return new AS_DiskDir(*this);
}

AS_NodePtr AS_DiskDir::FindChild(const boost::filesystem::path& path, const AS_IFilter* filter, const AS_NodeDiscovery* discovery) const
{
	using namespace boost::filesystem;
	auto res = __super::FindChild(path, filter, discovery);
	if (res)
		return res;
	auto abs_path = GetAbsolutePath(true, false) / path;
	if (!exists(abs_path))
		return nullptr;
	if (!discovery)
		discovery = &discover_dlt_;
	AS_NodePtr new_node = nullptr;
	if (is_regular_file(abs_path))
	{
		new_node.reset(discovery->DiscoverFile());
		if (!new_node)
		{
			assert(false);
			return nullptr;
		}
		if (new_node->GetType() != AS_Node::kFile)
		{
			assert(false);
			return nullptr;
		}
	}
	else if (is_directory(abs_path))
	{
		new_node.reset(discovery->DiscoverDir());
		if (!new_node)
		{
			assert(false);
			return nullptr;
		}
		if (new_node->GetType() != AS_Node::kDir)
		{
			assert(false);
			return nullptr;
		}
	}
	else
	{
		assert(false);
		return nullptr;
	}
	if (new_node->GetCate() != AS_Node::kDisk)
	{
		assert(false);
		return nullptr;
	}
	new_node->SetPath(path);
	if (filter && !filter->Do(*new_node))
		return nullptr;

	if (!((AS_Node*)this)->Add(new_node, false))
		return nullptr;
	return new_node;
}

AS_NodePtr AS_DiskDir::FindPosterity(const boost::filesystem::path& path, const AS_IFilter* filter, const AS_NodeDiscovery* discovery) const
{
	using namespace boost::filesystem;
	auto res = __super::FindPosterity(path, filter, discovery);
	if (res)
		return res;
	auto abs_path = GetAbsolutePath(true, false);
	if (!exists(abs_path))
		return nullptr;
	if (!discovery)
		discovery = &discover_dlt_;
	AS_NodePtr new_node;
	boost::filesystem::path new_path;
	recursive_directory_iterator end_iter;
	for (recursive_directory_iterator iter(abs_path); iter != end_iter; ++iter)
	{
		auto& the_path = iter->path();
		if (the_path != path && the_path.filename() != path)
			continue;
		if (is_regular_file(the_path))
		{
			new_node.reset(discovery->DiscoverFile());
			if (!new_node)
			{
				assert(false);
				return nullptr;
			}
			if (new_node->GetType() != AS_Node::kFile)
			{
				assert(false);
				return nullptr;
			}
			new_node->SetPath(the_path);
			if (filter && !filter->Do(*new_node))
			{
				new_node.reset();
				continue;
			}
			new_path = the_path;
			break;
		}
		else if (is_directory(the_path))
		{
			new_node.reset(discovery->DiscoverDir());
			if (!new_node)
			{
				assert(false);
				return nullptr;
			}
			if (new_node->GetType() != AS_Node::kDir)
			{
				assert(false);
				return nullptr;
			}
			new_node->SetPath(the_path);
			if (filter && !filter->Do(*new_node))
			{
				new_node.reset();
				continue;
			}
			new_path = the_path;
			break;
		}
		else
		{
			assert(false);
			return nullptr;
		}
	}
	if (new_path.empty())
	{
		assert(!new_node);
		return nullptr;
	}
	if (!new_node)
	{
		assert(false);
		return nullptr;
	}
	if (new_node->GetCate() != AS_Node::kDisk)
	{
		assert(false);
		return nullptr;
	}
	RectifyPath(new_path);
	RectifyPath(abs_path);
	if (!abs_path.empty())
	{
		abs_path += "/";
		abs_path.make_preferred();
	}
	auto npos = new_path.native().find(abs_path.native());
	if (npos == new_path.native().npos)
	{
		assert(false);
		return nullptr;
	}
	new_path = boost::filesystem::path(new_path.c_str() + npos + abs_path.native().size());
	auto node_ptr = (AS_Node*)this;
	for (auto& v : new_path.parent_path())
	{
		AS_Node* ptr = discovery->DiscoverDir();
		if (!ptr)
		{
			assert(false);
			return nullptr;
		}
		if (ptr->GetType() != AS_Node::kDir || ptr->GetCate() != AS_Node::kDisk)
		{
			assert(false);
			return nullptr;
		}
		ptr->SetPath(v);
		if (!node_ptr->Add(ptr, false))
		{
			assert(false);
			return nullptr;
		}
		node_ptr = ptr;
	}
	new_node->SetPath(new_path.filename());
	if (!node_ptr->Add(new_node))
	{
		assert(false);
		return nullptr;
	}
	return new_node;
}

const char* AS_DiskDir::ClsName() const
{
	return BOOST_PP_STRINGIZE(AS_DiskDir);
}

bool AS_DiskDir::Refresh(bool is_discory, const AS_NodeDiscovery* discovery)
{
	if (!__super::Refresh(is_discory, discovery))
		return false;
	if (!is_discory)
		return true;
	using namespace boost::filesystem;
	auto abs_path = GetAbsolutePath(true, false);
	if (!exists(abs_path))
		return true;
	if (!discovery)
		discovery = &discover_dlt_;
	directory_iterator end_iter;
	//int cnt = 0;
	LockGuard l(mutex_);
	for (directory_iterator iter(abs_path); iter != end_iter; ++iter)
	{
		auto& the_path = iter->path();
		auto& file_name = the_path.filename();
		if (__super::FindChild(file_name, nullptr, discovery))
			continue;
		AS_NodePtr new_node;
		if (is_regular_file(the_path))
		{
			new_node.reset(discovery->DiscoverFile());
			if (!new_node)
			{
				assert(false);
				return false;
			}
			if (new_node->GetType() != AS_Node::kFile)
			{
				assert(false);
				return false;
			}
		}
		else if (is_directory(the_path))
		{
			new_node.reset(discovery->DiscoverDir());
			if (!new_node)
			{
				assert(false);
				return false;
			}
			if (new_node->GetType() != AS_Node::kDir)
			{
				assert(false);
				return false;
			}
		}
		else
		{
			continue;
		}
		if (new_node->GetCate() != AS_Node::kDisk)
		{
			assert(false);
			return false;
		}
		new_node->SetPath(file_name);

		if (!Add(new_node, false))
		{
			assert(false);
			return false;
		}
		//++cnt;
	}
	//return cnt > 0;
	return true;
}

void AS_DiskDir::SetPath(const boost::filesystem::path& path, const AS_NodePtr& parent)
{
	return SetPathByParent(path, parent);
}

AS_DirBase* AS_DiskDir::D2MImpl() const
{
	return new AS_MemoryDir;
}

AS_NodePtr AS_DiskDir::D2M(const AS_NodeDiscovery* discovery)
{
	LockGuard l(mutex_);
	if (!d2m_)
		return nullptr;
	auto ptr = new AS_DiskDir;
	if (!ptr)
	{
		assert(false);
		return nullptr;
	}
	AS_NodePtr holders(ptr);
	auto p = Parent(false);
	if (p)
		ptr->SetParent(p);
	CopyFrom(*ptr, *this);
	for (auto& v : nodes_)
	{
		ptr->nodes_.insert(v);
	}
	//目录为空的情况下才会去刷新，即该目录下所有子元素都包含进来。
	if (ptr->nodes_.empty())
		ptr->Refresh(true, discovery);
	
	auto res = ptr->SupperD2M(discovery);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	res->SetPath(GetPath().filename(), nullptr);
	return res;
}

AS_NodePtr AS_DiskDir::SupperD2M(const AS_NodeDiscovery* discovery)
{
	return __super::D2M(discovery);
}

int AS_DiskDir::DeleteDisks() const
{
	auto abs_path = GetAbsolutePath(true, false);
	if (!exists(abs_path))
		return 0;
	boost::system::error_code err;
	return (int)boost::filesystem::remove_all(abs_path, err);
}

AS_MemoryDir::AS_MemoryDir() : AS_DirBase(kNL_DirMem)
{

}

AS_MemoryDir::AS_MemoryDir(const AS_MemoryDir& rhs) : AS_DirBase(rhs)
{

}

AS_Node* AS_MemoryDir::Clone() const
{
	return new AS_MemoryDir(*this);
}

AS_MemoryFile::enAS_NodeCate AS_MemoryDir::GetCate() const
{
	return kMemory;
}

const char* AS_MemoryDir::ClsName() const
{
	return BOOST_PP_STRINGIZE(AS_MemoryDir);
}

AS_DirBase* AS_MemoryDir::D2MImpl() const
{
	return new AS_MemoryDir;
}

bool AS_DirBase::LessNodePtr::operator()(const AS_NodePtr& lhs, const AS_NodePtr& rhs) const
{
	assert(lhs);
	assert(rhs);
	return *lhs < *rhs;
}
