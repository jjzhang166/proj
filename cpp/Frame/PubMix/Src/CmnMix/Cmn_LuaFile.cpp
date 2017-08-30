#include "stdafx.h"
#include <CmnMix/Cmn_LuaFile.h>
#include <CmnMix/Cmn_LuaOper.h>
#include <AssetSys/AS_File.h>
#include <map>


ASR_File* Cmn_LuaFileResolver::CreateFileImpl(const AS_NodePtr& node)
{
	return new Cmn_LuaFile(node, this);
}

Cmn_LuaFileResolver::Cmn_LuaFileResolver(const AS_NodePtr& root, lua_State* lstate) : AS_Resolver(root)
{
	assert(lstate);
	lstate_ = lstate;
}

void Cmn_LuaFileResolver::Resolve(const std::string& but_file)
{
	__super::Resolve(but_file);
	for (auto& v : GetFiles())
	{
		auto lua_file = v->As<Cmn_LuaFile>();
		if (!lua_file)
		{
			assert(false);
			continue;
		}
		if (lua_file->EqualFileName(but_file))
			continue;
		lua_file->ExecFile();
	}
}

lua_State* Cmn_LuaFileResolver::lua() const
{
	return lstate_;
}

bool Cmn_LuaFileResolver::SaveAll() const
{
	for (auto& v : GetFiles())
	{
		auto ptr = v->As<Cmn_LuaFile>();
		if (!ptr)
		{
			assert(false);
			return false;
		}
		if (!ptr->Save())
		{
			assert(false);
			return false;
		}
	}
	return true;
}

luabind::object Cmn_LuaFileResolver::GetBasesPermanents(const Cmn_LuaCfgPluto& file, bool is_resume) const
{
	assert(false);
	return luabind::object();
}

Cmn_LuaFile::Cmn_LuaFile(const AS_NodePtr& file, Cmn_LuaFileResolver* parent) : ASR_File(file)
{
	assert(parent);
	parent_ = parent;
}

luabind::object Cmn_LuaFile::ExecFile()
{
	if (!parent_ || !parent_->lstate_)
	{
		assert(false);
		return luabind::object();
	}
	auto file_node = GetFileNode();
	if (!file_node)
	{
		assert(false);
		return luabind::object();
	}
	const auto& file_name = file_node->GetPath().string();
	if (file_name.empty())
	{
		assert(false);
		return luabind::object();
	}
	FileBufferPtr str_buffer(new std::string());
	if (!str_buffer)
	{
		assert(false);
		return luabind::object();
	}
	file_node->GetString().swap(*str_buffer);
	return OnExec(str_buffer, file_name);
}

luabind::object Cmn_LuaFile::OnExec(const FileBufferPtr& buffer, const std::string& file_name)
{
	if (buffer->empty())
		return luabind::object();
	LStateWrapper wrapper(parent_->lstate_);
	return wrapper.DoBuffer(buffer->c_str(), buffer->size(), file_name.c_str());
}

bool Cmn_LuaFile::Save() const
{
	assert(false);
	return false;
}

Cmn_LuaCfgResolver::Cmn_LuaCfgResolver(const AS_NodePtr& root, lua_State* lstate, enTag tag) : Cmn_LuaFileResolver(root, lstate)
{
	tag_ = tag;
}

AS_Node* Cmn_LuaCfgResolver::DiscoverFile() const
{
	if (kNormal == tag_)
		return new AS_DiskFile;
	return new AS_DiskFileE;
}

ASR_File* Cmn_LuaCfgResolver::CreateFileImpl(const AS_NodePtr& node)
{
	if (kPluto == tag_)
		return new Cmn_LuaCfgPluto(node, this);
	return new Cmn_LuaCfgFile(node, this);
}

void Cmn_LuaCfgResolver::Resolve(const std::string& but_file)
{
	using namespace luabind;
	if (!lstate_)
	{
		assert(false);
		return;
	}
	__super::Resolve(but_file);
	for (auto& v : GetFiles())
	{
		auto lua_file = v->As<Cmn_LuaCfgFileBase_>();
		if (!lua_file)
		{
			assert(false);
			continue;
		}
		if (lua_file->EqualFileName(but_file))
			continue;
		if (!lua_file->ResolveSelf())
			assert(false);
	}
}

