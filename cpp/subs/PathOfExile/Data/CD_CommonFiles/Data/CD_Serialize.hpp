#pragma once
/*
@author zhp
@date 2017/1/28 17:27
@purpose for serialize
*/
#include <ProtocolX/P_DefinesX.h>
#include <cstddef>
#include "CD_Const.h"
#include <cassert>
#include <WgFacility/CmnRecvPacket.h>
#include "CD_StructBase.h"
#include <type_traits>

#pragma comment(lib, "Ws2_32.lib")

#pragma pack(push)
#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
#define CD_ZERO_MSGS() std::memset((char*)this + sizeof(stCDS_MsgBase), 0, sizeof(*this) - sizeof(stCDS_MsgBase))
#define CD_ZERO_MSGR() std::memset((char*)this, 0, sizeof(*this))

template<pt_word MsgId, pt_dword EncryptValue_ = 0, bool IsCheckSize = true>
struct stCDS_MsgBase{
	enum{
		kMsgId = MsgId
	};

	pt_word			msg_id_;
	static const pt_dword SendPacketEncryptValue = EncryptValue_;
	static const bool Policy_IsCheckSize = IsCheckSize;

	stCDS_MsgBase(){
		msg_id_ = MsgId;
	}
};

template<pt_word MsgId, bool IsCheckSize = true>
struct stCDR_MsgBase{
	enum{
		kMsgId = MsgId
	};
	static const bool Policy_IsCheckSize = IsCheckSize;
};
//////////////////////////////////////////////////////////////////////////
struct stCD_GameMsgBase{
	stCD_GameMsgBase();

	pt_pcvoid		vtbl_;
	pt_word			msg_id_;
	p_mem_placeholder(2);		//对齐
};

struct stCD_EntityMsgInfo{
	/*
	这两个实体id，对于大多数的技能，都用来描述技能运动方向。
	*/
	//这个消息该由哪个实体去处理
	pt_dword		entity_id_;			//按着技能不放时，id不为0，等于第一个字段的值；怪物攻击角色时，这个字段为角色的id
	//相关联的那个实体
	pt_dword		dst_entity_id_;		//施放该技能的实体id，有时候为0，表示角色自己
	pt_word			flag_;				//按一下=0，按住不放=0x4000、0x4001、0x4002。释放技能前若是要先移动，则=1.

	stCD_EntityMsgInfo();
	template<class Archive>
	void serialize(Archive& ar){
		ar & entity_id_ & dst_entity_id_ & flag_;
	}
	bool InitAsSelf();
};

struct stCD_GameEntityMsg : stCD_GameMsgBase{
	stCD_GameEntityMsg();
	bool InitAsSelf();

	stCD_EntityMsgInfo		entity_;
	pt_word					mem_placeholder_;		//对齐
};
static_assert(8 + 0xc == sizeof(stCD_GameEntityMsg), "");
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)
//////////////////////////////////////////////////////////////////////////
struct CD_TagEncrypt1{};
struct CD_TagEncrypt2{};
//////////////////////////////////////////////////////////////////////////
template<typename Tag, typename T>
struct CD_EncryptImpl{
	typedef typename pt_as_size<sizeof(T)>::type type;
	CD_EncryptImpl(const T& v) : value_(v){}
	template<typename SendPacketT>
	type Encrypt(const SendPacketT& send_packet) const{
		auto encryptor = send_packet.GetEncryptor();
		if (!encryptor)
		{
			assert(false);
			return 0;
		}
		type v = value_;
		return encryptor->Encrypt(v, Tag());
	}

	const T&	value_;
};
template<typename Tag, typename T>
inline CD_EncryptImpl<Tag, T> CD_Encrypt(const T& v){
	return CD_EncryptImpl<Tag, T>(v);
}

