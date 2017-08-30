#include "stdafx.h"
#include "AS_Node.h"
#include <cassert>
#include <vector>
#include <algorithm>
#include <boost/filesystem/operations.hpp>
#include <Poco/StreamCopier.h>

bool operator < (const AS_Node& lhs, const AS_Node& rhs){
	//先以node_lvl_排序，再以path_排序
	int cmp_value = lhs.path_ < rhs.path_;
	return ((int)lhs.node_lvl_ << 4) - cmp_value < ((int)rhs.node_lvl_ << 4) + cmp_value;


	/*static_assert(sizeof(void*) == 4, "");
	static_assert(sizeof(int) == 4, "");
	long long l = (int)lhs.node_lvl_, r = (int)rhs.node_lvl_;
	l <<= 32;
	l |= (int)&lhs;
	r <<= 32;
	r |= (int)&rhs;
	return l < r;*/
}

AS_Node::AS_Node(enAS_NodeLvl node_lvl) : node_lvl_(node_lvl)
{
	assert(node_lvl <= 256);
	smode_ = kSM_Other;
	d2m_ = true;
}

AS_Node::AS_Node(const AS_Node& rhs) : path_(rhs.path_), node_lvl_(rhs.node_lvl_), d2m_(rhs.d2m_)
{
	smode_ = kSM_Other;
}

AS_NodePtr AS_Node::Parent(bool same_cate) const
{
	if (parent_.expired())
		return nullptr;
	auto p = parent_.lock();
	if (!same_cate)
		return p;
	if (!p)
		return nullptr;
	if (p->GetCate() == GetCate())
		return p;
	return nullptr;
}

template<typename T>
static T _Rotate_(T v, T n){
	n %= sizeof(T) * 8;
	auto tmp = v << n;
	v >>= sizeof(T) * 8 - n;
	tmp |= v;
	return tmp;
}

const int kAS_BufferSize = 0xFF;
void AS_Node::Encryptor::Encrypt(const char* buffer, std::string::size_type size, char* out_buffer) const
{
	char src_buffer[kAS_BufferSize];
	auto key_tmp = key_;
	AS_Node::Encryptor::KeyType dummy_value = size;
	while (size != 0)
	{
		auto read_size = size > kAS_BufferSize ? kAS_BufferSize : size;
		std::copy(buffer, buffer + read_size, src_buffer);
		auto beg = (KeyType*)src_buffer;
		while (beg != (KeyType*)src_buffer + read_size / sizeof(key_))
		{
			auto old = *beg;
			*beg ^= key_tmp;
			key_tmp = _Rotate_(key_tmp, *beg + dummy_value);
			key_tmp ^= (old + dummy_value) * dummy_value;
			++beg;
			++dummy_value;
		}
		std::copy((char*)src_buffer, (char*)src_buffer + read_size, out_buffer);
		out_buffer += read_size;
		buffer += read_size;
		size -= read_size;
	}
}

AS_NodePtr AS_Node::GetRoot(bool same_cate) const
{
	auto ptr = shared_from_this();
	while (true)
	{
		const auto& parent = ptr->Parent(same_cate);
		if (!parent)
			return boost::const_pointer_cast<AS_Node>(ptr);
		ptr = parent;
	}
	
	assert(false);
	return nullptr;
}

const boost::filesystem::path& AS_Node::GetPath() const
{
	return path_;
}

void AS_Node::SetPath(const boost::filesystem::path& path)
{
	path_ = path;
	RectifyPath(path_);
}

void AS_Node::SetPath(const boost::filesystem::path& path, const AS_NodePtr& parent)
{
	return SetPath(path);
}

void AS_Node::RectifyPath(boost::filesystem::path& path)
{
	static boost::filesystem::path cur_path(".");
	//../这样的相对目录转换成绝对目录
	path.normalize();
	if (path.filename() == cur_path)
		path = path.parent_path();
	//'/'变成'\\'
	path.make_preferred();
}