luabind::object Cmn_LuaCfgResolver::GetBasesPermanents(const Cmn_LuaCfgPluto& file, bool is_resume) const
{
	using namespace luabind;
	luabind::object res;
	auto& dependeces = file.GetDependences();
	if (!dependeces.empty())
	{
		if (dependeces.size() == 1)
		{
			auto& base = FindFile(dependeces[0]);
			if (base)
			{
				auto base_ptr = base->As<Cmn_LuaCfgPluto>();
				if (base_ptr)
				{
					base_ptr->GetPlutoPermanents(is_resume).swap(res);
				}
			}
		}
		else
		{
			for (auto& d : dependeces)
			{
				auto& base = FindFile(d);
				if (!base)
				{
					assert(false);
					continue;
				}
				auto base_ptr = base->As<Cmn_LuaCfgPluto>();
				if (!base_ptr)
				{
					assert(false);
					continue;
				}
				const auto& base_perms = base_ptr->GetPlutoPermanents(is_resume);
				if (!res.is_valid())
				{
					newtable(base_perms.interpreter()).swap(res);
				}
				LStateWrapper::CopyTbl(base_perms, res, false);
			}
		}
	}
	
	if (!res.is_valid())
	{
		newtable(lstate_).swap(res);
	}
	//CreateMultiIdxFn，是闭包生成器，无论是否添加结果表(res)中，效果都一样，暂且把下面这行注释掉吧。
	//LStateWrapper::CopyTbl(GetGloPlutoPermanents(is_resume), res, false);
	return res;
}

ASR_FilePtr Cmn_LuaCfgResolver::AddFile(const std::string& file_name, int map_lvl)
{
	auto res = __super::AddFile(file_name);
	if (res)
	{
		auto ptr = res->As<Cmn_LuaCfgFileBase_>();
		if (ptr)
			ptr->SetMapLevel(map_lvl);
	}
	return res;
}

void Cmn_LuaCfgResolver::RegForLua(lua_State* l)
{
	using namespace luabind;
	module(l)
		[
			class_<ASR_File, ASR_FilePtr>("ASR_File")
			.def("AddDependence", &ASR_File::AddDependence)
			.def("RemoveDependence", &ASR_File::RemoveDependence)
			.def("DeleteDiskFile", &ASR_File::DeleteDiskFile)
			.def("EqualFileName", &ASR_File::EqualFileName)
			.def("GetFileName", &ASR_File::GetFileName),

			class_<AS_Resolver>("AS_Resolver")
			.def("FindFile", &AS_Resolver::FindFile)
			.def("AddFile", &AS_Resolver::AddFile)
			.def("RemoveFile", &AS_Resolver::RemoveFile)
			.def("Clear", &AS_Resolver::Clear)
			.def("DeleteAllDiskFile", &AS_Resolver::DeleteAllDiskFile)
			.def("Resolve", &AS_Resolver::Resolve)
			.def("IteratorFiles", &AS_Resolver::IteratorFiles)
			.def("IteratorDependence", &AS_Resolver::IteratorDependence)
			.def("IteratorInheritable", &AS_Resolver::IteratorInheritable)
			.def("ExistDependence", &AS_Resolver::ExistDependence)
			.def("CanInherit", &AS_Resolver::CanInherit),

			class_<Cmn_LuaFile, ASR_File>("Cmn_LuaFile")
			.def("ExecFile", &Cmn_LuaFile::ExecFile)
			.def("Save", &Cmn_LuaFile::Save),

			class_<Cmn_LuaCfgFileBase_, Cmn_LuaFile>("Cmn_LuaCfgFileBase_")
			.def("SetTable", &Cmn_LuaCfgFileBase_::SetTable)
			.def("GetTable", &Cmn_LuaCfgFileBase_::GetTable)
			.def("SetMapLevel", &Cmn_LuaCfgFileBase_::SetMapLevel)
			.def("GetMapLevel", &Cmn_LuaCfgFileBase_::GetMapLevel)
			.def("ResolveSelf", &Cmn_LuaCfgFileBase_::ResolveSelf)
			.def("LoadCfg", &Cmn_LuaCfgFileBase_::LoadCfg),

			class_<Cmn_LuaCfgFile, Cmn_LuaCfgFileBase_>("Cmn_LuaCfgFile"),

			class_<Cmn_LuaCfgPluto, Cmn_LuaCfgFileBase_>("Cmn_LuaCfgPluto"),

			class_<Cmn_LuaFileResolver, AS_Resolver>("Cmn_LuaFileResolver")
			.def("SaveAll", &Cmn_LuaFileResolver::SaveAll),

			class_<Cmn_LuaCfgResolver, Cmn_LuaFileResolver>("Cmn_LuaCfgResolver")
			.def("AddFile", &Cmn_LuaCfgResolver::_AddFile_c_)
			.def("AddFile", &Cmn_LuaCfgResolver::_AddFile_ci_),

			class_<ASR_FileIterator, IteratorBase, ASR_FileIteratorPtr>("ASR_FileIterator")
			.def("CurItem", &ASR_FileIterator::CurItem)
		];
}

