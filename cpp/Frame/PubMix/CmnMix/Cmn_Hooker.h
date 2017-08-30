#pragma once
/*
@author zhp
@date 2014-1-03
@purpose API/code hook
*/
#include <boost/shared_ptr.hpp>
#include <boost/mpl/int.hpp>
#include <set>
#include "Cmn_LogOut.h"
#include "Cmn_Exception.h"
#include "Cmn_UsefulClass.h"
#include "Cmn_UsefulFunc.h"
#include "../DependSelfLib.h"
#include "../DependExternLib.h"

#pragma warning(push)
#pragma warning(disable:4410)
#pragma warning(disable:4409)

//////////////////////////////////////////////////////////////////////////
//标志寄存器的位域/位段
struct stEflags{
	int	carry_ : 1;		//进位。当执行一个加法(或减法)运算，使最高位产生进位(或借位)时，CF为1；否则为0。
	int	unuse1_ : 1;
	int	parity_ : 1;	//奇偶。当执行一个加法(或减法)运算，使最高位产生进位(或借位)时，CF为1；否则为0。
	int	unuse2_ : 1;
	int auxiliary_ : 1;	//辅助进位。当执行一个加法(或减法)运算，使最高位产生进位(或借位)时，CF为1；否则为0。
	int	unuse3_ : 1;
	int	zero_ : 1;		//零标志位。若当前的运算结果为零，ZF=1；否则ZF=0。
	int	sign_ : 1;		//符号标志位。它和运算结果的最高位相同。
	int	trap_ : 1;		//跟踪标志位。它是为调试程序而设定的陷阱控制位。当该位置“1”时，8086 CPU处于单步状态，此时CPU每执行完一条指令就自动产生一次内部中断。当该位复位后，CPU恢复正常工作。
	int	interrupt_enable_ : 1;	//中断允许标志位。它用来控制8086是否允许接收外部中断请求。若IF=1，8086能响应外部中断，反之则不响应外部中断。
	int	direction_ : 1;			//方向标志位。它用以指定字符串处理时的方向，当该位置“1”时，字符串以递减顺序处理，即地址以从高到低顺序递减。反之，则以递增顺序处理。
	int	overflow_ : 1;			//溢出标志位。当补码运算有溢出时，OF=1；否则OF=0。	3个控制标志位用来控制CPU的操作，由指令进行置位和复位。
	int	iopl_ : 2;				//I/O privilege level. I/O特权标志用两位二进制位来表示，也称为I/O特权级字段。该字段指定了要求执行I/O指令的特权级。如果当前的特权级别在数值上小于等于IOPL的值，那么，该I/O指令可执行，否则将发生一个保护异常。
	int	nested_task_ : 1;		//嵌套任务标志NT用来控制中断返回指令IRET的执行。具体规定如下：	(1)、当NT = 0，用堆栈中保存的值恢复EFLAGS、CS和EIP，执行常规的中断返回操作；(2)、当NT = 1，通过任务转换实现中断返回。
	int	unuse4_ : 1;
	int	resume_ : 1;			//重启动标志RF用来控制是否接受调试故障。规定：RF=0时，表示"接受"调试故障，否则拒绝之。在成功执行完一条指令后，处理机把RF置为0，当接受到一个非调试故障时，处理机就把它置为1。
	int	vm_ : 1;	//Virtual-8086 Mode.如果该标志的值为1，则表示处理机处于虚拟的8086方式下的工作状态，否则，处理机处于一般保护方式下的工作状态。
	int	alignment_check_ : 1;	//
	int	VIF_ : 1;	//virtual interrupt flag
	int VIP_ : 1;	//virtual interrupt pending
	int	id_ : 1;	//ID Flag
	int	unuse5_ : 10;
};
static_assert(sizeof(stEflags) == 4, "");
class IHooker;
//跳转到自己的代码后的寄存器环境。这个结构一定不要加其他东西！
struct JmpHookerAmbient{
	stEflags	efl_;	//状态寄存器/标志寄存器
	int			edi_;
	int			esi_;
	int			ebp_;
	int			esp_;	//esp不能直接被修改
	int			ebx_;
	int			edx_;
	int			ecx_;
	int			eax_;

public:
	JmpHookerAmbient();
	JmpHookerAmbient(const JmpHookerAmbient&) = delete;
	JmpHookerAmbient& operator = (const JmpHookerAmbient&) = delete;
	//基于esp的栈中的元素的寻址
	template<typename T>
	T EspStackValueAs( int delta ) const{
		return *(T*)(void*)(esp_ + delta);
	}
	template<int P_N, typename T>
	T EspParamAs() const{
		static_assert(P_N >= 1 && P_N < 100, "");
		return *(T*)(void*)(esp_ + P_N * 4);
	}
	//得到函数返回地址
	pt_pvoid GetFnRetAddrAsEsp() const{
		return *(pt_pvoid*)(void*)esp_;
	}
	//基于ebp的栈中的元素的寻址
	template<typename T>
	T EbpStackValueAs(int delta) const{
		return *(T*)(void*)(ebp_ + delta);
	}
	template<int P_N, typename T>
	T EbpParamAs() const{
		static_assert(P_N >= 1 && P_N < 100, "");
		return *(T*)(void*)(ebp_ + P_N * 4 + 4);
	}
	template<typename T>
	void EspModifyStackValue(int delta, const T& v){
		*(T*)(void*)(esp_ + delta) = v;
	}
	template<int P_N, typename T>
	void EspModifyParam(const T& v){
		static_assert(P_N >= 1 && P_N < 100, "");
		*(T*)(void*)(esp_ + P_N * 4) = v;
	}
	template<typename T>
	void EbpModifyStackValue(int delta, const T& v){
		*(T*)(void*)(ebp_ + delta) = v;
	}
	template<int P_N, typename T>
	void EbpModifyParam(const T& v){
		static_assert(P_N >= 1 && P_N < 100, "");
		*(T*)(void*)(ebp_ + P_N * 4 + 4) = v;
	}
	void Cpuid(int eax_param);
	void AssignRegisterABCD(const JmpHookerAmbient& rhs);

private:
	friend class HookerFactory;
};
static_assert(sizeof(JmpHookerAmbient) == 4 + 0x20, "");

