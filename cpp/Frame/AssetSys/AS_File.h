#pragma once
/*
@author zhp
@date 2016/11/4 15:29
@purpose Asset System File
*/
#include "AS_Node.h"

class AS_FileBase : public AS_Node{
public:
	AS_FileBase(enAS_NodeLvl node_lvl);
	AS_FileBase(const AS_FileBase& rhs);
	bool Add(const AS_NodePtr& sub_node, bool replace_existed = false) override;
	bool Erase(const AS_NodePtr& sub_node) override;
	AS_NodePtr Find(const boost::filesystem::path& path, const AS_IFilter* filter = nullptr, const AS_NodeDiscovery* discovery = nullptr) const override;
	AS_NodePtr FindChild(const boost::filesystem::path& path, const AS_IFilter* filter = nullptr, const AS_NodeDiscovery* discovery = nullptr) const override;
	AS_NodePtr FindPosterity(const boost::filesystem::path& path, const AS_IFilter* filter = nullptr, const AS_NodeDiscovery* discovery = nullptr) const override;
	enAS_NodeType GetType() const override;
	int GetChildCount() const override;
	void clear() override;
};

class AS_DiskFile : public AS_FileBase{
public:
	AS_DiskFile();
	AS_DiskFile(const AS_DiskFile& rhs);
	int DeleteDisks() const override;
	AS_Node* Clone() const override;
	enAS_NodeCate GetCate() const override;
	const char* ClsName() const override;
	void SetPath(const boost::filesystem::path& path, const AS_NodePtr& parent) override;
	AS_NodePtr D2M(const AS_NodeDiscovery* discovery = nullptr) override;

public:
	bool Serialize(std::istream& istm) override;
	bool Serialize(std::ostream& ostm) const override;
	std::string GetString() const override;

protected:
	virtual bool DoRead(std::istream& istm, std::ostream& ostm);
	virtual bool DoWrite(std::istream& istm, std::ostream& ostm) const;
	virtual void DoWrite(std::istream& istm, std::string& out_str) const;
};

//encrypt
class AS_DiskFileE : public AS_DiskFile{
public:
	AS_DiskFileE();
	AS_DiskFileE(const AS_DiskFileE& rhs);
	AS_Node* Clone() const override;
	const char* ClsName() const override;

protected:
	bool DoRead(std::istream& istm, std::ostream& ostm) override;
	bool DoWrite(std::istream& istm, std::ostream& ostm) const override;
	void DoWrite(std::istream& istm, std::string& out_str) const override;
};

class AS_MemoryFile : public AS_FileBase{
public:
	AS_MemoryFile();
	AS_MemoryFile(const AS_MemoryFile& rhs);
	AS_Node* Clone() const override;
	enAS_NodeCate GetCate() const override;
	const char* ClsName() const override;
	AS_NodePtr D2M(const AS_NodeDiscovery* discovery = nullptr) override;

public:
	bool Serialize(std::istream& istm) override;
	bool Serialize(std::ostream& ostm) const override;
	std::string GetString() const override;

private:
	std::string			buffer_;
};