ASR_FilePtr Cmn_LuaCfgResolver::_AddFile_ci_(const std::string& file_name, int map_lvl)
{
	return AddFile(file_name, map_lvl);
}

ASR_FilePtr Cmn_LuaCfgResolver::_AddFile_c_(const std::string& file_name)
{
	return AddFile(file_name);
}

const luabind::object& Cmn_LuaCfgResolver::GetGloPlutoPermanents(bool is_resume) const
{
	assert(false);
	static luabind::object obj;
	return obj;
	/*using namespace luabind;
	if (is_resume)
	{
		if (!glo_resume_permanents_.is_valid())
		{
			luabind::newtable(lstate_).swap(glo_resume_permanents_);
			const auto& multi_idx = LuaObjGlobal(lstate_, "CreateMultiIdxFn");
			assert(LUA_TFUNCTION == type(multi_idx));
			rawset(glo_resume_permanents_, "CreateMultiIdxFn", multi_idx);
		}
		return glo_resume_permanents_;
	}
	if (!glo_save_permanents_.is_valid())
	{
		luabind::newtable(lstate_).swap(glo_save_permanents_);
		const auto& multi_idx = LuaObjGlobal(lstate_, "CreateMultiIdxFn");
		assert(LUA_TFUNCTION == type(multi_idx));
		rawset(glo_save_permanents_, multi_idx, "CreateMultiIdxFn");
	}
	return glo_save_permanents_;*/
}

void Cmn_LuaCfgFileBase_::ResolveBasesMapLvl(const luabind::object& derived, const BasesInfo& bases)
{
	if (bases.empty())
		return;
	using namespace luabind;
	assert(LUA_TTABLE == type(derived));
	{
		auto mt = getmetatable(derived);
		if (LUA_TTABLE != type(mt))
		{
			newtable(parent_->lua()).swap(mt);
			setmetatable(derived, mt);
		}
		if (bases.size() == 1)
		{
			rawset(mt, "__index", bases.begin()->first);
		}
		else
		{
			LStateWrapper wrapper(parent_->lua());
			rawset(mt, "__index", wrapper.CreateMultiIdxFn(bases, [](const BasesInfo::value_type& v){ return v.first; }));
		}
	}
	
	std::map<luabind::object, BasesInfo> key_value_map;
	iterator l_it_end;
	for (auto it = bases.begin(); it != bases.end(); ++it)
	{
		auto& v = *it;
		if (v.second == 0)
			continue;

		for (iterator l_it(v.first); l_it != l_it_end; ++l_it)
		{
			object l_obj(*l_it);
			if (LUA_TTABLE != type(l_obj))
				continue;
			auto& map_value = key_value_map[l_it.key()];
			map_value.push_back(BaseInfo());
			auto& tmp = map_value.back();
			tmp.first.swap(l_obj);
			tmp.second = v.second - 1;
		}
	}
	for (auto& v : key_value_map)
	{
		auto d_tbl = rawget(derived, v.first);
		if (LUA_TTABLE != type(d_tbl))
		{
			newtable(derived.interpreter()).swap(d_tbl);
			rawset(derived, v.first, d_tbl);
		}
		ResolveBasesMapLvl(d_tbl, v.second);
	}
}

