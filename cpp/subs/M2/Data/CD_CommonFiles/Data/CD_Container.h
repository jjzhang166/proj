#pragma once
/*
@author zhp
@date 2017/5/15 7:17
@purpose for game container
*/
#include <StdEx/TString.h>
#include <ProtocolX/P_DefinesX.h>
#include <StdEx/AlgorithmEx.h>
#include <CmnMix/Cmn_LogOut.h>

//////////////////////////////////////////////////////////////////////////
template<typename ValueType>
struct stCD_PoolChunk{
	pt_dword		ukn_;			//=0，标志？
	ValueType		values_[1];
};

template<typename ValueType>
struct stCD_Pool{
	typedef ValueType value_type;
	typedef stCD_PoolChunk<value_type> ChunkType;

	value_type* BufferBegin() const{
		if (!chunk_) return nullptr;
		if (IsUnValidPtr(chunk_))
		{
			assert(false);
			return nullptr;
		}
		return &chunk_->values_[0];
	}
	value_type* BufferEnd() const{
		if (!chunk_) return nullptr;
		if (IsUnValidPtr(end_))
		{
			assert(false);
			return nullptr;
		}
		return end_;
	}
	value_type* ChunkBegin() const{
		return BufferBegin();
	}
	value_type* ChunkEnd() const{
		if (!chunk_) return nullptr;
		if (IsUnValidPtr(chunk_))
		{
			assert(false);
			return nullptr;
		}
		return &chunk_->values_[max_cnt_];
	}

	value_type*		end_;
	ChunkType*		chunk_;
	pt_dword		max_cnt_;
};

template<typename ValueType>
struct stCD_XTreeNode{
	ValueType			value_;
	pt_dword			ukn_flag_;
	stCD_XTreeNode*		left_;
	stCD_XTreeNode*		right_;
	stCD_XTreeNode*		parent_;
};

template<typename ValueType>
struct stCD_XTree{
	typedef ValueType value_type;
	typedef stCD_XTreeNode<value_type> NodeType;
	typedef stCD_Pool<value_type> PoolType;

	template<typename FnT>
	int Traverse(const FnT& fn) const{
		if (node_cnt_ == 0) return 0;
		if (IsUnValidPtr(root_) || IsUnValidPtr(empty_node_))
		{
			assert(false);
			return 0;
		}
		return StdEx::TraverseBTree(root_, empty_node_, [&fn](NodeType* cur_node){
			assert(cur_node);
			return fn(cur_node->value_);
		});
	}
	void DebugInfo() const{
		auto cnt = Traverse([](const value_type& value){
			value.DebugInfo();
			return true;
		});
		LOG_O(Log_debug) << "游戏中的节点总数：" << node_cnt_ << ",遍历到的数量：" << cnt;
		assert(cnt == node_cnt_);
	}

	NodeType*		root_;
	pt_dword		node_cnt_;
	PoolType		pool_;
	NodeType*		empty_node_;
};
static_assert(0x18 == sizeof(stCD_XTree<int>), "");
//////////////////////////////////////////////////////////////////////////
template<typename ValueType>
struct stCD_Vector{
	typedef ValueType* iterator;

	iterator begin() const{
		if (buffer_cnt_ == 0) return nullptr;
		if (IsUnValidPtr(values_))
		{
			assert(false);
			return nullptr;
		}
		return values_;
	}
	iterator end() const{
		if (buffer_cnt_ == 0) return nullptr;
		if (IsUnValidPtr(values_))
		{
			assert(false);
			return nullptr;
		}
		return values_ + buffer_cnt_;
	}
	iterator begin(){
		if (buffer_cnt_ == 0) return nullptr;
		if (IsUnValidPtr(values_))
		{
			assert(false);
			return nullptr;
		}
		return values_;
	}
	iterator end(){
		if (buffer_cnt_ == 0) return nullptr;
		if (IsUnValidPtr(values_))
		{
			assert(false);
			return nullptr;
		}
		return values_ + buffer_cnt_;
	}

	iterator		values_;
	pt_dword		cnt_;
	pt_dword		buffer_cnt_;
};
//////////////////////////////////////////////////////////////////////////
template<typename ValueType>
struct stCD_List{

};
//////////////////////////////////////////////////////////////////////////