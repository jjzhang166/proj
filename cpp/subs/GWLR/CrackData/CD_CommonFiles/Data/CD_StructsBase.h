#pragma once
/*
@author zhp
@date 2016/5/20 14:57
*/
#include <ProtocolX/P_DefinesX.h>
#include <CmnMix/Cmn_Defines.h>
#include <iostream>
#include <cassert>
#include "CD_Const.h"
#include <BufCall/BufCall.h>

#pragma pack(push)
#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
struct stCD_Node{
	pt_pcvoid	vtbl_;
	stCD_Node*	parent_;
	stCD_Node*	left_;
	stCD_Node*	right_;

	bool RootNodeValid() const{
		return this != parent_;
	}
	p_define_cast
	stCD_Node() = delete;
	stCD_Node& operator = (const stCD_Node& rhs) = delete;
};
template<typename T>
struct stCD_NodeT : stCD_Node{
	int			id_;
	T*			obj_;

	stCD_NodeT<T>* FindById(int id) const{
		auto ptr = CastTo<stCD_NodeT<void> >();
		auto res = ptr->FindById(id);
		return res->CastTo<stCD_NodeT<T> >();
	}
	T* FindTheObj(int id) const{
		auto ptr = FindById(id);
		if (IsUnValidPtr(ptr))
			return nullptr;
		if (IsUnValidPtr(ptr->obj_))
		{
			assert(false);
			return nullptr;
		}
		return ptr->obj_;
	}
	friend std::ostream& operator << (std::ostream& ostm, const stCD_NodeT& v){
		ostm << "{ node_addr:" << &v << " id:" << v.id_;
		if (v.obj_)
			ostm << " obj:" << *v.obj_;
		ostm << " }";
		return ostm;
	}
};
template<>
struct stCD_NodeT<void> : stCD_Node{
	stCD_NodeT<void>* FindById(int id) const;

	int			id_;
	void*		obj_;
};

struct stCD_Vector3{
	float	fx;
	float	fy;
	float	fz;

	friend std::ostream& operator << (std::ostream& ostm, const stCD_Vector3& vec){
		ostm << "（" << vec.fx << ", " << vec.fy << ", " << vec.fz << "）";
		return ostm;
	}
	template<typename T>
	void serialize(T& stm){
		stm & fx & fy & fz;
	}
	p_define_cast
};
struct stCD_Direction2{
	float	dir_x;
	float	dir_y;
	template<typename T>
	void serialize(T& stm){
		stm & dir_x & dir_y;
	}

	p_define_cast
};

struct stCD_PosDirInfo{
	stCD_Vector3	dir_1;
	float			fx;
	stCD_Vector3	dir_2;
	float			fy;
	stCD_Vector3	dir_3;
	float			fz;

	friend std::ostream& operator << (std::ostream& ostm, const stCD_PosDirInfo& v);
};

struct stCD_EncryptInt{
	int		encrypt_value1_;
	int		encrypted_value2_;

	operator int() const;
	friend std::ostream& operator << (std::ostream& ostm, const stCD_EncryptInt& v);
};
static_assert(sizeof(stCD_EncryptInt) == 8, "");

template<typename T>
struct stCD_Array{
	typedef T type;
	typedef std::function<bool(type*)> DoFuncT;
	int Traverse(const DoFuncT& func) const{
		if (IsUnValidPtr(begin_) || IsUnValidPtr(end_))
		{
			//assert(false);
			return 0;
		}
		int cnt = 0;
		for (auto it = begin_; it < end_; ++it)
		{
			if (IsUnValidPtr(it))
			{
				assert(false);
				return cnt;
			}
			if (!func(it))
				break;
		}
		return cnt;
	}
	friend std::ostream& operator << (std::ostream& ostm, const stCD_Array& v){
		ostm << "{ ";
		v.Traverse([&ostm](type* item){
			ostm << *item;
			return true;
		});
		ostm << " }";
		return ostm;
	}

	type*	begin_;
	type*	end_;
	type*	ukn_end2_;
	//pt_csz	str_command_;
};
static_assert(sizeof(stCD_Array<pt_dint>) == 4 * 3, "");

