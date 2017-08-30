#pragma once
/*
@author zhp
@date 2017/4/11 12:39
@purpose for astar
*/
#include <CommonX/CmnX_HeapAlgoMacro.h>
#include <CommonX/CmnX_StaticConts.h>
#include <CommonX/CmnX_Math.h>
#include <Common/LogOut.h>
#include <CmnMix/Cmn_PocoMore.h>
#include <ProtocolX/P_BaseTypeX.h>
#include <bitset>

//////////////////////////////////////////////////////////////////////////
template<typename MatrixValueType_, int MaxSideSize>
class CmnMatrixAreaDivision{
public:
	typedef typename pt_as_size<sizeof(MatrixValueType_)>::type MatrixValueType;
	typedef typename pt_as_value<MaxSideSize * MaxSideSize>::type size_type;
	typedef MatrixValueType(MatrixType)[MaxSideSize][MaxSideSize];
	enum : MatrixValueType{
		kBlockPoint = 0,					//阻碍点，小于等于此值是阻碍点
		kFreePoint = kBlockPoint + 1,		//自由的点，大于等于此值是可通行的点
		kInvalidAreaId = -1,					//无效区域ID，无效房间ID
	};

private:
	static_assert(sizeof(MatrixType) == MaxSideSize * MaxSideSize, "");
	typedef CmnBufferVector<int, MaxSideSize * 2> OpenPositions;
	enum : MatrixValueType{
		kFreePlaceholer = -1,
		kFreePlaceholer_InOpenList = -2,
	};

public:
	CmnMatrixAreaDivision(MatrixType& matrix) : matrix_(matrix){
		width_ = 0;
		height_ = 0;
	}
	CmnMatrixAreaDivision(const CmnMatrixAreaDivision&) = delete;
	CmnMatrixAreaDivision& operator = (const CmnMatrixAreaDivision& rhs){
		width_ = rhs.width_;
		height_ = rhs.height_;
		memcpy(&matrix_, rhs.matrix_, sizeof(MatrixType));
		return *this;
	}
	pt_dword GetMovablePointCount() const{
		pt_dword cnt = 0;
		auto my_h = Height(), my_w = Width();
		stCmnVec2<int> pos_tmp;
		for (pos_tmp.y = 0; pos_tmp.y < my_h; ++pos_tmp.y)
		{
			for (pos_tmp.x = 0; pos_tmp.x < my_w; ++pos_tmp.x)
			{
				if (IsFree(pos_tmp))
					++cnt;
			}
		}
		return cnt;
	}
	void assign(size_type w, size_type h){
		if (w <= 0 || w > MaxSideSize)
		{
			assert(false);
			return;
		}
		if (h <= 0 || h > MaxSideSize)
		{
			assert(false);
			return;
		}
		width_ = w;
		height_ = h;
		for (int the_h = 0; the_h < MaxSideSize; ++the_h)
		{
			for (int the_w = 0; the_w < MaxSideSize; ++the_w)
				matrix_[the_h][the_w] = kBlockPoint;
		}
	}
	bool operator == (const CmnMatrixAreaDivision& rhs) const{
		if (width_ != rhs.width_)
			return false;
		if (height_ != rhs.height_)
			return false;
		return 0 == memcmp(&matrix_, &rhs.matrix_, sizeof(MatrixType));
	}
	//返回区域/房间的数量
	MatrixValueType DivisionArea(){
		if (width_ <= 0 || width_ > MaxSideSize)
		{
			assert(false);
			return 0;
		}
		if (height_ <= 0 || height_ > MaxSideSize)
		{
			assert(false);
			return 0;
		}
		UnEnsureInit();
		MatrixValueType area_cnt = 0;
		for (int h = 0; h < height_; ++h)
		{
			for (int w = 0; w < width_; ++w)
			{
				if (matrix_[h][w] != (MatrixValueType)kFreePlaceholer)
					continue;
				assert(matrix_[h][w] >= kFreePlaceholer);
				matrix_[h][w] = kFreePlaceholer_InOpenList;
				open_positions_.push_back(h * width_ + w);
				if (ScanToEnsure(area_cnt + kFreePoint) > 0)
					++area_cnt;
			}
		}
		return area_cnt;
	}
	inline size_type Width() const{
		return width_;
	}
	inline size_type Height() const{
		return height_;
	}
	bool ResetBySize(size_type w, size_type h){
		if (w <= 0 || w > MaxSideSize)
		{
			assert(false);
			return false;
		}
		if (h <= 0 || h > MaxSideSize)
		{
			assert(false);
			return false;
		}
		width_ = w;
		height_ = h;
		assert(w > 0 && w <= MaxSideSize);
		assert(h > 0 && h <= MaxSideSize);

		for (int the_h = 0; the_h < MaxSideSize; ++the_h)
		{
			if (the_h >= h)
			{
				for (int the_w = 0; the_w < MaxSideSize; ++the_w)
					matrix_[the_h][the_w] = kBlockPoint;
			}
			else
			{
				for (int the_w = 0; the_w < MaxSideSize; ++the_w)
				{
					if (the_w >= w)
						matrix_[the_h][the_w] = kBlockPoint;
					else
						matrix_[the_h][the_w] = kFreePoint;
				}
			}
		}
		return true;
	}
	template<typename T>
	inline bool IsBlock(const stCmnVec2<T>& position) const{
		if (position.x < 0 || position.x >= width_) return true;
		if (position.y < 0 || position.y >= height_) return true;
		return matrix_[position.y][position.x] <= kBlockPoint;
	}
	template<typename T>
	inline bool IsFree(const stCmnVec2<T>& position) const{
		if (position.x < 0 || position.x >= width_) return false;
		if (position.y < 0 || position.y >= height_) return false;
		return matrix_[position.y][position.x] >= kFreePoint;
	}
	template<typename T>
	inline bool IsFree(const stCmnVec2<T>& position, MatrixValueType area_id) const{
		if (position.x < 0 || position.x >= width_) return false;
		if (position.y < 0 || position.y >= height_) return false;
		return matrix_[position.y][position.x] == area_id + kFreePoint;
	}
	template<typename T>
	inline MatrixValueType GetAreaId(const stCmnVec2<T>& position) const{
		if (position.x < 0 || position.x >= width_) return kInvalidAreaId;
		if (position.y < 0 || position.y >= height_) return kInvalidAreaId;
		auto cur = matrix_[position.y][position.x];
		if (cur <= kBlockPoint) return kInvalidAreaId;
		return cur - kFreePoint;
	}
	template<typename T>
	inline void SetFreePoint(const stCmnVec2<T>& position){
		if (position.x < 0 || position.x >= width_) return;
		if (position.y < 0 || position.y >= height_) return;
		matrix_[position.y][position.x] = kFreePoint;
	}
	template<typename T>
	inline void SetBlockPoint(const stCmnVec2<T>& position){
		if (position.x < 0 || position.x >= width_) return;
		if (position.y < 0 || position.y >= height_) return;
		matrix_[position.y][position.x] = kBlockPoint;
	}
	template<typename T>
	inline MatrixValueType GetValue(const stCmnVec2<T>& position) const{
		if (position.x < 0 || position.x >= width_) return kBlockPoint;
		if (position.y < 0 || position.y >= height_) return kBlockPoint;
		return matrix_[position.y][position.x];
	}
	template<typename T>
	inline void SetValue(const stCmnVec2<T>& position, MatrixValueType value){
		if (position.x < 0 || position.x >= width_) return;
		if (position.y < 0 || position.y >= height_) return;
		matrix_[position.y][position.x] = value;
	}

private:
	void UnEnsureInit(){
		for (size_type h = 0; h < height_; ++h)
		{
			for (size_type w = 0; w < width_; ++w)
			{
				auto& cur_pos = matrix_[h][w];
				if (cur_pos <= kBlockPoint)
					continue;
				cur_pos = kFreePlaceholer;
			}
		}
	}
	int ScanToEnsure(MatrixValueType area_based){
		assert(area_based >= kFreePoint);
		static const stCmnVec2<size_type> dir_array[] = {
			{ 1, 0 }, { 0, -1 }, { -1, 0 }, { 0, 1 }
		};
		int convert_cnt = 0;
		while (!open_positions_.empty())
		{
			auto cur_position = open_positions_.back();
			open_positions_.pop_back();
			int cur_y = cur_position / width_, cur_x = cur_position % width_;
			int h_tmp, w_tmp;
			for (auto& dir : dir_array)
			{
				h_tmp = cur_y + dir.y;
				if (h_tmp < 0 || h_tmp >= height_)
					continue;
				w_tmp = cur_x + dir.x;
				if (w_tmp < 0 || w_tmp >= width_)
					continue;
				auto& v_tmp = matrix_[h_tmp][w_tmp];
				if (v_tmp != (MatrixValueType)kFreePlaceholer)
					continue;
				v_tmp = kFreePlaceholer_InOpenList;
				open_positions_.push_back(h_tmp * width_ + w_tmp);
			}
			matrix_[cur_y][cur_x] = area_based;
			++convert_cnt;
		}
		return convert_cnt;
	}

private:
	OpenPositions	open_positions_;
	MatrixType&		matrix_;
	size_type		width_;
	size_type		height_;
};
//////////////////////////////////////////////////////////////////////////
template<typename ParentClass_>
class CmnRectifyToFreePosAssist{
private:
	typedef ParentClass_ ParentClass;
	typedef typename ParentClass::vector_type vector_type;

public:
	inline CmnRectifyToFreePosAssist(const ParentClass& parent) : parent_(parent){}
	template<typename MatrixAssist_>
	bool RectifyToFreePos(const MatrixAssist_& matrix_assist, const vector_type& src_pos, const vector_type* liked_pos, 
		vector_type& my_pos) const{
		assert(&src_pos != &my_pos);
		int liked_area_id = -1;
		vector_type tmp_pos;
		if (liked_pos)
		{
			parent_.CastToMyPos(*liked_pos, tmp_pos);
			liked_area_id = matrix_assist.GetAreaId(tmp_pos);
		}
		parent_.CastToMyPos(src_pos, my_pos);
		if (matrix_assist.IsFree(my_pos))
		{
			if (liked_area_id < 0)
				return true;
			if (matrix_assist.GetAreaId(my_pos) == liked_area_id)
				return true;
		}
		struct stPosDistanceInfo{
			float			my_distance_;
			bool			is_used_;
		};
		const auto my_center_pos = my_pos;
		vector_type left_bottom(src_pos), pos_tmp, my_left_bottom;
		left_bottom -= parent_.RectifiedRadius;
		const auto side_len = parent_.RectifiedRadius * 2;
		const auto total_len = side_len * side_len;
		stPosDistanceInfo pos_dist_info[side_len][side_len];
		ZeroMemory(pos_dist_info, sizeof(pos_dist_info));
		parent_.CastToMyPos(left_bottom, my_left_bottom);
		int used_cnt = 0;
		for (auto idx = 0; idx < total_len; ++idx)
		{
			pos_tmp.x = idx % side_len;
			pos_tmp.y = idx / side_len;
			pos_tmp += left_bottom;
			parent_.CastToMyPos(pos_tmp, my_pos);
			if (matrix_assist.IsBlock(my_pos))
				continue;
			pos_tmp = my_pos;
			pos_tmp -= my_left_bottom;
			assert(pos_tmp.x >= 0 && pos_tmp.y >= 0 && pos_tmp.x < side_len && pos_tmp.y < side_len);
			auto& info = pos_dist_info[pos_tmp.y][pos_tmp.x];
			if (info.is_used_)
				continue;
			++used_cnt;
			info.is_used_ = true;
			info.my_distance_ = my_center_pos.Distance(my_pos);
		}
		if (used_cnt <= 0)
			return false;
		static const vector_type dir_array[] = {
			{ 1, 0 }, { 0, -1 }, { -1, 0 }, { 0, 1 }
		};
		float side_dist = side_len, last_weight = -(total_len * 10);
		side_dist /= 10;
		assert(side_dist > 0);
		int cnt_tmp = 0, last_weight_cnt = 0;
		for (pos_tmp.y = 0; pos_tmp.y < side_len; ++pos_tmp.y)
		{
			for (pos_tmp.x = 0; pos_tmp.x < side_len; ++pos_tmp.x)
			{
				auto& the_info = pos_dist_info[pos_tmp.y][pos_tmp.x];
				if (!the_info.is_used_)
					continue;
				float weight_tmp = 0;
				for (auto& dir : dir_array)
				{
					auto the_pos_tmp = pos_tmp + dir;
					if (the_pos_tmp.x < 0 || the_pos_tmp.y < 0 || the_pos_tmp.x >= side_len || the_pos_tmp.y >= side_len)
						continue;
					auto& tmp_the_info = pos_dist_info[the_pos_tmp.y][the_pos_tmp.x];
					if (!tmp_the_info.is_used_)
						continue;
					weight_tmp = weight_tmp + side_dist;
				}
				if (liked_area_id >= 0)
				{
					tmp_pos = pos_tmp;
					tmp_pos += my_left_bottom;
					if (matrix_assist.GetAreaId(tmp_pos) == liked_area_id)
						weight_tmp *= 1.5;
				}
				weight_tmp -= the_info.my_distance_;
				if (last_weight < weight_tmp || 0 == last_weight_cnt)
				{
					++last_weight_cnt;
					last_weight = weight_tmp;
					my_pos = pos_tmp;
				}
				++cnt_tmp;
				if (cnt_tmp >= used_cnt)
					goto LABLE_NEXT;
			}
		}
	LABLE_NEXT:
		assert(cnt_tmp == used_cnt);
		if (last_weight_cnt <= 0)
		{
			assert(false);
			return false;
		}
		my_pos += my_left_bottom;
		return true;
	}

private:
	const ParentClass&	parent_;
};
//////////////////////////////////////////////////////////////////////////
template<int RectifiedRadius_, int CompressRadius_, int RayStep_, bool IsDebugTest>
struct _CmnAStarBaseT{
	enum : int{
		CompressRadius = CompressRadius_,
		RayStep = RayStep_,
	};

