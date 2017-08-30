#include "stdafx.h"
#include "AS_File.h"
#include "AS_Sys.h"
#include <fstream>
#include <Poco/StreamCopier.h>
#include <boost/filesystem/operations.hpp>


AS_FACTORY_REG(AS_DiskFile)
AS_FACTORY_REG(AS_DiskFileE)
AS_FACTORY_REG(AS_MemoryFile)

const AS_Node::Encryptor::KeyType kAS_MemoryXorData = 0x489ead04;
const AS_Node::Encryptor::KeyType kAS_FileXorData = 0x549a0410;

bool AS_FileBase::Add(const AS_NodePtr& sub_node, bool replace_existed)
{
	assert(false);
	return false;
}

bool AS_FileBase::Erase(const AS_NodePtr& sub_node)
{
	assert(false);
	return false;
}

AS_NodePtr AS_FileBase::Find(const boost::filesystem::path& path, const AS_IFilter* filter, const AS_NodeDiscovery* discovery) const
{
	return nullptr;
}

AS_NodePtr AS_FileBase::FindChild(const boost::filesystem::path& path, const AS_IFilter* filter, const AS_NodeDiscovery* discovery) const
{
	return nullptr;
}

AS_Node::enAS_NodeType AS_FileBase::GetType() const
{
	return kFile;
}

AS_NodePtr AS_FileBase::FindPosterity(const boost::filesystem::path& path, const AS_IFilter* filter, const AS_NodeDiscovery* discovery) const
{
	return nullptr;
}

AS_FileBase::AS_FileBase(const AS_FileBase& rhs) : AS_Node(rhs)
{

}

AS_FileBase::AS_FileBase(enAS_NodeLvl node_lvl) : AS_Node(node_lvl)
{

}

int AS_FileBase::GetChildCount() const
{
	return -1;
}

void AS_FileBase::clear()
{

}

AS_DiskFile::AS_DiskFile() : AS_FileBase(kNL_FileDisk)
{

}

AS_DiskFile::AS_DiskFile(const AS_DiskFile& rhs) : AS_FileBase(rhs)
{

}

AS_Node* AS_DiskFile::Clone() const
{
	return new AS_DiskFile(*this);
}

AS_DiskFile::enAS_NodeCate AS_DiskFile::GetCate() const
{
	return kDisk;
}

std::string AS_DiskFile::GetString() const
{
	LockGuard l(mutex_);
	const auto& abs_file = GetAbsolutePath(true, false).string();
	std::ifstream fstm(abs_file, std::ios_base::in | std::ios_base::binary);
	if (!fstm)
	{
		//assert(false);
		return std::string();
	}
	std::string res;
	DoWrite(fstm, res);
	return res;
}

