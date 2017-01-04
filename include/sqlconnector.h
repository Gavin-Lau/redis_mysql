#ifndef _SQLCONNECTOR_H_
#define _SQLCONNECTOR_H_

#include <iostream>
#include <string>
#include <vector>

#include <mysql.h>

#include "common.h"

typedef MYSQL SQL;


class SQLtable {

public:

	typedef std::vector<std::string> SQLrow;
	typedef std::vector<SQLrow* > SQLtab;
	
	SQLtable() : colCount(-1), rowCount(-1) {}
	~SQLtable() { clear(); }
	void parse(MYSQL_RES* result);
	/** this parse for only one specific item by row&col */
	static std::string parse(MYSQL_RES* result, int col, int row); //started 0

	const std::string getColName(int col);
	SQLrow* getRow(int row);
	const std::string get(int row, int col);
	const std::string get(int row, std::string key);
	void clear();

	int getColNum() { return colCount; }
	int getRowNum() { return rowCount; }
	int getErrorNum() { return errnum; }
	void setErrorNum(int errnum_) { errnum = errnum_; }

private:
	
	static int errnum;
	int rowCount;
	int colCount;
	std::vector<std::string > colNames;
	SQLtab table;
};

/*********************sql handler*************************/
class SQLConnector{

public:

	enum SQL_DIRECTION {SQL_READ, SQL_WRITE};
	SQLConnector();
	~SQLConnector();
	/**get the original sql handler*/
	SQL* getSQLHandle();
	void init();
	void conn(const std::string host, const std::string user,
		const std::string pwd, const std::string db, unsigned short port);
	int getErrorNum() { return errnum; }

	/** affected lines number returned */
	int query(const std::string& sqlstr, SQL_DIRECTION direct);
	SQLtable& getTable() { return table; }

	// transaction
	inline void begin()
	{
		if (inTransaction)
			errnum = SQL_TRANSACTION_ERR;
		mysql_autocommit(sqlconn, 0); 
	}

	void commit()
	{
		if (!inTransaction)
			errnum = SQL_TRANSACTION_ERR;
		mysql_commit(sqlconn);
		mysql_autocommit(sqlconn, 1); 
	}

	void rollback()
	{
		if (!inTransaction)
			errnum = SQL_TRANSACTION_ERR;
		mysql_rollback(sqlconn);
		mysql_autocommit(sqlconn, 1);
	}

private:
		
	void queryCheck(int ret);
	
private:

	bool		inTransaction;
	SQL*		sqlconn;
	int			errnum;		//errno
	std::string	errmsg;		//errmsg
	SQLtable	table;
};



#endif //_SQLCONNECTOR_H_
