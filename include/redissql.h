#ifndef REDSQL_REDISSQL_H
#define REDSQL_REDISSQL_H

#include <iostream>
#include <hiredis.h>

#include "common.h"

#define REPLY_INT_MAX_LEN 32
#define SPLITER '|'


class RDSConnector {

public:

	RDSConnector::RDSConnector() :rdsreply(NULL),inTransaction(false) {}
	~RDSConnector();
	const std::string query(const char *format, ...);
	int replyCheck(redisReply* reply);
	void conn(std::string host, unsigned short port, double timeout);

	int getErrno() { return errnum; }
	void setErrno(int errnum_) { errnum = errnum_; }

	// transaction
	void begin() 
	{
		if (inTransaction)
			errnum = RDS_TRANSACTION_ERR;
		query("MULTI");
	}

	void commit() 
	{
		if (!inTransaction)
			errnum = RDS_TRANSACTION_ERR;
		query("EXEC");
	}

	void rollback()
	{
		if (!inTransaction)
			errnum = RDS_TRANSACTION_ERR;
		query("DISCARD");
	}

private:

	bool			inTransaction;
	redisContext	*rdsconn;
	redisReply		*rdsreply;
	int				errnum;
	std::string		data;
};

#endif //REDSQL_REDISSQL_H
