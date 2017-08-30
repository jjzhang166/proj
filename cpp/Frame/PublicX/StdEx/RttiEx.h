#pragma once
/*
@author zhp
@date 2017/5/17 13:25
@purpose for rtti
*/
#include <ProtocolX/P_BaseTypeX.h>
#include <typeinfo.h>
#include <functional>
#include <string>
#include "../DependSelfLib.h"

namespace StdEx{
	class RttiEx{
	public:
		typedef std::function<bool(const std::type_info&)> Fn_TraClassHierarchy;

	public:
		static const std::type_info* TypeId(pt_pcvoid obj_addr);
		static const std::type_info* VtblTypeId(pt_pcvoid vtbl);
		static pt_csz TypeName(pt_pcvoid obj_addr);
		static pt_csz VtblTypeName(pt_pcvoid vtbl);
		static pt_csz TypeFullName(pt_pcvoid obj_addr);
		static pt_csz VtblTypeFullName(pt_pcvoid vtbl);
		static pt_csz TypeRawName(pt_pcvoid obj_addr);
		static pt_csz VtblTypeRawName(pt_pcvoid vtbl);
		static int TraBaseClass(pt_pcvoid obj_addr, const Fn_TraClassHierarchy& fn);
		static int VtblTraBaseClass(pt_pcvoid vtbl, const Fn_TraClassHierarchy& fn);
		static pt_csz Name2ShortName(pt_csz full_name);
		static std::string UnDName(pt_csz symbol_name);

	private:
		struct PMD{
			//基类的数据开始位置相对于对象开始位置的偏移。据观察，虚基类的此字段一般为0，采用另外两个字段可取到准确的偏移。非虚基类采用此字段取偏移。
			int				mdisp;		//member displacement
			//虚基类表相对于对象开始位置的偏移。若类没有采用虚拟继承，则该类的对象也没有指向该类的虚基类表的指针，此时，pdisp一般为-1。
			int				pdisp;		//vbtable displacement
			/*
			若pdisp为非负，则说明该类有虚基类表。虚基类表记录了每一个虚基类的数据相对于对象开始位置的偏移。
			vdisp便记录了这一个虚基类对应虚基类表的那一项，其值为一个相对于虚基类表起始位置的偏移。
			我们通过对象的起始位置和pdsip找到虚基类表的位置，再通过虚基类表起始位置和vdisp找到该基类的数据的位置。
			*/
			int				vdisp;		//displacement inside vbtable
		};
		struct TypeDescriptor{
			const std::type_info& AsTypeInfo() const;

			pt_dword		ptrToVTable;
			pt_dword		spare;
			char			name[1];
		};
		static_assert(sizeof(TypeDescriptor) == sizeof(type_info), "");
		struct RTTIBaseClassDescriptor{
			// 指向type_info对象
			TypeDescriptor*							pTypeDescriptor;		//type descriptor of the class
			// 该基类包含的其他基类的个数
			pt_dword								numContainedBases;		//number of nested classes following in the Base Class Array
			// 描述该基类的成员在对象中的位置 
			PMD										where;					//pointer-to-member displacement info
			// 属性
			pt_dword								attributes;				//flags, usually 0
			struct RTTIClassHierarchyDescriptor*	pClassDescriptor;		//网上的资料中竟然没有这个字段？！
		};
		struct RTTIBaseClassArray{
			RTTIBaseClassDescriptor* arrayOfBaseClassDescriptors[1];
		};
		struct RTTIClassHierarchyDescriptor{
			pt_dword				signature;					//always zero?
			// 属性，按位或 
			pt_dword				attributes;					//bit 0 set = multiple inheritance, bit 1 set = virtual inheritance
			// 基类的个数
			pt_dword				numBaseClasses;				//number of classes in pBaseClassArray
			// 指向基类数组结构体的实例
			RTTIBaseClassArray*		pBaseClassArray;
		};
		struct RTTICompleteObjectLocator{
			// 单词的意思是签名，但实际中似乎总是为0 
			pt_dword						signature;			//always zero ?
			// 本部分相对于对象开始的偏移量(一般用来取完整对象的指针)
			pt_dword						offset;				//offset of this vtable in the complete class
			/*
			 cdOffset主要用于使用虚基类的时候。当子类重写了虚基类的虚函数时（至少一个，不算析构函数），
			 编译器在构造对象的内存布局时会为每一个虚基类部分都生成一个叫做vtordisp的成员。那么这个vtordisp是干什么的呢？
			 vtordisp记载了一个偏移，通过虚基类数据的起始地址和offset取对象的起始地址后，还需要加vtordisp的值，才是正确的对象起始地址。
			 cdOffset指明了从虚基类数据起始地址向前（注意这里是向前）偏移多少位可以取到vtordisp。当cdOffset为0表明没有生成这个东西，此时就不用再加上vtordisp。
			 由于这个字段容易产生混淆，而且难以理解，所以在后面的例子中我们都没有用vtordisp，cdOffset都是0。
			 */
			pt_dword						cdOffset;			//constructor displacement offset
			// 指向一个type_info对象
			TypeDescriptor*					pTypeDescriptor;	//TypeDescriptor of the complete class
			// 描述类的继承信息
			RTTIClassHierarchyDescriptor*	pClassDescriptor;	//describes inheritance hierarchy
		};

	private:
		static void* RTCastToVoid(void* inptr);
		static void* RTtypeid(void* inptr);
		static void* RTDynamicCast(void* inptr, long VfDelta, void* SrcType, void* TargetType, bool isReference);

	private:
		static const RTTICompleteObjectLocator* GetCompleteObjectLocator(pt_pcvoid vtbl);
	};
}
