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
		int				f_;
		int				g_;
		int				x_;
		int				y_;
		stPosNode*		parent_;

		static boost::pool<>	pos_node_pool_;
	};
	struct stPosNodeCont{
		stPosNode* Add(const stCD_VecInt& pos);
		stPosNode* PushHeap(const stCD_VecInt& pos);
		void Clear();

		std::vector<stPosNode*>			nodes_;
	};

public:
	CD_AStar();
	//ÉäÏßÌ½Â·
	bool GenRaySteps(const stCD_VecInt& pos_src, const stCD_VecInt& pos_dst, const stCD_MapData& map_data, PosContT& out_pos_info);
	//AÐÇÑ°Â·
	bool GenFindPath(const stCD_VecInt& pos_src, const stCD_VecInt& pos_dst, const stCD_MapData& map_data, PosContT& out_pos_info);

private:
	const stPosNode* GenFindPathImpl(const stCD_VecInt& pos_src, const stCD_VecInt& pos_dst, const stCD_MapData& map_data);
	bool RectifyPlayerStep(const stPosNode* pos_node, PosContT& out_pos_info);
	bool CompressSteps(PosContT& out_pos_info, const stCD_MapData& map_data);
	void CompressTmpPosCont(const PosContT& cont_tmp, PosContT& cont_res, const stCD_VecInt& dst_pos,
		const stCD_MapData& map_data, stCD_MapData::Fn_GetDstMapData get_dst_map_data);
	bool CanLineMoveTo(const stCD_VecInt& pos_src, const stCD_VecInt& pos_dst, stCD_VecInt* last_valid_post, 
		const stCD_MapData& map_data,
		stCD_MapData::Fn_GetDstMapData get_dst_map_data);

private:
	stPosNodeCont			open_;
	stPosNodeCont			close_;
};