void Cmn_LuaCfgPluto::ResolveSelfMapLvl(const luabind::object& obj, const std::string& str_tag, unsigned map_lvl)
{
	using namespace luabind;
	assert(LUA_TTABLE == type(obj));
	AddPlutoPermanents(obj, str_tag);
	if (0 == map_lvl)
		return;
	--map_lvl;
	for (iterator it(obj), it_end; it != it_end; ++it)
	{
		object v(*it);
		if (LUA_TTABLE == type(v))
		{
			object key = it.key();
			ResolveSelfMapLvl(v, str_tag + "." + LStateWrapper::My_ToString(key), map_lvl);
		}
	}
}

Cmn_LuaCfgFile::Cmn_LuaCfgFile(const AS_NodePtr& file, Cmn_LuaFileResolver* parent) : Cmn_LuaCfgFileBase_(file, parent)
{
}

luabind::object Cmn_LuaCfgFile::OnExec(const FileBufferPtr& buffer, const std::string& file_name)
{
	if (buffer->empty())
		return file_tbl_;
	LStateWrapper wrapper(parent_->lua());
	auto chunk = wrapper.LoadBuffer(buffer->c_str(), buffer->size(), file_name.c_str());
	if (!chunk.is_valid())
	{
		assert(false);
		return file_tbl_;
	}
	LuaNewFEnv new_env(chunk, file_tbl_);
	LuaObjCall<void>(new_env.fn_);
	file_tbl_.swap(new_env.new_fenv_);
	{
		luabind::object().swap(new_env.fn_);
	}
	return file_tbl_;
}

luabind::object& Cmn_LuaCfgFileBase_::GetTable()
{
	return file_tbl_;
}

bool Cmn_LuaCfgFileBase_::Save() const
{
	if (!file_tbl_.is_valid())
	{
		assert(false);
		return false;
	}
	auto file_node = GetFileNode();
	if (!file_node)
	{
		assert(false);
		return false;
	}
	const auto& new_buffer = GetSerialize();
	std::istringstream sstm(new_buffer, std::ios_base::in | std::ios_base::binary);
	if (!file_node->Serialize((std::istream&)sstm))
	{
		assert(false);
		return false;
	}
	return true;
}

Cmn_LuaCfgFileBase_::Cmn_LuaCfgFileBase_(const AS_NodePtr& file, Cmn_LuaFileResolver* parent) : Cmn_LuaFile(file, parent)
{
	map_lvl_ = 0;
}

void Cmn_LuaCfgFileBase_::SetMapLevel(int map_lvl)
{
	map_lvl_ = (unsigned)(unsigned int)map_lvl;
}

unsigned Cmn_LuaCfgFileBase_::GetMapLevel() const
{
	return map_lvl_;
}

std::string Cmn_LuaCfgFile::GetSerialize() const
{
	using namespace luabind;
	std::string res;
	iterator it(file_tbl_), end;
	for (; it != end; ++it)
	{
		res += LuaSerialize::Key2String(it.key());
		res += LuaSerialize::ToString(*it);
		res.append(1, '\n');
	}
	return res;
}