boost::filesystem::path AS_Node::GetAbsolutePath(bool same_cate, bool only_file_name, bool skip_when_parent_is_ref) const
{
	std::vector<boost::filesystem::path> vec;
	auto ptr = shared_from_this();
	auto root = GetRoot(false);
	while (ptr)
	{
		if (ptr == root)
			break;
		
		auto& path = ptr->GetPath();
		ptr = ptr->Parent(same_cate);
		if (skip_when_parent_is_ref && ptr)
		{
			if (ptr->GetType() == kRef)
				continue;
		}
		if (only_file_name)
			vec.push_back(path.filename());
		else
			vec.push_back(path);
	}
	
	if (!vec.empty())
		std::reverse(vec.begin(), vec.end());
	boost::filesystem::path res;
	for (auto& v : vec)
	{
		res /= v;
	}
	
	return res;
}

bool AS_Node::Serialize(std::istream& istm)
{
	LockGuard l(mutex_);
	AS_StrSizeT name_cnt = 0;
	istm.read((char*)&name_cnt, sizeof(name_cnt));
	if (name_cnt != 0)
	{
		assert(name_cnt <= AS_kStrMaxSize);
		char buffer[AS_kStrMaxSize + 1];
		istm.read(buffer, name_cnt);
		buffer[name_cnt] = 0;
		path_ = buffer;
	}
	else{
		assert(false);
	}
	istm.read((char*)&node_lvl_, sizeof(char));
	istm.read((char*)&d2m_, sizeof(d2m_));
	return true;
}

bool AS_Node::Serialize(std::ostream& ostm) const
{
	LockGuard l(mutex_);
	const auto& path_name = path_.string();
	auto tmp = (AS_StrSizeT)path_name.size();
	ostm.write((char*)&tmp, sizeof(tmp));
	if (tmp != 0)
		ostm.write(path_name.c_str(), tmp);
	else{
		assert(false);
	}
	ostm.write((char*)&node_lvl_, sizeof(char));
	ostm.write((char*)&d2m_, sizeof(d2m_));
	return true;
}

bool AS_Node::SetParent(const AS_NodePtr& parent)
{
	if (!parent)
	{
		assert(false);
		return false;
	}
	/*if (!parent_.expired())
	{
		assert(false);
		return false;
	}*/
	parent_ = parent;
	return true;
}

bool AS_Node::Add(AS_Node* sub_node, bool replace_existed)
{
	return Add(AS_NodePtr(sub_node), replace_existed);
}

void AS_Node::SetRefTo(const AS_NodePtr& handler)
{

}

AS_NodePtr AS_Node::GetPriDir() const
{
	auto root = GetRoot(false);
	if (!root)
	{
		assert(false);
		return nullptr;
	}
	if (root.get() == this)
		return nullptr;
	return root->GetPriDir();
}

AS_NodePtr AS_Node::GetSysDir() const
{
	auto root = GetRoot(false);
	if (!root)
	{
		assert(false);
		return nullptr;
	}
	if (root.get() == this)
		return nullptr;
	return root->GetSysDir();
}

AS_Node::enAS_NodeLvl AS_Node::GetNodeLvl() const
{
	return node_lvl_;
}

void AS_Node::SetNodeLvl(enAS_NodeLvl lvl)
{
	node_lvl_ = lvl;
	assert(lvl <= 256);
}

void AS_Node::lock() const
{
	mutex_.lock();
}

void AS_Node::unlock() const
{
	mutex_.unlock();
}

AS_Node::enSerializeMode AS_Node::GetSMode() const
{
	return smode_;
}

void AS_Node::SetSMode(enSerializeMode mode)
{
	smode_ = mode;
}

AS_NodeIteratorPtr AS_Node::Iterator(bool is_ref_to) const
{
	return nullptr;
}

