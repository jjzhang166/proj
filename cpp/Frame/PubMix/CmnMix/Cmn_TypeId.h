#pragma once
/*
@author zhp
@date 2016/4/17 21:18
@purpose fast typeid
*/
#include <typeinfo>

class NullType{};
class TypeId{
public:
	TypeId();
	TypeId( const std::type_info& id );
	bool operator!=( const TypeId& other ) const;
	bool operator==( const TypeId& other ) const;
	bool operator<( const TypeId& other ) const;
	const char* name() const;

private:
	const std::type_info* id_;
};
static_assert(sizeof(TypeId) == sizeof(void*), "");
