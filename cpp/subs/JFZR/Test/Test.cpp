// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Test.h"
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/stream_buffer.hpp>
#include <Common/LogOut.h>
#include <Common/UsefulFunc.h>
#include <Common/Hooker.h>


/*
#include <Common/LogOut.h>
#include <common/UsefulFunc.h>
#include <Common/UsefulClass.h>
#include <random>
#include <boost/property_tree/ptree.hpp>
#include <boost/flyweight.hpp>
#include <boost/regex.hpp>
#pragma comment(lib, "public.lib")*/
//using namespace std;
//using namespace boost;

std::string CheckSafeLpk(){
	std::ifstream stm_lpk("f:\\tmp\\lpkb.dll", std::ios_base::in | std::ios_base::binary);
	return GenUuidStr(stm_lpk);
}

int main(){
	auto str_tmp = CheckSafeLpk();
	//LoadLibrary(_T("d3d9.dll"));
	::system("pause");
	return 0;

}
