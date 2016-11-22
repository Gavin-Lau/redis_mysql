#ifdef _WIN32
# define NO_QFORKIMPL 
# include <Win32_Interop\win32fixes.h>
#endif

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "redissql.h"

#ifdef _DEBUG
# pragma  comment(lib, "hiredisd.lib")
# pragma  comment(lib, "Win32_Interopd.lib")
#else
# pragma  comment(lib, "hiredis.lib")
# pragma  comment(lib, "Win32_Interop.lib")
#endif
int main()
{
	RDSConnector conn("127.0.0.1",6379, 2.006);
	printf("result:%s\n", conn.query("set key1 %s", "value1").c_str());
	printf("result:%s\n", conn.query("get key1").c_str());

	printf("result:%s\n", conn.query("set key2 %s", "1001").c_str());
	printf("result:%s\n", conn.query("get key2").c_str());

	printf("result:%s\n", conn.query("INCR key2").c_str());
	printf("result:%s\n", conn.query("get key2").c_str());
	printf("result:%s\n", conn.query("EXISTS key2").c_str());
	getchar();
	return 0;
}