	void SetAStarParam(int compress_radius, int ray_step){}
};

template<int RectifiedRadius_, int CompressRadius_, int RayStep_>
struct _CmnAStarBaseT<RectifiedRadius_, CompressRadius_,
	RayStep_, true>{
	_CmnAStarBaseT(){
		CompressRadius = CompressRadius_;
		RayStep = RayStep_;
	}
	void SetAStarParam(int compress_radius, int ray_step){
		CompressRadius = compress_radius;
		RayStep = ray_step;
	}

	int		CompressRadius;
	int		RayStep;
};

template<typename VecT, typename MapDataT, typename MatrixValueType_,
	int MaxSideSize,
	int SideSize_,
	int RectifiedRadius_,
	int CompressRadius_,
	int RayStep_,
	bool IsDebugTest>
class CmnAStarBase : public _CmnAStarBaseT<RectifiedRadius_, CompressRadius_,
	RayStep_, IsDebugTest>{
	static_assert(SideSize_ > 0 && SideSize_ <= MaxSideSize, "");

public:
	enum : int{
		kMaxSideSize = MaxSideSize,
		kSideSize = SideSize_,
		RectifiedRadius = RectifiedRadius_,
	};
	typedef CmnMatrixAreaDivision<MatrixValueType_, MaxSideSize> MatrixAreaDivision;
	typedef typename MatrixAreaDivision::MatrixValueType MatrixValueType;
	typedef typename MatrixAreaDivision::MatrixType MatrixType;
	typedef VecT vector_type;
	typedef typename vector_type::value_type value_type;
	typedef stCmnRect<vector_type> RectType;
	typedef CmnBufferVector<vector_type, MaxSideSize * 8> PosContT;
	typedef CmnAStarBase AStarBaseType;
	struct stPosNode : vector_type{
		stPosNode*	parent_;
	};
	typedef CmnRectifyToFreePosAssist<CmnAStarBase> RectifyPosImpl;

public:
	CmnAStarBase() : matrix_assit_(matrix_){
		SideSizeZoomRate = (float)kSideSize / (float)MaxSideSize;
		if (SideSizeZoomRate > 1) SideSizeZoomRate = 1;
		zoom_rate_ = SideSizeZoomRate;
	}
	CmnAStarBase(const CmnAStarBase& rhs) : CmnAStarBase(){
		Init(rhs);
	}
	CmnAStarBase& operator = (const CmnAStarBase& rhs){
		Init(rhs);
		return *this;
	}
	bool operator == (const CmnAStarBase& rhs) const{
		if (zoom_rate_ != rhs.zoom_rate_)
			return false;
		if (valid_rect_ != rhs.valid_rect_)
			return false;
		if (translation_ != rhs.translation_)
			return false;
		return matrix_assit_ == rhs.matrix_assit_;
	}
	bool operator != (const CmnAStarBase& rhs) const{
		return !(*this == rhs);
	}
	inline int GetWidth() const{
		return matrix_assit_.Width();
	}
	inline int GetHeight() const{
		return matrix_assit_.Height();
	}
	const MatrixAreaDivision& GetMatrixAssist() const{
		return matrix_assit_;
	}
	void SetSideSize(int side_size){
		assert(side_size > 0 && side_size <= MaxSideSize);
		SideSizeZoomRate = (float)side_size / (float)MaxSideSize;
		if (SideSizeZoomRate > 1) SideSizeZoomRate = 1;
	}
	bool Init(const MapDataT& map_data){
		auto map_size = map_data.GetMapSize();
		if (map_size.x < 2 || map_size.y < 2)
		{
			assert(false);
			valid_rect_ = RectType();
			return false;
		}
		translation_ = map_data.GetTranslation();
		if (!InitForRect(map_data))
		{
			assert(false);
			return false;
		}
		auto valid_rect_size = valid_rect_.Size();
		if (valid_rect_size.AnyLess(1))
		{
			assert(false);
			return false;
		}
		const int kMySideSize = (float)MaxSideSize * (float)SideSizeZoomRate;
		zoom_rate_ = (float)kMySideSize / (float)valid_rect_size.x;
		zoom_rate_ = std::min((float)kMySideSize / (float)valid_rect_size.y, zoom_rate_);
		if (zoom_rate_ > 1)
			zoom_rate_ = 1;
		if (!matrix_assit_.ResetBySize((int)((float)valid_rect_size.x * zoom_rate_),
			(int)((float)valid_rect_size.y * zoom_rate_)))
		{
			assert(false);
			return false;
		}

		int total_length = valid_rect_size.x * valid_rect_size.y;
		vector_type pos_tmp, my_pos_tmp;
		for (int i = 0; i < total_length; ++i)
		{
			my_pos_tmp.assign(i % valid_rect_size.x, i / valid_rect_size.x);
			pos_tmp = my_pos_tmp;
			pos_tmp += valid_rect_.left_bottom_;
			pos_tmp += translation_;
			if (!map_data.CanMove(pos_tmp))
			{
				my_pos_tmp *= zoom_rate_;
				matrix_assit_.SetBlockPoint(my_pos_tmp);
			}
		}
		if (matrix_assit_.DivisionArea() <= 0)
		{
			assert(false);
			return false;
		}
		return true;
	}
	bool Init(const CmnAStarBase& rhs){
		if (this == &rhs)
		{
			assert(false);
			return false;
		}
		zoom_rate_ = rhs.zoom_rate_;
		SideSizeZoomRate = rhs.SideSizeZoomRate;
		matrix_assit_ = rhs.matrix_assit_;
		valid_rect_ = rhs.valid_rect_;
		translation_ = rhs.translation_;
		return true;
	}
	stSizeInt ToBitset(std::bitset<kMaxSideSize * kMaxSideSize>& bset) const{
		bset.reset();
		auto my_h = matrix_assit_.Height(), my_w = matrix_assit_.Width();
		vector_type pos_tmp;
		for (pos_tmp.y = 0; pos_tmp.y < my_h; ++pos_tmp.y)
		{
			auto h_idx = pos_tmp.y * my_w;
			for (pos_tmp.x = 0; pos_tmp.x < my_w; ++pos_tmp.x)
			{
				if (!TestCanMove(pos_tmp))
				{
					bset.set(h_idx + pos_tmp.x);
				}
			}
		}
		return stSizeInt(my_w, my_h);
	}
	bool UpdateArea(const vector_type& center_pos, int radius, const MapDataT& map_data){
		vector_type my_center_pos;
		if (!RectifyPos(center_pos, nullptr, my_center_pos))
			return false;
		auto self_value = matrix_assit_.GetValue(my_center_pos);
		auto left_bottom = center_pos, right_top = center_pos;
		left_bottom -= radius;
		right_top += radius;
		vector_type pos_tmp, my_pos_tmp;
		for (pos_tmp.y = left_bottom.y; pos_tmp.y <= right_top.y; ++pos_tmp.y)
		{
			for (pos_tmp.x = left_bottom.x; pos_tmp.x <= right_top.x; ++pos_tmp.x)
			{
				CastToMyPos(pos_tmp, my_pos_tmp);
				matrix_assit_.SetValue(my_pos_tmp, self_value);
			}
		}
		for (pos_tmp.y = left_bottom.y; pos_tmp.y <= right_top.y; ++pos_tmp.y)
		{
			for (pos_tmp.x = left_bottom.x; pos_tmp.x <= right_top.x; ++pos_tmp.x)
			{
				if (!map_data.CanMove(pos_tmp))
				{
					CastToMyPos(pos_tmp, my_pos_tmp);
					matrix_assit_.SetBlockPoint(my_pos_tmp);
				}
			}
		}
		return true;
	}
	MatrixValueType GetAreaId(const vector_type& pos) const{
		vector_type my_pos;
		if (!RectifyPos(pos, nullptr, my_pos))
		{
			//assert(false);
			return MatrixAreaDivision::kInvalidAreaId;
		}
		auto area_id = matrix_assit_.GetAreaId(my_pos);
		assert(area_id != (MatrixValueType)MatrixAreaDivision::kInvalidAreaId);
		return area_id;
	}
	inline bool TestCanMove(const vector_type& pos, int area_id) const{
		return matrix_assit_.IsFree(pos, area_id);
	}
	inline bool TestCanMove(const vector_type& pos) const{
		return matrix_assit_.IsFree(pos);
	}
	inline void CastToMyPos(const vector_type& pos, vector_type& out_pos) const{
		out_pos = pos;
		out_pos -= translation_;
		out_pos -= valid_rect_.left_bottom_;
		out_pos *= zoom_rate_;
		RectifyToValidRect(out_pos);
	}
	inline void CastToMyDistance(float distance, float& out_distance) const{
		out_distance = (float)distance * (float)zoom_rate_;
	}
	inline void CastToGamePos(const vector_type& pos, vector_type& out_pos) const{
		out_pos = pos;
		out_pos /= zoom_rate_;
		out_pos += valid_rect_.left_bottom_;
		out_pos += translation_;
	}
	bool RectifyPos(const vector_type& src_pos, const vector_type* liked_pos, vector_type& my_pos) const{
		return RectifyPosImpl(*this).RectifyToFreePos(matrix_assit_, src_pos, liked_pos, my_pos);
	}
	inline bool CanLineMoveTo_Game(const vector_type& pos_src, const vector_type& pos_dst) const{
		vector_type pos_tmp_src, pos_tmp_dst;
		CastToMyPos(pos_src, pos_tmp_src);
		CastToMyPos(pos_dst, pos_tmp_dst);
		return CanLineMoveTo_My(pos_tmp_src, pos_tmp_dst);
	}
	inline bool CanLineMoveTo_My(const vector_type& pos_src, const vector_type& pos_dst) const{
		if (pos_src == pos_dst) return matrix_assit_.IsFree(pos_src);
		const auto& form = MakeLineGeneralForm(pos_src, pos_dst);
		int cursor = 1;
		vector_type pos_tmp;
		while (form.GetPosByStep(1, cursor, pos_tmp))
		{
			if (matrix_assit_.IsBlock(pos_tmp))
				return false;
		}
		return true;
	}
	//圆是否能被击中，radius不是缩放之后的，radius是未缩放的坐标系
	template<typename RadiusT>
	bool CanShootToCircle(const vector_type& center, RadiusT radius,
		const vector_type& pos1, const vector_type& pos2) const{
		if (pos2 == pos1)
			return center.CirclePointIntersects(radius, pos1);
		return center.CircleSegmentIntersects(radius, pos1, pos2);
	}
	//矩形中不被击中的坐标点，half_width和half_height都不是缩放之后的，即都是未缩放的坐标系
	bool RectangleSafePosByShoot(const vector_type& center, int half_width, int half_height, float center_distance,
		const vector_type* dst_pos,
		const vector_type& pos_from, const vector_type& pos_to, vector_type& out_pos) const{
		assert(half_width > 0);
		assert(half_height > 0);
		auto the_width = half_width * 2;
		auto the_height = half_height * 2;
		assert(the_width < matrix_assit_.Width());
		assert(the_height < matrix_assit_.Height());

		auto center_tmp = center;
		vector_type my_center_tmp;
		if (!RectifyPos(center_tmp, nullptr, my_center_tmp))
		{
			assert(false);
			return false;
		}
		auto center_area_id = matrix_assit_.GetAreaId(my_center_tmp);
		assert(center_area_id != (MatrixValueType)MatrixAreaDivision::kInvalidAreaId);
		auto pos_from_tmp = pos_from;
		//CastToMyPos(pos_from, pos_from_tmp);
		auto pos_to_tmp = pos_to;
		//CastToMyPos(pos_to, pos_to_tmp);
		bool is_pos_equal = pos_from_tmp == pos_to_tmp;
		bool is_center_src_equal = pos_from_tmp == center_tmp;
		vector_type dst_pos_tmp;
		if (dst_pos)
		{
			dst_pos_tmp = *dst_pos;
			//CastToMyPos(*dst_pos, dst_pos_tmp);
		}
		vector_type pos_tmp, my_pos_tmp, cross_pos, left_bottom(center_tmp);
		left_bottom.x -= half_width;
		left_bottom.y -= half_height;
		if (left_bottom.x < 0)
			left_bottom.x = 0;
		if (left_bottom.y < 0)
			left_bottom.y = 0;

		float distance_total = -1, distance_center_res_max = -1;
		static const int kConstDist = 100000;
		static const float kDistRate = 1.5;
		for (int y = 0; y < the_height; ++y)
		{
			pos_tmp.y = left_bottom.y + y;
			for (int x = 0; x < the_width; ++x)
			{
				pos_tmp.x = left_bottom.x + x;
				if (pos_tmp == center_tmp)
					continue;
				CastToMyPos(pos_tmp, my_pos_tmp);
				if (!TestCanMove(my_pos_tmp, center_area_id))
					continue;
				if (!CanLineMoveTo_My(my_center_tmp, my_pos_tmp))
					continue;
				if (is_pos_equal)
				{
					if (pos_tmp.Distance(pos_from_tmp) < center_distance)
						continue;
				}
				else
				{
					if (pos_tmp.IsInLine(pos_from_tmp, pos_to_tmp, center_distance))
						continue;
					if (vector_type::SegmentCrossPoint(pos_tmp, center_tmp, pos_from_tmp, pos_to_tmp, cross_pos))
					{
						if (cross_pos != center_tmp)
							continue;
					}
				}
				auto the_distance = pos_tmp.Distance(center_tmp);
				if (the_distance < center_distance)
				{
					if (distance_center_res_max < 0 || the_distance > distance_center_res_max)
					{
						if (!is_center_src_equal)
						{
							the_distance += kConstDist - pos_tmp.Distance(center_tmp, pos_from_tmp) * kDistRate;
						}
						distance_center_res_max = the_distance;
						out_pos = pos_tmp;
					}
					continue;
				}
				if (!is_center_src_equal)
				{
					the_distance += kConstDist - pos_tmp.Distance(center_tmp, pos_from_tmp) * kDistRate;
				}
				if (dst_pos)
					the_distance += pos_tmp.Distance(dst_pos_tmp);
				if (distance_total < 0 || the_distance < distance_total)
				{
					distance_total = the_distance;
					out_pos = pos_tmp;
				}
			}
		}
		if (distance_total > 0)
		{
			//CastToGamePos(out_pos, out_pos);
			return true;
		}
		if (distance_center_res_max > 0)
		{
			//CastToGamePos(out_pos, out_pos);
			return true;
		}
		return false;
	}
	//射线探路
	bool GenRaySteps(const vector_type& pos_src, const vector_type& pos_dst, PosContT& out_pos_info) const{
		if (pos_src == pos_dst) return false;
		out_pos_info.clear();
		vector_type pos_src_tmp, pos_dst_tmp;
		CastToMyPos(pos_src, pos_src_tmp);
		CastToMyPos(pos_dst, pos_dst_tmp);
		if (pos_src_tmp == pos_dst_tmp)
		{
			out_pos_info.push_back(pos_dst);
			return true;
		}
		const auto& form = MakeLineGeneralForm(pos_src, pos_dst);
		int cursor = 1;
		vector_type pos_tmp(pos_dst), my_pos_tmp;
		const vector_type* last_pos = &pos_src_tmp;
		while (form.GetPosByStep(RayStep, cursor, pos_tmp))
		{
			CastToMyPos(pos_tmp, my_pos_tmp);
			if (CanLineMoveTo_My(*last_pos, my_pos_tmp))
			{
				out_pos_info.push_back(pos_tmp);
				last_pos = &out_pos_info.back();
				continue;
			}
			break;
		}
		if (out_pos_info.empty())
			last_pos = &pos_src;
		else
			last_pos = &out_pos_info.back();
		if (*last_pos != pos_tmp)
		{
			const auto& form_tmp = MakeLineGeneralForm(*last_pos, pos_tmp);
			cursor = 1;
			vector_type last_valid_pos;
			bool is_valid = false;
			CastToMyPos(*last_pos, pos_src_tmp);
			while (form_tmp.GetPosByStep(1, cursor, pos_tmp))
			{
				CastToMyPos(pos_tmp, my_pos_tmp);
				if (!CanLineMoveTo_My(pos_src_tmp, my_pos_tmp))
					break;
				last_valid_pos = pos_tmp;
				is_valid = true;
			}
			assert(cursor <= RayStep + 1);
			if (is_valid && last_valid_pos != *last_pos)
				out_pos_info.push_back(last_valid_pos);
		}
		if (out_pos_info.empty())
			return false;
		return true;
	}
	void DebugPosInfo(bool dbg_data, const std::string& file_name, const vector_type& mark_pos) const{
		auto mark_pos_tmp = mark_pos;
		CastToMyPos(mark_pos, mark_pos_tmp);
		std::ofstream ofstm(file_name);
		ofstm << std::hex;
		auto my_h = matrix_assit_.Height(), my_w = matrix_assit_.Width();
		vector_type pos_tmp;
		for (pos_tmp.y = 0; pos_tmp.y < my_h; ++pos_tmp.y)
		{
			for (pos_tmp.x = 0; pos_tmp.x < my_w; ++pos_tmp.x)
			{
				if (mark_pos_tmp == pos_tmp)
				{
					ofstm << 'A';
					continue;
				}
				if (dbg_data)
				{
					//ofstm << (char)(matrix_assit_.GetValue(pos_tmp) + 'a');
					ofstm << (int)matrix_assit_.GetValue(pos_tmp);
				}
				else
				{
					if (matrix_assit_.IsFree(pos_tmp))
						ofstm << '*';
					else
						ofstm << '&';
				}
			}
			ofstm << std::endl;
		}
	}
	template<typename T>
	void DebugPosRectInfo(const T& map_data, const std::string& file_name, const vector_type& mark_pos, int range) const{
		RectType rect(mark_pos, mark_pos);
		rect.left_bottom_ -= range;
		rect.right_top_ += range;
		std::ofstream ofstm(file_name);
		vector_type pos_tmp;
		for (pos_tmp.y = rect.left_bottom_.y; pos_tmp.y <= rect.right_top_.y; ++pos_tmp.y)
		{
			for (pos_tmp.x = rect.left_bottom_.x; pos_tmp.x <= rect.right_top_.x; ++pos_tmp.x)
			{
				if (mark_pos == pos_tmp)
					ofstm << 'A';
				else if (map_data.CanMove(pos_tmp))
					ofstm << '*';
				else
					ofstm << '&';
			}
			ofstm << std::endl;
		}
	}
	template<typename T>
	void DebugRectDataInfo(const T& map_data, const std::string& file_name, const vector_type& mark_pos, int range) const{
		RectType rect(mark_pos, mark_pos);
		rect.left_bottom_ -= range;
		rect.right_top_ += range;
		std::ofstream ofstm(file_name);
		ofstm << std::hex;
		vector_type pos_tmp;
		for (pos_tmp.y = rect.left_bottom_.y; pos_tmp.y <= rect.right_top_.y; ++pos_tmp.y)
		{
			for (pos_tmp.x = rect.left_bottom_.x; pos_tmp.x <= rect.right_top_.x; ++pos_tmp.x)
			{
				if (mark_pos == pos_tmp)
				{
					ofstm << 'A';
					continue;
				}
				ofstm << (value_type)map_data.MapData(pos_tmp);
			}
			ofstm << std::endl;
		}
	}
	template<typename T>
	void DebugRectPosDataInfo(const T& map_data, const std::string& file_name, const vector_type& mark_pos, int range) const{
		RectType rect(mark_pos, mark_pos);
		rect.left_bottom_ -= range;
		rect.right_top_ += range;
		std::ofstream ofstm(file_name);
		ofstm << std::hex;
		vector_type pos_tmp;
		for (pos_tmp.y = rect.left_bottom_.y; pos_tmp.y <= rect.right_top_.y; ++pos_tmp.y)
		{
			for (pos_tmp.x = rect.left_bottom_.x; pos_tmp.x <= rect.right_top_.x; ++pos_tmp.x)
			{
				if (mark_pos == pos_tmp)
				{
					ofstm << pos_tmp << 'A';
					continue;
				}
				ofstm << pos_tmp << (value_type)map_data.MapData(pos_tmp);
			}
			ofstm << std::endl;
		}
	}
	
protected:
	bool InitForRect(const MapDataT& map_data){
		auto map_size = map_data.GetMapSize();
		valid_rect_.left_bottom_ = map_size;
		valid_rect_.right_top_.x = 0;
		valid_rect_.right_top_.y = 0;
		vector_type pos_tmp, game_pos_tmp;
		for (pos_tmp.x = 0; pos_tmp.x < map_size.x; ++pos_tmp.x)
		{
			for (pos_tmp.y = 0; pos_tmp.y < map_size.y; ++pos_tmp.y)
			{
				game_pos_tmp = pos_tmp;
				game_pos_tmp += translation_;
				if (!map_data.CanMoveForValidRect(game_pos_tmp))
					continue;
				valid_rect_.left_bottom_.x = pos_tmp.x;
				goto LABEL_FIND_LeftBottomX;
			}
		}
		assert(false);
		return false;
	LABEL_FIND_LeftBottomX:
		for (pos_tmp.y = 0; pos_tmp.y < map_size.y; ++pos_tmp.y)
		{
			for (pos_tmp.x = valid_rect_.left_bottom_.x; pos_tmp.x < map_size.x; ++pos_tmp.x)
			{
				game_pos_tmp = pos_tmp;
				game_pos_tmp += translation_;
				if (!map_data.CanMoveForValidRect(game_pos_tmp))
					continue;
				valid_rect_.left_bottom_.y = pos_tmp.y;
				goto LABEL_FIND_LeftBottomY;
			}
		}
		assert(false);
		return false;
	LABEL_FIND_LeftBottomY:
		auto max_height = map_size.y - 1;
		for (pos_tmp.x = map_size.x - 1; pos_tmp.x > valid_rect_.left_bottom_.x; --pos_tmp.x)
		{
			for (pos_tmp.y = max_height; pos_tmp.y > valid_rect_.left_bottom_.y; --pos_tmp.y)
			{
				game_pos_tmp = pos_tmp;
				game_pos_tmp += translation_;
				if (!map_data.CanMoveForValidRect(game_pos_tmp))
					continue;
				valid_rect_.right_top_.x = pos_tmp.x;
				goto LABEL_FIND_RightTopX;
			}
		}
		assert(false);
		return false;
	LABEL_FIND_RightTopX:
		for (pos_tmp.y = max_height; pos_tmp.y > valid_rect_.left_bottom_.y; --pos_tmp.y)
		{
			for (pos_tmp.x = valid_rect_.right_top_.x; pos_tmp.x > valid_rect_.left_bottom_.x; --pos_tmp.x)
			{
				game_pos_tmp = pos_tmp;
				game_pos_tmp += translation_;
				if (!map_data.CanMoveForValidRect(game_pos_tmp))
					continue;
				valid_rect_.right_top_.y = pos_tmp.y;
				goto LABEL_FIND_RightTopY;
			}
		}
		assert(false);
		return false;
	LABEL_FIND_RightTopY:
		return true;
	}
	inline void RectifyToValidRect(vector_type& pos) const{
		if (pos.x < 0) pos.x = 0;
		if (pos.y < 0) pos.y = 0;
		auto my_h = matrix_assit_.Height(), my_w = matrix_assit_.Width();
		if (pos.x >= my_w) pos.x = my_w - 1;
		if (pos.y >= my_h) pos.y = my_h - 1;
	}
	template<int ContSize>
	bool RectifyMyPositions(const stPosNode* pos_node, CmnBufferVector<vector_type, ContSize>& out_pos_info) const{
		out_pos_info.clear();
		while (pos_node && pos_node->parent_){
			out_pos_info.push_back(*pos_node);
			pos_node = pos_node->parent_;
		}
		if (out_pos_info.empty())
		{
			assert(false);
			return false;
		}
		std::reverse(out_pos_info.begin(), out_pos_info.end());
		return true;
	}
	template<int ContSize>
	bool CompressSteps(const vector_type& my_pos_src, CmnBufferVector<vector_type, ContSize>& out_my_pos_info) const{
		if (out_my_pos_info.empty()) return true;
		float compress_radius = (float)CompressRadius * (float)zoom_rate_;
		auto it = out_my_pos_info.begin(), it_end = out_my_pos_info.end();
		vector_type my_pos_src_tmp(my_pos_src), game_pos_tmp;
		PosContT res_tmp;
		while (it != it_end)
		{
			auto it_tmp = it;
			auto it_last_valid = it;
			while (it_tmp != it_end)
			{
				const auto& my_pos_dst = *it_tmp;
				if (my_pos_src_tmp.Distance(my_pos_dst) <= compress_radius)
				{
					if (CanLineMoveTo_My(my_pos_src_tmp, my_pos_dst))
						it_last_valid = it_tmp;
				}
				else
					break;
				++it_tmp;
			}
			my_pos_src_tmp = *it_last_valid;
			CastToGamePos(my_pos_src_tmp, game_pos_tmp);
			it = it_last_valid;
			res_tmp.push_back(game_pos_tmp);
			++it;
		}
		out_my_pos_info.clear();
		for (auto& v : res_tmp)
			out_my_pos_info.push_back(v);
		return true;
	}
	template<int ContSize>
	bool RectifyGamePositions(const vector_type& pos_src, CmnBufferVector<vector_type, ContSize>& out_pos_info) const{
		auto it = out_pos_info.begin(), it_end = out_pos_info.end();
		auto it_last = it_end;
		while (it != it_end)
		{
			if (*it != pos_src)
			{
				it_last = it;
				break;
			}
			++it;
		}
		it = out_pos_info.begin();
		if (it != it_last)
			out_pos_info.erase(it, it_last);
		return true;
	}

protected:
	MatrixType			matrix_;
	MatrixAreaDivision	matrix_assit_;
	RectType			valid_rect_;
	float				zoom_rate_;
	float				SideSizeZoomRate;
	vector_type			translation_;
};

