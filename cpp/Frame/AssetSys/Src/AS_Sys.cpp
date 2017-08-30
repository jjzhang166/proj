#include "stdafx.h"
#include "AS_Sys.h"
#include <boost/scope_exit.hpp>
#include <fstream>
#include <Poco/StreamCopier.h>
#include "AS_Dir.h"
#include <boost/iostreams/filtering_stream.hpp>  
#include <boost/iostreams/copy.hpp>  
#include <boost/iostreams/filter/zlib.hpp>  


AS_Factory::AS_Factory()
{
}

AS_Factory& AS_Factory::GetMe()
{
	static AS_Factory me;
	return me;
}

AS_NodePtr AS_Factory::RegCls(AS_Node* node)
{
	assert(node);
	auto cls_name = node->ClsName();
	assert(cls_name);
	AS_NodePtr res(node);
	auto it = Find(cls_name);
	if (it != holder_.end())
	{
		assert(false);
		return nullptr;
	}
	holder_[cls_name] = res;
	return res;
}

AS_NodePtr AS_Factory::CreateNode(const char* cls_name) const
{
	if (!cls_name)
	{
		assert(false);
		return nullptr;
	}
	auto it = Find(cls_name);
	if (it == holder_.end())
	{
		assert(false);
		return nullptr;
	}
	return AS_NodePtr(it->second->Clone());
}

bool AS_Factory::WriteToSerialize(const AS_NodePtr& node, std::ostream& ostm) const
{
	if (!node)
	{
		assert(false);
		return false;
	}
	auto the_name = node->ClsName();
	assert(the_name);
	auto len = (AS_StrSizeT)strlen(the_name);
	ostm.write((char*)&len, sizeof(len));
	ostm.write(the_name, len);

	AS_Node::GuardLock l(*node);
	auto old = node->GetSMode();
	node->SetSMode(AS_Node::kSM_Sys);
	{
		BOOST_SCOPE_EXIT_ALL(&node, old){
			node->SetSMode(old);
		};
		return node->Serialize(ostm);
	}
}

AS_NodePtr AS_Factory::CreateWithSerialize(std::istream& istm) const
{
	if (!istm)
	{
		//文件可能不存在
		assert(false);
		return nullptr;
	}
	AS_StrSizeT cls_name_cnt = 0;
	istm.read((char*)&cls_name_cnt, sizeof(cls_name_cnt));
	if (cls_name_cnt <= 0)
	{
		assert(false);
		return nullptr;
	}
	auto ptr = DoCreateByName_(istm, cls_name_cnt);
	if (!ptr)
	{
		assert(false);
		return nullptr;
	}

	AS_Node::GuardLock l(*ptr);
	auto old = ptr->GetSMode();
	ptr->SetSMode(AS_Node::kSM_Sys);
	{
		BOOST_SCOPE_EXIT_ALL(&ptr, old){
			ptr->SetSMode(old);
		};
		if (!ptr->Serialize(istm))
		{
			assert(false);
			return nullptr;
		}
	}
	return ptr;
}

AS_NodePtr AS_Factory::DoCreateByName_(std::istream& istm, AS_StrSizeT cnt) const
{
	assert(istm);
	assert(cnt <= AS_kStrMaxSize);
	char buffer[AS_kStrMaxSize + 1];
	istm.read(buffer, cnt);
	buffer[cnt] = 0;
	return CreateNode(buffer);
}

AS_Factory::MapT::const_iterator AS_Factory::Find(const char* name) const
{
	auto end = holder_.end();
	if (!name)
	{
		assert(false);
		return end;
	}
	for (auto it = holder_.begin(); it != end; ++it)
	{
		if (strcmp(it->first, name) == 0)
			return it;
	}
	return end;
}

void AS_Factory::RegForLua(lua_State* l)
{
	assert(l);
	AS_Node::RegForLua(l);
	using namespace luabind;
	module(l)
		[
			class_<AS_Factory>("AS_Factory")
			.def("CreateNode", &AS_Factory::CreateNode)
			.def("ReadFromFile", &AS_Factory::ReadFromFile)
			.def("WriteToFile", &AS_Factory::WriteToFile)
			.def("Iterator", &AS_Factory::Iterator)
			.def("ValidClsNmae", &AS_Factory::ValidClsNmae),
			def("AS_FactoryMe", &AS_Factory::GetMe),

			class_<IteratorT, IteratorBase, IteratorPtr>("AS_FactoryIterator")
			.def("CurItem", &IteratorT::CurItem)
		];
}

