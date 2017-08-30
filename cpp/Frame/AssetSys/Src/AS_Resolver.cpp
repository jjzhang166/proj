#include "stdafx.h"
#include "AS_Resolver.h"
#include "AS_File.h"
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
#include <iterator>

ASR_File::ASR_File(const AS_NodePtr& file)
{
	assert(file);
	file_ = file;
}

void ASR_File::AddDependence(const std::string& file_path)
{
	if (file_path.empty())
	{
		assert(false);
		return;
	}
	dependence_.push_back(file_path);
}

const ASR_File::DependenceContT& ASR_File::GetDependences() const
{
	return dependence_;
}

AS_NodePtr ASR_File::GetFileNode() const
{
	return file_;
}

bool ASR_File::EqualFileName(const std::string& file_name) const
{
	if (file_name.empty())
		return false;
	assert(file_);
	const auto& file_path = file_->GetPath();
	return file_path == file_name/* || file_path.filename() == file_name*/;
}

void ASR_File::RemoveDependence(const std::string& file_path)
{
	for (auto it = dependence_.begin(); it != dependence_.end(); ++it)
	{
		if (*it == file_path)
		{
			dependence_.erase(it);
			break;
		}
	}
}

std::string ASR_File::GetFileName() const
{
	if (!file_)
	{
		assert(false);
		return std::string();
	}
	return file_->GetPath().string();
}

int ASR_File::DeleteDiskFile() const
{
	if (!file_)
	{
		assert(false);
		return 0;
	}
	return file_->DeleteDisks();
}

AS_Resolver::AS_Resolver(const AS_NodePtr& root)
{
	assert(root);
	root_ = root;
}

AS_NodePtr AS_Resolver::GetRoot() const
{
	return root_;
}

ASR_FilePtr AS_Resolver::FindFile(const std::string& file_name) const
{
	if (file_name.empty())
	{
		assert(false);
		return nullptr;
	}
	for (auto& v : files_)
	{
		if (v->EqualFileName(file_name))
			return v;
	}
	return nullptr;
}

ASR_File* AS_Resolver::CreateFileImpl(const AS_NodePtr& node)
{
	return new ASR_File(node);
}

