#pragma once
/*
@author zhp
@date 2016/11/4 15:23
@purpose Asset System Node
*/
#include "./DependSelfLib.h"
#include "AS_Filter.h"
#include "./DependExternLib.h"
#include <string>
#include <boost/filesystem/path.hpp>
#include <iosfwd>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/lock_guard.hpp>
#include <CmnMix/Cmn_Iterator.h>
#include <luabind/luabind.hpp>


//////////////////////////////////////////////////////////////////////////
class AS_Node;
typedef boost::shared_ptr<AS_Node> AS_NodePtr;
typedef boost::weak_ptr<AS_Node> AS_NodeWeakPtr;
//////////////////////////////////////////////////////////////////////////
typedef IteratorTraits<AS_NodePtr, IterContTag_ByCopy> AS_NodeIteratorTraits;
typedef AS_NodeIteratorTraits::iterator AS_NodeIterator;
typedef AS_NodeIteratorTraits::iterator_pointer AS_NodeIteratorPtr;
//////////////////////////////////////////////////////////////////////////
typedef unsigned char AS_StrSizeT;
const AS_StrSizeT AS_kStrMaxSize = 255;
typedef unsigned char AS_DirSizeT;
//系统路径，默认为exe或dll所在路径
const char* const AS_kSysDir = "*";
//私有路径
const char* const AS_kPriDir = "|";
//////////////////////////////////////////////////////////////////////////
//返回的都必须是Disk
class AS_NodeDiscovery{
public:
	virtual AS_Node* DiscoverDir() const;
	virtual AS_Node* DiscoverFile() const;
};
//////////////////////////////////////////////////////////////////////////
//操作文件和目录时是线程安全的，其他操作不是线程安全的，其他操作一般只在同一个线程，比如创建一个node，然后SetPath。
class AS_Node : public boost::enable_shared_from_this<AS_Node>{
	friend class AS_Factory;

public:
	typedef boost::recursive_mutex MutexT;
	//write lock
	typedef boost::lock_guard<MutexT> LockGuard;
	typedef boost::lock_guard<const AS_Node> GuardLock;
	enum enAS_NodeType{
		kDir,
		kFile,
		kRef,	//快捷方式
	};

	//种类
	enum enAS_NodeCate{
		kDisk,		//硬盘上的
		kMemory,	//内存上的
	};
	class Encryptor{
	public:
		typedef unsigned int KeyType;
		Encryptor(KeyType key);
		void Encrypt(const char* buffer, std::string::size_type size, char* out_buffer) const;
		void Decrypt(const char* buffer, std::string::size_type size, std::ostream& ostm) const;
		void Decrypt(const char* buffer, std::string::size_type size, char* out_buffer) const;

	private:
		KeyType		key_;
	};
	enum enSaveFlag{
		kSF_Unknown = 0,
		kSF_NoOverwrite = 1 << 1,
		kSF_NoCreateDir = 1 << 2,	//如果路径不存在，也不会先创建路径
	};

protected:
	//数值越小，越排在前面；排在前面的优先被搜索。
	enum enAS_NodeLvl{
		kNL_FileMem = 0,
		kNL_DirMem,
		kNL_FileDisk,
		kNL_DirDisk,
		kNL_SysDir,
		kNL_PriDir,
		kNL_EmptyRef,
	};
	friend bool operator < (const AS_Node& lhs, const AS_Node& rhs);

