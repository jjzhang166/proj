#pragma once
/*
@author zhp
@date 2016/11/25 17:12
@purpose for lua file dependence,loader...
*/
#include <AssetSys/As_Resolver.h>
#include <luabind/luabind.hpp>

class Cmn_LuaFileResolver;
class Cmn_LuaFile : public ASR_File{
	friend class Cmn_LuaFileResolver;
protected:
	typedef boost::shared_ptr<std::string> FileBufferPtr;
	Cmn_LuaFile(const AS_NodePtr& file, Cmn_LuaFileResolver* parent);

public:
	luabind::object ExecFile();
	virtual bool Save() const;

protected:
	virtual luabind::object OnExec(const FileBufferPtr& buffer, const std::string& file_name);

protected:
	Cmn_LuaFileResolver*		parent_;
};

class Cmn_LuaCfgFileBase_ : public Cmn_LuaFile{
	friend class Cmn_LuaCfgResolver;
protected:
	Cmn_LuaCfgFileBase_(const AS_NodePtr& file, Cmn_LuaFileResolver* parent);

public:
	bool Save() const override final;
	void SetTable(const luabind::object& tbl);
	luabind::object& GetTable();
	//映射等级，0表示只映射文件，1表示下一级表也映射，依此类推。-1表示映射所有层次的表。
	void SetMapLevel(int map_lvl);
	unsigned GetMapLevel() const;
	virtual bool ResolveSelf();
	bool LoadCfg();
	int DeleteDiskFile() const override;

protected:
	virtual std::string GetSerialize() const = 0;

private:
	typedef std::pair<luabind::object, unsigned> BaseInfo;
	typedef std::vector<BaseInfo> BasesInfo;
	void ResolveBasesMapLvl(const luabind::object& derived, const BasesInfo& bases);

protected:
	mutable luabind::object		file_tbl_;
	unsigned					map_lvl_;
};

class Cmn_LuaCfgFile : public Cmn_LuaCfgFileBase_{
	friend class Cmn_LuaCfgResolver;
protected:
	Cmn_LuaCfgFile(const AS_NodePtr& file, Cmn_LuaFileResolver* parent);

protected:
	luabind::object OnExec(const FileBufferPtr& buffer, const std::string& file_name) override;
	std::string GetSerialize() const override;
};

class Cmn_LuaCfgPluto : public Cmn_LuaCfgFileBase_{
	friend class Cmn_LuaCfgResolver;
protected:
	Cmn_LuaCfgPluto(const AS_NodePtr& file, Cmn_LuaFileResolver* parent);

public:
	bool ResolveSelf() override;
	int DeleteDiskFile() const override;

protected:
	luabind::object OnExec(const FileBufferPtr& buffer, const std::string& file_name) override;
	std::string GetSerialize() const override;

private:
	void ResolveSelfMapLvl(const luabind::object& obj, const std::string& str_tag, unsigned map_lvl);
	luabind::object GetPlutoPermanents(bool is_resume) const;
	void AddPlutoPermanents(const luabind::object& obj, const std::string& str_tag);

private:
	mutable luabind::object	save_permanents_;
	mutable luabind::object	resume_permanents_;
	FileBufferPtr			str_buffer_;
};

class Cmn_LuaFileResolver : public AS_Resolver{
	friend class Cmn_LuaFile;
	friend class Cmn_LuaCfgPluto;
public:
	Cmn_LuaFileResolver(const AS_NodePtr& root, lua_State* lstate);
	void Resolve(const std::string& but_file) override;
	lua_State* lua() const;
	bool SaveAll() const;

protected:
	ASR_File* CreateFileImpl(const AS_NodePtr& node) override;

private:
	virtual luabind::object GetBasesPermanents(const Cmn_LuaCfgPluto& file, bool is_resume) const;

protected:
	lua_State*				lstate_;
};

class Cmn_LuaCfgResolver : public Cmn_LuaFileResolver{
public:
	enum enTag{
		kNormal,		//生成的是正常lua文本，文本不加密
		kEncrypt,		//生成的是正常的lua文本，文本加密
		kPluto,			//生成的是pluto文本，文本加密
	};

public:
	Cmn_LuaCfgResolver(const AS_NodePtr& root, lua_State* lstate, enTag tag);
	using Cmn_LuaFileResolver::AddFile;
	ASR_FilePtr AddFile(const std::string& file_name, int map_lvl);
	void Resolve(const std::string& but_file) override;
	static void RegForLua(lua_State* l);

protected:
	AS_Node* DiscoverFile() const override;
	ASR_File* CreateFileImpl(const AS_NodePtr& node) override;

private:
	luabind::object GetBasesPermanents(const Cmn_LuaCfgPluto& file, bool is_resume) const override;
	const luabind::object& GetGloPlutoPermanents(bool is_resume) const;

private:
	ASR_FilePtr _AddFile_c_(const std::string& file_name);
	ASR_FilePtr _AddFile_ci_(const std::string& file_name, int map_lvl);

private:
	enTag					tag_;
	//mutable luabind::object	glo_save_permanents_;
	//mutable luabind::object	glo_resume_permanents_;
};
