#include "stdafx.h"
#include "CD_AStar.h"
#include <Common/UsefulFunc.h>

const CD_AStar::stPosNode* CD_AStar::GenFindPathImpl(const stCD_VecInt& pos_src, const stCD_VecInt& pos_dst, const stCD_MapData& map_data)
{
	open_.Clear();
	close_.Clear();
	open_.PushHeap(pos_src);

	stCD_VecInt pos_tmp;
	stPosNode node_tmp(pos_src);
	struct stHelp{
		char	x;
		char	y;
	};
	static const stHelp dir_array[] = {
		{ 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }
	};

	while (!open_.nodes_.empty())
	{
		auto cur_node = open_.PopHeap();
		close_.Add(cur_node);
		for (auto& dir_info : dir_array)
		{
			if (CalcNextGridStep(cur_node, dir_info.x, dir_info.y, map_data, pos_tmp))
			{
				auto tmp = pos_dst.x - pos_tmp.x;
				if (tmp < 0)
					tmp = -tmp;
				if (tmp < CD_kAStar_GridStep)
				{
					tmp = pos_dst.y - pos_tmp.y;
					if (tmp < 0)
						tmp = -tmp;
					if (tmp < CD_kAStar_GridStep)
					{
						auto res = open_.Add(pos_tmp);
						res->parent_ = cur_node;
						return res;
					}
				}

				auto open_node = open_.FindNode(pos_tmp);
				if (!open_node)
				{
					open_.PushHeap(pos_tmp, cur_node, pos_dst);
					continue;
				}
				node_tmp.pos_.x = pos_tmp.x;
				node_tmp.pos_.y = pos_tmp.y;
				node_tmp.FillInfo(cur_node, pos_dst);
				if (open_node->g_ + open_node->h_ < node_tmp.g_ + node_tmp.h_)
					continue;
				open_node->g_ = node_tmp.g_;
				open_node->h_ = node_tmp.h_;
				open_node->parent_ = node_tmp.parent_;
				open_.MakeHeap();
			}
		}
	}
	assert(false);
	return nullptr;
}

CD_AStar::CD_AStar()
{
}

bool CD_AStar::CalcNextGridStep(const stPosNode* cur_node, int delta_x, int delta_y, const stCD_MapData& map_data, stCD_VecInt& out_pos)
{
	auto cur_x = cur_node->pos_.x;
	auto cur_y = cur_node->pos_.y;
	auto end_x = delta_x * CD_kAStar_GridStep + cur_x;
	auto end_y = delta_y * CD_kAStar_GridStep + cur_y;
	out_pos.y = cur_y;
	for (; cur_x != end_x; cur_x += delta_x)
	{
		out_pos.x = cur_x + delta_x;
		if (map_data.GetDstMapData(out_pos) <= enCD_MapDataFlag::kMountain)
			break;
	}
	out_pos.x = cur_x;
	for (; cur_y != end_y; cur_y += delta_y)
	{
		out_pos.y = cur_y + delta_y;
		if (map_data.GetDstMapData(out_pos) <= enCD_MapDataFlag::kMountain)
			break;
	}
	if (cur_x == cur_node->pos_.x && cur_y == cur_node->pos_.y)
		return false;
	out_pos.x = cur_x;
	out_pos.y = cur_y;
	auto p = close_.FindNode(out_pos);
	if (p && p != cur_node)
		return false;
	return true;
}

bool CD_AStar::GenFindPath(const stCD_VecInt& pos_src, const stCD_VecInt& pos_dst, const stCD_MapData& map_data, PosContT& out_pos_info)
{
	auto tmp = pos_dst.x - pos_src.x;
	if (tmp < 0)
		tmp = -tmp;
	if (tmp < CD_kAStar_GridStep)
	{
		tmp = pos_dst.y - pos_src.y;
		if (tmp < 0)
			tmp = -tmp;
		if (tmp < CD_kAStar_GridStep)
			goto LABEL_RET;
	}

	auto node = GenFindPathImpl(pos_src, pos_dst, map_data);
	if (!node)
		return false;
	if (!RectifyPlayerStep(node, out_pos_info))
	{
		assert(false);
		return false;
	}
LABEL_RET:
	if (out_pos_info.empty())
	{
		out_pos_info.push_back(pos_dst);
		return true;
	}
	if (out_pos_info.back() != pos_dst)
		out_pos_info.push_back(pos_dst);
	return true;
}