#define HEAP_LT(lhs, rhs) lhs->weight_ > rhs->weight_
template<typename VecT, typename MapDataT, typename MatrixValueType_,
	int MaxSideSize,			//最大边长尺寸
	int SideSize_, 				//边长尺寸
	int RectifiedRadius_,		//缩放之允许调整的半径，未缩放的坐标系
	int CompressRadius_,		//压缩半径，未缩放的坐标系
	int RayStep_,				//射线探路之step，未缩放的坐标系
	bool IsDebugTest>
class CmnAStar : public CmnAStarBase<VecT, MapDataT, MatrixValueType_,
	MaxSideSize, SideSize_, RectifiedRadius_, CompressRadius_, RayStep_, IsDebugTest>{
private:
	struct stMyPosNode : stPosNode{
		value_type	path_delta_;	//路径增量
		float		weight_;		//必须是小数类型，不能是整型
	};
	typedef stMyPosNode* (PosNodeMatrix)[MaxSideSize][MaxSideSize];
	struct stPosNodeCont : CmnStaticVector<stMyPosNode, MaxSideSize * MaxSideSize>{
		typedef CmnStaticVector<stMyPosNode*, MaxSideSize * MaxSideSize> NodesPtrCont;

		stMyPosNode* Add(const vector_type& pos){
			stMyPosNode tmp;
			tmp.assign(pos);
			tmp.weight_ = 0;
			tmp.path_delta_ = 0;
			tmp.parent_ = nullptr;
			push_back(tmp);
			nodes_.push_back(&back());
			return nodes_.back();
		}
		stMyPosNode* PushHeap(const vector_type& pos){
			auto node = Add(pos);
			HEAP_PushHeap(nodes_.begin(), nodes_.end())
			return node;
		}
		void clear(){
			__super::clear();
			nodes_.clear();
		}

		NodesPtrCont	nodes_;
	};
	//////////////////////////////////////////////////////////////////////////
	struct stWeightAlgo{
		static inline int Weight(const vector_type& astar_pos){ return 0; }
		template<typename T>
		static inline bool RectifyPath(T& astar_positions){ return true; }
		template<typename T>
		static inline void EnsureAddDstPos(T& game_positions, const vector_type& dst_pos){
			if (game_positions.empty())
			{
				game_positions.push_back(dst_pos);
				return;
			}
			if (game_positions.back() != dst_pos)
				game_positions.push_back(dst_pos);
		}
	};
	struct stBlockAlgo{
		static inline bool IsBlock(const CmnAStar& astar, const vector_type& pos){
			return astar.matrix_assit_.IsBlock(pos);
		}
	};
	struct stBlockAlgoWithDist{
		stBlockAlgoWithDist(const vector_type& center_pos, float radius)
			: center_pos_(center_pos), radius_(radius){}

		inline bool IsBlock(const CmnAStar& astar, const vector_type& pos) const{
			if (astar.matrix_assit_.IsBlock(pos)) return true;
			return center_pos_.Distance(pos) > radius_;
		}

		const vector_type&		center_pos_;
		float					radius_;
	};
	//////////////////////////////////////////////////////////////////////////
	static stWeightAlgo MakeWeightAlgo(){
		return stWeightAlgo();
	}
	template<typename Fn>
	static const Fn& MakeWeightAlgo(const Fn& fn){
		return fn;
	}
	//////////////////////////////////////////////////////////////////////////

public:
	CmnAStar() = default;
	CmnAStar(const CmnAStar& rhs) : CmnAStarBase(rhs){}
	CmnAStar(const CmnAStarBase& rhs) : CmnAStarBase(rhs){}
	CmnAStar& operator = (const CmnAStar& rhs){
		__super::operator = (rhs);
		open_.clear();
		return *rhs;
	}
	CmnAStar& operator = (const CmnAStarBase& rhs){
		__super::operator = (rhs);
		open_.clear();
		return *rhs;
	}
	//A星寻路
	template<typename DstT, int ContSize, typename... Args>
	bool GenFindPath(const vector_type& pos_src, const DstT& pos_dst, 
		CmnBufferVector<vector_type, ContSize>& out_pos_info, const Args&... args) const{
		stBlockAlgo block_algo;
		const auto& weight_algo = MakeWeightAlgo(args...);
		return GenFindPathT(block_algo, weight_algo, pos_src, pos_dst, out_pos_info);
	}
	template<typename DstT, int ContSize, typename... Args>
	bool GenFindPath(const vector_type& pos_src, float radius, const DstT& pos_dst, 
		CmnBufferVector<vector_type, ContSize>& out_pos_info, const Args&... args) const{
		auto my_pos_tmp = pos_src;
		CastToMyPos(pos_src, my_pos_tmp);
		radius *= zoom_rate_;
		stBlockAlgoWithDist block_algo(my_pos_tmp, radius);
		const auto& weight_algo = MakeWeightAlgo(args...);
		return GenFindPathT(block_algo, weight_algo, pos_src, pos_dst, out_pos_info);
	}
	void DebugPathInfo(const std::string& file_name, const vector_type& pos_src, const vector_type& pos_dst) const{
		PosContT path_info;
		if (!GenFindPath(pos_src, pos_dst, path_info))
		{
			assert(false);
			return;
		}
		auto my_h = matrix_assit_.Height(), my_w = matrix_assit_.Width();
		CmnBufferVector<int, 1024> idx_info;
		for (auto& v : path_info)
		{
			CastToMyPos(v, v);
			idx_info.push_back(v.y * my_w + v.x);
		}
		std::ofstream ofstm(file_name);
		vector_type pos_tmp;
		for (pos_tmp.y = 0; pos_tmp.y < my_h; ++pos_tmp.y)
		{
			for (pos_tmp.x = 0; pos_tmp.x < my_w; ++pos_tmp.x)
			{
				auto idx = pos_tmp.y * my_w + pos_tmp.x;
				for (auto v : idx_info)
				{
					if (v == idx)
					{
						ofstm << 'A';
						goto LABLE_COTINUE;
					}
				}
				if (matrix_assit_.IsFree(pos_tmp))
					ofstm << '*';
				else
					ofstm << '&';
			LABLE_COTINUE:
				continue;
			}
			ofstm << std::endl;
		}
	}

private:
	template<typename BlockAlgo, typename WeightAlgo, int ContSize>
	bool GenFindPathT(const BlockAlgo& block_algo, const WeightAlgo& weight_algo,
		const vector_type& pos_src, const vector_type& pos_dst, 
		CmnBufferVector<vector_type, ContSize>& out_pos_info) const{
		vector_type pos_src_tmp;
		if (!RectifyPos(pos_src, nullptr, pos_src_tmp))
		{
			//assert(!"A星之调整源坐标失败");
			return false;
		}
		vector_type pos_dst_tmp;
		if (!RectifyPos(pos_dst, &pos_src, pos_dst_tmp))
		{
			//assert(!"A星之调整目标坐标失败");
			return false;
		}
		if (pos_src_tmp == pos_dst_tmp)
		{
			out_pos_info.push_back(pos_dst_tmp);
		}
		else
		{
			auto src_area_id = matrix_assit_.GetAreaId(pos_src_tmp), dst_area_id = matrix_assit_.GetAreaId(pos_dst_tmp);
			if (src_area_id != dst_area_id)
				return false;
			assert(src_area_id != (MatrixValueType)MatrixAreaDivision::kInvalidAreaId);
			auto node = GenFindPathImpl(block_algo, weight_algo, pos_src_tmp, pos_dst_tmp);
			if (!node)
				return false;
			if (!RectifyMyPositions(node, out_pos_info))
			{
				assert(!"A星之调整输出坐标失败");
				return false;
			}
			if (out_pos_info.empty())
			{
				assert(false);
				return false;
				//这里最好不要添加目标坐标，以避免目标坐标距离最后一个坐标距离过大的问题。
				//out_pos_info.push_back(pos_dst);
				//return true;
			}
			assert(out_pos_info.front() != pos_src_tmp);
		}
		if (!weight_algo.RectifyPath(out_pos_info))
		{
			assert(false);
			return false;
		}
		if (!CompressSteps(pos_src_tmp, out_pos_info))
		{
			assert(!"A星之压缩失败2");
			return false;
		}
		if (!RectifyGamePositions(pos_src, out_pos_info))
		{
			assert(false);
			return false;
		}
		weight_algo.EnsureAddDstPos(out_pos_info, pos_dst);
		return true;
	}
	template<typename BlockAlgo, typename WeightAlgo, typename DstT, int ContSize>
	bool GenFindPathT(const BlockAlgo& block_algo, const WeightAlgo& weight_algo,
		const vector_type& pos_src, const DstT& pos_dst, 
		CmnBufferVector<vector_type, ContSize>& out_pos_info) const{
		vector_type pos_src_tmp;
		if (!RectifyPos(pos_src, nullptr, pos_src_tmp))
			return false;
		auto node = GenFindPathImpl(block_algo, weight_algo, pos_src_tmp, pos_dst);
		if (!node)
			return false;
		if (!RectifyMyPositions(node, out_pos_info))
		{
			assert(!"A星之调整输出坐标失败1");
			return false;
		}
		if (out_pos_info.empty())
		{
			assert(false);
			return false;
		}
		assert(out_pos_info.front() != pos_src_tmp);
		if (!weight_algo.RectifyPath(out_pos_info))
		{
			assert(false);
			return false;
		}
		if (!CompressSteps(pos_src_tmp, out_pos_info))
		{
			assert(!"A星之压缩失败3");
			return false;
		}
		if (!RectifyGamePositions(pos_src, out_pos_info))
		{
			assert(false);
			return false;
		}
		return true;
	}
	template<typename BlockAlgo, typename WeightAlgo, typename DstT>
	const stMyPosNode* GenFindPathImpl(const BlockAlgo& block_algo, const WeightAlgo& weight_algo,
		const vector_type& pos_src, const DstT& pos_dst) const{
		ZeroMemory(&pos_node_matrix_, sizeof(pos_node_matrix_));
		auto my_h = GetHeight(), my_w = GetWidth();
		open_.clear();
		open_.PushHeap(pos_src);
		stMyPosNode node_tmp;
		static const vector_type dir_array[] = {
			{ 1, 0 }, { 0, -1 }, { -1, 0 }, { 0, 1 }
		};
		while (open_.nodes_.begin_ != open_.nodes_.end_)
		{
			HEAP_PopHeap(open_.nodes_.begin_, open_.nodes_.end_)
			auto cur_node = *(open_.nodes_.end_ - 1);
			open_.nodes_.pop_back();
			assert(cur_node->x >= 0 && cur_node->x < my_w && cur_node->y >= 0 && cur_node->y < my_h);
			//加入CloseList中
			pos_node_matrix_[cur_node->y][cur_node->x] = &node_tmp;

			for (auto& dir_info : dir_array)
			{
				//////////////////////////////////////////////////////////////////////////
				node_tmp.x = (int)cur_node->x + dir_info.x;
				node_tmp.y = (int)cur_node->y + dir_info.y;
				if (pos_dst == node_tmp)
				{
					auto res = open_.Add(node_tmp);
					res->parent_ = cur_node;
					return res;
				}
				//////////////////////////////////////////////////////////////////////////
				if (node_tmp.x < 0 || node_tmp.x >= my_w || node_tmp.y < 0 || node_tmp.y >= my_h)
					continue;
				auto open_node = pos_node_matrix_[node_tmp.y][node_tmp.x];
				if (open_node == &node_tmp)		//在CloseList中？
					continue;
				if (block_algo.IsBlock(*this, node_tmp))
				{
					//加入到CloseList中，用于优化，减少函数调用
					pos_node_matrix_[node_tmp.y][node_tmp.x] = &node_tmp;
					continue;
				}
				//////////////////////////////////////////////////////////////////////////
				//*******************计算路径增量
				node_tmp.path_delta_ = cur_node->path_delta_;
				auto tmp = cur_node->x - node_tmp.x;
				if (tmp < 0)
					node_tmp.path_delta_ -= tmp;
				else
					node_tmp.path_delta_ += tmp;
				tmp = cur_node->y - node_tmp.y;
				if (tmp < 0)
					node_tmp.path_delta_ -= tmp;
				else
					node_tmp.path_delta_ += tmp;
				assert(node_tmp.path_delta_ > cur_node->path_delta_);
				//权重=路径增量+到终点的估算距离
				node_tmp.weight_ = (float)node_tmp.path_delta_ + (float)pos_dst.Distance(node_tmp);
				node_tmp.weight_ += weight_algo.Weight(node_tmp);
				//////////////////////////////////////////////////////////////////////////
				if (!open_node)
				{
					node_tmp.parent_ = cur_node;
					open_.push_back(node_tmp);
					open_node = open_.end_ - 1;
					open_.nodes_.push_back(open_node);
					//加入到OpenList中
					pos_node_matrix_[node_tmp.y][node_tmp.x] = open_node;
					HEAP_PushHeap(open_.nodes_.begin_, open_.nodes_.end_)
					continue;
				}
				if (open_node->weight_ <= node_tmp.weight_)
					continue;
				assert(open_node->x == node_tmp.x);
				assert(open_node->y == node_tmp.y);
				open_node->path_delta_ = node_tmp.path_delta_;
				open_node->weight_ = node_tmp.weight_;
				open_node->parent_ = cur_node;
				HEAP_MakeHeap(open_.nodes_.begin_, open_.nodes_.end_)
			}
		}
		//assert(!"A星寻路之没生成路径，寻路失败");
		return nullptr;
	}

private:
	mutable stPosNodeCont	open_;
	mutable PosNodeMatrix	pos_node_matrix_;
};
#undef HEAP_LT
//////////////////////////////////////////////////////////////////////////
template<typename AStarT>
class CmnAStarLineMoveToPolicy{
public:
	typedef typename AStarT::vector_type vector_type;

public:
	CmnAStarLineMoveToPolicy(AStarT& astar) : astar_(astar){}
	inline bool CanLineMoveTo_My(const vector_type& pos_src, const vector_type& pos_dst) const{
		return astar_.CanLineMoveTo_My(pos_src, pos_dst);
	}

protected:
	AStarT&		astar_;
};

