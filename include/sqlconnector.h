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
<<<<<<< HEAD
	enum SQL_DIRECTION {SQL_READ, SQL_WRITE};
	
=======
	enum QUERY_DIRECT { READ, WRITE };

>>>>>>> 0c25a280608959ad67bfedac451853aee1d7893c
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
<<<<<<< HEAD
		
	void queryCheck(int ret);	
	
=======

>>>>>>> 0c25a280608959ad67bfedac451853aee1d7893c
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
	const std::string getColName(unsigned int col);
	SQLrow* getRow(unsigned int row);
	const std::string get(unsigned int row, unsigned int col);
	const std::string get(unsigned int row, std::string key);
	void clear();

private:
	

	unsigned int rowCount;
	unsigned int colCount;
	std::vector<std::string > colNames;
	SQLtab table;
};


#endif //_SQLCONNECTOR_H_