void AS_Node::RegForLua(lua_State* l)
{
	using namespace luabind;
	module(l)
		[
			class_<enAS_NodeType>("enAS_NodeType")
			.enum_("constants")
			[
				value("kDir", kDir),
				value("kFile", kFile),
				value("kRef", kRef)
			],

			class_<enAS_NodeCate>("enAS_NodeCate")
			.enum_("constants")
			[
				value("kDisk", kDisk),
				value("kMemory", kMemory)
			],

			class_<enAS_NodeLvl>("enAS_NodeLvl")
			.enum_("constants")
			[
				value("kNL_FileMem", kNL_FileMem),
				value("kNL_DirMem", kNL_DirMem),
				value("kNL_FileDisk", kNL_FileDisk),
				value("kNL_DirDisk", kNL_DirDisk),
				value("kNL_SysDir", kNL_SysDir),
				value("kNL_PriDir", kNL_PriDir),
				value("kNL_EmptyRef", kNL_EmptyRef)
			],

			class_<AS_Node, AS_NodePtr>("AS_Node")
			.def("Add", (bool(AS_Node::*)(const AS_NodePtr&, bool))&AS_Node::Add)
			.def("Erase", (bool(AS_Node::*)(const AS_NodePtr&))&AS_Node::Erase)
			.def("Erase", (bool(AS_Node::*)(const char*))&AS_Node::_Erase_)
			.def("EraseEmptySubDir", &AS_Node::EraseEmptySubDir)
			.def("DeleteDisks", &AS_Node::DeleteDisks)
			.def("Find", &AS_Node::_Find_)
			.def("FindChild", &AS_Node::_FindChild_)
			.def("FindPosterity", &AS_Node::_FindPosterity_)
			.def("FindPosteritySmart", &AS_Node::_FindPosteritySmart_)
			.def("SetRefTo", &AS_Node::SetRefTo)
			.def("Parent", &AS_Node::Parent)
			.def("SetParent", &AS_Node::SetParent)
			.def("GetRoot", &AS_Node::GetRoot)
			.def("GetPriDir", &AS_Node::GetPriDir)
			.def("GetSysDir", &AS_Node::GetSysDir)
			.def("GetType", &AS_Node::GetType)
			.def("GetCate", &AS_Node::GetCate)
			.def("GetNodeLvl", &AS_Node::GetNodeLvl)
			.def("SetNodeLvl", &AS_Node::SetNodeLvl)
			.def("SetPath", &AS_Node::_SetPath_)
			.def("SetPath", &AS_Node::_SetPath_1)
			.def("GetPath", &AS_Node::_GetPath_)
			.def("GetFileName", &AS_Node::GetFileName)
			.def("GetAbsolutePath", &AS_Node::_GetAbsolutePath_)
			.def("Clone", &AS_Node::_Clone_)
			.def("lock", &AS_Node::lock)
			.def("unlock", &AS_Node::unlock)
			.def("Iterator", &AS_Node::_Iterator_)
			.def("Iterator", &AS_Node::_Iterator_1)
			.def("GetString", &AS_Node::GetString)
			.def("ClsName", &AS_Node::ClsName)
			.def("Equal", &AS_Node::_Equal_)
			.def("Refresh", &AS_Node::_Refresh_)
			.def("Empty", &AS_Node::Empty)
			.def("IsD2M", &AS_Node::IsD2M)
			.def("SetD2M", &AS_Node::SetD2M)
			.def("D2M", &AS_Node::_D2M_)
			.def("GetChildCount", &AS_Node::GetChildCount)
			.def("SaveToDisk", &AS_Node::_SaveToDisk_),

			class_<AS_NodeIterator, IteratorBase, AS_NodeIteratorPtr>("AS_NodeIterator")
			.def("CurItem", &AS_NodeIterator::CurItem)
		];
}

bool AS_Node::_Erase_(const char* path)
{
	return Erase(path);
}

AS_NodePtr AS_Node::_Find_(const char* path) const
{
	return Find(path);
}

AS_NodePtr AS_Node::_FindChild_(const char* path) const
{
	return FindChild(path);
}

AS_NodePtr AS_Node::_FindPosterity_(const char* path) const
{
	return FindPosterity(path);
}

void AS_Node::_SetPath_(const char* path)
{
	return SetPath(path);
}

void AS_Node::_SetPath_1(const char* path, const AS_NodePtr& parent)
{
	return SetPath(path, parent);
}

