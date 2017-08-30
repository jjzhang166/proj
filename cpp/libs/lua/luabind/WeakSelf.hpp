#pragma once
/*
@author zhp
@date 2016/12/6 17:22
@purpose weak ref self
*/
#include <boost/shared_ptr.hpp>

namespace luabind{
	namespace detail{
		class IWeakSelfPtr{
		public:
			virtual ~IWeakSelfPtr(){}
			virtual void Expire() = 0;
		};
		template<typename T> 
		class WeakSelfPtr : public IWeakSelfPtr{
		public:
			explicit WeakSelfPtr(T* p){
				ptr_ = p;
			}
			/*~WeakSelfPtr(){
				delete ptr_;
			}*/
			T* get() const{
				return ptr_;
			}
			void Expire() override final{
				ptr_ = nullptr;
			}

		private:
			T*		ptr_;
		};
	}

	class weak_base{
		template<typename T>
		friend class weak_self_ptr;

	protected:
		weak_base(){
			weak_ref_self_ptr_ = nullptr;
		}

	public:
		virtual ~weak_base(){
			if (weak_ref_self_ptr_)
				weak_ref_self_ptr_->Expire();
		}

	private:
		void SetRefSelfPtr(detail::IWeakSelfPtr* ptr){
			weak_ref_self_ptr_ = ptr;
		}


	private:
		detail::IWeakSelfPtr*		weak_ref_self_ptr_;
	};

	template<typename T>
	class weak_self_ptr{
		typedef boost::shared_ptr<detail::WeakSelfPtr<T> > ImplPtrT;

	public:
		template<typename Y>
		explicit weak_self_ptr(Y* p) : impl_ptr_(new detail::WeakSelfPtr<T>(p)){
			assert(p);
			p->SetRefSelfPtr(impl_ptr_.get());
		}
		T* get() const{
			if (impl_ptr_)
				return impl_ptr_->get();
			return nullptr;
		}

	private:
		ImplPtrT	impl_ptr_;
	};

	template<class T> T * get_pointer(weak_self_ptr<T> const& p)
	{
		return p.get();
	}
	template <class T>
	weak_self_ptr<T const>* get_const_holder(weak_self_ptr<T>*)
	{
		return 0;
	}
}