#pragma once
/*
@author zhp
@date 2014/8/19 15:43
@purpose for game connect protocol
*/

struct CMD_HeaderBase{
protected:
	CMD_HeaderBase();
	CMD_HeaderBase& operator = ( const CMD_HeaderBase& );
public:
	//后续地址
	void* GetFollowAddr(){
		return (char*)this + sizeof(*this);
	}
	const void* GetFollowAddr() const{
		return (const char*)this + sizeof(*this);
	}
	//后续长度
	WORD GetFollowSize() const{
		assert(total_length_ > sizeof(*this));
		return total_length_ - sizeof(*this);
	}

public:
	WORD			total_length_;			//总长度
	WORD			stable_length_;			//固定长度,0x55e0
};