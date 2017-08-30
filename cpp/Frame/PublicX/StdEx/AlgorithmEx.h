#pragma once
/*
@author zhp
@date 2017/5/15 8:21
@purpose for algorithm
*/
#include <CommonX/CmnX_StaticConts.h>

namespace StdEx{
	//±éÀú¶þ²æÊ÷
	template<typename NodeT, typename FnT>
	int TraverseBTree(NodeT* root, NodeT* empty_node, const FnT& fn){
		if (!root)
			return 0;
		CmnBufferVector<NodeT*, 0x2000> btree_stack;
		btree_stack.push_back(root);
		int cnt = 0;
		while (!btree_stack.empty())
		{
			auto cur_node = btree_stack.back();
			while (true)
			{
				if (!cur_node || cur_node == empty_node)
					break;
				++cnt;
				if (!fn(cur_node))
					return cnt;
				btree_stack.push_back(cur_node->left_);
				cur_node = cur_node->left_;
			}
			btree_stack.pop_back();
			if (!btree_stack.empty())
			{
				cur_node = btree_stack.back();
				btree_stack.pop_back();
				btree_stack.push_back(cur_node->right_);
			}
		}
		return cnt;
	}
}
