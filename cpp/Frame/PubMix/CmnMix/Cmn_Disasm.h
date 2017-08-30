#pragma once
/*
@author zhp
@date 2016/4/14 0:50
@purpose for disasm
*/
#include "../DependExternLib.h"
#include "../DependSelfLib.h"
#include <string>

/*
instruction->insn
*/
class Cmn_Disasm{
	Cmn_Disasm( const Cmn_Disasm& );
	Cmn_Disasm& operator = ( const Cmn_Disasm& );

public:
	Cmn_Disasm();
	~Cmn_Disasm();

public:
	void SetInputBuffer( const unsigned char* buffer, unsigned int buffer_size );

public:
	//反汇编一次
	//Returns:	the number of bytes disassembled. A 0 indicates end of input.
	unsigned int DisassembleOnce() const;
	//解码一次，DisassembleOnce == DecodeOnce + TranslateIntel
	unsigned int DecodeOnce() const;
	//转换成intel
	void TranslateIntel() const;

public:
	//得到当前的字符串命令
	const char* GetCurInsnAsm() const;
	//得到当前的hex形式的指令
	const char* GetCurInsnHex() const;
	//得到当前指令长度
	unsigned int GetCurInsnLen() const;
	//得到当前指令相对于起始汇编处的偏移
	unsigned long long GetCurInsnOff() const;
	//得到当前指令的缓冲区指针
	const unsigned char* GetCurBytesPtr() const;

public:
	std::string GenFeatureCode() const;

private:
	void*		impl_;
};