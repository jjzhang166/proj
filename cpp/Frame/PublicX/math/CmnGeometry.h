#pragma once
/*
@author zhp
@date 2017/3/13 22:51
@purpose for geometry
*/
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/ring.hpp>  
#include <boost/geometry/geometries/polygon.hpp>  

namespace Geometry{
	template<typename T>
	using point_xy = boost::geometry::model::d2::point_xy<T>;
	template<typename PointT>
	using segment = boost::geometry::model::segment<PointT>;
	template<typename PointT, 
		template<typename, typename> class Container = std::vector,
		template<typename> class Allocator = std::allocator>
	using linestring = boost::geometry::model::linestring<PointT, Container, Allocator>;
	template<typename PointT>
	using box = boost::geometry::model::box<PointT>;
	template<typename PointT,
		bool ClockWise = true, bool Closed = true,
		template<typename, typename> class Container = std::vector,
		template<typename> class Allocator = std::allocator>
	using ring = boost::geometry::model::ring<PointT, ClockWise, Closed, Container, Allocator>;
	template<typename PointT,
		bool ClockWise = true,
		bool Closed = true,
		template<typename, typename> class PointList = std::vector,
		template<typename, typename> class RingList = std::vector,
		template<typename> class PointAlloc = std::allocator,
		template<typename> class RingAlloc = std::allocator>
	using polygon = boost::geometry::model::polygon<PointT, ClockWise, Closed, PointList, RingList, PointAlloc, RingAlloc>;
	//////////////////////////////////////////////////////////////////////////
	//是否存在交点
	using boost::geometry::intersects;
	//得到交点
	using boost::geometry::intersection;
	//////////////////////////////////////////////////////////////////////////
}