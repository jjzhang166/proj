#pragma once
/*
@author zhp
@date 2017/2/8 16:31
@purpose heap algorithm
*/

//必须预先定义该宏
//#define HEAP_LT(lhs, rhs) lhs < rhs


#define _HEAP_PushHeap	\
{	\
	for (auto _Idx = (_Heap_Hole - 1) / 2;	\
		_HEAP_Top < _Heap_Hole && HEAP_LT((*(_Heap_First_ + _Idx)), _Heap_Value);	\
		_Idx = (_Heap_Hole - 1) / 2)	\
	{	\
		*(_Heap_First_ + _Heap_Hole) = *(_Heap_First_ + _Idx);	\
		_Heap_Hole = _Idx;	\
	}	\
	*(_Heap_First_ + _Heap_Hole) = _Heap_Value;	\
}
/**/

#define HEAP_Adjust_heap	\
{	\
	auto _HEAP_Top = _Heap_Hole;	\
	auto _HEAP_Ajust_Idx = 2 * _Heap_Hole + 2;	\
	for (; _HEAP_Ajust_Idx < _Heap_Ajust_Bottom; _HEAP_Ajust_Idx = 2 * _HEAP_Ajust_Idx + 2)	\
	{	\
		auto _HEAP_lhs = *(_Heap_First_ + _HEAP_Ajust_Idx);	\
		auto _HEAP_rhs = *(_Heap_First_ + (_HEAP_Ajust_Idx - 1));	\
		if (HEAP_LT(_HEAP_lhs, _HEAP_rhs))	\
			--_HEAP_Ajust_Idx;	\
		*(_Heap_First_ + _Heap_Hole) = *(_Heap_First_ + _HEAP_Ajust_Idx);	\
		_Heap_Hole = _HEAP_Ajust_Idx;	\
	}	\
	if (_HEAP_Ajust_Idx == _Heap_Ajust_Bottom)	\
	{	\
		*(_Heap_First_ + _Heap_Hole) = *(_Heap_First_ + (_Heap_Ajust_Bottom - 1));	\
		_Heap_Hole = _Heap_Ajust_Bottom - 1;	\
	}	\
	_HEAP_PushHeap	\
}
/**/

#define _HEAP_PopHeap	\
{	\
	*_Heap_Dest_Ptr = *_Heap_First_;	\
	auto _Heap_Hole = 0;	\
	auto _Heap_Ajust_Bottom = _Heap_Last_ - _Heap_First_;	\
	HEAP_Adjust_heap	\
}
/**/

#define HEAP_PopHeap_0_	\
{	\
	auto _Heap_Dest_Ptr = _Heap_Last_ - 1;	\
	_Heap_Last_ = _Heap_Dest_Ptr;	\
	auto _Heap_Value = *_Heap_Dest_Ptr;	\
	_HEAP_PopHeap	\
}
/**/

#define HEAP_PopHeap(_First, _Last)	\
{	\
	auto _Heap_First_ = _First;	\
	auto _Heap_Last_ = _Last;	\
	HEAP_PopHeap_0_	\
}
/**/

#define _HEAP_PushHeap_0	\
{	\
	auto _HEAP_count = _Heap_Last_ - _Heap_First_;	\
	if (0 < _HEAP_count)	\
	{	\
		auto _Heap_Value = *_Heap_Last_;	\
		auto _Heap_Hole = _HEAP_count;	\
		auto _HEAP_Top = 0;	\
		_HEAP_PushHeap	\
	}	\
}
/**/

#define HEAP_PushHeap(_First, _Last)	\
{	\
	auto _Heap_First_ = _First;	\
	auto _Heap_Last_ = _Last;	\
	if (_Heap_First_ != _Heap_Last_)	\
	{	\
		--_Heap_Last_;	\
		_HEAP_PushHeap_0	\
	}	\
}
/**/

#define _HEAP_MakeHeap	\
{	\
	auto _Heap_Hole_tmp = _Heap_Ajust_Bottom / 2;	\
	while (0 < _Heap_Hole_tmp)	\
	{	\
		--_Heap_Hole_tmp;	\
		auto _Heap_Value = *(_Heap_First_ + _Heap_Hole_tmp);	\
		auto _Heap_Hole = _Heap_Hole_tmp;	\
		HEAP_Adjust_heap	\
	}	\
}
/**/

#define HEAP_MakeHeap(_First, _Last)	\
{	\
	auto _Heap_First_ = _First;	\
	auto _Heap_Last_ = _Last;	\
	auto _Heap_Ajust_Bottom = _Heap_Last_ - _Heap_First_;	\
	if (1 < _Heap_Ajust_Bottom)	\
		_HEAP_MakeHeap	\
}
/**/