template<typename AStarT, typename LineMoveToPolicy>
class CmnAStarDanger{
public:
	typedef typename AStarT::vector_type vector_type;
	struct stPosRadius{
		vector_type		position_;
		float			danger_radius_;
	};
	typedef CmnBufferVector<stPosRadius, 1024 * 4> PosRadiusCont;

public:
	CmnAStarDanger(AStarT& astar, const vector_type& pos_src, const LineMoveToPolicy& line_move_to)
		: astar_(astar), line_move_to_(line_move_to){
		dst_danger_radius_ = 0;
		astar.CastToMyPos(pos_src, astar_pos_src_);
	}
	void clear(){
		pos_radius_.clear();
		dst_danger_radius_ = 0;
	}
	void AddPosition(const vector_type& pos, float danger_radius){
		assert(danger_radius >= 0);
		vector_type astar_pos;
		astar_.CastToMyPos(pos, astar_pos);
		for (auto& v : pos_radius_)
		{
			if (v.position_ == astar_pos)
				return;
		}
		astar_.CastToMyDistance(danger_radius, danger_radius);
		pos_radius_.push_back({ astar_pos, danger_radius });
	}
	void SetDstDangerDistance(float dst_danger_radius){
		astar_.CastToMyDistance(dst_danger_radius, dst_danger_radius_);
	}
	inline float Weight(const vector_type& astar_pos) const{
		if (pos_radius_.empty()) return 0;
		int cnt = 0;
		float weight = 0;
		vector_type vector1(astar_pos);
		vector1 -= astar_pos_src_;
		const float kAngleRate = 10;
		for (auto& v : pos_radius_)
		{
			auto distance = v.position_.Distance(astar_pos);
			if (distance > v.danger_radius_)
				continue;
			weight += v.danger_radius_ - distance;
			//////////////////////////////////////////////////////////////////////////
			//**********计算夹角，夹角越大，则权重越小（优先级则越高）。
			vector_type vector2(astar_pos);
			vector2 -= v.position_;
			auto angle = vector1.AbsSinAngle(vector2);
			weight += kAngleRate - angle * kAngleRate;
			//////////////////////////////////////////////////////////////////////////
			++cnt;
		}
		if (cnt > 1)
			weight /= (float)cnt;
		return weight;
	}
	inline bool IsSafe(const vector_type& astar_pos) const{
		for (auto& v : pos_radius_)
		{
			if (v.position_.Distance(astar_pos) <= v.danger_radius_)
				return false;
		}
		return true;
	}
	template<int ContSize>
	inline bool RectifyPath(CmnBufferVector<vector_type, ContSize>& astar_positions) const{
		if (astar_positions.empty())
		{
			assert(false);
			return false;
		}
		auto dst_pos = astar_positions.back();
		while (!astar_positions.empty())
		{
			auto& back_pos = astar_positions.back();
			//这里必须是大于等于，不能只是大于，因为要考虑dst_danger_radius_=0的情况。
			if (back_pos.Distance(dst_pos) >= dst_danger_radius_)
				break;
			if (!line_move_to_.CanLineMoveTo_My(back_pos, dst_pos))
				break;
			astar_positions.pop_back();
		}
		return true;
	}
	template<int ContSize>
	static inline void EnsureAddDstPos(CmnBufferVector<vector_type, ContSize>& game_positions, const vector_type& dst_pos){}

private:
	AStarT&						astar_;
	const LineMoveToPolicy&		line_move_to_;
	PosRadiusCont				pos_radius_;
	vector_type					astar_pos_src_;
	float						dst_danger_radius_;
};

