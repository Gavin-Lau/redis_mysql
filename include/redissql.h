#ifndef REDSQL_REDISSQL_H
#define REDSQL_REDISSQL_H

#include <iostream>
#include <hiredis.h>

#define REPLY_INT_MAX_LEN 32
#define SPLITER '|'


class RDSConnector {

public:

	RDSConnector(std::string host, unsigned short port, double timeout);
	~RDSConnector();
	const std::string query(const char *format, ...);
	int getRDSerrno();
	std::string getRDSerrstr();
	void replyCheck(redisReply* reply);
	void reset(std::string host, unsigned short port, double timeout);

	// transaction
	void begin();
	void commit();
	void rollback();

private:

	redisContext	*rdsconn;
	redisReply		*rdsreply;
	int				errnum;
	std::string		errmsg;
	std::string		data;
};

#endif //REDSQL_REDISSQL_H
