#pragma once
/*
@author zhp
@date 2016/11/25 20:01
@purpose for dependece resolve
*/
#include <vector>
#include "AS_Node.h"


//ASR->Asset System Resolver
class ASR_File{
	friend class AS_Resolver;
public:
	typedef std::vector<std::string> DependenceContT;

protected:
	explicit ASR_File(const AS_NodePtr& file);

public:
	virtual ~ASR_File(){}
	void AddDependence(const std::string& file_path);
	void RemoveDependence(const std::string& file_path);
	virtual int DeleteDiskFile() const;
	const DependenceContT& GetDependences() const;
	AS_NodePtr GetFileNode() const;
	bool EqualFileName(const std::string& file_name) const;
	std::string GetFileName() const;
	template<typename T>
	T* As(){
		return dynamic_cast<T*>(this);
	}

private:
	DependenceContT		dependence_;
	AS_NodePtr			file_;
};
typedef boost::shared_ptr<ASR_File> ASR_FilePtr;
typedef IteratorTraits<ASR_FilePtr, IterContTag_ByCopy, IterFilterTag_Custom> ASR_FileIteratorTraits;
typedef ASR_FileIteratorTraits::iterator ASR_FileIterator;
typedef ASR_FileIteratorTraits::iterator_pointer ASR_FileIteratorPtr;

class AS_Resolver : public AS_NodeDiscovery{
	typedef std::vector<ASR_FilePtr> ContT;
public:
	explicit AS_Resolver(const AS_NodePtr& root);
	AS_NodePtr GetRoot() const;
	ASR_FilePtr FindFile(const std::string& file_name) const;
	ASR_FilePtr AddFile(const std::string& file_name);
	void RemoveFile(const ASR_FilePtr& file);
	void Clear();
	int DeleteAllDiskFile(bool delete_root_dir) const;
	virtual void Resolve(const std::string& but_file);
	const ContT& GetFiles() const;
	ASR_FileIteratorPtr IteratorFiles();
	ASR_FileIteratorPtr IteratorDependence(const ASR_FilePtr& file, bool ancestors);
	ASR_FileIteratorPtr IteratorInheritable(const ASR_FilePtr& file);
	bool ExistDependence(const ASR_FilePtr& file, const ASR_FilePtr& file_dependence) const;
	bool CanInherit(const ASR_FilePtr& file, const ASR_FilePtr& file_dependence) const;
	template<typename T>
	T* As(){
		return dynamic_cast<T*>(this);
	}

protected:
	virtual ASR_File* CreateFileImpl(const AS_NodePtr& node);

private:
	void DependenceToVec(const ASR_FilePtr& file, ContT& cont);
	bool ExistDependenceImpl(const ASR_FilePtr& file, const ASR_FilePtr& file_dependence) const;
	bool CanInheritImpl(const ASR_FilePtr& file, const ASR_FilePtr& file_dependence) const;

private:
	ContT		files_;
	AS_NodePtr	root_;
};