void AS_Resolver::Resolve(const std::string& but_file)
{
	if (files_.empty())
	{
		assert(false);
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	//½â¾öÑ­»·ÒÀÀµ
	for (const auto& v : files_)
	{
		ASR_File::DependenceContT tmp_cont;
		for (auto& d : v->GetDependences())
		{
			auto& base = FindFile(d);
			if (!base)
			{
				tmp_cont.push_back(d);
				assert(false);
				continue;
			}
			if (!CanInherit(v, base))
			{
				tmp_cont.push_back(d);
				assert(false);
			}
		}
		if (!tmp_cont.empty())
		{
			for (auto& e : tmp_cont)
			{
				v->RemoveDependence(e);
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	using namespace boost;
	typedef adjacency_list<vecS, vecS, directedS, ASR_FilePtr> GraphT;
	typedef graph_traits<GraphT>::vertex_descriptor Vertex;
	std::map<ASR_FilePtr, Vertex> vertexes;
	GraphT g;
	for (const auto& v : files_)
	{
		vertexes[v] = add_vertex(v, g);
	}
	auto vetex_end = vertexes.end();
	for (const auto& v : files_)
	{
		auto derived = vetex_end;
		for (auto& d : v->GetDependences())
		{
			const auto& base = FindFile(d);
			if (!base)
			{
				assert(false);
				continue;
			}
			auto it = vertexes.find(base);
			if (it == vertexes.end())
			{
				assert(false);
				continue;
			}
			if (derived == vetex_end)
			{
				derived = vertexes.find(v);
				assert(derived != vetex_end);
			}
			add_edge(derived->second, it->second, g);
		}
	}
	typedef std::vector<Vertex> MakeOrder;
	MakeOrder make_order;
	boost::topological_sort(g, std::back_inserter(make_order));
	files_.clear();
	for (auto& v : make_order)
	{
		files_.push_back(g[v]);
	}
}

const AS_Resolver::ContT& AS_Resolver::GetFiles() const
{
	return files_;
}

ASR_FilePtr AS_Resolver::AddFile(const std::string& file_name)
{
	if (file_name.empty())
	{
		assert(false);
		return nullptr;
	}
	if (FindFile(file_name))
	{
		assert(false);
		return nullptr;
	}
	if (!root_)
	{
		assert(false);
		return nullptr;
	}
	auto node = root_->Find(file_name, nullptr, this);
	if (!node)
	{
		node = root_->FindPosterity(file_name, nullptr, this);
		if (!node)
		{
			node.reset(DiscoverFile());
			if (!node)
			{
				assert(false);
				return nullptr;
			}
			node->SetPath(file_name);
			if (!root_->Add(node))
			{
				assert(false);
				return nullptr;
			}
		}
	}
	ASR_FilePtr new_file(CreateFileImpl(node));
	files_.push_back(new_file);
	return new_file;
}

bool AS_Resolver::ExistDependence(const ASR_FilePtr& file, const ASR_FilePtr& file_dependence) const
{
	if (!file)
	{
		assert(false);
		return false;
	}
	if (!file_dependence)
	{
		assert(false);
		return false;
	}
	if (file == file_dependence)
		return true;
	return ExistDependenceImpl(file, file_dependence);
}

ASR_FileIteratorPtr AS_Resolver::IteratorDependence(const ASR_FilePtr& file, bool ancestors)
{
	if (!file)
	{
		assert(false);
		return nullptr;
	}
	if (ancestors)
	{
		ContT tmp;
		DependenceToVec(file, tmp);
		return MakeIterator<ASR_FileIteratorTraits>(tmp);
	}
	else
	{
		typedef const ASR_File::DependenceContT::value_type& ItemT;
		const auto& impl = MakeIterator<IteratorTraits<ItemT, IterContTag_ByRef, IterFilterTag_Custom> >(
			(ASR_File::DependenceContT&)file->GetDependences(), [this](ItemT item){ return !!this->FindFile(item); });
		return MakeWrapperIterator<ASR_FileIteratorTraits>(impl, [this](ItemT item){
			return this->FindFile(item);
		});
	}
}

void AS_Resolver::DependenceToVec(const ASR_FilePtr& file, ContT& cont)
{
	auto& dependece = file->GetDependences();
	for (auto& v : dependece)
	{
		auto& ptr = FindFile(v);
		if (ptr)
		{
			cont.push_back(ptr);
		}
	}
	for (auto& v : dependece)
	{
		auto& ptr = FindFile(v);
		if (ptr)
		{
			DependenceToVec(ptr, cont);
		}
	}
}

ASR_FileIteratorPtr AS_Resolver::IteratorInheritable(const ASR_FilePtr& file)
{
	if (!file)
	{
		assert(false);
		return nullptr;
	}
	return MakeIterator<ASR_FileIteratorTraits>(files_, [this, file](const ASR_FilePtr& ptr){
		return this->CanInherit(file, ptr);
	});
}

bool AS_Resolver::ExistDependenceImpl(const ASR_FilePtr& file, const ASR_FilePtr& file_dependence) const
{
	for (auto& v : file->GetDependences())
	{
		auto& f = FindFile(v);
		if (f)
		{
			if (f == file_dependence || ExistDependenceImpl(f, file_dependence))
				return true;
		}
	}
	return false;
}

bool AS_Resolver::CanInherit(const ASR_FilePtr& file, const ASR_FilePtr& file_dependence) const
{
	if (!file)
	{
		assert(false);
		return false;
	}
	if (!file_dependence)
	{
		assert(false);
		return false;
	}
	if (file == file_dependence)
		return false;
	return CanInheritImpl(file, file_dependence);
}

bool AS_Resolver::CanInheritImpl(const ASR_FilePtr& file, const ASR_FilePtr& file_dependence) const
{
	for (auto& v : file_dependence->GetDependences())
	{
		auto& f = FindFile(v);
		if (f)
		{
			if (f == file || !CanInheritImpl(file, f))
				return false;
		}
	}
	return true;
}

int AS_Resolver::DeleteAllDiskFile(bool delete_root_dir) const
{
	int n = 0;
	for (auto& v : files_)
	{
		n += v->DeleteDiskFile();
	}
	if (delete_root_dir)
	{
		if (!root_)
		{
			assert(false);
			return 0;
		}
		n += root_->DeleteDisks();
	}
	return n;
}

void AS_Resolver::RemoveFile(const ASR_FilePtr& file)
{
	if (!file)
	{
		assert(false);
		return;
	}
	auto it = std::find(files_.begin(), files_.end(), file);
	if (it == files_.end())
	{
		assert(false);
		return;
	}
	const auto& file_name = file->GetFileName();
	files_.erase(it);
	for (auto& v : files_)
	{
		v->RemoveDependence(file_name);
	}
}

void AS_Resolver::Clear()
{
	files_.clear();
}

ASR_FileIteratorPtr AS_Resolver::IteratorFiles()
{
	if (files_.empty())
		return nullptr;
	return MakeIterator<ASR_FileIteratorTraits>(files_);
}
