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
	sqlconn.conn("localhost", "gavin", "lgk", "test1", 3306);

	getchar();
	return 0;
}
