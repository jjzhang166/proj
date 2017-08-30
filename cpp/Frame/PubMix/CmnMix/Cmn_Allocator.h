#pragma once
/*
@author zhp
@date 2016/2/11 11:17
@purpose for allocate memory
*/
#include <CommonX/CmnX_Allocator.h>
#include <CommonX/CmnX_UsefulClass.h>
#include <CommonX/CmnX_Lock.h>
#include "../DependSelfLib.h"

class CmnAllocateBufferMgr{
public:
	typedef unsigned short size_type;
	typedef short difference_type;

#pragma pack(push)
#pragma pack(1)
	struct stPageInfo{
		size_type		page_size_;
		bool			is_used_;
	};
#pragma pack(pop)
	static_assert(sizeof(stPageInfo) == 3, "");

public:
	virtual ~CmnAllocateBufferMgr();
	bool Reset();
	virtual bool Reset(void* buffer_begin, void* buffer_end, bool force_reset = true);
	virtual void* Malloc(size_type size);
	virtual bool Free(void* addr);

private:
	CmnX_CodeInfo FindNextFirstFree(const void* begin_addr, const void* end_addr, size_type size) const;
	CmnX_CodeInfo FindPreFirstFree(const void* the_addr) const;
	CmnX_CodeInfo FindFirstUsedFrom(const void* begin_addr) const;

private:
	CmnX_CodeInfo		buffer_info_;
};

template<typename LockT_>
class CmnAllocateBufferMgrT : public CmnAllocateBufferMgr, public LockT_{
public:
	typedef LockT_ LockT;

public:
	bool Reset(){
		return __super::Reset();
	}
	bool Reset(void* buffer_begin, void* buffer_end, bool force_reset = true){
		Cmnx_Lockable lockable(*this);
		return __super::Reset(buffer_begin, buffer_end, force_reset);
	}
	void* Malloc(size_type size){
		Cmnx_Lockable lockable(*this);
		return __super::Malloc(size);
	}
	bool Free(void* addr){
		Cmnx_Lockable lockable(*this);
		return __super::Free(addr);
	}
};

template<typename TagT_, typename LockT_>
class CmnAllcator_BufferBase{
public:
	typedef TagT_ TagT;
	typedef CmnAllocateBufferMgrT<LockT_> BuffMgrT;
	typedef typename BuffMgrT::LockT LockT;
	typedef typename BuffMgrT::size_type size_type;
	typedef typename BuffMgrT::difference_type difference_type;

public:
	static bool ResetMemoryBuffer(){
		return buffer_mgr_.Reset();
	}
	static bool ResetMemoryBuffer(void* buffer_begin, void* buffer_end, bool force_reset = true){
		return buffer_mgr_.Reset(buffer_begin, buffer_end, force_reset);
	}
	static BuffMgrT& GetBufferMgr(){
		return buffer_mgr_;
	}

protected:
	static BuffMgrT		buffer_mgr_;
};

template<typename _Ty, typename TagT_, typename LockT_>
class CmnAllcator_Buffer : public CmnAllcator_BufferBase<TagT_, LockT_>{
public:
	typedef _Ty value_type;
	typedef CmnAllcator_BufferBase<TagT, LockT> BaseT;

	typedef CmnAllcator_Buffer<value_type, TagT, LockT_> other;
	template<class _Other, typename OtherTag_, typename OtherLockT_>
	struct rebind{
		typedef CmnAllcator_Buffer<_Other, OtherTag_, OtherLockT_> other;
	};

	typedef value_type *pointer;
	typedef const value_type *const_pointer;
	typedef void *void_pointer;
	typedef const void *const_void_pointer;

	typedef value_type& reference;
	typedef const value_type& const_reference;

	pointer address(reference _Val) const
	{	// return address of mutable _Val
		return (std::addressof(_Val));
	}

	const_pointer address(const_reference _Val) const
	{	// return address of nonmutable _Val
		return (std::addressof(_Val));
	}

	CmnAllcator_Buffer()
	{	// construct default allocator (do nothing)
	}

	CmnAllcator_Buffer(const other&)
	{	// construct by copying (do nothing)
	}

	template<class _Other, typename OtherTag_, typename OtherLockT_>
	CmnAllcator_Buffer(const CmnAllcator_Buffer<_Other, OtherTag_, OtherLockT_>&)
	{	// construct from a related allocator (do nothing)
	}

	template<class _Other, typename OtherTag_, typename OtherLockT_>
	other& operator=(const CmnAllcator_Buffer<_Other, OtherTag_, OtherLockT_>&)
	{	// assign from a related allocator (do nothing)
		return (*this);
	}

	void deallocate(pointer _Ptr, size_type)
	{
		Free(_Ptr);
	}

	pointer allocate(size_type _Count)
	{	// allocate array of _Count elements
		return (_Allocate(_Count, (pointer)0));
	}

	pointer allocate(size_type _Count, const void *)
	{	// allocate array of _Count elements, ignore hint
		return (allocate(_Count));
	}

	void construct(value_type *_Ptr)
	{	// default construct object at _Ptr
		::new ((void *)_Ptr) value_type();
	}

	void construct(value_type *_Ptr, const value_type& _Val)
	{	// construct object at _Ptr with value _Val
		::new ((void *)_Ptr) value_type(_Val);
	}

	template<typename T1, typename T2, typename... Args>
	void construct(value_type *_Ptr, const T1& v1, const T2& v2, const Args&... args)
	{	// construct object at _Ptr with value _Val
		::new ((void *)_Ptr) value_type(v1, v2, args...);
	}

	template<class _Uty>
	void destroy(_Uty *_Ptr)
	{	// destroy object at _Ptr
		_Ptr->~_Uty();
	}

	size_t max_size() const
	{	// estimate maximum array size
		return (std::numeric_limits<BuffMgrT::size_type>::max() / (sizeof(value_type) + sizeof(BuffMgrT::stPageInfo)));
	}

private:
	template<class T> inline
		T*_Allocate(size_t _Count, T*)
	{	// allocate storage for _Count elements of type _Ty
		void *_Ptr = 0;

		if (_Count == 0)
			;
		else if ((max_size() < _Count)
			|| (_Ptr = Malloc(_Count * sizeof(T))) == nullptr)
			throw std::bad_alloc();	// report no memory

		return ((T*)_Ptr);
	}

public:
	static void* Malloc(size_type size){
		auto res = buffer_mgr_.Malloc(size);
		if (!res)
		{
			assert(false);
		}
		return res;
	}
	static void Free(void* addr){
		if (!buffer_mgr_.Free(addr))
			assert(false);
	}
};

//////////////////////////////////////////////////////////////////////////
#define DEFINE_CMN_ALLOCATOR_BUFFER( cls_name, cls_tag, cls_mutex)	\
template<typename _Ty>	\
class cls_name : public CmnAllcator_Buffer<_Ty, cls_tag, cls_mutex>{	\
public:	\
	typedef cls_name<_Ty> other;	\
	template<class _Other>	\
	struct rebind{	\
		typedef cls_name<_Other> other;	\
	};	\
};	\
/**/
//////////////////////////////////////////////////////////////////////////
template<typename TagT_, typename LockT_>
p_selectany
typename CmnAllcator_BufferBase<TagT_, LockT_>::BuffMgrT CmnAllcator_BufferBase<TagT_, LockT_>::buffer_mgr_;
