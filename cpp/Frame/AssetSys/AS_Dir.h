#pragma once
/*
@author zhp
@date 2016/11/4 15:27
@purpose Asset System Dir
*/
#include "AS_Node.h"
#include <list>
#include <CommonX/CmnX_Container.h>


class AS_DirBase : public AS_Node{
	struct LessNodePtr{
		bool operator()(const AS_NodePtr& lhs, const AS_NodePtr& rhs) const;
	};
	typedef Cmnx_OrderSet<AS_NodePtr, std::list<AS_NodePtr>, LessNodePtr> SetT;

public:
	AS_DirBase(enAS_NodeLvl node_lvl);
	AS_DirBase(const AS_DirBase& rhs);
	bool Add(const AS_NodePtr& sub_node, bool replace_existed = false) override;
	bool Erase(const AS_NodePtr& sub_node) override;
	void EraseEmptySubDir() override;
	AS_NodePtr Find(const boost::filesystem::path& path, const AS_IFilter* filter = nullptr, const AS_NodeDiscovery* discovery = nullptr) const override;
	AS_NodePtr FindChild(const boost::filesystem::path& path, const AS_IFilter* filter = nullptr, const AS_NodeDiscovery* discovery = nullptr) const override;
	AS_NodePtr FindPosterity(const boost::filesystem::path& path, const AS_IFilter* filter = nullptr, const AS_NodeDiscovery* discovery = nullptr) const override;
	enAS_NodeType GetType() const override;
	AS_NodeIteratorPtr Iterator(bool is_ref_to) const override;
	bool Refresh(bool is_discory, const AS_NodeDiscovery* discovery = nullptr) override;
	bool Empty() const override;
	AS_NodePtr D2M(const AS_NodeDiscovery* discovery = nullptr) override;
	int GetChildCount() const override;
	void clear() override;

public:
	bool Serialize(std::istream& istm) override;
	bool Serialize(std::ostream& ostm) const override;
	std::string GetString() const override;

protected:
	virtual AS_DirBase* D2MImpl() const = 0;

private:
	static AS_NodePtr FindSub_(const AS_NodePtr& node, const boost::filesystem::path& sub, 
		const AS_IFilter* filter, bool include_self, const AS_NodeDiscovery* discovery);

protected:
	mutable SetT	nodes_;
};

class AS_DiskDir : public AS_DirBase{
public:
	AS_DiskDir();
	AS_DiskDir(const AS_DiskDir& rhs);
	int DeleteDisks() const override;
	AS_Node* Clone() const override;
	enAS_NodeCate GetCate() const override;
	const char* ClsName() const override;
	bool Refresh(bool is_discory, const AS_NodeDiscovery* discovery = nullptr) override;
	void SetPath(const boost::filesystem::path& path, const AS_NodePtr& parent) override;
	AS_NodePtr D2M(const AS_NodeDiscovery* discovery = nullptr) override;

public:
	AS_NodePtr FindChild(const boost::filesystem::path& path, const AS_IFilter* filter = nullptr, const AS_NodeDiscovery* discovery = nullptr) const override;
	AS_NodePtr FindPosterity(const boost::filesystem::path& path, const AS_IFilter* filter = nullptr, const AS_NodeDiscovery* discovery = nullptr) const override;

protected:
	AS_DirBase* D2MImpl() const override;

private:
	AS_NodePtr SupperD2M(const AS_NodeDiscovery* discovery);
};

class AS_MemoryDir : public AS_DirBase{
public:
	AS_MemoryDir();
	AS_MemoryDir(const AS_MemoryDir& rhs);
	AS_Node* Clone() const override;
	enAS_NodeCate GetCate() const override;
	const char* ClsName() const override;

protected:
	AS_DirBase* D2MImpl() const override;
};

class AS_Root : public AS_MemoryDir{
public:
	//如果不设置SetSysDir，就表示不需要系统路径
	AS_Root();
	AS_Root(const AS_Root& rhs);
	AS_Node* Clone() const override;
	AS_NodePtr GetPriDir() const override;
	AS_NodePtr GetSysDir() const override;
	bool Serialize(std::istream& istm) override;
	bool Serialize(std::ostream& ostm) const override;
	const char* ClsName() const override;
	AS_NodePtr D2M(const AS_NodeDiscovery* discovery = nullptr) override;

public:
	void SetSysDir(const std::string& sys_dir);

protected:
	AS_DirBase* D2MImpl() const override;

private:
	static void CreateSysDir_(AS_NodePtr& node_sys, AS_NodePtr& node_real, const boost::filesystem::path& sys_dir);

private:
	AS_NodePtr				sys_dir_;
	AS_NodePtr				real_sys_dir_;
	mutable AS_NodePtr		pri_dir_;
};