template<bool IsEnum>
struct CD_EnumSerialize{
	template<typename SerT, typename T>
	static void DoWrite(SerT& s, const T& v){
		return s.BuildPacketImpl(&v, sizeof(T));
	}
	template<typename SerT, typename T>
	static bool DoRead(SerT& s, T& v){
		return s.DoReadCopyTo_(&v, sizeof(T));
	}
};
template<>
struct CD_EnumSerialize<true>{
	template<typename SerT, typename T>
	static void DoWrite(SerT& s, const T& v){
		typedef typename pt_as_size<sizeof(T)>::type the_value_type;
		the_value_type v_tmp = v;
		return s.Write(v_tmp);
	}
	template<typename SerT, typename T>
	static bool DoRead(SerT& s, T& v){
		typedef typename pt_as_size<sizeof(T)>::type the_value_type;
		the_value_type* v_tmp = (the_value_type*)&v;
		return s.Read(*v_tmp);
	}
};
//////////////////////////////////////////////////////////////////////////
template<typename SendPacketT>
struct CD_SerializeS{
	//////////////////////////////////////////////////////////////////////////
	CD_SerializeS(SendPacketT& send_packet) : send_packet_(send_packet){}
	template<typename T>
	bool DoSend(const T& packet){
		Write(packet.msg_id_);
		packet.serialize(*this);
		send_packet_.DoSendPacket(packet.SendPacketEncryptValue);
		return true;
	}
	template<typename T>
	CD_SerializeS& operator& (const T& v){
		Write(v);
		return *this;
	}
	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	void Write(const T& v){
		CD_EnumSerialize<std::is_enum<T>::value>::DoWrite(*this, v);
	}
	//最好不要把const pt_dword&变成pt_dword，因为const pt_dword&不会隐式转换。
	void Write(const pt_dword& v){
		auto res = htonl(v);
		BuildPacketImpl(&res, sizeof(res));
	}
	void Write(const pt_word& v){
		auto res = htons(v);
		BuildPacketImpl(&res, sizeof(res));
	}
	void Write(const int& v){
		auto res = htonl(v);
		BuildPacketImpl(&res, sizeof(res));
	}
	void Write(const short& v){
		auto res = htons(v);
		BuildPacketImpl(&res, sizeof(res));
	}
	void Write(pt_cwsz str){
		if (!str)
		{
			assert(false);
			return;
		}
		auto len = (pt_word)wcslen(str);
		assert(len <= CD_kMaxStrLen);
		Write(len);
		BuildPacketImpl(str, len * sizeof(wchar_t));
	}
	void Write(const std::wstring& v){
		auto len = (pt_word)v.size();
		assert(len <= CD_kMaxStrLen);
		Write(len);
		BuildPacketImpl(v.c_str(), len * sizeof(wchar_t));
	}
	template<typename T>
	void Write(const stCD_Vec2<T>& v){
		Write(v.x);
		Write(v.y);
	}
	template<typename Tag, typename T>
	void Write(const CD_EncryptImpl<Tag, T>& v){
		Write(v.Encrypt(send_packet_));
	}
	//////////////////////////////////////////////////////////////////////////
	void BuildPacketImpl(pt_pcvoid data, pt_dword size){
		send_packet_.BuildPacket(data, size);
	}

