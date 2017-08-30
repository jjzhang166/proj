#pragma once
/*
@author zhp
@date 2016/11/4 15:48
@purpose Asset System Reference
*/
#include "AS_Node.h"

class AS_Ref : public AS_Node{
public:
	AS_Ref();
	AS_Ref(const AS_Ref& rhs);
	bool Add(const AS_NodePtr& sub_node, bool replace_existed = false) override;
	bool Erase(const AS_NodePtr& sub_node) override;
	void EraseEmptySubDir() override;
	int DeleteDisks() const override;
	AS_NodePtr Find(const boost::filesystem::path& path, const AS_IFilter* filter = nullptr, const AS_NodeDiscovery* discovery = nullptr) const override;
	AS_NodePtr FindChild(const boost::filesystem::path& path, const AS_IFilter* filter = nullptr, const AS_NodeDiscovery* discovery = nullptr) const override;
	AS_NodePtr FindPosterity(const boost::filesystem::path& path, const AS_IFilter* filter = nullptr, const AS_NodeDiscovery* discovery = nullptr) const override;
	void SetRefTo(const AS_NodePtr& handler) override;
	enAS_NodeType GetType() const override;
	enAS_NodeCate GetCate() const override;
	AS_Node* Clone() const override;
	AS_NodeIteratorPtr Iterator(bool is_ref_to) const override;
	const char* ClsName() const override;
	bool Refresh(bool is_discory, const AS_NodeDiscovery* discovery = nullptr) override;
	bool Empty() const override;
	AS_NodePtr D2M(const AS_NodeDiscovery* discovery = nullptr) override;
	int GetChildCount() const override;
	void clear() override;

public:
	bool Serialize(std::istream& istm) override;
	bool Serialize(std::ostream& ostm) const override;
	std::string GetString() const override;

private:
	AS_NodePtr GetHandler() const;
	bool ReadRefPath(std::istream& istm);

private:
	mutable AS_NodeWeakPtr	handler_;
	std::string				ref_path_;
};