template<typename AStarDanger>
class CmnAStarSafer{
	typedef AStarDanger DangerT;
	typedef typename DangerT::vector_type vector_type;

public:
	CmnAStarSafer(const DangerT& danger) : danger_(danger){}
	static inline int Distance(const vector_type&){ return 0; }
	bool operator == (const vector_type& astar_dst_pos) const{
		return danger_.IsSafe(astar_dst_pos);
	}

private:
	const DangerT&		danger_;
};
//////////////////////////////////////////////////////////////////////////
//*************CPM->common path mark, common path memory, 路径痕迹，路径记忆
template<int RectRange_>
struct CmnPathMarkBase{
	static const int RectRange = RectRange_;

	void SetParam(int range_step){}
};

template<int RectRange_>
struct CmnPathMarkBaseRunTime{
	int RectRange;

	CmnPathMarkBaseRunTime(){
		RectRange = RectRange_;
	}
	void SetParam(int range_step){
		RectRange = range_step;
	}
};

template<int RectRange_, bool IsRuntime>
struct CmnPathMarkBaseT : CmnPathMarkBase<RectRange_>{};

template<int RectRange_>
struct CmnPathMarkBaseT<RectRange_, true> : CmnPathMarkBaseRunTime<RectRange_>{};

template<typename AStarT,
	int MaxSideSize,		//最大边长尺寸
	int RectRange_,			//矩形，从中心点到边的长度。没有缩放的坐标系，即游戏坐标系
	bool IsRuntime>