bool AS_DiskFile::Serialize(std::istream& istm)
{
	LockGuard l(mutex_);
	if (smode_ == kSM_Sys)
		return __super::Serialize(istm);
	const auto& abs_path = GetAbsolutePath(true, false);
	if (!MakeDirs(abs_path))
	{
		assert(false);
		return false;
	}
	std::ofstream fstm(abs_path.string(), std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
	if (!fstm)
	{
		assert(false);
		return false;
	}
	return DoRead(istm, fstm);
}

bool AS_DiskFile::Serialize(std::ostream& ostm) const
{
	LockGuard l(mutex_);
	if (smode_ == kSM_Sys)
		return __super::Serialize(ostm);
	const auto& abs_file = GetAbsolutePath(true, false).string();
	std::ifstream fstm(abs_file, std::ios_base::in | std::ios_base::binary);
	if (!fstm)
	{
		assert(false);
		return false;
	}
	return DoWrite(fstm, ostm);
}

bool AS_MemoryFile::Serialize(std::istream& istm)
{
	LockGuard l(mutex_);
	if (smode_ == kSM_Sys)
	{
		if (!__super::Serialize(istm))
		{
			assert(false);
			return false;
		}
		std::string::size_type size = 0;
		istm.read((char*)&size, sizeof(size));
		buffer_.resize(size);
		istm.read((char*)buffer_.c_str(), size);
		if (istm.gcount() == size)
			return true;
		else
		{
			assert(false);
			return false;
		}
	}
	buffer_.clear();
	Poco::StreamCopier::copyToString(istm, buffer_, 1024);
	Encryptor encryt(kAS_MemoryXorData);
	encryt.Encrypt(buffer_.c_str(), buffer_.size(), (char*)buffer_.c_str());
	return true;
}

const char* AS_DiskFile::ClsName() const
{
	return BOOST_PP_STRINGIZE(AS_DiskFile);
}

void AS_DiskFile::SetPath(const boost::filesystem::path& path, const AS_NodePtr& parent)
{
	return SetPathByParent(path, parent);
}

AS_NodePtr AS_DiskFile::D2M(const AS_NodeDiscovery* discovery)
{
	LockGuard l(mutex_);
	if (!d2m_)
		return __super::D2M(discovery);
#ifdef _DEBUG
	auto parent = Parent(true);
	if (parent)
	{
		assert(parent->IsD2M());
	}
#endif
	AS_NodePtr cpy(new AS_MemoryFile);
	if (!cpy)
	{
		assert(false);
		return nullptr;
	}
	CopyFrom(*cpy, *this);
	cpy->SetPath(GetPath().filename());
	std::stringstream sstm(std::ios_base::in | std::ios_base::out | std::ios_base::binary);
	Serialize((std::ostream&)sstm);
	cpy->Serialize((std::istream&)sstm);
	return cpy;
}

bool AS_DiskFile::DoRead(std::istream& istm, std::ostream& ostm)
{
	Poco::StreamCopier::copyStream(istm, ostm, 1024);
	return true;
}

bool AS_DiskFile::DoWrite(std::istream& istm, std::ostream& ostm) const
{
	Poco::StreamCopier::copyStream(istm, ostm, 1024);
	return true;
}

void AS_DiskFile::DoWrite(std::istream& istm, std::string& out_str) const
{
	Poco::StreamCopier::copyToString(istm, out_str, 1024);
}

int AS_DiskFile::DeleteDisks() const
{
	auto abs_path = GetAbsolutePath(true, false);
	if (!exists(abs_path))
		return 0;
	boost::system::error_code err;
	return (int)boost::filesystem::remove(abs_path, err);
}

AS_MemoryFile::AS_MemoryFile() : AS_FileBase(kNL_FileMem)
{

}

AS_MemoryFile::AS_MemoryFile(const AS_MemoryFile& rhs) : AS_FileBase(rhs), buffer_(rhs.buffer_)
{

}

AS_Node* AS_MemoryFile::Clone() const
{
	return new AS_MemoryFile(*this);
}

AS_MemoryFile::enAS_NodeCate AS_MemoryFile::GetCate() const
{
	return kMemory;
}

std::string AS_MemoryFile::GetString() const
{
	LockGuard l(mutex_);
	std::ostringstream ostm(std::ios_base::out | std::ios_base::binary);
	Serialize(ostm);
	return ostm.str();
}

const char* AS_MemoryFile::ClsName() const
{
	return BOOST_PP_STRINGIZE(AS_MemoryFile);
}

bool AS_MemoryFile::Serialize(std::ostream& ostm) const
{
	LockGuard l(mutex_);
	std::string::size_type size = buffer_.size();
	if (smode_ == kSM_Sys)
	{
		if (!__super::Serialize(ostm))
		{
			assert(false);
			return false;
		}
		ostm.write((char*)&size, sizeof(size));
	}
	if (smode_ == kSM_Sys)
	{
		ostm.write(buffer_.c_str(), size);
	}
	else
	{
		Encryptor encrypt(kAS_MemoryXorData);
		encrypt.Decrypt(buffer_.c_str(), size, ostm);
	}
	return true;
}

AS_NodePtr AS_MemoryFile::D2M(const AS_NodeDiscovery* discovery)
{
	return shared_from_this();
}

AS_DiskFileE::AS_DiskFileE()
{

}

AS_DiskFileE::AS_DiskFileE(const AS_DiskFileE& rhs) : AS_DiskFile(rhs)
{

}

AS_Node* AS_DiskFileE::Clone() const
{
	return new AS_DiskFileE(*this);
}

const char* AS_DiskFileE::ClsName() const
{
	return BOOST_PP_STRINGIZE(AS_DiskFileE);
}

bool AS_DiskFileE::DoRead(std::istream& istm, std::ostream& ostm)
{
	std::string buffer;
	Poco::StreamCopier::copyToString(istm, buffer, 1024);
	Encryptor ecrypt(kAS_FileXorData);
	ecrypt.Encrypt(buffer.c_str(), buffer.size(), (char*)buffer.c_str());
	ostm.write(buffer.c_str(), buffer.size());
	return true;
}

bool AS_DiskFileE::DoWrite(std::istream& istm, std::ostream& ostm) const
{
	std::string buffer;
	Poco::StreamCopier::copyToString(istm, buffer, 1024);
	Encryptor ecrypt(kAS_FileXorData);
	ecrypt.Decrypt(buffer.c_str(), buffer.size(), ostm);
	return true;
}

void AS_DiskFileE::DoWrite(std::istream& istm, std::string& out_str) const
{
	Poco::StreamCopier::copyToString(istm, out_str, 1024);
	Encryptor ecrypt(kAS_FileXorData);
	ecrypt.Decrypt(out_str.c_str(), out_str.size(), (char*)out_str.c_str());
}
