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
	conn.query("set key1 %s", "value1");
	getchar();
	return 0;
}