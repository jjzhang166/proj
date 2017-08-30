#include "stdafx.h"
#include <Common/CmnRecordMap.h>
#include <cassert>
#include <vector>
#include <Common/UsefulFunc.h>
#include <Common/LogOut.h>


RECM_StageMgr::RECM_StageMgr()
{
	sink_ = nullptr;
	zoom_rate_ = 3;
}

bool RECM_StageMgr::Init(CDC& wnd_dc, const CRect& rect, RECM_StageSink* sink)
{
	if (!sink)
	{
		assert(false);
		return false;
	}
	if (!dc_.CreateCompatibleDC(&wnd_dc))
	{
		assert(false);
		return false;
	}
	if (!bit_map_.CreateCompatibleBitmap(&wnd_dc, rect.Width(), rect.Height()))
	{
		assert(false);
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	const auto kRgbNormal = RGB(255, 0, 0);
	const auto kRgbHover = RGB(0, 0, 95);
	const auto kRgbSelected = RGB(0, 233, 0);
	const auto kRgbRole = RGB(0, 0, 0);
	if (!pen_normal_.CreatePen(PS_SOLID, 1, kRgbNormal))
	{
		assert(false);
		return false;
	}
	if (!pen_hover_.CreatePen(PS_SOLID, 1, kRgbHover))
	{
		assert(false);
		return false;
	}
	if (!pen_selected_.CreatePen(PS_SOLID, 1, kRgbSelected))
	{
		assert(false);
		return false;
	}
	if (!pen_line_.CreatePen(PS_SOLID, 2, RGB(54, 109, 201)))
	{
		assert(false);
		return false;
	}
	if (!pen_role_.CreatePen(PS_SOLID, 4, kRgbRole))
	{
		assert(false);
		return false;
	}

	LOGBRUSH logbrush;
	logbrush.lbHatch = HS_CROSS;
	logbrush.lbStyle = BS_SOLID;
	logbrush.lbColor = kRgbNormal;
	if (!brush_normal_.CreateBrushIndirect(&logbrush))
	{
		assert(false);
		return false;
	}
	logbrush.lbColor = kRgbSelected;
	if (!brush_selected_.CreateBrushIndirect(&logbrush))
	{
		assert(false);
		return false;
	}
	logbrush.lbColor = kRgbHover;
	if (!brush_hover_.CreateBrushIndirect(&logbrush))
	{
		assert(false);
		return false;
	}
	logbrush.lbColor = kRgbRole;
	if (!brush_role_.CreateBrushIndirect(&logbrush))
	{
		assert(false);
		return false;
	}
	//////////////////////////////////////////////////////////////////////////

	rect_ = rect;
	dc_.SelectObject(bit_map_);
	sink_ = sink;
	return true;
}

bool RECM_StageMgr::AddPos(const stRC_Vector3& pos)
{
	if (Pos2Id(pos) >= 0)
	{
		LOG_O(Log_error) << "已存在这样的坐标了：" << pos;
		return false;
	}
	auto new_id = GenRandomId();
	RECM_PosBase tmp;
	tmp.id_ = new_id;
	tmp.pos_ = pos;
	tmp.old_state_ = enRECM_PosState::kNormal;
	tmp.state_ = enRECM_PosState::kNormal;
	pos_info_.push_back(tmp);
	return true;
}

bool RECM_StageMgr::AddPos(int pos_id, const stRC_Vector3& pos)
{
	if (FindByPosId(pos_id))
	{
		LOG_O(Log_error) << "已存在这样的坐标id了：" << pos_id << " pos:" << pos;
		return false;
	}
	if (Pos2Id(pos) >= 0)
	{
		LOG_O(Log_error) << "已存在这样的坐标了，坐标ID：" << pos_id << " pos:" << pos;
		return false;
	}
	RECM_PosBase tmp;
	tmp.id_ = pos_id;
	tmp.pos_ = pos;
	tmp.old_state_ = enRECM_PosState::kNormal;
	tmp.state_ = enRECM_PosState::kNormal;
	pos_info_.push_back(tmp);
	return true;
}

int RECM_StageMgr::Pos2Id(const stRC_Vector3& pos) const
{
	auto it = std::find_if(pos_info_.begin(), pos_info_.end(), [&pos](const RECM_PosBase& v){
		return v.pos_ == pos;
	});
	if (it == pos_info_.end())
		return -1;
	return it->id_;
}

int RECM_StageMgr::GenRandomId() const
{
	NameList<int, std::set<int> > ids;
	for (auto& v : pos_info_)
	{
		ids.Add(v.id_);
	}
	int id = 1;
	auto it_end = ids.GetValues().end();
	for (auto it = ids.GetValues().begin(); it != it_end; ++id)
	{
		if (id < *it)
			return id;
		do 
		{
			++it;
		} while (it != it_end && id >= *it);
	}
	++id;
	assert(!ids.IsIn(id));
	return id;
}

bool RECM_StageMgr::ErasePos(const stRC_Vector3& pos)
{
	auto it_end = pos_info_.end();
	for (auto it = pos_info_.begin(); it != it_end; ++it)
	{
		if (it->pos_ == pos)
		{
			EraseEdges(it->id_);
			pos_info_.erase(it);
			return true;
		}
	}
	return false;
}

bool RECM_StageMgr::ErasePos(int pos_id)
{
	auto it_end = pos_info_.end();
	for (auto it = pos_info_.begin(); it != it_end; ++it)
	{
		if (it->id_ == pos_id)
		{
			pos_info_.erase(it);
			EraseEdges(pos_id);
			return true;
		}
	}
	return false;
}

bool RECM_StageMgr::AddEdge(int id_lhs, int id_rhs)
{
	if (!HasThePosId(id_lhs))
	{
		LOG_O(Log_error) << "没有这样的ID：" << id_lhs;
		return false;
	}
	if (!HasThePosId(id_rhs))
	{
		LOG_O(Log_error) << "2没有这样的ID：" << id_rhs;
		return false;
	}
	if (id_lhs == id_rhs)
	{
		assert(false);
		return false;
	}
	auto old_cnt = edge_info_.size();
	edge_info_.Add(EdgeT(id_lhs, id_rhs));
	if (old_cnt == edge_info_.size())
	{
		LOG_O(Log_warning) << "已存在该边, id_lhs:" << id_lhs << " id_rhs:" << id_rhs;
		return false;
	}
	return true;
}

bool RECM_StageMgr::AddEdge(const stRC_Vector3& pos_lhs, const stRC_Vector3& pos_rhs)
{
	auto id_lhs = Pos2Id(pos_lhs);
	if (id_lhs < 0)
	{
		LOG_O(Log_error) << "2没有这样的坐标：" << pos_lhs;
		return false;
	}
	auto id_rhs = Pos2Id(pos_rhs);
	if (id_rhs < 0)
	{
		LOG_O(Log_error) << "2没有这样的坐标2：" << pos_rhs;
		return false;
	}
	if (id_lhs == id_rhs)
	{
		assert(false);
		return false;
	}
	auto old_cnt = edge_info_.size();
	edge_info_.Add(EdgeT(id_lhs, id_rhs));
	if (old_cnt == edge_info_.size())
	{
		LOG_O(Log_warning) << "已存在该边1, id_lhs:" << id_lhs << " id_rhs:" << id_rhs;
		return false;
	}
	return true;
}

bool RECM_StageMgr::HasThePosId(int pos_id) const
{
	auto it_end = pos_info_.end();
	for (auto it = pos_info_.begin(); it != it_end; ++it)
	{
		if (it->id_ == pos_id)
			return true;
	}
	return false;
}

bool RECM_StageMgr::EraseEdge(int id_lhs, int id_rhs)
{
	auto old_cnt = edge_info_.size();
	edge_info_.Delete(EdgeT(id_lhs, id_rhs));
	if (old_cnt == edge_info_.size())
	{
		LOG_O(Log_warning) << "不存在该边，无法删除，id_lhs:" << id_lhs << " id_rhs:" << id_rhs;
		return false;
	}
	return true;
}

bool RECM_StageMgr::EraseEdge(const stRC_Vector3& pos_lhs, const stRC_Vector3& pos_rhs)
{
	auto id_lhs = Pos2Id(pos_lhs);
	if (id_lhs < 0)
	{
		LOG_O(Log_error) << "无法删除，因为不存在这样的坐标：" << pos_lhs;
		return false;
	}
	auto id_rhs = Pos2Id(pos_rhs);
	if (id_rhs < 0)
	{
		LOG_O(Log_error) << "无法删除，因为不存在这样的坐标2：" << pos_rhs;
		return false;
	}
	return EraseEdge(id_lhs, id_rhs);
}

bool RECM_StageMgr::AddSelectPos(int pos_id)
{
	auto pos = FindByPosId(pos_id);
	if (!pos)
		return false;
	auto old_cnt = select_info_.size();
	select_info_.Add(pos_id);
	if (old_cnt == select_info_.size())
	{
		LOG_O(Log_warning) << "已经存在该选择坐标了，pos_id:" << pos_id;
		return false;
	}
	pos->state_ = enRECM_PosState::kSelected;	
	return true;
}

RECM_PosBase* RECM_StageMgr::FindByPosId(int pos_id)
{
	auto it_end = pos_info_.end();
	for (auto it = pos_info_.begin(); it != it_end; ++it)
	{
		if (it->id_ == pos_id)
			return &*it;
	}
	return nullptr;
}

int RECM_StageMgr::EraseEdges(int pos_id)
{
	auto tmp_edge = edge_info_.GetValues();
	auto old_size = tmp_edge.size();
	edge_info_.clear();
	for (auto& v : tmp_edge)
	{
		if (v.first != pos_id && v.second != pos_id)
			edge_info_.Add(v);
	}
	
	return old_size - edge_info_.size();
}

int RECM_StageMgr::ClearSelectPos()
{
	for (auto v : select_info_.GetValues())
	{
		auto pos = FindByPosId(v);
		if (!pos)
		{
			assert(false);
			continue;
		}
		pos->state_ = enRECM_PosState::kNormal;
	}
	int cnt = select_info_.size();
	select_info_.clear();
	return cnt;
}

int RECM_StageMgr::AddSelectPosEdges()
{
	if (select_info_.size() < 2)
	{
		LOG_O(Log_warning) << "选择坐标数量小于2";
		return 0;
	}
	int cnt = 0;
	int last = -1;
	for (auto v : select_info_.GetValues())
	{
		if (last >= 0)
		{
			cnt += AddEdge(last, v);
		}
		last = v;
	}
	ClearSelectPos();
	return cnt;
}

int RECM_StageMgr::EraseSelectPosEdges()
{
	if (select_info_.size() < 2)
	{
		LOG_O(Log_error) << "选择坐标数量小于2，无法删除";
		return 0;
	}
	int cnt = 0;
	int last = -1;
	for (auto v : select_info_.GetValues())
	{
		if (last >= 0)
		{
			cnt += EraseEdge(last, v);
		}
		last = v;
	}
	ClearSelectPos();
	return cnt;
}

int RECM_StageMgr::EraseSelectPosInfo()
{
	if (select_info_.size() < 1)
	{
		LOG_O(Log_warning) << "选择坐标数量不能小于1";
		return 0;
	}
	int cnt = 0;
	for (auto v : select_info_.GetValues())
	{
		cnt += ErasePos(v);
	}
	select_info_.clear();
	return cnt;
}

bool RECM_StageMgr::DoSaveData() const
{
	if (!sink_)
	{
		assert(false);
		return false;
	}
	for (auto& v : pos_info_)
	{
		if (!sink_->OnAddPos(v.id_, v.pos_))
			return false;
	}
	for (auto& v : edge_info_.GetValues())
	{
		if (!sink_->OnAddEdge(v.first, v.second))
			return false;
	}
	return true;
}

RECM_PosBase* RECM_StageMgr::PosInfoFromMouse(int x, int y)
{
	auto pos_x = (x - relative_pos_.x) / zoom_rate_ + GetMostPosX(false);
	auto pos_y = (y - relative_pos_.y) / zoom_rate_ + GetMostPosY(false);
	const float kMaxDist = 6;
	return FindNearestPos(pos_x, pos_y, &kMaxDist);
}

RECM_PosBase* RECM_StageMgr::FindNearestPos(float x, float y, const float* max_distance)
{
	if (pos_info_.empty())
		return nullptr;
	struct stHlp{
		float			dist_;
		RECM_PosBase*	pos_;
	};
	std::vector<stHlp> vec;
	for (auto& v : pos_info_)
	{
		stHlp tmp;
		tmp.dist_ = v.pos_.Distance(x, y);
		if (max_distance && tmp.dist_ >= *max_distance)
			continue;
		tmp.pos_ = &v;
		vec.push_back(tmp);
	}
	if (vec.empty())
		return nullptr;
	std::sort(vec.begin(), vec.end(), [](const stHlp& lhs, const stHlp& rhs){
		return lhs.dist_ < rhs.dist_;
	});
	return vec.begin()->pos_;
}

float RECM_StageMgr::GetMostPosX(bool is_max) const
{
	if (is_max)
	{
		auto it = std::max_element(pos_info_.begin(), pos_info_.end(), [](const RECM_PosBase& lhs, const RECM_PosBase& rhs){
			return lhs.pos_.x < rhs.pos_.x;
		});
		if (it == pos_info_.end())
		{
			if (role_pos_.x < 1)
				return -1;
			return role_pos_.x;
		}
		if (role_pos_.x < 1)
			return it->pos_.x;
		return std::max(role_pos_.x, it->pos_.x);
	}
	else
	{
		auto it = std::min_element(pos_info_.begin(), pos_info_.end(), [](const RECM_PosBase& lhs, const RECM_PosBase& rhs){
			return lhs.pos_.x < rhs.pos_.x;
		});
		if (it == pos_info_.end())
		{
			if (role_pos_.x < 1)
				return -1;
			return role_pos_.x;
		}
		if (role_pos_.x < 1)
			return it->pos_.x;
		return std::min(role_pos_.x, it->pos_.x);
	}	
}

float RECM_StageMgr::GetMostPosY(bool is_max) const
{
	if (is_max)
	{
		auto it = std::max_element(pos_info_.begin(), pos_info_.end(), [](const RECM_PosBase& lhs, const RECM_PosBase& rhs){
			return lhs.pos_.y < rhs.pos_.y;
		});
		if (it == pos_info_.end())
		{
			if (role_pos_.x < 1)
				return -1;
			return role_pos_.y;
		}
		if (role_pos_.x < 1)
			return it->pos_.y;
		return std::max(role_pos_.y, it->pos_.y);
	}
	else
	{
		auto it = std::min_element(pos_info_.begin(), pos_info_.end(), [](const RECM_PosBase& lhs, const RECM_PosBase& rhs){
			return lhs.pos_.y < rhs.pos_.y;
		});
		if (it == pos_info_.end())
		{
			if (role_pos_.x < 1)
				return -1;
			return role_pos_.y;
		}
		if (role_pos_.x < 1)
			return it->pos_.y;
		return std::min(role_pos_.y, it->pos_.y);
	}
}

bool RECM_StageMgr::Paint(CDC& wnd_dc)
{
	dc_.FillSolidRect(&rect_, RGB(255, 255, 200));

	auto top_x = GetMostPosX(false);
	auto top_y = GetMostPosY(false);

	for (auto& v : pos_info_)
	{
		CBrush*	brush = &brush_normal_;
		CPen* pen = &pen_normal_;
		if (enRECM_PosState::kSelected == v.state_)
		{
			pen = &pen_selected_;
			brush = &brush_selected_;
		}
		else if (enRECM_PosState::kMouseHover == v.state_)
		{
			pen = &pen_hover_;
			brush = &brush_hover_;
		}
		auto pos_x = (int)((v.pos_.x - top_x) * zoom_rate_) + relative_pos_.x;
		auto pos_y = (int)((v.pos_.y - top_y) * zoom_rate_) + relative_pos_.y;
		dc_.SelectObject(*pen);
		dc_.SelectObject(*brush);
		dc_.Rectangle(pos_x, pos_y, pos_x + 5, pos_y + 5);
	}

	for (auto& v : edge_info_.GetValues())
	{
		auto pos_lhs = FindByPosId(v.first);
		if (!pos_lhs)
		{
			assert(false);
			continue;
		}
		auto pos_rhs = FindByPosId(v.second);
		if (!pos_rhs)
		{
			assert(false);
			continue;
		}
		auto pos_x = (int)((pos_lhs->pos_.x - top_x) * zoom_rate_) + relative_pos_.x;
		auto pos_y = (int)((pos_lhs->pos_.y - top_y) * zoom_rate_) + relative_pos_.y;
		dc_.SelectObject(pen_line_);
		dc_.MoveTo(pos_x, pos_y);
		pos_x = (int)((pos_rhs->pos_.x - top_x) * zoom_rate_) + relative_pos_.x;
		pos_y = (int)((pos_rhs->pos_.y - top_y) * zoom_rate_) + relative_pos_.y;
		dc_.LineTo(pos_x, pos_y);
	}
	if (role_pos_.x > 1)
	{
		auto pos_x = (int)((role_pos_.x - top_x) * zoom_rate_) + relative_pos_.x;
		auto pos_y = (int)((role_pos_.y - top_y) * zoom_rate_) + relative_pos_.y;
		dc_.SelectObject(pen_role_);
		dc_.SelectObject(brush_role_);
		dc_.Rectangle(pos_x, pos_y, pos_x + 7, pos_y + 7);
	}
	if (!wnd_dc.BitBlt(0, 0, rect_.Width(), rect_.Height(), &dc_, 0, 0, SRCCOPY))
	{
		assert(false);
		return false;
	}
	return true;
}

void RECM_StageMgr::SetRolePos(const stRC_Vector3& pos)
{
	role_pos_ = pos;
}

void RECM_StageMgr::ClearAll()
{
	pos_info_.clear();
	edge_info_.GetValues().clear();
	select_info_.GetValues().clear();
	std::memset(&role_pos_, 0, sizeof(role_pos_));
}

void RECM_StageMgr::SetZoomRate(float zoom_rate)
{
	zoom_rate_ = zoom_rate;
}

const CRect& RECM_StageMgr::GetRect() const
{
	return rect_;
}

float& RECM_StageMgr::GetZoomRate()
{
	return zoom_rate_;
}

stRC_Vector3 RECM_StageMgr::GetPosFromMouse(int x, int y)
{
	auto pos_x = (x - relative_pos_.x) / zoom_rate_ + GetMostPosX(false);
	auto pos_y = (y - relative_pos_.y) / zoom_rate_ + GetMostPosY(false);
	auto the_pos = FindNearestPos(pos_x, pos_y, nullptr);
	if (!the_pos)
	{
		LOG_O(Log_warning) << "还没有坐标信息，找不到最近的坐标";
		return stRC_Vector3();
	}
	stRC_Vector3 res;
	res.x = pos_x;
	res.y = pos_y;
	res.z = the_pos->pos_.z;
	return res;
}

CPoint& RECM_StageMgr::GetRelativePos()
{
	return relative_pos_;
}

bool operator == (const stRC_Vector3& lhs, const stRC_Vector3& rhs){
	return FloatEqual(lhs.x, rhs.x) && FloatEqual(lhs.y, rhs.y);
}
std::ostream& operator << (std::ostream& ostm, const stRC_Vector3& v){
	ostm << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return ostm;
}

stRC_Vector3::stRC_Vector3()
{
}

stRC_Vector3::stRC_Vector3(float the_x, float the_y, float the_z) : stCmnVec3(the_x, the_y, the_z)
{
}

bool operator == (const RECM_StageMgr::EdgeT& lhs, const RECM_StageMgr::EdgeT& rhs){
	if (lhs.first == rhs.first && lhs.second == rhs.second)
		return true;
	if (lhs.second == rhs.first && lhs.first == rhs.second)
		return true;
	return false;
}

RECM_StageMgr::EdgeT::EdgeT(int lhs, int rhs)
{
	first = lhs;
	second = rhs;
}
