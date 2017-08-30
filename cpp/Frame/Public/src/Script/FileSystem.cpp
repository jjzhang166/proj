#include "stdafx.h"
#include <Script/FileSystem.h>
#include <boost/filesystem/path.hpp>


void RegFileSystemToLua(lua_State* l)
{
	using namespace luabind;
	using namespace boost::filesystem;
	typedef path& (path::*Fn)(const char* const&);
	typedef path& (path::*Fn1)(const char*);
	typedef const std::string(path::*Fn2)() const;
	typedef int(path::*Fn3)(const path&) const;
	typedef int(path::*Fn4)(const std::string&) const;
	module(l, "filesystem")[
			class_<path>("path")
			.def(constructor<>())
			.def(constructor<const path&>())
			.def(constructor<const char*>())
			.def(constructor<const std::string&>())
			.def("assign", (Fn)&path::operator=<const char* const&>)
			.def("concat", (Fn)&path::concat<const char* const&>)
			.def("append", (Fn)&path::append<const char* const&>)
			.def("clear", &path::clear)
			.def("make_preferred", &path::make_preferred)
			.def("remove_filename", &path::remove_filename)
			.def("remove_trailing_separator", &path::remove_trailing_separator)
			.def("replace_extension", &path::replace_extension)
			.def("swap", &path::swap)
			.def("size", &path::size)
			.def("string", (Fn2)&path::string)
			.def("generic", &path::generic)
			.def("generic_string", (Fn2)&path::generic_string)
			.def("compare", (Fn3)&path::compare)
			.def("compare", (Fn4)&path::compare)
			.def("root_path", &path::root_path)
			.def("root_name", &path::root_name)
			.def("root_directory", &path::root_directory)
			.def("relative_path", &path::relative_path)
			.def("parent_path", &path::parent_path)
			.def("filename", &path::filename)
			.def("stem", &path::stem)
			.def("extension", &path::extension)
			.def("empty", &path::empty)
			.def("has_root_path", &path::has_root_path)
			.def("has_root_name", &path::has_root_name)
			.def("has_root_directory", &path::has_root_directory)
			.def("has_relative_path", &path::has_relative_path)
			.def("has_parent_path", &path::has_parent_path)
			.def("has_filename", &path::has_filename)
			.def("has_stem", &path::has_stem)
			.def("has_extension", &path::has_extension)
			.def("is_relative", &path::is_relative)
			.def("is_absolute", &path::is_absolute)
			.def("lexically_normal", &path::lexically_normal)
			.def("lexically_relative", &path::lexically_relative)
			.def("lexically_proximate", &path::lexically_proximate)
			.def("normalize", &path::normalize)
			.def("remove_leaf", &path::remove_leaf)
			.def("leaf", &path::leaf)
			.def("branch_path", &path::branch_path)
			.def("has_leaf", &path::has_leaf)
			.def("has_branch_path", &path::has_branch_path)
			.def("is_complete", &path::is_complete)
		];
}
