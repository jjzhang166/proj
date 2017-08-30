#pragma once
/*
@author zhp
@date 2016/5/3 10:32
@purpose class info for reg
*/
#include "../BC_Config.h"
#include <map>
#include <vector>
#include <memory>
#include "../BC_Function.h"

namespace bufcall{
	namespace Detail{
		template<typename... Bases>
		struct ClassBase;
	}
	class ClassProxy{
		template<typename ClassT, typename... Bases>
		friend struct class_;
		template<typename... Bases>
		friend struct Detail::ClassBase;

		typedef std::shared_ptr<BC_Function> FunctionT;
		struct stFuncInfo{
			FunctionT		func_impl_;
#ifdef _DEBUG
			const char*		func_name_;
#endif
		};
		typedef std::map<::Detail::BC_Uuid, stFuncInfo> MemFnsT;
		typedef std::pair<ClassId, const ClassProxy*> BaseClsInfo;
		typedef std::vector<BaseClsInfo> BasesT;

	public:
		explicit ClassProxy(pt_csz cls_name);
		virtual ~ClassProxy(){}
		const stFuncInfo* FindMemFn(const ::Detail::BC_Uuid& fn_id) const;
		pt_csz BC_GetClassName() const;

	private:
		void AddBaseClass(ClassId cls_id, const ClassProxy* base);
		void AddMemFn(pt_csz mem_fn_name, BC_Function* fn);

	private:
		mutable BasesT	base_;
		MemFnsT			mem_fns_;
#ifdef _DEBUG
		pt_csz			cls_name_;
#endif
	};

	class ClassProxyMgr{
		typedef std::shared_ptr<ClassProxy> ClassProxyPtr;

		ClassProxyMgr() = default;
		ClassProxyMgr(const ClassProxyMgr&) = delete;
		ClassProxyMgr& operator=(const ClassProxyMgr&) = delete;
	public:
		void AddClass(ClassId class_id, ClassProxy* class_proxy);
		ClassProxy* FindClass(ClassId class_id) const;
		static ClassProxyMgr& GetMe();

	private:
		std::map<ClassId, ClassProxyPtr>	class_proxy_;
		std::vector<RegClassFnT>			reg_cls_fns_;
	};

	struct BC_RegClassImpl{
		BC_RegClassImpl(RegClassFnT f);
	};

	namespace Detail{
		template<typename Base, typename... Bases>
		struct ClassBase<Base, Bases...> : ClassBase<Bases...>{
			static void AddBase(ClassProxy& class_proxy){
				auto cls_id = BC_GetTypeId<Base>();
				class_proxy.AddBaseClass(cls_id, ClassProxyMgr::GetMe().FindClass(cls_id));
				ClassBase<Bases...>::AddBase(class_proxy);
			}
		};
		template<>
		struct ClassBase<>{
			static void AddBase(ClassProxy&){}
		};
	}
	template<typename ClassT, typename... Bases>
	struct class_ : Detail::ClassBase<Bases...>{
		typedef ClassT derived_type;
		typedef Detail::ClassBase<Bases...> base_type;
		explicit class_( pt_csz cls_name ){
			class_proxy_ = new ClassProxy(cls_name);
			if (!class_proxy_)
			{
				assert(false);
				return;
			}
			base_type::AddBase(*class_proxy_);
			ClassProxyMgr::GetMe().AddClass(Detail::BC_GetTypeId<derived_type>(), class_proxy_);
		}
		class_() : class_(nullptr){}

		template<typename F>
		class_& def(pt_csz mem_fn_name, F f){
			if (!class_proxy_)
			{
				assert(false);
				return *this;
			}
			class_proxy_->AddMemFn(mem_fn_name, ::Detail::BC_MakeFunction(f));
			return *this;
		}

	private:
		ClassProxy*		class_proxy_;
	};
}	//bufcall