template<typename T>
struct stCD_DummyArray{
	stCD_DummyArray(){
		value_ = nullptr;
		cnt_ = 0;
	}
	T* begin(){
		if (IsUnValidPtr(value_))
			return nullptr;
		return value_;
	}
	const T* begin() const{
		if (IsUnValidPtr(value_))
			return nullptr;
		return value_;
	}
	T* end(){
		auto it = begin();
		if (!it)
			return nullptr;
		return it + cnt_;
	}
	const T* end() const{
		auto it = begin();
		if (!it)
			return nullptr;
		return it + cnt_;
	}

	T*		value_;
	int		cnt_;
};
//////////////////////////////////////////////////////////////////////////
using CD_MsgBufferType = pt_byte[kMaxSendBufferSize];

struct stCD_EncryptMsgInfo{
	CD_MsgBufferType*	encrypted_buffer_;	//存放加密后的数据
	pt_dword			msg_size_;			//当前已加密的数据大小
	int					msg_max_size_;		//缓冲区剩余长度
};
static_assert(sizeof(stCD_EncryptMsgInfo) == 4 * 3, "");

template<typename StringT>
struct BC_OriStr{
	BC_OriStr(const StringT& v) : value_(v){}
	template<typename T>
	friend T& operator << (T& stm_wrapper, const BC_OriStr& v){
		auto size = TStrLen(v.value_);
		auto& stm = stm_wrapper.GetStream();
		auto total_len = size * sizeof(decltype(v.value_[0])) + sizeof(decltype(v.value_[0]));
		if (stm.LeftSize() < total_len)
		{
			assert(false);
			return stm_wrapper;
		}
		stm.WriteValue(v.value_, total_len);
		return stm_wrapper;
	}
	template<typename T>
	friend T& operator >> (T& stm_wrapper, BC_OriStr& v){
		auto& stm = stm_wrapper.GetStream();
		auto left_size = stm.LeftSize();
		auto cur_ptr = stm.CurPtr();
		auto size = Cmnx_StrMaxLen(cur_ptr, left_size);
		if (size >= left_size)
		{
			assert(false);
			return stm_wrapper;
		}
		if (!stm.Skip(size * sizeof(decltype(v.value_[0])) + sizeof(decltype(v.value_[0]))))
		{
			assert(false);
			return stm_wrapper;
		}
		v.value_ = cur_ptr;
		return stm_wrapper;
	}

	StringT	value_;
};

template<typename StringT>
struct BC_OriStrArray{
	BC_OriStrArray(const StringT& v) : value_(v){}
	template<typename T>
	friend T& operator << (T& stm_wrapper, const BC_OriStrArray& v){
		auto& stm = stm_wrapper.GetStream();
		auto left_size = stm.LeftSize();
		if (sizeof(v.value_) > left_size)
		{
			assert(false);
			return stm_wrapper;
		}
		stm.WriteValue(v.value_, sizeof(v.value_));
		return stm_wrapper;
	}

	StringT	value_;
};

//original array
template<typename ContT>
struct BC_OriCont{
	BC_OriCont(const ContT& v) : cont_(v){}
	template<typename T>
	friend T& operator << (T& stm_wrapper, const BC_OriCont& cont){
		auto& stm = stm_wrapper.GetStream();
		for (auto& v : cont.cont_)
		{
			stm << v;
		}
		return stm_wrapper;
	}
	template<typename T>
	friend T& operator >> (T& stm_wrapper, BC_OriCont& cont){
		auto& stm = stm_wrapper.GetStream();
		return stm_wrapper;
	}

	ContT	cont_;
};

template<typename T_, typename CntT_, int max_cnt_ = 1>
struct stCD_MsgArray{
	CntT_				cnt_;
	std::array<T_, max_cnt_>	value_;

	template<typename T>
	friend T& operator << (T& stm_wrapper, const stCD_MsgArray& value){
		auto& stm = stm_wrapper.GetStream();
		stm << value.cnt_;
		auto it_end = value.end();
		for (auto it = value.begin(); it < it_end; ++it)
		{
			stm << *it;
		}
		return stm_wrapper;
	}
	T_* begin(){
		return &*value_.begin();
	}
	T_* end(){
		return begin() + cnt_;
	}
	const T_* begin() const{
		return &*value_.begin();
	}
	const T_* end() const{
		return begin() + cnt_;
	}
};

