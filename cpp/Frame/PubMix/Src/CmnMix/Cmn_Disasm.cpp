#include "stdafx.h"
#include <CmnMix/Cmn_Disasm.h>
#include <udis86/udis86.h>
#include <cassert>

/*
url->http://udis86.sourceforge.net/¡£
*/
#define ToUd_t	((ud_t*)impl_)

Cmn_Disasm::Cmn_Disasm()
{
	impl_ = new ud_t;
	ud_init(ToUd_t);
	ud_set_mode(ToUd_t, 32);
	ud_set_vendor(ToUd_t, UD_VENDOR_INTEL);
	ud_set_syntax(ToUd_t, UD_SYN_INTEL);
}

Cmn_Disasm::~Cmn_Disasm()
{
	delete ToUd_t;
	impl_ = nullptr;
}

void Cmn_Disasm::SetInputBuffer( const unsigned char* buffer, unsigned int buffer_size )
{
	ud_set_input_buffer(ToUd_t, buffer, buffer_size);
}

unsigned int Cmn_Disasm::DisassembleOnce() const
{
	return ud_disassemble(ToUd_t);
}

const char* Cmn_Disasm::GetCurInsnAsm() const
{
	return ud_insn_asm(ToUd_t);
}

const char* Cmn_Disasm::GetCurInsnHex() const
{
	return ud_insn_hex(ToUd_t);
}

unsigned int Cmn_Disasm::GetCurInsnLen() const
{
	return ud_insn_len(ToUd_t);
}

unsigned long long Cmn_Disasm::GetCurInsnOff() const
{
	return ud_insn_off(ToUd_t);
}

const unsigned char* Cmn_Disasm::GetCurBytesPtr() const
{
	return ud_insn_ptr(ToUd_t);
}

unsigned int Cmn_Disasm::DecodeOnce() const
{
	return ud_decode(ToUd_t);
}

void Cmn_Disasm::TranslateIntel() const
{
	ud_translate_intel(ToUd_t);
}

std::string Cmn_Disasm::GenFeatureCode() const
{
	std::string res;
	
	unsigned int d_size, last_size = 0;
	while ( (d_size = DecodeOnce()) > 0 )
	{
		if ( last_size > 0 )
			res.append(last_size * 2, '?');

		auto cur_ptr = GetCurBytesPtr();
		if ( !cur_ptr )
		{
			assert(false);
			break;
		}
		char buffer[3];
		sprintf_s(buffer, "%x", *cur_ptr);
		if ( strlen(buffer) < 2 )
			res += '0';
		res += buffer;
		--d_size;
		last_size = d_size;
	}
	
	assert(!res.empty());
	return res;
}

#undef ToUd_t