	SendPacketT&		send_packet_;
};
//////////////////////////////////////////////////////////////////////////
class CD_CMDR_SerializeImpl_ : public Cmn_CMDR_Serialize{
public:
	template<bool IsCheckSize>
	struct stCheckSizePolicy{
		template<typename MsgT>
		static bool Check(CD_CMDR_SerializeImpl_& s){
			if (!s.is_valid_)
			{
				assert(false);
				return false;
			}
			if (0 != s.LeftSize())
			{
#ifdef _DEBUG
				LOG_O(Log_debug) << "收包，但结构体大小不一样：" << typeid(MsgT).name();
				//assert(false);
#endif
				return false;
			}
			return true;
		}
	};
	template<>
	struct stCheckSizePolicy<false>{
		template<typename MsgT>
		static bool Check(CD_CMDR_SerializeImpl_& s){
			assert(s.is_valid_);
			return s.is_valid_;
		}
	};

public:
	CD_CMDR_SerializeImpl_(){
		is_valid_ = true;
	}
	template<typename T>
	CD_CMDR_SerializeImpl_& operator& (T& v){
		if (!is_valid_)
			return *this;
		Read(v);
		return *this;
	}
	template<typename MsgT>
	int ReadMsg(MsgT& msg){
		int left_size = LeftSize();
		msg.serialize(*this);
		left_size -= LeftSize();
		if (stCheckSizePolicy<msg.Policy_IsCheckSize>::Check<MsgT>(*this))
			return left_size;
		left_size = -left_size;
		return left_size;
	}
	bool empty() const{
		return LeftSize() == 0;
	}
	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	bool Read(T& v){
		return CD_EnumSerialize<std::is_enum<T>::value>::DoRead(*this, v);
	}
	bool Read(pt_dword& v){
		if (!DoReadCopyTo_(&v, sizeof(v)))
			return false;
		v = ntohl(v);
		return true;
	}
	bool Read(pt_word& v){
		if (!DoReadCopyTo_(&v, sizeof(v)))
			return false;
		v = ntohs(v);
		return true;
	}
	bool Read(int& v){
		if (!DoReadCopyTo_(&v, sizeof(v)))
			return false;
		v = ntohl(v);
		return true;
	}
	bool Read(short& v){
		if (!DoReadCopyTo_(&v, sizeof(v)))
			return false;
		v = ntohs(v);
		return true;
	}
	bool Read(std::wstring& v){
		pt_word cnt = 0;
		if (!DoReadCopyTo_(&cnt, sizeof(cnt)))
			return false;
		if (0 == cnt)
		{
			v.clear();
			return false;
		}
		cnt = ntohs(cnt);
		v.resize(cnt);
		return DoReadCopyTo_((pt_wsz)v.c_str(), cnt * sizeof(wchar_t));
	}
	template<int Size_>
	bool Read(Cmn_CMDR_Ignore<Size_>& v){
		is_valid_ = Ignore(v.GetSize());
		return is_valid_;
	}
	template<typename T>
	bool Read(stCD_Vec2<T>& v){
		if (!Read(v.x)) return false;
		if (!Read(v.y)) return false;
		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool DoReadCopyTo_(pt_pvoid data, pt_dword size){
		assert(is_valid_);
		is_valid_ = ReadCopyTo(data, size);
		return is_valid_;
	}

private:
	bool	is_valid_;
};

class CD_CMDS_SerializeImpl_ : public CD_CMDR_SerializeImpl_{
public:
	template<typename MsgT>
	int ReadMsg(MsgT& msg){
		int left_size = LeftSize();
		if (msg.CheckRead(*this))
			left_size -= LeftSize();
		else
		{
			left_size -= LeftSize();
			left_size = -left_size;
			return left_size;
		}
		if (stCheckSizePolicy<msg.Policy_IsCheckSize>::Check<MsgT>(*this))
			return left_size;
		left_size = -left_size;
		return left_size;
	}
};

template<typename MsgT, typename MsgHandlerT>
class CD_CMDR_Serialize : public Cmn_CMDR_MsgSeriaHandler<MsgT, MsgHandlerT, CD_CMDR_SerializeImpl_>{};

template<typename MsgT>
class CD_CMDS_Serialize : public Cmn_CMDR_MsgSeria<MsgT, CD_CMDS_SerializeImpl_>{
	bool IsPostToIoService() const override{ return false; }
	int DoDirect(pt_pcvoid msg_data, pt_dword msg_size) override{
		auto read_cnt = __super::DoDirect(msg_data, msg_size);
		if (read_cnt >= 0) return read_cnt;
		LOG_O(Log_debug) << "发包校验不通过：" << typeid(MsgT).name() << ",msg_id:" << (pt_pcvoid)MsgT::kMsgId << ",字节数：" << read_cnt;
		return read_cnt;
	}
};