bool Cmn_LuaCfgFileBase_::ResolveSelf()
{
	using namespace luabind;
	if (!file_tbl_.is_valid() || LUA_TTABLE != type(file_tbl_))
	{
		newtable(parent_->lua()).swap(file_tbl_);
	}

	auto& dependences = GetDependences();
	if (dependences.empty())
	{
		//这个不能删
		setmetatable(file_tbl_, luabind::object());
	}
	else
	{
		BasesInfo bases;
		for (auto& d : dependences)
		{
			auto& base = parent_->FindFile(d);
			if (!base)
			{
				assert(false);
				continue;
			}
			auto base_ptr = base->As<Cmn_LuaCfgFileBase_>();
			if (!base_ptr)
			{
				assert(false);
				continue;
			}
			const auto& base_res = base_ptr->GetTable();
			if (!base_res.is_valid())
			{
				assert(false);
				continue;
			}
			assert(LUA_TTABLE == type(base_res));
			bases.push_back(BaseInfo());
			auto& v = bases.back();
			v.first = base_res;
			v.second = base_ptr->GetMapLevel();
		}
		ResolveBasesMapLvl(file_tbl_, bases);
	}
	return true;
}

void Cmn_LuaCfgFileBase_::SetTable(const luabind::object& tbl)
{
	assert(tbl.is_valid() && LUA_TTABLE == luabind::type(tbl));
	file_tbl_ = tbl;
}

int Cmn_LuaCfgFileBase_::DeleteDiskFile() const
{
	luabind::object().swap(file_tbl_);
	return __super::DeleteDiskFile();
}

bool Cmn_LuaCfgFileBase_::LoadCfg()
{
	ExecFile();
	return ResolveSelf();
}

Cmn_LuaCfgPluto::Cmn_LuaCfgPluto(const AS_NodePtr& file, Cmn_LuaFileResolver* parent) : Cmn_LuaCfgFileBase_(file, parent)
{
}

luabind::object Cmn_LuaCfgPluto::OnExec(const FileBufferPtr& buffer, const std::string& file_name)
{
	str_buffer_ = buffer;
	return file_tbl_;
}

std::string Cmn_LuaCfgPluto::GetSerialize() const
{
	return LuaSerialize::PlutoPersist(parent_->GetBasesPermanents(*this, false), file_tbl_);
}

bool Cmn_LuaCfgPluto::ResolveSelf()
{
	auto& file_node = GetFileNode();
	if (!file_node)
	{
		assert(false);
		return false;
	}
	bool is_solved = false;
	if (str_buffer_)
	{
		FileBufferPtr str_buffer_tmp(str_buffer_);
		str_buffer_.reset();
		auto file_tbl = LuaSerialize::PlutoUnpersist(parent_->GetBasesPermanents(*this, true), *str_buffer_tmp);
		if (file_tbl.is_valid())
		{
			if (file_tbl_.is_valid())
			{
				if (LStateWrapper::DeepCoverTbl(file_tbl, file_tbl_))
					is_solved = true;
				else
					is_solved = false;
			}
			else
			{
				file_tbl.swap(file_tbl_);
				is_solved = true;
			}
		}
		else
		{
			is_solved = false;
		}
	}
	if (!is_solved && !__super::ResolveSelf())
		return false;
	ResolveSelfMapLvl(file_tbl_, file_node->GetPath().string(), GetMapLevel());
	return true;
}

luabind::object Cmn_LuaCfgPluto::GetPlutoPermanents(bool is_resume) const
{
	if (is_resume)
	{
		if (!resume_permanents_.is_valid())
		{
			luabind::newtable(parent_->lua()).swap(resume_permanents_);
		}
		return resume_permanents_;
	}
	if (!save_permanents_.is_valid())
	{
		luabind::newtable(parent_->lua()).swap(save_permanents_);
	}
	return save_permanents_;
}

void Cmn_LuaCfgPluto::AddPlutoPermanents(const luabind::object& obj, const std::string& str_tag)
{
	assert(!str_tag.empty());
	auto& resume_tbl = GetPlutoPermanents(true);
	resume_tbl[str_tag] = obj;
	auto& save_tbl = GetPlutoPermanents(false);
	save_tbl[obj] = str_tag;
}

int Cmn_LuaCfgPluto::DeleteDiskFile() const
{
	luabind::object().swap(save_permanents_);
	luabind::object().swap(resume_permanents_);
	return __super::DeleteDiskFile();
}