class IHooker{
public:
	IHooker() = default;
	IHooker(const IHooker&) = delete;
	IHooker& operator = (const IHooker&) = delete;
	operator void*(){
		return GetCodeOrig();
	}
	virtual ~IHooker();
	//get hooked API origin
	virtual PROC GetApiOrig();
	//code hooker,hooked code addr
	virtual void* GetHookedCodeAddr();
	//code hooker,next execute code addr
	//origin code addr
	//被hook住的指令被搬到返回地址处了
	virtual void* GetCodeOrig();
	//是否有效
	virtual bool IsValid() = 0;
	//设置是否恢复原来的代码
	virtual void SetResume(bool is_resume) = 0;
};
typedef boost::shared_ptr<IHooker> HookerPtr;

//////////////////////////////////////////////////////////////////////////
#define _HOOKER_COMMON_PRE_DEF_1	\
	static IHooker* hooker = nullptr;	\
	static auto g_call_back = call_back;	\
	void* label_addr;	\
	_asm mov label_addr, offset LABLE_DETOUR;	\
/**/
#define _HOOKER_COMMON_PRE_DEF_2	\
	static auto g_call_back = call_back;	\
	void* label_addr;	\
	_asm mov label_addr, offset LABLE_DETOUR;	\
/**/
#define _HOOKER_COMMON_ASM_CODE_PRE	\
		goto LABLE_RET;		\
	LABLE_DETOUR:		\
		/*第一条指令只能是_asm pushad，切记*/	\
		__asm	\
		{	\
			__asm pushad	\
			__asm pushfd	\
			/*变量*/		\
			__asm push eax	\
			/*压入第四个参数*/	\
			__asm mov eax, esp	\
			__asm add eax, 4	\
			__asm push eax	\
		}	\
/**/
#define _HOOKER_COMMON_ASM_CODE_END	\
		__asm	\
		{	\
			/*弹出第四个参数*/		\
			__asm pop ebx	\
			/*保存跳转地址到变量中*/		\
			__asm mov [esp], eax		\
			/*释放变量*/		\
			__asm add esp, 4	\
			__asm popfd		\
			__asm popad		\
			/*从释放的变量中跳转*/		\
			__asm jmp [esp - 28h]		\
		}	\
		assert(!"不能执行到这里");	\
