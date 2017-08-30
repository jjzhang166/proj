#pragma once
/*
@author zhp
@date 2017/2/19 14:25
@purpose for math
*/
#include "../DependSelfLib.h"
#include <iostream>
#include <ProtocolX/P_DefinesX.h>
#include <math/CmnGeometry.h>
#include <CommonX/CmnX_UsefulFunc.h>
#include <math/GraphForm.h>

struct stCmnVecBase{
	CMN_CAST_TO
};

template<typename valueT_>
struct stCmnVec2 : stCmnVecBase{
	typedef valueT_ value_type;
	typedef Geometry::point_xy<valueT_> point_type;

	value_type		x;
	value_type		y;

	inline stCmnVec2(){
		x = 0;
		y = 0;
	}
	inline stCmnVec2(value_type the_x, value_type the_y) : x(the_x), y(the_y){}
	void assign(value_type the_x, value_type the_y){
		x = the_x;
		y = the_y;
	}
	void assign(const stCmnVec2& rhs){
		x = rhs.x;
		y = rhs.y;
	}
	operator point_type() const{ return point_type(x, y); }
	inline bool operator == (const stCmnVec2& rhs) const{
		return x == rhs.x && y == rhs.y;
	}
	inline bool operator != (const stCmnVec2& rhs) const{
		return !(*this == rhs);
	}
	inline stCmnVec2& operator += (const stCmnVec2& rhs){
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	inline stCmnVec2& operator += (value_type rhs){
		x += rhs;
		y += rhs;
		return *this;
	}
	inline stCmnVec2 operator + (const stCmnVec2& rhs) const{
		stCmnVec2 res(*this);
		res += rhs;
		return res;
	}
	inline stCmnVec2 operator + (value_type rhs) const{
		stCmnVec2 res(*this);
		res += rhs;
		return res;
	}
	inline stCmnVec2& operator -= (const stCmnVec2& rhs){
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	inline stCmnVec2 operator - (const stCmnVec2& rhs) const{
		stCmnVec2 res(*this);
		res -= rhs;
		return res;
	}
	inline stCmnVec2& operator -= (value_type rhs){
		x -= rhs;
		y -= rhs;
		return *this;
	}
	inline stCmnVec2 operator - (value_type rhs) const{
		stCmnVec2 res(*this);
		res -= rhs;
		return res;
	}
	inline stCmnVec2& operator *= (int rhs){
		x *= rhs;
		y *= rhs;
		return *this;
	}
	inline stCmnVec2 operator * (int rhs) const{
		stCmnVec2 res(*this);
		res *= rhs;
		return res;
	}
	inline stCmnVec2& operator *= (float rhs){
		x = value_type(float(x) * rhs);
		y = value_type(float(y) * rhs);
		return *this;
	}
	inline stCmnVec2 operator * (float rhs) const{
		stCmnVec2 res(*this);
		res *= rhs;
		return res;
	}
	inline stCmnVec2& operator /= (int rhs){
		x /= rhs;
		y /= rhs;
		return *this;
	}
	inline stCmnVec2 operator / (int rhs) const{
		stCmnVec2 res(*this);
		res /= rhs;
		return res;
	}
	inline stCmnVec2& operator /= (float rhs){
		x = value_type(float(x) / rhs);
		y = value_type(float(y) / rhs);
		return *this;
	}
	inline stCmnVec2 operator / (float rhs) const{
		stCmnVec2 res(*this);
		res /= rhs;
		return res;
	}
	void RectifyAtRange(const stCmnVec2& min, const stCmnVec2& max){
		if (x < min.x) x = min.x;
		if (x > max.x) x = max.x;
		if (y < min.y) y = min.y;
		if (y > max.y) y = max.y;
	}
	inline bool AllInRange(value_type v_begin, value_type v_end) const{
		return x >= v_begin && x < v_end && y >= v_begin && y < v_end;
	}
	inline bool AllLessZero() const { return x < 0 && y < 0; }
	inline bool AllGreaterZero() const{ return x > 0 && y > 0; }
	inline bool AllLess(value_type v) const{ return x < v && y < v; }
	inline bool AnyLess(value_type v) const{ return x < v || y < v; }
	inline bool AnyLess(const stCmnVec2& v) const{ return x < v.x || y < v.y; }
	inline bool AllGreater(value_type v) const{ return x > v && y > v; }
	inline bool AnyGreater(value_type v) const{ return x > v || y > v; }
	inline bool AnyGreater(const stCmnVec2& v) const{ return x > v.x || y > v.y; }
	inline value_type CalcIndex(value_type const_value) const{
		return y * const_value + x;
	}
	//模
	inline float Module() const{
		return (float)sqrt(x * x + y * y);
	}
	//法向量
	inline stCmnVec2 NormalVector() const{
		return stCmnVec2(y, -x);
	}
	//向量的积，点积(dot)，向量的内积、数量积
	inline value_type DotProduct(const stCmnVec2& rhs) const{
		return x * rhs.x + y * rhs.y;
	}
	//向量积、向量叉积、叉乘
	inline value_type CrossProduct(const stCmnVec2& rhs) const{
		return x * rhs.y - y * rhs.x;		//减的顺序一定不要轻易改变，否则先要去看下对这个函数的所有的引用，看是否能够影响其算法。
	}
	//余弦夹角，值域[-1, 1]。
	inline float CosAngle(const stCmnVec2& rhs) const{
		return (float)DotProduct(rhs) / (Module() * rhs.Module());
	}
	//正弦夹角，值域[0,1]，数值越大说明角度越大。
	inline float AbsSinAngle(const stCmnVec2& rhs) const{
		auto cos = CosAngle(rhs);
		return (float)sqrt((float)1 - cos * cos);
	}
	//得到的是向量夹角度数，即PI(pai,3.14)之类的数值。
	inline float AngleDegree(const stCmnVec2& rhs) const{
		return acos(CosAngle(rhs));
	}
	inline float Distance(const stCmnVec2& rhs) const{
		double x_diff = x - rhs.x;
		double y_diff = y - rhs.y;
		return (float)std::sqrt(x_diff * x_diff + y_diff * y_diff);
	}
	inline float Distance(value_type the_x, value_type the_y) const{
		double x_diff = x - the_x;
		double y_diff = y - the_y;
		return (float)std::sqrt(x_diff * x_diff + y_diff * y_diff);
	}
	inline float Distance(const stCmnVec2& line_pos1, const stCmnVec2& line_pos2) const{
		return (float)MakeLineGeneralForm(line_pos1, line_pos2).Distance(*this);
	}
	stCmnVec2 CenterFromLeftTop(value_type width, value_type height) const{
		return stCmnVec2(x + width / 2, y - height / 2);
	}
	//两点之间
	inline bool IsInPoints(const stCmnVec2& pos1, const stCmnVec2& pos2) const{
		if ((x > pos1.x && x > pos2.x) || (x < pos1.x && x < pos2.x))
			return false;
		if ((y > pos1.y && y > pos2.y) || (y < pos1.y && y < pos2.y))
			return false;
		return true;
	}
	//是否在矩形内
	inline bool IsInBox(value_type half_width, value_type half_height, const stCmnVec2& vec) const{
		auto tmp = vec - *this;
		return abs(tmp.x) <= half_width && abs(tmp.y) <= half_height;
	}
	//是否在直线上
	inline bool IsInLine(const stCmnVec2& pos1, const stCmnVec2& pos2) const{
		auto vector1(*this);
		vector1 -= pos1;
		auto vector2 = pos1 - pos2;
		return vector1.CrossProduct(vector2) == 0;
	}
	inline bool IsInLine(const stCmnVec2& pos1, const stCmnVec2& pos2, value_type line_wide) const{
		stCmnVec2 pos_tmp;
		if (!GetLineFootPoint(pos1, pos2, pos_tmp))
			return false;
		return Distance(pos_tmp) <= line_wide;
	}
	//点是否在线段上
	inline bool IsInSegment(const stCmnVec2& pos1, const stCmnVec2& pos2) const{
		if (!IsInLine(pos1, pos2))
			return false;
		return IsInPoints(pos1, pos2);
	}
	//圆与线段是否存交集
	template<typename RadiusT>
	inline bool CircleSegmentIntersects(RadiusT radius, const stCmnVec2& seg_pos1, const stCmnVec2& seg_pos2) const{
		return MakeCircleStdForm(*this, radius).SegmentIntersects(MakeLineGeneralForm(seg_pos1, seg_pos2));
	}
	//圆与点是否存交集
	template<typename RadiusT>
	inline bool CirclePointIntersects(RadiusT radius, const stCmnVec2& point) const{
		return MakeCircleStdForm(*this, radius).GetPointPos(point) <= 0;
	}
	//矩形与线段的是否存在交集
	inline bool BoxSegmentIntersects(value_type half_width, value_type half_height, 
		const stCmnVec2& pos1, const stCmnVec2& pos2) const{
		Geometry::box<point_type> box(point_type(x - half_width, y - half_height),
			point_type(x + half_width, y + half_height));
		Geometry::segment<point_type> seg(pos1, pos2);
		return Geometry::intersects(box, seg);
	}
	//矩形到中心点的线段的交点的边
	inline bool BoxCenterSegmentCrossPoint(value_type half_width, value_type half_height, const stCmnVec2& pos, 
		stCmnVec2& out_seg_pos1, stCmnVec2& out_seg_pos2) const{
		stCmnVec2 pos1(x - half_width, y + half_height);		//左上
		stCmnVec2 pos2(x + half_width, y + half_height);		//右上
		stCmnVec2 out_point;
		if (SegmentCrossPoint(*this, pos, pos1, pos2, out_point))
		{
			out_seg_pos1 = pos1;
			out_seg_pos2 = pos2;
			return true;
		}
		stCmnVec2 pos3(x - half_width, y - half_height);		//左下
		if (SegmentCrossPoint(*this, pos, pos1, pos3, out_point))
		{
			out_seg_pos1 = pos1;
			out_seg_pos2 = pos3;
			return true;
		}
		stCmnVec2 pos4(x + half_width, y - half_height);		//右下
		if (SegmentCrossPoint(*this, pos, pos3, pos4, out_point))
		{
			out_seg_pos1 = pos3;
			out_seg_pos2 = pos4;
			return true;
		}
		if (SegmentCrossPoint(*this, pos, pos2, pos4, out_point))
		{
			out_seg_pos1 = pos2;
			out_seg_pos2 = pos4;
			return true;
		}
		return false;
	}
	//求点与直线的垂足
	inline bool GetLineFootPoint(const stCmnVec2& line_pos1, const stCmnVec2& line_pos2, stCmnVec2& foot_point) const{
		return MakeLineGeneralForm(line_pos1, line_pos2).GetFootPoint(*this, foot_point);
	}
	//求点与线段的垂足
	inline bool GetSegmentFootPoint(const stCmnVec2& seg_pos1, const stCmnVec2& seg_pos2, stCmnVec2& foot_point) const{
		if (!GetLineFootPoint(seg_pos1, seg_pos2, foot_point))
			return false;
		return foot_point.IsInPoints(seg_pos1, seg_pos2);
	}
	//求关于点的对称点
	inline bool GetPointSymmetryPoint(const stCmnVec2& middle_pos, stCmnVec2& out_point) const{
		out_point.x = middle_pos.x * 2 - x;
		out_point.y = middle_pos.y * 2 - y;
		return true;
	}
	//求关于直线的对称点
	inline bool GetLineSymmetryPoint(const stCmnVec2& line_pos1, const stCmnVec2& line_pos2, stCmnVec2& out_point) const{
		if (!GetLineFootPoint(line_pos1, line_pos2, out_point))
			return false;
		return GetPointSymmetryPoint(out_point, out_point);
	}
	//求关于线段中心对称点
	inline bool GetCenterSegmentSymmetryPoint(const stCmnVec2& line_pos1, const stCmnVec2& line_pos2, stCmnVec2& out_point) const{
		out_point = (line_pos1 + line_pos2) / 2;
		return GetPointSymmetryPoint(out_point, out_point);
	}
	inline bool GetCenterSegmentSymmetryPoint(const stCmnVec2& line_pos1, const stCmnVec2& line_pos2, value_type distance_x, stCmnVec2& out_point) const{
		out_point = (line_pos1 + line_pos2) / 2;
		if (!GetPointSymmetryPoint(out_point, out_point))
			return false;
		out_point.x += distance_x;
		return true;
	}
	//得到相邻矩形中心坐标
	inline bool GetAdjacentBoxCenter(value_type half_width, value_type half_height, const stCmnVec2& pos, stCmnVec2& out_center) const{
		stCmnVec2 line_pos1, line_pos2;
		if (!BoxCenterSegmentCrossPoint(half_width, half_height, pos, line_pos1, line_pos2))
		{
			assert(false);
			return false;
		}
		GetCenterSegmentSymmetryPoint(line_pos1, line_pos2, 1, out_center);
		return true;
	}
	//计算和原点的角度
	//atan2原点至点(x,y)的方位角
	inline float Atan2() const{
		return (float)atan2(y, x);
	}
	friend std::ostream&
		operator << (std::ostream& ostm, const stCmnVec2& vec){
		ostm << "（" << vec.x << ", " << vec.y << "）";
		return ostm;
	}
	template<class Archive>
	void serialize(Archive & ar){
		ar & x & y;
	}
	//////////////////////////////////////////////////////////////////////////
	//直线交点
	static inline bool LineCrossPoint(const stCmnVec2& line1_pos1, const stCmnVec2& line1_pos2,
		const stCmnVec2& line2_pos1, const stCmnVec2& line2_pos2, stCmnVec2& out_res){
		return MakeLineGeneralForm(line1_pos1, line1_pos2).GetCrossPoint(MakeLineGeneralForm(line2_pos1, line2_pos2), out_res);
	}
	//线段交点
	static inline bool SegmentCrossPoint(const stCmnVec2& seg1_pos1, const stCmnVec2& seg1_pos2,
		const stCmnVec2& seg2_pos1, const stCmnVec2& seg2_pos2, stCmnVec2& cross_point){
		if (!LineCrossPoint(seg1_pos1, seg1_pos2, seg2_pos1, seg2_pos2, cross_point))
			return false;
		if (!cross_point.IsInPoints(seg1_pos1, seg1_pos2))
			return false;
		if (!cross_point.IsInPoints(seg2_pos1, seg2_pos2))
			return false;
		return true;
	}
	//////////////////////////////////////////////////////////////////////////
};
typedef stCmnVec2<int> stVec2Int;
static_assert(sizeof(stVec2Int) == sizeof(int) * 2, "");

template<typename T>
struct stCmnVec3 : stCmnVec2<T>{
	value_type	z;

public:
	inline stCmnVec3(){
		z = 0;
	}
	inline stCmnVec3(value_type the_x, value_type the_y, value_type the_z) : stCmnVec2(the_x, the_y), z(the_z){}
	inline bool operator == (const stCmnVec3& rhs) const{
		return FloatEqual(x, rhs.x) && FloatEqual(y, rhs.y) && FloatEqual(z, rhs.z);
	}
	friend std::ostream&
		operator << (std::ostream& ostm, const stCmnVec3& vec){
		ostm << "（" << vec.x << ", " << vec.y << ", " << vec.z << "）";
		return ostm;
	}
};
typedef stCmnVec3<int> stVec3Int;
typedef stCmnVec3<float> stVec3Float;

template<typename ValueT>
struct stCmnSize : stCmnVec2<ValueT>{
	stCmnSize() = default;
	stCmnSize(value_type width, value_type height) : stCmnVec2(width, height){}
	stCmnSize(const stCmnVec2& rhs) : stCmnVec2(rhs){}
	inline value_type Width() const{ return x; }
	inline value_type Height() const{ return y; }
	inline value_type GetWidth() const{ return x; }
	inline value_type GetHeight() const{ return y; }
	inline void SetWidth(value_type width){ x = width; }
	inline void SetHeight(value_type height){ y = height; }
	inline value_type Size() const{ return x * y; }
	inline void SetSize(value_type width, value_type height){
		x = width;
		y = height;
	}
	inline void PlusWidth(value_type addition){ x += addition; }
	inline void PlusHeight(value_type addition){ y += addition; }
	inline bool InSize(value_type the_x, value_type the_y) const{
		if (the_x < 0 || the_y < 0) return false;
		return the_x < x && the_y < y;
	}
	inline bool operator < (const stCmnSize& rhs) const{
		return x < rhs.x && y < rhs.y;
	}
	inline bool operator <= (const stCmnSize& rhs) const{
		return x <= rhs.x && y <= rhs.y;
	}
	inline bool operator > (const stCmnSize& rhs) const{
		return x > rhs.x && y > rhs.y;
	}
	inline bool operator >= (const stCmnSize& rhs) const{
		return x >= rhs.x && y >= rhs.y;
	}
};
typedef stCmnSize<int> stSizeInt;

template<typename VecT>
struct stCmnRect{
	typedef VecT point_type;
	typedef typename point_type::value_type value_type;
	typedef stCmnSize<value_type> size_type;

	inline stCmnRect() = default;
	inline stCmnRect(const point_type& left_bottom, const point_type& right_top) : left_bottom_(left_bottom), right_top_(right_top){}
	template<typename T>
	inline stCmnRect& operator*= (T v){
		left_bottom_ *= v;
		right_top_ *= v;
		return *this;
	}
	inline bool operator == (const stCmnRect& rhs) const{
		return left_bottom_ == rhs.left_bottom_ && right_top_ == rhs.right_top_;
	}
	inline bool operator != (const stCmnRect& rhs) const{
		return !(*this == rhs);
	}
	inline bool IsIn(const point_type& point) const{
		return point.IsInPoints(left_bottom_, right_top_);
	}
	inline size_type Size() const{
		return right_top_ - left_bottom_;
	}
	void SetRect(const point_type& left_bottom, const point_type& right_top){
		left_bottom_ = left_bottom;
		right_top_ = right_top;
	}
	void RectifyAtRange(const point_type& min, const point_type& max){
		left_bottom_.RectifyAtRange(min, max);
		right_top_.RectifyAtRange(min, max);
	}

	point_type		left_bottom_;
	point_type		right_top_;
};
typedef stCmnRect<stCmnVec2<int> > stRectInt;
typedef stCmnRect<stCmnVec2<float> > stRectFloat;

template<typename VecT>
struct stCmnLine{
	typedef VecT point_type;
	typedef typename point_type::value_type value_type;

public:
	stCmnLine() = default;
	stCmnLine(const point_type& pos1, const point_type& pos2) : pos1_(pos1), pos2_(pos2){}
	
private:
	point_type		pos1_;
	point_type		pos2_;
};