	enum enSerializeMode{
		kSM_Sys,
		kSM_Other,
	};

public:
	AS_Node(enAS_NodeLvl node_lvl);
	AS_Node(const AS_Node& rhs);
	virtual ~AS_Node(){}
	//添加子节点
	bool Add(AS_Node* sub_node, bool replace_existed = false);
	virtual bool Add(const AS_NodePtr& sub_node, bool replace_existed = false) = 0;
	//不能删除自身节点
	//只能删除子节点
	virtual bool Erase(const AS_NodePtr& sub_node) = 0;
	//能删除后辈节点
	bool Erase(const boost::filesystem::path& path);
	//删除空子文件夹
	virtual void EraseEmptySubDir();
	//删除硬盘文件或文件夹
	virtual int DeleteDisks() const;
	//可以查询父辈节点和后辈节点
	virtual AS_NodePtr Find(const boost::filesystem::path& path, const AS_IFilter* filter = nullptr, const AS_NodeDiscovery* discovery = nullptr) const = 0;
	//只查询子节点
	virtual AS_NodePtr FindChild(const boost::filesystem::path& path, const AS_IFilter* filter = nullptr, const AS_NodeDiscovery* discovery = nullptr) const = 0;
	//查询后代，查询后代中GetPath()==path的节点
	virtual AS_NodePtr FindPosterity(const boost::filesystem::path& path, const AS_IFilter* filter = nullptr, const AS_NodeDiscovery* discovery = nullptr) const = 0;
	AS_NodePtr FindPosteritySmart(const boost::filesystem::path& path, const AS_IFilter* filter = nullptr, const AS_NodeDiscovery* discovery = nullptr) const;
	virtual void SetRefTo(const AS_NodePtr& handler);
	AS_NodePtr Parent(bool same_cate) const;
	bool SetParent(const AS_NodePtr& parent);
	AS_NodePtr GetRoot(bool same_cate) const;
	virtual AS_NodePtr GetPriDir() const;
	virtual AS_NodePtr GetSysDir() const;
	virtual enAS_NodeType GetType() const = 0;
	virtual enAS_NodeCate GetCate() const = 0;
	//用于目录中节点的排序
	enAS_NodeLvl GetNodeLvl() const;
	void SetNodeLvl(enAS_NodeLvl lvl);
	template<typename T>
	T* As(){
		return dynamic_cast<T*>(this);
	}
	void SetPath(const boost::filesystem::path& path);
	virtual void SetPath(const boost::filesystem::path& path, const AS_NodePtr& parent);
	const boost::filesystem::path& GetPath() const;
	std::string GetFileName() const;
	boost::filesystem::path GetAbsolutePath(bool same_cate = true, bool only_file_name = true, bool skip_when_parent_is_ref = false) const;
	//得到文件系统绝对目录
	boost::filesystem::path GetAbsolutePath_() const;
	virtual AS_Node* Clone() const = 0;
	virtual void lock() const;
	virtual void unlock() const;
	virtual AS_NodeIteratorPtr Iterator(bool is_ref_to) const;
	AS_NodeIteratorPtr IteratorAncestor(const AS_NodePtr& ancestor, bool ancestor_first, bool include_child, bool include_ancestor_end) const;
	virtual bool Refresh(bool is_discory, const AS_NodeDiscovery* discovery = nullptr);
	virtual bool Empty() const;
	bool IsD2M() const;
	void SetD2M(bool is_disk2memory);
	virtual int GetChildCount() const = 0;
	bool SaveToDisk(const boost::filesystem::path& file_new_path, enSaveFlag flag = kSF_Unknown) const;
	virtual void clear() = 0;
	bool FileSameContent(const std::string& file_name) const;

public:
	virtual const char* ClsName() const = 0;
	//可以以std::ios_base::binary的方式打开流，也可以默认文本的方式打开流。
	virtual bool Serialize(std::istream& istm);
	virtual bool Serialize(std::ostream& ostm) const;
	virtual std::string GetString() const = 0;
	//disk to memory，有可能会改变parent_属性(为了效率)，所以调用后最好就不要再用该对象了
	virtual AS_NodePtr D2M(const AS_NodeDiscovery* discovery = nullptr);
	static bool MakeDirs(const boost::filesystem::path& abs_path);

protected:
	static void RectifyPath(boost::filesystem::path& path);
	void SetPathByParent(const boost::filesystem::path& path, const AS_NodePtr& parent);
	static void CopyFrom(AS_Node& lhs, const AS_Node& rhs);

private:
	enSerializeMode GetSMode() const;
	void SetSMode(enSerializeMode mode);

private:
	static void RegForLua(lua_State* l);
	bool _Erase_(const char* path);
	AS_NodePtr _Find_(const char* path) const;
	AS_NodePtr _FindChild_(const char* path) const;
	AS_NodePtr _FindPosterity_(const char* path) const;
	AS_NodePtr _FindPosteritySmart_(const char* path) const;
	void _SetPath_(const char* path);
	void _SetPath_1(const char* path, const AS_NodePtr& parent);
	std::string _GetPath_() const;
	std::string _GetAbsolutePath_(bool same_cate, bool only_file_name, bool skip_when_parent_is_ref) const;
	AS_NodePtr _Clone_() const;
	bool _Equal_(const AS_NodePtr& rhs) const;
	AS_NodeIteratorPtr _Iterator_() const;
	AS_NodeIteratorPtr _Iterator_1(bool is_ref_to) const;
	bool _Refresh_(bool is_discory);
	AS_NodePtr _D2M_();
	bool _SaveToDisk_(const char* file_new_path) const;

private:
	AS_NodeWeakPtr				parent_;
	boost::filesystem::path		path_;
	enAS_NodeLvl				node_lvl_;

protected:
	mutable MutexT				mutex_;
	enSerializeMode				smode_;
	bool						d2m_;
};
