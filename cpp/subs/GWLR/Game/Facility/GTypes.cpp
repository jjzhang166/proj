#include "stdafx.h"
#include "GTypes.h"

using namespace GType;

double GType::stGameVector3::Distance3d( const stGameVector3& rhs ) const
{
	return std::sqrt(std::pow<double>(fx - rhs.fx, 2) + std::pow<double>(fy - rhs.fy, 2) + std::pow<double>(fz - rhs.fz, 2));
}

double GType::stGameVector3::Distance2d( const stGameVector3& rhs ) const
{
	return std::sqrt(std::pow<double>(fx - rhs.fx, 2) + std::pow<double>(fy - rhs.fy, 2));
}

float GType::stGameVector3::GetAngleXY() const
{
	return std::atan2(fy, fx);
}

void GType::stGameVector3::RotateXY_Origin( float angle )
{
	fx = fx * std::cos(angle) - fy * std::sin(angle);
	fy = fx * std::sin(angle) + fy * std::cos(angle);
}

void GType::stGameVector3::PlusXY( const stGameVector3& rhs )
{
	fx += rhs.fx;
	fy += rhs.fy;
}

void GType::stGameVector3::ConvertToPos(const stCD_PosDirInfo& actor_vec)
{
	fx = actor_vec.fx;
	fy = actor_vec.fy;
	fz = actor_vec.fz;
}

void GType::stGameVector3::SetXYZ( float x, float y, float z )
{
	fx = x;
	fy = y;
	fz = z;
}

bool GType::stGameVector3::OriginDistLessThan( const stGameVector3& rhs ) const
{
	return Distance3d(kVecEmpty) < rhs.Distance3d(kVecEmpty);
}

bool GType::stGameVector3::ValueEqual( const stGameVector3& rhs ) const
{
	return fx == rhs.fx && fy == rhs.fy && fz == rhs.fz;
}

bool GType::stGameVector3::FloatAllEqual( const stGameVector3& rhs ) const
{
	return FloatEqual(fx, rhs.fx) && FloatEqual(fy, rhs.fy) && FloatEqual(fz, rhs.fz);
}

float GType::stGameVector3::CrossProductXY( const stGameVector3& rhs ) const
{
	return fx * rhs.fx + fy * rhs.fy;
}

float GType::stGameVector3::ModuleXY() const
{
	return sqrt(fx * fx + fy * fy);
}

float GType::stGameVector3::IncludedAngleXY( const stGameVector3& rhs ) const
{
	return acos(CrossProductXY(rhs) / (ModuleXY() * rhs.ModuleXY()));
}

bool GType::stGameVector3::IsXYAllEqual( const stGameVector3& rhs ) const
{
	return FloatEqual(fx, rhs.fx) && FloatEqual(fy, rhs.fy);
}

double GType::stGameVector3::Distance3d_ZoomInZ( const stGameVector3& rhs, int zoom_in_z_rate ) const
{
	return std::sqrt(std::pow<double>(fx - rhs.fx, 2) + std::pow<double>(fy - rhs.fy, 2) + std::pow<double>((fz - rhs.fz) * zoom_in_z_rate, 2));
}
