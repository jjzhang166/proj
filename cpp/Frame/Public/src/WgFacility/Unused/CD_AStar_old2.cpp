#include "stdafx.h"
#include "CD_AStar.h"
#include <Common/UsefulFunc.h>
#include <CommonX/CmnX_HeapAlgoMacro.h>
#include <CommonX/CmnX_Math.h>

#define HEAP_LT(lhs, rhs) lhs->f_ > rhs->f_

const CD_AStar::stPosNode* CD_AStar::GenFindPathImpl(const stCD_VecInt& pos_src, const stCD_VecInt& pos_dst, const stCD_MapData& map_data)
{
	open_.Clear();
	close_.Clear();
	open_.PushHeap(pos_src);

	stCD_VecInt pos_tmp;
	stPosNode node_tmp;

	struct stHelp{
		char	x;
		char	y;
	};
	static const stHelp dir_array[] = {
		{ 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }
	};
	auto get_dst_map_data = stCD_MapData::GetFn_GetDstMapData();
	while (open_.nodes_._Myfirst != open_.nodes_._Mylast)
	{
		HEAP_PopHeap(open_.nodes_._Myfirst, open_.nodes_._Mylast)
		auto cur_node = *(open_.nodes_._Mylast - 1);
		open_.nodes_.pop_back();
		close_.nodes_.push_back(cur_node);
		auto close_it = close_.nodes_._Myfirst;
		auto close_it_end = close_.nodes_._Mylast;

		for (auto& dir_info : dir_array)
		{
			//////////////////////////////////////////////////////////////////////////
			auto cur_x = cur_node->x_;
			auto cur_y = cur_node->y_;
			auto end_x = dir_info.x * CD_kAStar_GridStep + cur_x;
			auto end_y = dir_info.y * CD_kAStar_GridStep + cur_y;
			pos_tmp.y = cur_y;
			for (; cur_x != end_x; cur_x += dir_info.x)
			{
				pos_tmp.x = cur_x + dir_info.x;
				if ((map_data.*get_dst_map_data)(pos_tmp, 0) <= enCD_MapDataFlag::kMountain)
					break;
				for (auto it = close_it; it != close_it_end; ++it)
				{
					auto v = *it;
					if (v->x_ != pos_tmp.x)
						continue;
					if (v->y_ != pos_tmp.y)
						continue;
					goto LABLE_IN_CLOSE1;
				}
				continue;
			LABLE_IN_CLOSE1:
				break;
			}
			pos_tmp.x = cur_x;
			for (; cur_y != end_y; cur_y += dir_info.y)
			{
				pos_tmp.y = cur_y + dir_info.y;
				if ((map_data.*get_dst_map_data)(pos_tmp, 0) <= enCD_MapDataFlag::kMountain)
					break;
				for (auto it = close_it; it != close_it_end; ++it)
				{
					auto v = *it;
					if (v->x_ != pos_tmp.x)
						continue;
					if (v->y_ != pos_tmp.y)
						continue;
					goto LABLE_IN_CLOSE2;
				}
				continue;
			LABLE_IN_CLOSE2:
				break;
			}
			if (cur_x == cur_node->x_ && cur_y == cur_node->y_)
				continue;

			//////////////////////////////////////////////////////////////////////////
			//这里判断最终坐标的地方算法上虽然有问题，但是由于游戏中也有自动寻路的功能，所以没问题。
			/*auto tmp = pos_dst.x - cur_x;
			if (tmp < 0)
				tmp = -tmp;
			if (tmp < CD_kAStar_GridStep)
			{
				tmp = pos_dst.y - cur_y;
				if (tmp < 0)
					tmp = -tmp;
				if (tmp < CD_kAStar_GridStep)
				{
					pos_tmp.x = cur_x;
					pos_tmp.y = cur_y;
					auto res = open_.Add(pos_tmp);
					res->parent_ = cur_node;
					return res;
				}
			}*/
			if (pos_dst.x == cur_x && pos_dst.y == cur_y)
			{
				pos_tmp.x = cur_x;
				pos_tmp.y = cur_y;
				auto res = open_.Add(pos_tmp);
				res->parent_ = cur_node;
				return res;
			}

			stPosNode* open_node = nullptr;
			for (auto it = open_.nodes_._Myfirst, it_end = open_.nodes_._Mylast; it != it_end; ++it)
			{
				auto v = *it;
				if (v->x_ != cur_x)
					continue;
				if (v->y_ != cur_y)
					continue;
				open_node = v;
				break;
			}
			if (!open_node)
			{
				auto node = (stPosNode*)stPosNode::pos_node_pool_.malloc();
				node->x_ = cur_x;
				node->y_ = cur_y;
				node->parent_ = cur_node;
				node->g_ = cur_node->g_;
				auto tmp = cur_node->x_ - node->x_;
				if (tmp < 0)
					node->g_ -= tmp;
				else
					node->g_ += tmp;
				tmp = cur_node->y_ - node->y_;
				if (tmp < 0)
					node->g_ -= tmp;
				else
					node->g_ += tmp;
				assert(node->g_ > cur_node->g_);

				tmp = pos_dst.x - node->x_;
				auto tmp_y = pos_dst.y - node->y_;
				tmp *= tmp;
				tmp += tmp_y * tmp_y;
				node->f_ = node->g_ + (int)sqrt((double)tmp);
				open_.nodes_.push_back(node);
				HEAP_PushHeap(open_.nodes_._Myfirst, open_.nodes_._Mylast)
				continue;
			}
			node_tmp.x_ = cur_x;
			node_tmp.y_ = cur_y;
			node_tmp.parent_ = cur_node;
			node_tmp.g_ = cur_node->g_;
			auto tmp = cur_node->x_ - node_tmp.x_;
			if (tmp < 0)
				node_tmp.g_ -= tmp;
			else
				node_tmp.g_ += tmp;
			tmp = cur_node->y_ - node_tmp.y_;
			if (tmp < 0)
				node_tmp.g_ -= tmp;
			else
				node_tmp.g_ += tmp;

			tmp = pos_dst.x - node_tmp.x_;
			auto tmp_y = pos_dst.y - node_tmp.y_;
			tmp *= tmp;
			tmp += tmp_y * tmp_y;
			node_tmp.f_ = node_tmp.g_ + (int)sqrt((double)tmp);
			if (open_node->f_ < node_tmp.f_)
				continue;
			open_node->x_ = node_tmp.x_;
			open_node->y_ = node_tmp.y_;
			open_node->g_ = node_tmp.g_;
			open_node->f_ = node_tmp.f_;
			open_node->parent_ = node_tmp.parent_;
			HEAP_MakeHeap(open_.nodes_._Myfirst, open_.nodes_._Mylast)
		}
	}
	assert(false);
	return nullptr;
}

