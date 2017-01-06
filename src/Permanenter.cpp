/**
 * the main func of write memory cached database 
 * to disk database.
 */

#ifdef _WIN32
# define NO_QFORKIMPL
# include <Win32_Interop\win32fixes.h>
#endif

//lib redis
#if defined(_MSC_VER)
# if defined(_DEBUG) || defined(DEBUG)
#  pragma  comment(lib, "hiredisd.lib")
#  pragma  comment(lib, "Win32_Interopd.lib")
# else
#  pragma  comment(lib, "hiredis.lib")
#  pragma  comment(lib, "Win32_Interop.lib")
# endif
#endif

//lib mysql
#ifdef _MSC_VER
# pragma comment(lib,"libmysql.lib")
#endif


#include <iostream>

#include "common.h"
#include "redissql.h"
#include "sqlconnector.h"
#include "saver.h"

using std::cout;
using std::endl;

int main(int argc, char** argv)
{
	SQLConnector sHandle;
	sHandle.conn("192.168.200.102", "root", "lgk", "test", 3306);
	RDSConnector rHandle;
	rHandle.conn("127.0.0.1", 6379, 5.0);

	while (1)
	{
		saveDirtyRows(sHandle, rHandle, "scole", "id");
		cout << "saving data from redis->mysql";
		secSleep(10 * 60); //10 min
	}
	getchar();
	return 0;
}
