/**
 * the main func of write memory cached database 
 * to disk database.
 */

#ifdef _WIN32
# define NO_QFORKIMPL
# include <Win32_Interop\win32fixes.h>
#endif

#include <iostream>

#include "redissql.h"
#include "sqlconnector.h"



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

using std::cout;
using std::endl;

int main()
{

	getchar();
	return 0;
}
