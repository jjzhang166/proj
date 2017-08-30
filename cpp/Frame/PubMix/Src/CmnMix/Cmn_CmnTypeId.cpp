#include "stdafx.h"
#include <CmnMix/Cmn_TypeId.h>

const std::type_info* g_null_type_id = &typeid(NullType);
TypeId::TypeId() : id_(g_null_type_id){}

TypeId::TypeId( const std::type_info& id ) : id_(&id)
{
}

bool TypeId::operator!=( const TypeId& other ) const
{
	return id_ != other.id_;
}

bool TypeId::operator==( const TypeId& other ) const
{
	return id_ == other.id_;
}

bool TypeId::operator<( const TypeId& other ) const
{
	return id_ < other.id_;
	//return id_->before(*other.id_);
}

const char* TypeId::name() const
{
	return id_->name();
}