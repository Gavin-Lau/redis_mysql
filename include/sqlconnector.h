#ifndef _SQLCONNECTOR_H_
#define _SQLCONNECTOR_H_

#include <iostream>
#include <string>
#include <vector>

#include <mysql.h>

#define  SQL_INIT_FAIL		-1000
#define  SQL_CONN_FAIL		-1000
#define  SQL_ERROR_UNKOWN	-1;

typedef MYSQL SQL;

class SQLConnector{

public:
	enum QUERY_DIRECT { READ, WRITE };

	SQLConnector();

	~SQLConnector();

	/**get the original sql handler*/
	SQL* getSQLHandle();

	void init();

	void conn(const std::string host, const std::string user,
		const std::string pwd, const std::string db, unsigned short port);

	const std::string getSQLerrstr();
	int getSQLerrno();

	/** affected lines number returned */
	int query(const char* sqlstr, QUERY_DIRECT direct); //sz string
	int query(const char* sqlstr, unsigned long len, QUERY_DIRECT direct); //binary string

private:

private:

	SQL*		sqlconn;
	int			errorno;	//errno
	std::string	errmsg;		//errmsg
	SQLtable	table;
};

class SQLtable {
public:
	typedef std::vector<std::string> SQLrow;
	typedef std::vector<SQLrow* > SQLtab;
	
	SQLtable();
	void init(MYSQL_RES* result);
	~SQLtable();
	unsigned int getColNum();
	unsigned int getRowNum();
	std::string getColName(unsigned int col);
	SQLrow* getRow(unsigned int row);
	std::string get(unsigned int row, unsigned int col);
	std::string get(unsigned int row, std::string key);
	void clear();

private:
	

	unsigned int row;
	unsigned int col;
	std::vector<std::string > colNames;
	SQLtab table;
};


#endif //_SQLCONNECTOR_H_