AS_NodePtr AS_Factory::ReadFromFile(const std::string& file_name) const
{
	std::ifstream istm(file_name, std::ios_base::in | std::ios_base::binary);
	return CreateWithSerialize_Zlib(istm);
}

bool AS_Factory::WriteToFile(const AS_NodePtr& node, const std::string& file_name) const
{
	std::ifstream istm(file_name, std::ios_base::in | std::ios_base::binary);
	if (istm)
	{
		std::ostringstream sstm(std::ios_base::out | std::ios_base::binary);
		if (!WriteToSerialize_Zlib(node, sstm))
			return false;
		std::string old_str;
		Poco::StreamCopier::copyToString(istm, old_str, 1024);
		const auto& new_str = sstm.str();
		assert(new_str.size() > sizeof(int));
		if (old_str.size() > sizeof(int) &&
			new_str.compare(sizeof(int), new_str.size() - sizeof(int), old_str.c_str() + sizeof(int), old_str.size() - sizeof(int)) == 0)
			return true;
		//要先关闭，否则就写不了文件
		istm.close();
		//boost::filesystem::remove(file_name);
		std::ofstream ostm(file_name, std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
		ostm.write(new_str.c_str(), new_str.size());
		return true;
	}
	std::ofstream ostm(file_name, std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
	return WriteToSerialize_Zlib(node, ostm);
}

AS_Factory::IteratorPtr AS_Factory::Iterator() const
{
	auto this_ptr = const_cast<AS_Factory*>(this);
	const auto& impl = MakeIterator<AS_NodeTraits>(this_ptr->holder_);
	return MakeWrapperIterator<AS_FactoryTraits>(impl, [](const MapT::value_type& v){
		return v.first;
	});
}

AS_Factory::IteratorPtr AS_Factory::ValidClsNmae(const AS_NodePtr& parent) const
{
	if (!parent)
		return Iterator();
	auto this_ptr = const_cast<AS_Factory*>(this);
	auto root = parent->GetRoot(false);
	auto p_cate = parent->GetCate();
	auto cls_name = root->ClsName();
	const auto& impl = MakeIterator<AS_NodeTraits>(this_ptr->holder_, [parent, cls_name, p_cate, this](const MapT::value_type& v){
		if (cls_name == v.first || strcmp(cls_name, v.first) == 0)
			return false;
		if (p_cate == AS_Node::kDisk && p_cate != v.second->GetCate())
			return false;
		return true;
	});
	return MakeWrapperIterator<AS_FactoryTraits>(impl, [](const MapT::value_type& v){
		return v.first;
	});
}

AS_NodePtr AS_Factory::CreateWithSerialize_Zlib(std::istream& istm) const
{
	int dummpy;
	istm.read((char*)&dummpy, sizeof(dummpy));
	std::stringstream ss_decomp(std::ios_base::in | std::ios_base::out | std::ios_base::binary);
	boost::iostreams::filtering_istream in;
	in.push(boost::iostreams::zlib_decompressor());
	in.push(istm);
	auto size = boost::iostreams::copy(in, ss_decomp);
	if (size <= 0)
	{
		assert(false);
		return 0;
	}
	return CreateWithSerialize(ss_decomp);
}

bool AS_Factory::WriteToSerialize_Zlib(const AS_NodePtr& node, std::ostream& ostm) const
{
	std::stringstream ss_comp;
	if (!WriteToSerialize(node, ss_comp))
	{
		assert(false);
		return false;
	}
	int dummpy;
	ostm.write((const char*)&dummpy, sizeof(dummpy));
	boost::iostreams::filtering_ostream out;
	out.push(boost::iostreams::zlib_compressor());
	out.push(ostm);
	boost::iostreams::copy(ss_comp, out);
	return true;
}

AS_Sys::AS_Sys()
{

}

void AS_Sys::SetMe(std::istream& istm, const std::string& sys_dir)
{
	static bool is_init = false;
	if (is_init)
	{
		assert(false);
		return;
	}
	is_init = true;
	assert(!me_.root_);
	auto& factory = AS_Factory::GetMe();
	auto the_ptr = new AS_Root;
	factory.RegCls(the_ptr);
	the_ptr->SetSysDir(sys_dir);
	me_.root_ = factory.CreateWithSerialize_Zlib(istm);
	assert(me_.root_);
}

AS_Sys& AS_Sys::GetMe()
{
	return me_;
}

AS_NodePtr AS_Sys::GetRoot() const
{
	assert(root_);
	return root_;
}

void AS_Sys::RegForLua(lua_State* l)
{
	assert(l);
	AS_Factory::RegForLua(l);
	using namespace luabind;
	module(l)
		[
			class_<AS_Sys>("AS_Sys")
			.def("GetRoot", &AS_Sys::GetRoot),
			def("AS_SysMe", &AS_Sys::GetMe)
		];
}

static bool HasTheFileImpl__(const luabind::object& tbl, const char* file_name){
	using namespace luabind;
	for (iterator it(tbl), it_end; it != it_end; ++it)
	{
		object v = *it;
		auto t = type(v);
		if (LUA_TSTRING == t)
		{
			const auto& str = object_cast<std::string>(v);
			if (str == file_name)
				return true;
		}
		else if (LUA_TTABLE == t)
		{
			if (HasTheFileImpl__(v, file_name))
				return true;
		}
	}
	return false;
}

bool AS_Sys::HasTheFile(const luabind::object& tbl, const char* file_name) const
{
	if (!tbl.is_valid())
		return false;
	if (!file_name)
	{
		assert(false);
		return false;
	}
	using namespace luabind;
	if (LUA_TTABLE != type(tbl))
	{
		assert(false);
		return false;
	}
	return HasTheFileImpl__(tbl, file_name);
}

bool AS_Sys::AddFileToTbl(luabind::object& tbl, const AS_NodePtr& node_root, const AS_NodePtr& file_node) const
{
	using namespace luabind;
	if (!tbl.is_valid())
	{
		assert(false);
		return false;
	}
	if (LUA_TTABLE != type(tbl))
	{
		tbl = newtable(tbl.interpreter());
	}
	if (!node_root || !file_node)
	{
		assert(false);
		return false;
	}
	auto itor = file_node->IteratorAncestor(node_root, true, false, false);
	if (!itor)
	{
		assert(false);
		return false;
	}
	object tbl_ref = tbl;
	for (itor->First(); !itor->IsDone();itor->Next())
	{
		auto item = itor->CurItem();
		if (!item)
		{
			assert(false);
			return false;
		}
		auto dir = item->GetPath().filename().string();
		if (dir.empty())
		{
			assert(false);
			return false;
		}
		object new_tbl = tbl_ref[dir];
		if (LUA_TTABLE != type(new_tbl))
		{
			new_tbl = newtable(tbl_ref.interpreter());
			tbl_ref[dir] = new_tbl;
		}
		tbl_ref = new_tbl;
	}
	auto the_idx = table_maxn(tbl_ref) + 1;
	auto file_name = file_node->GetPath().filename().string();
	assert(!file_name.empty());
	tbl_ref[the_idx] = file_name;
	return true;
}

static bool AddFilesByTblImpl__(const luabind::object& tbl, AS_NodePtr& node_root){
	using namespace luabind;
	for (iterator it(tbl), it_end; it != it_end; ++it)
	{
		object v = *it;
		auto t = type(v);
		if (LUA_TSTRING == t)
		{
			const auto& str = object_cast<std::string>(v);
			if (!node_root->FindPosterity(str))
			{
				assert(false);
				return false;
			}
		}
		else if (LUA_TTABLE == t)
		{
			if (!AddFilesByTblImpl__(v, node_root))
				return false;
		}
	}
	return true;
}

bool AS_Sys::AddFilesByTbl(const luabind::object& tbl, AS_NodePtr& node_root) const
{
	using namespace luabind;
	if (!tbl.is_valid() || LUA_TTABLE != type(tbl))
	{
		assert(false);
		return false;
	}
	if (!node_root)
	{
		assert(false);
		return false;
	}
	return AddFilesByTblImpl__(tbl, node_root);
}

AS_Sys AS_Sys::me_;
