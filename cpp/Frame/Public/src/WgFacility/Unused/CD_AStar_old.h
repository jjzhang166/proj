#pragma once
/*
@author zhp
@date 2017/2/6 19:40
@purpose for a star algorithm
*/
#include "CD_Structs.h"
#include <vector>
#include <boost/pool/pool.hpp>

class CD_AStar{
public:
	typedef std::vector<stCD_VecInt> PosContT;
	struct stPosNode{
		int				g_;
		int				h_;
		stCD_VecInt		pos_;
		stPosNode*		parent_;

		inline stPosNode(const stCD_VecInt& pos) : g_(0), h_(0), parent_(nullptr), pos_(pos){}
		void FillInfo(stPosNode* parent, const stCD_VecInt& pos_dst);

		void* operator new(std::size_t size);
		void operator delete(void* p) throw();

	private:
		static boost::pool<>	pos_node_pool_;
	};
	struct stPosNodeCont{
		stPosNode* Add(const stCD_VecInt& pos);
		void Add(stPosNode* node);
		stPosNode* PushHeap(const stCD_VecInt& pos);
		stPosNode* PushHeap(const stCD_VecInt& pos, stPosNode* parent, const stCD_VecInt& pos_dst);
		stPosNode* PopHeap();
		void MakeHeap();
		void Clear();
		stPosNode* FindNode(const stCD_VecInt& pos);

		std::vector<stPosNode*>			nodes_;
	};

public:
	CD_AStar();
	bool GenFindPath(const stCD_VecInt& pos_src, const stCD_VecInt& pos_dst, const stCD_MapData& map_data, PosContT& out_pos_info);

private:
	const stPosNode* GenFindPathImpl(const stCD_VecInt& pos_src, const stCD_VecInt& pos_dst, const stCD_MapData& map_data);
	bool RectifyPlayerStep(const stPosNode* pos_node, PosContT& out_pos_info);
	bool CalcNextGridStep(const stPosNode* cur_node, int delta_x, int delta_y, const stCD_MapData& map_data, stCD_VecInt& out_pos);
	bool CalcReplaceOpenList();

private:
	stPosNodeCont			open_;
	stPosNodeCont			close_;
};
