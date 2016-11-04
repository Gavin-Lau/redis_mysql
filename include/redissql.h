#ifndef REDSQL_REDISSQL_H
#define REDSQL_REDISSQL_H

#include <iostream>

#include <hiredis.h>
#include <Win32_Interop\win32fixes.h>

#define REPLY_INT_MAX_LEN 32

class RDSConnector {

public:

	RDSConnector(std::string host, unsigned short port, double timeout);
	~RDSConnector();
	void query(const char *format, ...);
	int getRDSerrno();
	std::string getRDSerrstr();
	void replyCheck(redisReply* reply);
	void reset();

private:
	
	redisContext	*rdsconn;
	redisReply		*rdsreply;
	int				errno;
	std::string		errmsg;
	std::string		data;
};

#endif //REDSQL_REDISSQL_H
