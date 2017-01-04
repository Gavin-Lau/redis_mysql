/**
 * the main func of 
 */

//must be first of all
#ifdef _WIN32
# define NO_QFORKIMPL 
# include <Win32_Interop\win32fixes.h>
#endif

//lib redis
#if defined(_MSC_VER)
# ifdef _DEBUG
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

#include "redissql.h"
#include "sqlconnector.h"
#include "common.h"
#include "loader.h"


using std::cout;
using std::endl;

int main()
{
	SQLConnector sHandle;
	sHandle.conn("192.168.200.102","root","lgk","test",3306);
	RDSConnector rHandle;
	rHandle.conn("127.0.0.1", 6379, 5.0);
	cacheTable(sHandle, rHandle, "student", "id");
	//equal to sql "SELECT english FROM scole WHERE id = 3";
	//if not find in redis this row will be cached(mysql->redis)
	cout << get(sHandle, rHandle, "scole", "id", "3", "english");
	getchar();
	return 0;
}