struct stCD_CheckMsgAsPOD{};
struct stCD_CheckMsgAtRuntime{};
namespace Detail{
	class CalcSizeOstm{
	public:
		CalcSizeOstm(){
			size_ = 0;
		}
		template<typename T>
		CalcSizeOstm& operator << (const BC_OriStrArray<T>& v){
			size_ += 4;
			size_ += TStrLen(v.value_) + sizeof(v.value_[0]);
			return *this;
		}
		template<typename T>
		CalcSizeOstm& operator<< (const T& v){
			BC_DataWrapper<T, BC_DataWrapperPolicy_NoDoValid>::Write(*this, v);
			return *this;
		}
	public:
		struct StreamWrapper{
			explicit StreamWrapper(CalcSizeOstm& stm) : stm_(stm){
			}
			template<typename T>
			StreamWrapper& operator& (const T& v){
				stm_ << v;
				return *this;
			}
			CalcSizeOstm& GetStream() const{
				return stm_;
			}

		private:
			CalcSizeOstm&	stm_;
		};

	public:
		void WriteTypeIdx(BC_TypeIndexT type_idx){

		}
		void WriteValue(const void* src, std::size_t src_size){
			size_ += src_size;
		}
		void Fill(std::size_t size, unsigned char value){
			size_ += size;
		}
		std::size_t LeftSize() const{
			return 0x1000000;
		}
		std::size_t GetWriteSize() const{
			return size_;
		}

	public:
		std::size_t		size_;
	};

	template<bool IsAsPOD, bool IsAtRuntime, typename T>
	struct stCD_MsgCheck{
		static T* get(T* msg, pt_dword){
			return msg;
		}
	};
	template<typename T>
	struct stCD_MsgCheck<true, false, T>{
		static T* get(T* msg, pt_dword msg_size){
			if (sizeof(*msg) != msg_size)
			{
				LOG_O(Log_debug) << typeid(T).name() << " 的包长度与实际长度不符，sizeof(msg):" << sizeof(*msg) << " 而实际长度：" << msg_size;
				return nullptr;
			}
			return msg;
		}
	};
	template<typename T>
	struct stCD_MsgCheck<false, true, T>{
		static T* get(T* msg, pt_dword msg_size){
			Detail::CalcSizeOstm calc_size;
			calc_size << *msg;
			if (calc_size.size_ != msg_size)
			{
				LOG_O(Log_debug) << typeid(T).name() << " 的包长度与实际长度不符，本地计算长度:" << calc_size.size_ << " 而实际长度：" << msg_size;
				return nullptr;
			}
			return msg;
		}
	};
	template<typename T>
	T* CDF_CheckMsg(T* msg, pt_dword msg_size){
		return stCD_MsgCheck<std::is_base_of<stCD_CheckMsgAsPOD, T>::value,
			std::is_base_of<stCD_CheckMsgAtRuntime, T>::value,
			T>::get(msg, msg_size);
	}
}

//MMO_PACKET_HEADER
struct stCD_MsgBase{
	pt_word			msg_id_;
	pt_word			main_msg_id_;	//一直等于1	wPacketMsgType
	pt_dword		msg_size_;		//消息结构体序列化后的长度，并非是sizeof(msg)，但是大多数都是sizeof(msg)。
	pt_dword		ukn1_[2];

public:
	stCD_MsgBase() = delete;
	template<typename T>
	T* MsgAs() const{
		return Detail::CDF_CheckMsg((T*)((char*)this + sizeof(*this)), msg_size_);
	}
	//发包有效
	template<typename T>
	bool EncodeMsg(const T& msg){
		//static_assert(sizeof(msg) + sizeof(*this) < kMaxSendBufferSize, "");
		std::memset(this, 0, kMaxSendBufferSize);
		//std::memset(this, 0, sizeof(*this));
		msg_id_ = T::kMsgId;
		auto real_msg = MsgAs<void>();
		BC_NoTypeIdxStreamW ostm(real_msg, kMaxSendBufferSize - sizeof(*this));
		ostm << msg;

#ifdef _DEBUG
		auto real_size = GetMsgLength();
		if (!Detail::CDF_CheckMsg((T*)real_msg, real_size))
		{
			assert(false);
			return false;
		}
#endif
		return true;
	}
	//发包有效
	int GetMsgLength() const;
};
static_assert(sizeof(stCD_MsgBase) == 0x10, "");
//////////////////////////////////////////////////////////////////////////
enum CD_En_WeekCreditIndex{
	kUkn = 2,
};
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)
