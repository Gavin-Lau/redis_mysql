#include "sqlconnector.h"

#include <errmsg.h>

SQLConnector::SQLConnector() :
	sqlconn(NULL),
	errnum(0)
{
	sqlconn = mysql_init(sqlconn);
	if (!sqlconn) {
		errnum = SQL_INIT_FAIL;
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
	if (SQL_INIT_FAIL == errnum)
	{
		errnum = SQL_ERROR_UNKOWN;
		return;
	}

	sqlconn = mysql_init(sqlconn);
	if (!sqlconn) {
		errnum = SQL_INIT_FAIL;
	}
}

void SQLConnector::conn(const std::string host, const std::string user, 
		const std::string pwd, const std::string db, unsigned short port)
{
	if (errnum < 0)
	{
		errnum = SQL_ERROR_UNKOWN;
		return;
	}
	if (!mysql_real_connect(sqlconn, host.c_str(), user.c_str(),
			pwd.c_str(), db.c_str(), port, NULL, CLIENT_FOUND_ROWS))
	{
		errnum = SQL_CONN_FAIL;
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


int SQLConnector::query(const char* sqlstr, SQL_DIRECTION direct)
{
	int ret = mysql_query(sqlconn, sqlstr);
	if (!ret)
	{
		errnum = -1002; //query-error
		errmsg = "query error" + getSQLerrstr();
		return errnum;
	}
	queryCheck(ret);
	if (direct == SQL_DIRECTION::SQL_READ)
	{
		MYSQL_RES *result = mysql_store_result(sqlconn);
		if (result == NULL)
		{
			unsigned int fieldCount = mysql_field_count(sqlconn);
			if (fieldCount == 0) { //error occur
				errnum = -10023;
				errmsg = "get sql result fail";
				return errnum;
			} else {
				errnum = -1100;
				errmsg = "no result available";
				return 0;
			}
		}
		table.init(result);
	}
}

int SQLConnector::query(const char* sqlstr, unsigned long len,SQL_DIRECTION direct)
{
	return 0;
}

void SQLConnector::queryCheck(int ret)
{
	switch (ret) {
		
	case CR_COMMANDS_OUT_OF_SYNC :
		errnum = -105; //query err
		errmsg = "Commands were executed in an improper order";
		break;
	case CR_SERVER_GONE_ERROR :
	case CR_SERVER_LOST:
		errnum = -106; //connect lost
		errmsg = "connection lost";
		break;
	default:
		errnum = -200; //unkown error
		errmsg = " unkown error";
		break;
	}
}

/* SQL table */
SQLtable::SQLtable() :
	colCount(-1),
	rowCount(-1)
{	
}

void SQLtable::init(MYSQL_RES* result)
{
	//step0: clear table
	clear();
	colCount = mysql_num_fields(result);
	rowCount = mysql_num_rows(result);
	MYSQL_ROW sql_row;
	MYSQL_FIELD* sql_field;
	//step1: field name
	while (sql_field = mysql_fetch_field(result))
	{
		colNames.push_back(std::string(sql_field->name, sql_field->name_length));
	}
	//step2: rows of values
	while (sql_row = mysql_fetch_row(result))
	{
		SQLrow* rowObj = new SQLrow();
		unsigned long *lengths;
		lengths = mysql_fetch_lengths(result);
		for (int i = 0; i < colCount ; i++)
		{
			rowObj->push_back(std::string(sql_row[i], lengths[i]));
		}
		table.push_back(rowObj);
	};
}

SQLtable::~SQLtable()
{
	clear();
}

unsigned int SQLtable::getColNum()
{
	return colCount;
}

unsigned int SQLtable::getRowNum()
{
	return rowCount;
}

const std::string SQLtable::getColName(unsigned int col)
{
	if (col < 0 || col >= colCount) return "";

	return colNames[col];
}

SQLtable::SQLrow* SQLtable::getRow(unsigned int row)
{
	if (row < 0 || row >= rowCount) return NULL;

	return table[row];
}

const std::string SQLtable::get(unsigned int row, unsigned int col)
{
	if (col < 0 || col >= colCount || row < 0 || row <= rowCount) return "";

	SQLrow* rowobj = table[row];
	return rowobj->at(col);
}

const std::string SQLtable::get(unsigned int row, const std::string key)
{
	if (row < 0 || row >= rowCount) return "";

	int colPos = -1;
	for (int i = 0; i < colCount; ++i)
	{
		if (key == colNames[i]) colPos = i;
	}

	if (colPos < 0) return "";

	SQLrow* rowObj = table[row];
	return rowObj->at(colPos);
}

void SQLtable::clear()
{
	for (int i = 0; i < rowCount; ++i)
	{
		delete table[i];
		table[i] = NULL;
	}
}
