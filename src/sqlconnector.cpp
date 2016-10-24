#include "sqlconnector.h"

SQLConnector::SQLConnector() :
	sqlconn(NULL),
	errorno(0)
{
	sqlconn = mysql_init(sqlconn);
	if (!sqlconn) {
		errorno = SQL_INIT_FAIL;
	}
}

SQLConnector::~SQLConnector()
{
	mysql_close(sqlconn);
}

MYSQL* SQLConnector::getSQLHandle()
{
	return sqlconn;
}

void SQLConnector::init()
{
	if (SQL_INIT_FAIL == errorno)
	{
		errorno = SQL_ERROR_UNKOWN;
		return;
	}

	sqlconn = mysql_init(sqlconn);
	if (!sqlconn) {
		errorno = SQL_INIT_FAIL;
	}
}

void SQLConnector::conn(const std::string host, const std::string user, 
		const std::string pwd, const std::string db, unsigned short port)
{
	if (errorno < 0)
	{
		errorno = SQL_ERROR_UNKOWN;
		return;
	}
	if (!mysql_real_connect(sqlconn, host.c_str(), user.c_str(),
			pwd.c_str(), db.c_str(), port, NULL, CLIENT_FOUND_ROWS))
	{
		errorno = SQL_CONN_FAIL;
	}
	printf("Connection character set: %s\n", mysql_character_set_name(sqlconn));
}

const std::string SQLConnector::getSQLerrstr()
{
	return mysql_error(sqlconn);
}

int SQLConnector::getSQLerrno()
{
	return mysql_errno(sqlconn);
}

int SQLConnector::query(const char* sqlstr)
{
	mysql_query(sqlconn, sqlstr);
}

int SQLConnector::query(const char* sqlstr, unsigned long len)
{
	mysql_real_query(sqlconn, sqlstr, len);
}

