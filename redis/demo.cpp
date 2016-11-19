#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <wchar.h>

#include "redissql.h"


int main()
{
	RDSConnector conn("127.0.0.1",6379, 2.006);
	conn.query("set key1 %s", "value1");
	getchar();
	return 0;
}