std::string AS_Node::_GetPath_() const
{
	return GetPath().string();
}

std::string AS_Node::_GetAbsolutePath_(bool same_cate, bool only_file_name, bool skip_when_parent_is_ref) const
{
	return GetAbsolutePath(same_cate, only_file_name, skip_when_parent_is_ref).string();
}

AS_NodePtr AS_Node::_Clone_() const
{
	return AS_NodePtr(Clone());
}

bool AS_Node::_Equal_(const AS_NodePtr& rhs) const
{
	return this == rhs.get();
}

bool AS_Node::Refresh(bool is_discory, const AS_NodeDiscovery* discovery)
{
	return false;
}

void AS_Node::SetPathByParent(const boost::filesystem::path& path, const AS_NodePtr& parent)
{		
	if (parent && parent->GetCate() == kDisk)
	{
		auto path_tmp(path);
		RectifyPath(path_tmp);
		return SetPath(path_tmp.filename());
	}
	return SetPath(path);
}

bool AS_Node::Empty() const
{
	return true;
}

AS_NodePtr AS_Node::D2M(const AS_NodeDiscovery* discovery)
{
	return nullptr;
}

bool AS_Node::IsD2M() const
{
	LockGuard l(mutex_);
	return d2m_;
}

void AS_Node::SetD2M(bool is_disk2memory)
{
	LockGuard l(mutex_);
	d2m_ = is_disk2memory;
}

void AS_Node::CopyFrom(AS_Node& lhs, const AS_Node& rhs)
{
	lhs.SetPath(rhs.GetPath());
	//这个不能加
	//lhs.SetNodeLvl(rhs.GetNodeLvl());
	lhs.SetD2M(rhs.IsD2M());
}

AS_NodeIteratorPtr AS_Node::_Iterator_() const
{
	return Iterator(true);
}

AS_NodeIteratorPtr AS_Node::_Iterator_1(bool is_ref_to) const
{
	return Iterator(is_ref_to);
}

bool AS_Node::_Refresh_(bool is_discory)
{
	return Refresh(is_discory);
}

AS_NodePtr AS_Node::_D2M_()
{
	return D2M();
}

std::string AS_Node::GetFileName() const
{
	return path_.filename().string();
}

int AS_Node::DeleteDisks() const
{
	return 0;
}

boost::filesystem::path AS_Node::GetAbsolutePath_() const
{
	return GetAbsolutePath(true, false);
}

AS_NodeIteratorPtr AS_Node::IteratorAncestor(const AS_NodePtr& ancestor_end, bool ancestor_first, bool include_child, bool include_ancestor_end) const
{
	typedef std::vector<AS_NodePtr> Cont;
	Cont nodes;
	auto ptr = boost::const_pointer_cast<AS_Node>(shared_from_this());
	if (!include_child)
		ptr = ptr->Parent(false);
	while (ptr && ptr != ancestor_end)
	{
		nodes.push_back(ptr);
		ptr = ptr->Parent(false);
	}
	if (include_ancestor_end && ancestor_end)
		nodes.push_back(ancestor_end);
	if (ancestor_first)
	{
		std::reverse(nodes.begin(), nodes.end());
	}
	return MakeIterator<AS_NodeIteratorTraits>(nodes);
}

void AS_Node::EraseEmptySubDir()
{

}

AS_NodePtr AS_Node::FindPosteritySmart(const boost::filesystem::path& path, const AS_IFilter* filter, const AS_NodeDiscovery* discovery) const
{
	auto node = Find(path, filter, discovery);
	if (node)
		return node;
	if (path.is_absolute())
		return nullptr;
	return node->FindPosterity(path, filter, discovery);
}

AS_NodePtr AS_Node::_FindPosteritySmart_(const char* path) const
{
	return FindPosteritySmart(path);
}