class CmnPathMark : public CmnPathMarkBaseT<RectRange_, IsRuntime>{
public:
	enum{
		kMaxSideSize = MaxSideSize,
		RectifiedRadius = AStarT::RectifiedRadius,
	};
	typedef typename AStarT::MatrixValueType MatrixValueType;
	typedef CmnMatrixAreaDivision<MatrixValueType, kMaxSideSize> MatrixAreaDivision;
	typedef typename MatrixAreaDivision::MatrixType MatrixType;
	typedef typename AStarT::vector_type vector_type;
	typedef typename vector_type::value_type value_type;
	typedef stCmnRect<vector_type> RectType;
	typedef AStarT AStarType;
	typedef CmnRectifyToFreePosAssist<CmnPathMark> RectifyPosImpl;

public:
	CmnPathMark(const AStarT& astar) : astar_(astar), matrix_assit_(matrix_), matrix_assit_back_up_(matrix_back_up_){
		zoom_rate_ = 0;
		movable_count_total_ = 0;
	}
	CmnPathMark(const CmnPathMark&) = delete;
	CmnPathMark& operator = (const CmnPathMark&) = delete;
	//必须先初始化A星
	bool Init(){
		auto a_width = astar_.GetWidth();
		auto a_height = astar_.GetHeight();
		zoom_rate_ = (float)kMaxSideSize / (float)a_width;
		zoom_rate_ = std::min((float)kMaxSideSize / (float)a_height, zoom_rate_);
		if (zoom_rate_ > 1)
			zoom_rate_ = 1;
		matrix_assit_.assign(int((float)a_width * zoom_rate_),
			int((float)a_height * zoom_rate_));

		int astar_total_len = a_width * a_height;
		vector_type pos_tmp;
		auto& a_matrix_assist = astar_.GetMatrixAssist();
		for (int i = 0; i < astar_total_len; ++i)
		{
			pos_tmp.x = i % a_width;
			pos_tmp.y = i / a_width;
			auto a_v = a_matrix_assist.GetValue(pos_tmp);
			pos_tmp *= zoom_rate_;
			matrix_assit_.SetValue(pos_tmp, a_v);
		}
		matrix_assit_back_up_ = matrix_assit_;
		movable_count_total_ = matrix_assit_back_up_.GetMovablePointCount();
		return true;
	}
	//遍历地图百分比
	float GetCurMovableRate() const{
		if (movable_count_total_ == 0)
		{
			assert(false);
			return 0;
		}
		float movable_count_total = movable_count_total_;
		return (movable_count_total - (float)matrix_assit_.GetMovablePointCount()) / movable_count_total;
	}
	inline void CastToMyPos(const vector_type& pos, vector_type& out_pos) const{
		astar_.CastToMyPos(pos, out_pos);
		out_pos *= zoom_rate_;
		auto my_h = matrix_assit_.Height(), my_w = matrix_assit_.Width();
		if (out_pos.x >= my_w)
			out_pos.x = my_w - 1;
		if (out_pos.y >= my_h)
			out_pos.y = my_h - 1;
	}
	inline void CastToGamePos(const vector_type& pos, vector_type& out_pos) const{
		out_pos = pos;
		out_pos /= zoom_rate_;
		astar_.CastToGamePos(out_pos, out_pos);
	}
	inline void CastToMyDistance(float distance, float& out_distance) const{
		astar_.CastToMyDistance(distance, out_distance);
		out_distance *= (float)zoom_rate_;
	}
	void ClearMarkPos(){
		matrix_assit_ = matrix_assit_back_up_;
	}
	bool MarkPos(const vector_type& pos){
		//对于标记坐标来说，没必要调整成可通行的点的。
		assert(matrix_assit_.Width() > 0 && matrix_assit_.Height() > 0 && "MarkPath小于等于0，是不是没有初始化？");
		RectType the_rect(pos, pos);
		the_rect.left_bottom_ -= RectRange;
		the_rect.right_top_ += RectRange;
		CastToMyPos(the_rect.left_bottom_, the_rect.left_bottom_);
		CastToMyPos(the_rect.right_top_, the_rect.right_top_);

		vector_type pos_tmp;
		for (pos_tmp.y = the_rect.left_bottom_.y; pos_tmp.y <= the_rect.right_top_.y; ++pos_tmp.y)
		{
			for (pos_tmp.x = the_rect.left_bottom_.x; pos_tmp.x <= the_rect.right_top_.x; ++pos_tmp.x)
			{
				matrix_assit_.SetBlockPoint(pos_tmp);
			}
		}
		return true;
	}
	//得到建议坐标
	bool GetSuggestPos(const vector_type& pos_from, vector_type& out_pos) const{
		vector_type the_from, astar_pos;
		if (!RectifyPos(pos_from, nullptr, the_from, astar_pos))
		{
			//assert(false);
			return false;
		}
		//auto from_area_id = matrix_assit_back_up_.GetAreaId(the_from);
		auto from_area_id = astar_.GetMatrixAssist().GetAreaId(astar_pos);
		assert(from_area_id != (MatrixValueType)MatrixAreaDivision::kInvalidAreaId);
		float distance = -1;
		vector_type pos_tmp;
		auto my_h = matrix_assit_.Height(), my_w = matrix_assit_.Width();
		for (pos_tmp.y = 0; pos_tmp.y < my_h; ++pos_tmp.y)
		{
			for (pos_tmp.x = 0; pos_tmp.x < my_w; ++pos_tmp.x)
			{
				if (!TestCanMove(pos_tmp, from_area_id))
					continue;
				auto d = pos_tmp.Distance(the_from);
				if (distance < 0 || d < distance)
				{
					out_pos = pos_tmp;
					distance = d;
				}
			}
		}
		if (distance < 0)
			return false;
		CastToGamePos(out_pos, out_pos);
		return true;
	}
	bool GetSuggestPos(const vector_type& pos_from, int area_id, vector_type& out_pos) const{
		vector_type the_from;
		CastToGamePos(pos_from, the_from);
		assert(area_id >= 0);
		float distance = -1;
		vector_type pos_tmp;
		auto my_h = matrix_assit_.Height(), my_w = matrix_assit_.Width();
		for (pos_tmp.y = 0; pos_tmp.y < my_h; ++pos_tmp.y)
		{
			for (pos_tmp.x = 0; pos_tmp.x < my_w; ++pos_tmp.x)
			{
				if (!TestCanMove(pos_tmp, area_id))
					continue;
				auto d = pos_tmp.Distance(the_from);
				if (distance < 0 || d < distance)
				{
					out_pos = pos_tmp;
					distance = d;
				}
			}
		}
		if (distance < 0)
			return false;
		CastToGamePos(out_pos, out_pos);
		return true;
	}
	//得到最近的坐标
	bool GetNearestPos(bool is_backup, const vector_type& pos_from, vector_type& out_pos) const{
		vector_type the_from, astar_pos;
		if (!RectifyPos(pos_from, nullptr, the_from, astar_pos))
		{
			//assert(false);
			return false;
		}
		//auto from_area_id = matrix_assit_back_up_.GetAreaId(the_from);
		auto from_area_id = astar_.GetMatrixAssist().GetAreaId(astar_pos);
		assert(from_area_id != (MatrixValueType)MatrixAreaDivision::kInvalidAreaId);
		auto the_pos_info = is_backup ? &matrix_assit_back_up_ : &matrix_assit_;
		float distance = -1;
		vector_type pos_tmp;
		auto my_h = matrix_assit_.Height(), my_w = matrix_assit_.Width();
		for (pos_tmp.y = 0; pos_tmp.y < my_h; ++pos_tmp.y)
		{
			for (pos_tmp.x = 0; pos_tmp.x < my_w; ++pos_tmp.x)
			{
				if (pos_tmp == the_from)
					continue;
				if (the_pos_info->IsFree(pos_tmp, from_area_id))
					continue;
				auto d = pos_tmp.Distance(the_from);
				if (distance < 0 || d < distance)
				{
					out_pos = pos_tmp;
					distance = d;
				}
			}
		}
		if (distance < 0)
			return false;
		CastToGamePos(out_pos, out_pos);
		return true;
	}
	void DebugPosInfo(bool dbg_data, const std::string& file_name, const vector_type& mark_pos) const{
		auto mark_pos_tmp = mark_pos;
		CastToMyPos(mark_pos, mark_pos_tmp);

		std::ofstream ofstm(file_name);
		ofstm << std::hex;
		auto my_h = matrix_assit_.Height(), my_w = matrix_assit_.Width();
		vector_type pos_tmp;
		for (pos_tmp.y = 0; pos_tmp.y < my_h; ++pos_tmp.y)
		{
			for (pos_tmp.x = 0; pos_tmp.x < my_w; ++pos_tmp.x)
			{
				if (mark_pos_tmp == pos_tmp)
				{
					ofstm << "A";
					continue;
				}
				if (dbg_data)
				{
					ofstm << (int)matrix_assit_.GetValue(pos_tmp);
				}
				else
				{
					if (matrix_assit_.IsFree(pos_tmp))
						ofstm << "*";
					else
						ofstm << "&";
				}
			}
			ofstm << std::endl;
		}
	}

private:
	inline bool TestCanMove(const vector_type& pos, int area_id) const{
		return matrix_assit_.IsFree(pos, area_id);
	}
	bool RectifyPos(const vector_type& src_pos, const vector_type* liked_pos, vector_type& my_pos, vector_type& astar_pos) const{
		if (!astar_.RectifyPos(src_pos, liked_pos, my_pos)) return false;
		astar_pos = my_pos;
		my_pos *= zoom_rate_;
		return true;
		//return RectifyPosImpl(*this).RectifyToFreePos(matrix_assit_back_up_, src_pos, liked_pos, my_pos);
	}

private:
	MatrixType			matrix_back_up_;
	MatrixAreaDivision	matrix_assit_back_up_;
	MatrixType			matrix_;
	MatrixAreaDivision	matrix_assit_;
	const AStarType&	astar_;
	float				zoom_rate_;
	pt_dword			movable_count_total_;
};
//////////////////////////////////////////////////////////////////////////
//*************最优建议坐标
template<typename VecT, typename LockerT,
	int kDiscardTime,			//超过这个时间的建议坐标会被废弃掉，单位毫秒
	int kCalcTimeMin,			//多久的时间间隔才允许计算最优坐标，单位毫秒
	int kDenseDistance			//密集距离，即多长的距离之内才认为是密集的。游戏的坐标系。
