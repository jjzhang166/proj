#include "_idaex.h"
#include <pro.h>
#include <netnode.hpp>
#include <iostream>


#pragma comment(lib,"ida.lib")
#pragma comment(lib,"pro.lib")

//////////////////////////////////////////////////////////////////////////
extern "C" __declspec(dllexport) nodeidx_t GetNetnodeId(const char* node_name){
	return netnode(node_name);
}

//////////////////////////////////////////////////////////////////////////

bool Init(){
	return true;
}


