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

int SQLConnector::query(const char* sqlstr, QUERY_DIRECT direct)
{
	int ret = mysql_query(sqlconn, sqlstr);
	if (!ret)
	{
		errorno = -1002; //query-error
		errmsg = "query error" + getSQLerrstr();
		return errorno;
	}
	if (direct == QUERY_DIRECT::READ)
	{
		MYSQL_RES *result = mysql_store_result(sqlconn);
		if (result == NULL)
		{
			unsigned int fieldCount = mysql_field_count(sqlconn);
			if (fieldCount == 0) { //error occur
				errorno = -10023;	
				errmsg = "get sql result fail";
				return errorno;
			} else {
				errorno = -1100;
				errmsg = "no result available";
				return 0;
			}
		}
		table.init(result);
	}
}

int SQLConnector::query(const char* sqlstr, unsigned long len, QUERY_DIRECT direct)
{
	mysql_real_query(sqlconn, sqlstr, len);
}

/* SQL table */
SQLtable::SQLtable()
{

}

void SQLtable::init(MYSQL_RES* result)
{

}

SQLtable::~SQLtable()
{

}

unsigned int SQLtable::getColNum()
{

}

unsigned int SQLtable::getRowNum()
{

}

std::string SQLtable::getColName(unsigned int col)
{

}

SQLtable::SQLrow* SQLtable::getRow(unsigned int row)
{

}

std::string SQLtable::get(unsigned int row, unsigned int col)
{

}

std::string SQLtable::get(unsigned int row, std::string key)
{

}

void clear()
{

}