bool CD_AStar::RectifyPlayerStep(const stPosNode* pos_node, PosContT& out_pos_info)
{
	out_pos_info.clear();

	while (pos_node && pos_node->parent_){
		out_pos_info.push_back(pos_node->pos_);
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

CD_AStar::stPosNode* CD_AStar::stPosNodeCont::Add(const stCD_VecInt& pos)
{
	auto node = new stPosNode(pos);
	nodes_.push_back(node);
	return node;
}

void CD_AStar::stPosNodeCont::Add(stPosNode* node)
{
	nodes_.push_back(node);
}

void CD_AStar::stPosNodeCont::Clear()
{
	for (auto v : nodes_)
	{
		delete v;
	}
	nodes_.clear();
}

CD_AStar::stPosNode* CD_AStar::stPosNodeCont::FindNode(const stCD_VecInt& pos)
{
	for (auto v : nodes_)
	{
		auto tmp = v->pos_.x - pos.x;
		if (tmp < 0)
			tmp = -tmp;
		if (tmp >= CD_kAStar_GridStep)
			continue;
		tmp = v->pos_.y - pos.y;
		if (tmp < 0)
			tmp = -tmp;
		if (tmp >= CD_kAStar_GridStep)
			continue;
		return v;
	}
	return nullptr;
}

CD_AStar::stPosNode* CD_AStar::stPosNodeCont::PushHeap(const stCD_VecInt& pos)
{
	auto node = Add(pos);
	std::push_heap(nodes_.begin(), nodes_.end(), [](const stPosNode* lhs, const stPosNode* rhs){
		return lhs->g_ + lhs->h_ > rhs->g_ + rhs->h_;
	});
	return node;
}

CD_AStar::stPosNode* CD_AStar::stPosNodeCont::PushHeap(const stCD_VecInt& pos, stPosNode* parent, const stCD_VecInt& pos_dst)
{
	auto node = Add(pos);
	node->FillInfo(parent, pos_dst);
	std::push_heap(nodes_.begin(), nodes_.end(), [](const stPosNode* lhs, const stPosNode* rhs){
		return lhs->g_ + lhs->h_ > rhs->g_ + rhs->h_;
	});
	return node;
}

CD_AStar::stPosNode* CD_AStar::stPosNodeCont::PopHeap()
{
	std::pop_heap(nodes_.begin(), nodes_.end(), [](const stPosNode* lhs, const stPosNode* rhs){
		return lhs->g_ + lhs->h_ > rhs->g_ + rhs->h_;
	});
	auto res = nodes_.back();
	nodes_.pop_back();
	return res;
}

void CD_AStar::stPosNodeCont::MakeHeap()
{
	std::make_heap(nodes_.begin(), nodes_.end(), [](const stPosNode* lhs, const stPosNode* rhs){
		return lhs->g_ + lhs->h_ > rhs->g_ + rhs->h_;
	});
}

void CD_AStar::stPosNode::FillInfo(stPosNode* parent, const stCD_VecInt& pos_dst)
{
	parent_ = parent;
	g_ = parent->g_;
	auto tmp = parent->pos_.x - pos_.x;
	if (tmp < 0)
		g_ -= tmp;
	else
		g_ += tmp;
	tmp = parent->pos_.y - pos_.y;
	if (tmp < 0)
		g_ -= tmp;
	else
		g_ += tmp;
	assert(g_ > parent->g_);

	tmp = pos_dst.x - pos_.x;
	auto tmp_y = pos_dst.y - pos_.y;
	tmp *= tmp;
	tmp += tmp_y * tmp_y;
	h_ = (int)sqrt((double)tmp);
}

void* CD_AStar::stPosNode::operator new(std::size_t size)
{
	return pos_node_pool_.malloc();
}

void CD_AStar::stPosNode::operator delete(void* p) throw()
{
	return pos_node_pool_.free(p);
}

boost::pool<> CD_AStar::stPosNode::pos_node_pool_(sizeof(stPosNode), 1024);
