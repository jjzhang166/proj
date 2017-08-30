#pragma once
/*
@author zhp
@date 2016/1/27 16:29
@purpose 通用函数定义
*/
#include "../DependSelfLib.h"
#include <ProtocolX/P_DefinesX.h>
#include <StdEx/TString.h>

//float compare
inline bool FloatEqual(float lhs, float rhs){
	return fabs(lhs - rhs) < 0.000001;
}
inline bool FloatEqual(double lhs, double rhs){
	return fabs(lhs - rhs) < 0.000001;
}
template<typename T>
inline bool FloatEqual(T lhs, T rhs){
	return lhs == rhs;
}

bool Cmnx_CpyMem( void* dst_addr, const void* src_addr, unsigned int mem_size );
bool Cmnx_CpyMemRecycle( void* dst_addr, unsigned int dst_buffer_size, const void* src_addr, unsigned int mem_size );
bool Cmnx_MemSetValue( void* dst_addr, unsigned int mem_size, unsigned char the_value );

SECURITY_ATTRIBUTES* GetNullDACL();

//改写内存中的数据(不必关心页面的读写属性)
bool WriteMemory(void* dest, const void* src, size_t size);
bool WriteMemory_AntiCrc(const char* hooker_name, void* dest, const void* src, size_t size);
bool SetHook(const char* hooker_name, void* the_hooked_addr, const void* new_jmp_addr, unsigned char old_code[5] = nullptr);
//////////////////////////////////////////////////////////////////////////
//timer do tag
struct tagTimerDoSleep{};
struct tagTimerDoDummySleep{};
namespace Details{
	bool CmnTimerDoImpl_(pt_dword every_sleep, pt_dword time_out, SleepFunctionT,
		const FunctionBool& func_do);

	template<typename Policy>
	struct stTimerDoByPolicy;
	template<>
	struct stTimerDoByPolicy<tagTimerDoSleep>{
		static bool TimerDoImpl(pt_dword every_sleep, pt_dword time_out, const FunctionBool& func_do);
	};
	//////////////////////////////////////////////////////////////////////////
	class _SlowExecBase_{
	public:
		virtual void Exec() const = 0;
	};

	template<typename FnT>
	class _SlowExecImpl_ : public _SlowExecBase_{
	public:
		_SlowExecImpl_(const FnT& fn) : fn_(fn){}
		void Exec() const override{
			fn_();
		}

	private:
		const FnT&		fn_;
	};
	//////////////////////////////////////////////////////////////////////////
}
template<typename Policy = tagTimerDoDummySleep>
bool TimerDo(pt_dword every_sleep, pt_dword time_out, const FunctionBool& func_do){
	return Details::stTimerDoByPolicy<Policy>::TimerDoImpl(every_sleep, time_out, func_do);
}
//////////////////////////////////////////////////////////////////////////
//可以防止被优化
template<typename Fn>
void SlowExecute(const Fn& fn){
	Details::_SlowExecBase_* slow_exec = (Details::_SlowExecBase_*)(const void*)&fn;
	Details::_SlowExecImpl_<Fn> exec_impl(fn);
	if ((int)(void*)slow_exec != 1)
		slow_exec = &exec_impl;
	slow_exec->Exec();
}
//////////////////////////////////////////////////////////////////////////
inline pc_naked void NakedRetP0(){
	_asm ret;
}
inline pc_naked void NakedRetP1(){
	_asm ret 4;
}
inline pc_naked void NakedRetP2(){
	_asm ret 8;
}
inline pc_naked void NakedRetP3(){
	_asm ret 12;
}
inline pc_naked void NakedRetP4(){
	_asm ret 16;
}
inline pc_naked void NakedRetP5(){
	_asm ret 20;
}
inline pc_naked void NakedRetP6(){
	_asm ret 24;
}
inline pc_naked void NakedRetP7(){
	_asm ret 28;
}
inline pc_naked void NakedRetP8(){
	_asm ret 32;
}
inline pc_naked void NakedRetP9(){
	_asm ret 36;
}
inline pc_naked void NakedRetP10(){
	_asm ret 40;
}
inline pc_naked void NakedRetP11(){
	_asm ret 44;
}
inline pc_naked void NakedRetP12(){
	_asm ret 48;
}
//////////////////////////////////////////////////////////////////////////
