#ifndef LUABIND_FROMLUAHANDLER_HPP
#define LUABIND_FROMLUAHANDLER_HPP
/*
@author zhp
@date 2016/12/4 19:26
@purpose from lua handle
*/
#include <luabind/handle.hpp>
#include <luabind/WeakSelf.hpp>

namespace luabind{
	namespace adl{
		class object;
	}
	namespace detail{
		//by zhp,2016/12/4 14:17
		class IFromLuaHandle{
		public:
			virtual ~IFromLuaHandle(){}
			virtual void AddLuaBase(const adl::object& base);
			virtual int DoIndex(lua_State* l, int self, int k) const;
		};

		class FromLuaHandleImpl{
		protected:
			void AddLuaBaseImpl(const adl::object& base);
			int DoIndexImpl_(lua_State* L, int self, int k) const;
			void ClearAllLuaBase();

		private:
			//GetLuaBase允许lua中调用
			static int GetLuaBase(lua_State* L);
			static int ChooseCall(lua_State* L);

		private:
			handle		lua_bases_;
		};

		class lua_baseImpl : protected FromLuaHandleImpl, public weak_base{
			template <typename T, typename ObjT>
			friend void FromLuaHandleSetter(lua_State* L, const T* p1, const lua_baseImpl* p2, ObjT* obj);
			template<typename T>
			friend void _DoAddLuaBase_(T*, const lua_baseImpl* p, const adl::object& base);
			template<typename T>
			friend int _DoDoIndex_(const T*, const lua_baseImpl* p, lua_State* l, int self, int k);
		protected:
			lua_baseImpl();
			virtual void OnInitLuaBase(lua_State* L);

		private:
			bool	is_init_lua_base_impl_;
		};

		inline void FromLuaHandleSetter(lua_State*, const void*, const void*, ...)
		{}

		template <typename T, typename ObjT>
		void FromLuaHandleSetter(lua_State* L, const T* p1, const lua_baseImpl* p2, ObjT* obj)
		{
			if (p2->is_init_lua_base_impl_)
				return;
			(const_cast<lua_baseImpl*>(p2))->is_init_lua_base_impl_ = true;
			(const_cast<lua_baseImpl*>(p2))->OnInitLuaBase(L);
		}
		inline void _DoAddLuaBase_(const void*, const void*, const adl::object& base){}
		template<typename T>
		void _DoAddLuaBase_(T*, const lua_baseImpl* p, const adl::object& base){
			return (const_cast<lua_baseImpl*>(p))->AddLuaBaseImpl(base);
		}
		inline int _DoDoIndex_(const void*, const void*, lua_State* l, int self, int k){ return 0; }
		template<typename T>
		int _DoDoIndex_(const T*, const lua_baseImpl* p, lua_State* l, int self, int k){
			return p->DoIndexImpl_(l, self, k);
		}
	}
}
#endif	//LUABIND_FROMLUAHANDLER_HPP
