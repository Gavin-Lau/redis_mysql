#include "sqlconnector.h"

#include <errmsg.h>

SQLConnector::SQLConnector() :
	sqlconn(NULL),
	errnum(OK)
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
	if (errnum < 0)  return;

	sqlconn = mysql_init(sqlconn);
	if (!sqlconn) {
		errnum = SQL_INIT_FAIL;
	}
}

void SQLConnector::conn(const std::string host = "127.0.0.1", const std::string user = "root", 
		const std::string pwd="lgk", const std::string db = "test", unsigned short port = 3306)
{
	if (errnum < 0)  return;
	
	if (!mysql_real_connect(sqlconn, host.c_str(), user.c_str(),
			pwd.c_str(), db.c_str(), port, NULL, CLIENT_FOUND_ROWS))
	{
		errnum = SQL_CONN_FAIL;
	}
	printf("Connection character set: %s\n", mysql_character_set_name(sqlconn));
}

int SQLConnector::query(const std::string& sqlstr, SQL_DIRECTION direct)
{
	//mysql doc said:
	//	mysql_real_query() is faster than mysql_query() because it does not call strlen() on the statement string.
	int ret = mysql_real_query(sqlconn, sqlstr.c_str(), sqlstr.length());
	if (ret)
	{
		queryCheck(ret);
		return errnum;
	}

	if (direct == SQL_DIRECTION::SQL_WRITE)	return mysql_affected_rows(sqlconn);//sql write statement end

	MYSQL_RES *result = mysql_store_result(sqlconn);
	if (result == NULL)
	{
		errnum = SQL_QUERY_ERROR;
		return errnum;
	} else {
		int fieldCount = mysql_field_count(sqlconn);
		if (fieldCount == 0) { //error occur
			errnum = SQL_QUERY_ERROR;
			return errnum;
		}
		else {
			int rowCount = mysql_num_rows(result);
			if (rowCount <= 0)
			{
				errnum = SQL_QUERIED_NOTHING; //no error occurred ,but queried nothing.
				return OK;
			}
		}
	}
	table.parse(result);
	if (table.getErrorNum() < 0)
		errnum = SQL_PARSE_TABLE_ERR;
	return errnum;
}

void SQLConnector::queryCheck(int ret)
{
	switch (ret) {
		
	case CR_COMMANDS_OUT_OF_SYNC :
		errnum = SQL_IMPROPER_ORDER; //query err
		break;

	case CR_SERVER_GONE_ERROR :
	case CR_SERVER_LOST:
		errnum = SQL_CONN_LOST; //connect lost
		break;

	default:
		errnum = UNKOWN_ERROR;
		break;
	}
}

/******************************************************************************/
/* SQL table */
int SQLtable::errnum = OK;


void SQLtable::parse(MYSQL_RES* result)
{
	//step0: clear table
	clear();
	colCount = mysql_num_fields(result);
	rowCount = mysql_num_rows(result);
	if (colCount < 0 || rowCount < 0)
	{
		errnum = SQL_PARSE_TABLE_ERR;
		return;
	}

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
		unsigned long *lengths = mysql_fetch_lengths(result);
		for (int i = 0; i < colCount ; i++)
		{
			rowObj->push_back(std::string(sql_row[i], lengths[i]));
		}
		table.push_back(rowObj);
	};
}

std::string SQLtable::parse(MYSQL_RES* result, int col, int row)
{
	MYSQL_ROW sqlRow;
	unsigned int numFields = mysql_num_fields(result);
	unsigned int numRows = mysql_num_rows(result);
	if (numFields < 0 || numRows < 0 || 
			col > numFields || row > numRows)
	{
		errnum = SQL_PARSE_TABLE_ERR;
		return "";
	}
	
	MYSQL_ROW sql_row;
	MYSQL_FIELD* sql_field;
	mysql_data_seek(result, col); //goto the specific row.
	while (sql_row = mysql_fetch_row(result))
	{
		unsigned long *lengths = mysql_fetch_lengths(result);
		return std::string(sql_row[col], lengths[col]);
	}
}


const std::string SQLtable::getColName(int col)
{
	if (col < 0 || col >= colCount)
	{
		errnum = SQL_OUT_RANGE;
		return "";
	}

	return colNames[col];
}

SQLtable::SQLrow* SQLtable::getRow(int row)
{
	if (row < 0 || row >= rowCount)
	{
		errnum = SQL_OUT_RANGE;
		return NULL;
	}

	return table[row];
}

const std::string SQLtable::get(int row, int col)
{
	if (col < 0 || col >= colCount ||
		row < 0 || row >= rowCount)
	{
		errnum = SQL_OUT_RANGE;
		return "";
	}

	SQLrow* rowobj = table[row];
	return rowobj->at(col);
}

const std::string SQLtable::get(int row, const std::string key)
{
	if (row < 0 || row >= rowCount)
	{
		errnum = SQL_OUT_RANGE;
		return "";
	}

	int colPos = -1;
	for (int i = 0; i < colCount; ++i)
	{
		if (key == colNames[i])
		{
			colPos = i;
			break;
		}
	}

	if (colPos < 0)
	{
		errnum = SQL_OUT_RANGE;
		return "";
	}


	SQLrow* rowObj = table[row];
	return rowObj->at(colPos);
}

void SQLtable::clear()
{
	errnum = OK;
	rowCount = 0;
	colCount = 0;
	colNames.clear();
	for (int i = 0; i < rowCount; ++i)
	{
		delete table[i];
		table[i] = NULL;
	}
	table.clear();
}
