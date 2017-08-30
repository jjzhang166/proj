#include "stdafx.h"
#include <Winsock2.h>
#include <stdio.h>
#include <iostream>
#include <cassert>

#pragma comment(lib, "ws2_32")   //加截lib

using namespace std;
int main()
{

	int WSA_return;
	WSADATA WSAData;


	WSA_return = WSAStartup(0x0101,&WSAData);
	if ( 0 != WSA_return )
	{
		assert(false);
		return 0;
	}

	HOSTENT *host_entry;

	std::cout << "请输入主机名：";
	char host_name[256] = {0}, buff[20] = {0};
	std::cin >> host_name;
	std::string domain = host_name;
	domain += ".hotyxfz.com";
	do
	{
		host_entry = gethostbyname(domain.c_str());
		if( host_entry != 0 )
		{
			std::cout << "解析IP地址: " << (WORD)(BYTE)host_entry->h_addr_list[0][0] << '.' << (WORD)(BYTE)host_entry->h_addr_list[0][1] << '.' << (WORD)(BYTE)host_entry->h_addr_list[0][2] << '.' << (WORD)(BYTE)host_entry->h_addr_list[0][3];
		}
		else
		{
			std::cout << "error";
		}
		std::cout << std::endl << "press n for exit\n";
		std::cin >> buff;
		if ( buff[0] == 'n' )
			break;
	} while (true);
	
	WSACleanup();
	return 0;
}