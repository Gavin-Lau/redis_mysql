#include <iostream>

using std::cout;
using std::endl;

#ifdef _MSC_VER
# pragma comment(lib,"libmysql.lib")
#endif

#include "sqlconnector.h"

#include <mysql.h>

int main(int argc, char** argv)
{
	SQLConnector sqlconn;	
	sqlconn.conn("192.168.200.102", "root", "lgk", "test", 9906);

	getchar();
	return 0;
}