CD_AStar::CD_AStar()
{
}

bool CD_AStar::GenFindPath(const stCD_VecInt& pos_src, const stCD_VecInt& pos_dst, const stCD_MapData& map_data, PosContT& out_pos_info)
{
	/*auto tmp = pos_dst.x - pos_src.x;
	if (tmp < 0)
		tmp = -tmp;
	if (tmp < CD_kAStar_GridStep)
	{
		tmp = pos_dst.y - pos_src.y;
		if (tmp < 0)
			tmp = -tmp;
		if (tmp < CD_kAStar_GridStep)
			goto LABEL_RET;
	}*/

	auto node = GenFindPathImpl(pos_src, pos_dst, map_data);
	if (!node)
		return false;
	if (!RectifyPlayerStep(node, out_pos_info))
	{
		assert(false);
		return false;
	}
//LABEL_RET:
	if (out_pos_info.empty())
	{
		out_pos_info.push_back(pos_dst);
		return true;
	}
	if (out_pos_info.back() != pos_dst)
		out_pos_info.push_back(pos_dst);
	if (!CompressSteps(out_pos_info, map_data))
	{
		assert(false);
		return false;
	}
	return true;
}

bool CD_AStar::RectifyPlayerStep(const stPosNode* pos_node, PosContT& out_pos_info)
{
	out_pos_info.clear();

	while (pos_node && pos_node->parent_){
		out_pos_info.push_back(stCD_VecInt(pos_node->x_, pos_node->y_));
		pos_node = pos_node->parent_;
	} 
	if (out_pos_info.empty())
	{
		assert(false);
		return false;
	}
	std::reverse(out_pos_info.begin(), out_pos_info.end());
	if (CD_kAStar_PlayerStep <= 1)
		return true;
	PosContT pos_cont;
	bool last_angle_valid = false;
	float last_angle;
	auto it = out_pos_info.begin(), it_end = out_pos_info.end();
	pos_cont.push_back(*it);
	int cur_player_step = 0;
	const stCD_VecInt* last_pos = nullptr;
	for (; it != out_pos_info.end(); ++it)
	{
		auto& v = *it;
		if (last_angle_valid)
		{
			auto cur_angle = pos_cont.back().IncludedAngle(v);
			if (FloatEqual(last_angle, cur_angle))
			{
				last_angle = cur_angle;
				++cur_player_step;
				if (cur_player_step >= CD_kAStar_PlayerStep)
				{
					cur_player_step = 0;
					pos_cont.push_back(v);
				}
			}
			else
			{
				last_angle = cur_angle;
				if (cur_player_step > 0)
				{
					assert(last_pos);
					pos_cont.push_back(*last_pos);
				}
				cur_player_step = 0;
				pos_cont.push_back(v);
			}
		}
		else
		{
			last_angle = pos_cont.back().IncludedAngle(v);
			last_angle_valid = true;
			pos_cont.push_back(v);
		}
		last_pos = &v;
	}
	if (last_pos != &out_pos_info.back())
	{
		pos_cont.push_back(out_pos_info.back());
	}
	return true;
}

