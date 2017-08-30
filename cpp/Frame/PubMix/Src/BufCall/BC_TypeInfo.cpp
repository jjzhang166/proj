#include "stdafx.h"
#include <BufCall/BC_TypeInfo.h>

TypeId BC_NullTypeInfo::GetTypeId() const
{
	return TypeId();
}
