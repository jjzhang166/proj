#include <boost/python.hpp>
#include "_idaex.h"
#include <iostream>
#include <pro.h>
#include <netnode.hpp>


using namespace boost::python;

//////////////////////////////////////////////////////////////////////////
void Test(){
	object main_module = import("__main__");
	object main_namespace = main_module.attr("__dict__");
	/*exec("print '123456'",
		main_namespace);*/
	//exec("result = 5 ** 2", main_namespace);
	//int five_squared = extract<int>(main_namespace["result"]);
	//cout << "The five_squeared caculated by python is " << five_squared << endl;

	// Load the sys module.
	//object sys = import("sys");

	//// Extract the python version.
	//std::string version = extract<std::string>(sys.attr("version"));
	//std::cout << version << std::endl;


	//要求simple.py与可执行文件在相同路径下! 运行ok
	object simple = exec_file(R"(F:\proj\python\AUT\test.py)", main_namespace);
	//dict global;
	//object result = exec_file("simple.py", global, global);
	
}

//////////////////////////////////////////////////////////////////////////
extern "C" __declspec(dllexport) nodeidx_t GetNetnodeId(const netnode* node){
	if (node)
		return *node;
	return BADNODE;
}

//////////////////////////////////////////////////////////////////////////

BOOST_PYTHON_MODULE(_idaex)
{
	using namespace boost::python;
	/*class_<stTest>("stTest")
	.def("Test", &stTest::Test);
	def("TestPtr", TestPtr, arg("data"));
	def("Test1", Test1, return_value_policy<return_opaque_pointer>());*/
}

bool Init(){
	/*try
	{
		Test();
	}
	catch (...)
	{
		PyErr_Print();
		PyErr_Clear();
	}*/
	return true;
}