bool CD_AStar::CompressSteps(PosContT& out_pos_info, const stCD_MapData& map_data)
{
	assert(!out_pos_info.empty());
	auto get_dst_map_data = stCD_MapData::GetFn_GetDstMapData();
	PosContT cont_tmp, cont_res;
	auto last_pos = out_pos_info.begin();
	auto frist_pos = last_pos;
	cont_res.push_back(*last_pos);
	for (auto it = last_pos + 1, it_end = out_pos_info.end(); it < it_end; last_pos = it++)
	{
		auto& dst_pos = *it;
		if (last_pos->Distance(dst_pos) + frist_pos->Distance(dst_pos) + (float)cont_tmp.size() * 1.3 < CD_kAStar_CompressStepLen)
		{
			cont_tmp.push_back(dst_pos);
			continue;
		}
		CompressTmpPosCont(cont_tmp, cont_res, dst_pos, map_data, get_dst_map_data);
		cont_tmp.clear();
		cont_res.push_back(dst_pos);
		frist_pos = it;
	}
	if (!cont_tmp.empty())
	{
		auto dst_pos = cont_tmp.back();
		cont_tmp.pop_back();
		CompressTmpPosCont(cont_tmp, cont_res, dst_pos, map_data, get_dst_map_data);
		cont_res.push_back(dst_pos);
	}
	out_pos_info = cont_res;
	return true;
}

bool CD_AStar::CanLineMoveTo(const stCD_VecInt& pos_src, const stCD_VecInt& pos_dst, stCD_VecInt* last_valid_post,
	const stCD_MapData& map_data,
	stCD_MapData::Fn_GetDstMapData get_dst_map_data)
{
	TwoPointForm form(pos_src.x, pos_src.y, pos_dst.x, pos_dst.y);
	auto the_end = pos_dst.x - pos_src.x;
	int sign;
	if (the_end > 0)
		sign = 1;
	else if (the_end < 0)
		sign = -1;
	else
		sign = 0;
	stCD_VecInt last_pos = pos_src;
	stCD_VecInt pos_tmp;
	for (int x = pos_src.x + sign; x != pos_dst.x; x += sign)
	{
		pos_tmp.x = x;
		pos_tmp.y = (int)form.GetY(x);
		if ((map_data.*get_dst_map_data)(pos_tmp, 0) <= enCD_MapDataFlag::kMountain)
		{
			if (last_valid_post)
				*last_valid_post = last_pos;
			return false;
		}
		if (last_valid_post)
			last_pos = pos_tmp;
	}
	the_end = pos_dst.y - pos_src.y;
	if (the_end > 0)
		sign = 1;
	else if (the_end < 0)
		sign = -1;
	else
		sign = 0;
	for (int y = pos_src.y + sign; y != pos_dst.y; y += sign)
	{
		pos_tmp.x = (int)form.GetX(y);
		pos_tmp.y = y;
		if ((map_data.*get_dst_map_data)(pos_tmp, 0) <= enCD_MapDataFlag::kMountain)
		{
			if (last_valid_post)
				*last_valid_post = last_pos;
			return false;
		}
		if (last_valid_post)
			last_pos = pos_tmp;
	}
	return true;
}