>
class CmnOptimalSuggestPos{
public:
	typedef VecT vector_type;
	typedef typename vector_type::value_type value_type;
	typedef LockerT LockerType;
	struct stPosInfo{
		DateTimeLocal		time_;
		vector_type			pos_;
	};
	typedef CmnBufferVector<stPosInfo, 1024> PosContT;

private:
	struct stPosTimeDiffInfo{
		Poco::Timespan::TimeDiff	time_diff_;
		VecT						pos_;
		int							dense_deep_;
	};
	typedef CmnBufferVector<stPosTimeDiffInfo, 1024> PosTimeDiffContT;

public:
	CmnOptimalSuggestPos(){}
	CmnOptimalSuggestPos(const CmnOptimalSuggestPos&) = delete;
	CmnOptimalSuggestPos& operator = (const CmnOptimalSuggestPos&) = delete;
	void AddSuggestPos(const vector_type& pos){
		LockerType locker;
		stPosInfo tmp;
		tmp.pos_ = pos;
		pos_cont_.push_back(tmp);
	}
	bool GetOptimalPos(vector_type& out_pos){
		if (pos_cont_.empty())
			return false;
		DateTimeLocal time_now;
		if ((time_now - last_calc_time_).totalMilliseconds() < kCalcTimeMin)
			return false;
		PosTimeDiffContT pos_diff_cont_tmp;
		{
			LockerType locker;
			PosContT pos_cont_tmp;
			for (auto& v : pos_cont_)
			{
				auto time_diff = (time_now - v.time_).totalMilliseconds();
				if (time_diff <= kDiscardTime)
				{
					pos_diff_cont_tmp.push_back({ time_diff, v.pos_, 0 });
					pos_cont_tmp.push_back(v);
				}
			}
			std::swap(pos_cont_tmp, pos_cont_);
		}
		if (pos_diff_cont_tmp.empty())
			return false;
		auto it_begin = pos_diff_cont_tmp.begin(), it_end = pos_diff_cont_tmp.end();
		for (auto it = it_begin; it != it_end; ++it)
		{
			auto& the_value = *it;
			std::for_each(it_begin, it_end, [&the_value](stPosTimeDiffInfo& v){
				if (the_value.pos_.Distance(v.pos_) <= kDenseDistance)
					++the_value.dense_deep_;
			});
		}
		std::sort(it_begin, it_end, [](const stPosTimeDiffInfo& lhs, const stPosTimeDiffInfo& rhs){
			return lhs.dense_deep_ > rhs.dense_deep_;
		});
		auto it_last = it_begin;
		for (auto it = it_begin + 1; it < it_end; ++it)
		{
			if (it->dense_deep_ < it_last->dense_deep_)
				break;
			assert(it->dense_deep_ == it_last->dense_deep_);
			if (it->time_diff_ < it_last->time_diff_)
				it_last = it;
		}
		last_calc_time_ = DateTimeLocal();
		out_pos = it_last->pos_;
		{
			LockerType locker;
			for (auto it = pos_cont_.begin(), it_end = pos_cont_.end(); it != pos_cont_.end(); ++it)
			{
				if (it->pos_ == out_pos)
				{
					pos_cont_.erase(it);
					break;
				}
			}
		}
		return true;
	}

private:
	PosContT		pos_cont_;
	DateTimeLocal	last_calc_time_;
};
//////////////////////////////////////////////////////////////////////////