bool AS_Node::SaveToDisk(const boost::filesystem::path& file_new_path, enSaveFlag flag) const
{
	using namespace boost::filesystem;
	if (flag & kSF_NoOverwrite)
	{
		if (exists(file_new_path))
			return true;
	}
	if ((flag & kSF_NoCreateDir) == 0)
	{
		if (!MakeDirs(file_new_path))
		{
			assert(false);
			return false;
		}
	}
	std::ofstream ofstm(file_new_path.string(), std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
	if (!ofstm)
	{
		assert(false);
		return false;
	}
	return Serialize(ofstm);
}

bool AS_Node::_SaveToDisk_(const char* file_new_path) const
{
	return SaveToDisk(file_new_path);
}

bool AS_Node::Erase(const boost::filesystem::path& path)
{
	LockGuard l(mutex_);
	auto it = Find(path, nullptr);
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
	auto the_parent = it->Parent(false);
	if (!the_parent)
	{
		assert(false);
		return false;
	}
	return the_parent->Erase(it);
}

bool AS_Node::FileSameContent(const std::string& file_name) const
{
	std::ifstream istm(file_name, std::ios_base::in | std::ios_base::binary);
	if (!istm)
		return false;
	std::ostringstream sstm(std::ios_base::out | std::ios_base::binary);
	if (!Serialize(sstm))
		return false;
	std::string old_str;
	Poco::StreamCopier::copyToString(istm, old_str, 1024);
	const auto& new_str = sstm.str();
	return new_str == old_str;
}

bool AS_Node::MakeDirs(const boost::filesystem::path& abs_path)
{
	using namespace boost::filesystem;
	if (exists(abs_path))
		return true;
	auto dir = abs_path;
	dir.remove_filename();
	if (exists(dir))
		return true;
	return boost::filesystem::create_directories(dir);
}

class __AS_Decrypt__{
public:
	virtual void write(const char* buffer, std::string::size_type size) = 0;
};
class __AS_Decrypt__Ostm__ : public __AS_Decrypt__{
	void write(const char* buffer, std::string::size_type size) override{
		ostm_.write(buffer, size);
	}

public:
	__AS_Decrypt__Ostm__(std::ostream& ostm) : ostm_(ostm){}

private:
	std::ostream&	ostm_;
};
class __AS_Decrypt__Str__ : public __AS_Decrypt__{
	void write(const char* buffer, std::string::size_type size) override{
		std::copy(buffer, buffer + size, str_);
		str_ += size;
	}

public:
	__AS_Decrypt__Str__(char* str) : str_(str){}

private:
	char*	str_;
};
static void __Encryptor_Decrypt_Impl__(const char* buffer, std::string::size_type size, AS_Node::Encryptor::KeyType key, __AS_Decrypt__* impl){
	char src_buffer[kAS_BufferSize];
	AS_Node::Encryptor::KeyType dummy_value = size;
	while (size != 0)
	{
		auto write_size = size > kAS_BufferSize ? kAS_BufferSize : size;
		std::copy(buffer, buffer + write_size, src_buffer);
		auto beg = (AS_Node::Encryptor::KeyType*)src_buffer;
		while (beg != (AS_Node::Encryptor::KeyType*)src_buffer + write_size / sizeof(key))
		{
			auto old = *beg;
			*beg ^= key;
			key = _Rotate_(key, old + dummy_value);
			key ^= (*beg + dummy_value) * dummy_value;
			++beg;
			++dummy_value;
		}
		impl->write(src_buffer, write_size);
		buffer += write_size;
		size -= write_size;
	}
}

void AS_Node::Encryptor::Decrypt(const char* buffer, std::string::size_type size, std::ostream& ostm) const
{
	__AS_Decrypt__Ostm__ tmp(ostm);
	__Encryptor_Decrypt_Impl__(buffer, size, key_, &tmp);
}

void AS_Node::Encryptor::Decrypt(const char* buffer, std::string::size_type size, char* out_buffer) const
{
	__AS_Decrypt__Str__ tmp(out_buffer);
	__Encryptor_Decrypt_Impl__(buffer, size, key_, &tmp);
}

AS_Node::Encryptor::Encryptor(KeyType key)
{
	key_ = key;
}