void CD_AStar::CompressTmpPosCont(const PosContT& cont_tmp, PosContT& cont_res, const stCD_VecInt& dst_pos,
	const stCD_MapData& map_data, stCD_MapData::Fn_GetDstMapData get_dst_map_data)
{
	auto it_valid_tmp = cont_tmp.begin(), tmp_it_end = cont_tmp.end();
	if (it_valid_tmp == tmp_it_end)
		return;
	assert(!cont_res.empty());
	for (auto tmp_it = it_valid_tmp + 1; tmp_it < tmp_it_end; ++tmp_it)
	{
		auto& tmp_pos = *tmp_it;
		if (CanLineMoveTo(tmp_pos, dst_pos, nullptr, map_data, get_dst_map_data)
			&& CanLineMoveTo(cont_res.back(), tmp_pos, nullptr, map_data, get_dst_map_data))
		{
			it_valid_tmp = tmp_it;
			continue;
		}
		if (it_valid_tmp != tmp_it_end)
		{
			cont_res.push_back(*it_valid_tmp);
			it_valid_tmp = tmp_it_end;
		}
		cont_res.push_back(tmp_pos);
	}
	if (it_valid_tmp != tmp_it_end)
		cont_res.push_back(*it_valid_tmp);
}

bool CD_AStar::GenRaySteps(const stCD_VecInt& pos_src, const stCD_VecInt& pos_dst, const stCD_MapData& map_data, PosContT& out_pos_info)
{
	auto get_dst_map_data = stCD_MapData::GetFn_GetDstMapData();
	stCD_VecInt last_valid_pos;
	if (CanLineMoveTo(pos_src, pos_dst, &last_valid_pos, map_data, get_dst_map_data))
	{
		last_valid_pos = pos_dst;
	}
	else
	{
		if (last_valid_pos == pos_src)
			return false;
	}
	TwoPointForm form(pos_src.x, pos_src.y, pos_dst.x, pos_dst.y);
	if (pos_src.x <= pos_dst.x)
	{
		for (auto x = pos_src.x + CD_kAStar_RayStep; x < last_valid_pos.x; x += CD_kAStar_RayStep)
		{
			out_pos_info.push_back(stCD_VecInt(x, (int)form.GetY(x)));
		}
	}
	else
	{
		for (auto x = pos_src.x - CD_kAStar_RayStep; x > last_valid_pos.x; x -= CD_kAStar_RayStep)
		{
			out_pos_info.push_back(stCD_VecInt(x, (int)form.GetY(x)));
		}
	}	
	out_pos_info.push_back(last_valid_pos);
	return true;
}

CD_AStar::stPosNode* CD_AStar::stPosNodeCont::Add(const stCD_VecInt& pos)
{
	auto node = (stPosNode*)stPosNode::pos_node_pool_.malloc();
	node->x_ = pos.x;	node->y_ = pos.y; node->g_ = 0; node->f_ = 0; node->parent_ = nullptr;
	nodes_.push_back(node);
	return node;
}

void CD_AStar::stPosNodeCont::Clear()
{
	for (auto v : nodes_)
	{
		stPosNode::pos_node_pool_.free(v);
	}
	nodes_.clear();
}

CD_AStar::stPosNode* CD_AStar::stPosNodeCont::PushHeap(const stCD_VecInt& pos)
{
	auto node = Add(pos);
	HEAP_PushHeap(nodes_.begin(), nodes_.end())
	return node;
}

boost::pool<> CD_AStar::stPosNode::pos_node_pool_(sizeof(stPosNode), 1024);
#undef HEAP_LT
