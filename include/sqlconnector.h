#ifndef _SQLCONNECTOR_H_
#define _SQLCONNECTOR_H_

#include <iostream>
#include <string>

#include <mysql.h>

#define  SQL_INIT_FAIL		-1000
#define  SQL_CONN_FAIL		-1000
#define  SQL_ERROR_UNKOWN	-1;

typedef MYSQL SQL;

class SQLConnector{

public:

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
	int query(const char* sqlstr); //sz string
	int query(const char* sqlstr, unsigned long len); //binary string

private:
	SQL*		sqlconn;
	int			errorno;	//errno
	std::string	errmsg;		//errmsg
};

#endif //_SQLCONNECTOR_H_