/**/
//////////////////////////////////////////////////////////////////////////
//用作hook api
void SetDetourHook_(const char* hooker_name, void* hooked_addr, const void* detour_addr, void** code_orig);
template<typename T>
void SetDetourHook(const char* hooker_name, T hooked_addr, const T detour_addr, T* code_orig){
	SetDetourHook_(hooker_name, (void*)hooked_addr, (void*)detour_addr, (void**)(void*)code_orig);
}
template<typename T>
struct stDetourHooker{
	T			hooked_addr_;
	T			detour_addr_;
	T			code_orig_;

	stDetourHooker(T hooked_addr = nullptr, T detour_addr = nullptr){
		hooked_addr_ = hooked_addr;
		detour_addr_ = detour_addr;
		code_orig_ = nullptr;
	}
};
template<typename T>
bool SetDetourHook(const char* hooker_name, stDetourHooker<T>& hooker_info){
	SetDetourHook<T>(hooker_name, hooker_info.hooked_addr_, hooker_info.detour_addr_, &hooker_info.code_orig_);
	if (!hooker_info.code_orig_)
	{
		assert(false);
		return false;
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////
#define HOOKER_CreateCodeHooker(hooker_name, hooked_addr, call_back) \
	struct stHookerTag_ ## hooker_name{};	\
	HookerFactory::GetMe().CreateCodeHooker<stHookerTag_ ## hooker_name>(#hooker_name, (void*)(hooked_addr), call_back)
/**/
#define HOOKER_SetCodeHooker(hooker_name, hooked_addr, call_back) \
	struct stHookerTag_ ## hooker_name{};	\
	HookerFactory::GetMe().SetCodeHooker<stHookerTag_ ## hooker_name>(#hooker_name, (void*)(hooked_addr), call_back)
/**/
//////////////////////////////////////////////////////////////////////////
class HookerFactory{
	HookerFactory();
	HookerFactory& operator = ( const HookerFactory& );
public:
	//singleton
	static HookerFactory& GetMe();
	//API hooker
	HookerPtr CreateApiHooker(PCSTR pszCalleeModName, PCSTR pszFuncName, 
		PROC pfnHook, bool is_resume = true);
	//code hooker
	HookerPtr CreateCodeHooker(const char* hooker_name, void* hooked_addr, void* detour_addr, bool is_resume = true);
	//code hooker,返回一个jmp的地址
	template<void* hooked_addr, typename FnT>
	HookerPtr CreateCodeHooker(const char* hooker_name, const FnT& call_back, bool is_resume = true){
		_HOOKER_COMMON_PRE_DEF_1
		auto hook_ptr = CreateCodeHooker(hooker_name, hooked_addr, label_addr, is_resume);
		if (hook_ptr)
			hooker = hook_ptr.get();
		_HOOKER_COMMON_ASM_CODE_PRE
			SafeExec(g_call_back, *hooker, 0);
		_HOOKER_COMMON_ASM_CODE_END
LABLE_RET:
		return hook_ptr;
	}
	template<typename Tag, typename FnT>
	HookerPtr CreateCodeHooker(const char* hooker_name, void* hooked_addr, const FnT& call_back, bool is_resume = true){
		_HOOKER_COMMON_PRE_DEF_1
		auto hook_ptr = CreateCodeHooker(hooker_name, hooked_addr, label_addr, is_resume);
		if (hook_ptr)
			hooker = hook_ptr.get();
		_HOOKER_COMMON_ASM_CODE_PRE
			SafeExec(g_call_back, *hooker, 0);
		_HOOKER_COMMON_ASM_CODE_END
LABLE_RET:
		return hook_ptr;
	}
	//hooker的释放可由用户控制
	//hooker的清理工作最迟是deattach dll
	//若想执行一次就释放code hook，可调用HookerPtr::reset
	template<void* hooked_addr, typename FnT>
	void SetCodeHooker(const char* hooker_name, const FnT& call_back, bool is_resume = true){
		_HOOKER_COMMON_PRE_DEF_2
		static auto hook_ptr(CreateCodeHooker(hooker_name, hooked_addr, label_addr, is_resume));
		_HOOKER_COMMON_ASM_CODE_PRE
			SafeExec(g_call_back, *hook_ptr, 0);
		_HOOKER_COMMON_ASM_CODE_END
LABLE_RET:
		return;
	}
	template<typename Tag, typename FnT>
	void SetCodeHooker(const char* hooker_name, void* hooked_addr, const FnT& call_back, bool is_resume = true){
		_HOOKER_COMMON_PRE_DEF_2
		static auto hook_ptr(CreateCodeHooker(hooker_name, hooked_addr, label_addr, is_resume));
		_HOOKER_COMMON_ASM_CODE_PRE
			SafeExec(g_call_back, *hook_ptr, 0);
		_HOOKER_COMMON_ASM_CODE_END
LABLE_RET:
		return;
	}

private:
	static void* GetTheHookCodeOrig(IHooker& the_hook){
		return the_hook.GetCodeOrig();
	}
	static void* GetTheHookCodeOrig(HookerPtr& the_hook){
		assert(the_hook);
		return the_hook->GetCodeOrig();
	}
	template<typename T, typename U>
	static void* SafeExecImpl(const T& the_call, U& the_hook, JmpHookerAmbient& ambient){
		try{
			return the_call(the_hook, ambient);
		}
		DO_CATCH_ALL_EXCEPTION({
			LOG_ENTER_FUNC;
			LOG_O(Log_fatal) << "hkept1";
			assert(false);
			return GetTheHookCodeOrig(the_hook);
		})
	}
	//参数必须是引用，切记
	template<typename T, typename U>
	static void* __stdcall SafeExec(const T& the_call, U& the_hook, int dummy_value, ...){
		va_list ap;
		va_start(ap, dummy_value);
		auto ambient_dummy_ptr = (JmpHookerAmbient*)((char*)(void*)va_arg(ap, int));
		__try{
			return SafeExecImpl(the_call, the_hook, *ambient_dummy_ptr);
		}
		__except (HandleCrash(EXCEPTION_EXECUTE_HANDLER)){
			auto f = [](){
				LOG_O(Log_debug) << "SafeExec之出现了C++未能捕获的SEH异常";
			};
			f();
			assert(false);
		}
		return GetTheHookCodeOrig(the_hook);
	}
};

class HookerPtrStored{
public:
	void StoreHook(const HookerPtr& hook_ptr);
	void Clear();

	std::set<HookerPtr>		hooks_;
};

//////////////////////////////////////////////////////////////////////////
class HookSomeThing{
public:
	enum enAntiCpuidRes{
		kACR_RetNormal,
		kACR_RetPassCpuid,
	};

public:
	HookSomeThing() = default;
	HookSomeThing(const HookSomeThing&) = delete;
	HookSomeThing& operator = (const HookSomeThing&) = delete;
	static HookSomeThing& GetMe();
	void SetMachineCodeBased(const std::string& machine_code_based);
	template<int tagHook>
	void AntiCpuid(pt_pcvoid cpuid_addr) const{
		assert(cpuid_addr);
		//0xa20f==cpuid
		assert(*(unsigned short*)cpuid_addr == 0xa20f);		
		HookerFactory::GetMe().SetCodeHooker<boost::mpl::int_<tagHook> >("_Hooker_ForAntiCpuid_", (void*)(cpuid_addr), _Hooker_ForAntiCpuid_);
	}
	template<int tagHook>
	void AntiCpuid(pt_dword cpuid_addr) const{
		return AntiCpuid<tagHook>((pt_pcvoid)cpuid_addr);
	}

public:
	enAntiCpuidRes DoAntiCpuid(JmpHookerAmbient& ambient);

private:
	static void* _Hooker_ForAntiCpuid_(IHooker& hooker, JmpHookerAmbient& ambient);

private:
	pt_dword GetDummyCpuid(pt_dword cpuid, int idx) const;

private:
	std::string		machine_code_based_;
};
//////////////////////////////////////////////////////////////////////////

#undef _HOOKER_COMMON_PRE_DEF_1
#undef _HOOKER_COMMON_PRE_DEF_2
#undef _HOOKER_COMMON_ASM_CODE_PRE
#undef _HOOKER_COMMON_ASM_CODE_END

#pragma warning(pop)
