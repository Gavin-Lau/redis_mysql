#include "redissql.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <vector>

RDSConnector::RDSConnector(std::string host, unsigned short port, double timeout)
	:rdsreply(NULL)
{
	struct timeval tm;
	tm.tv_sec = timeout;
	tm.tv_usec = (timeout - tm.tv_sec) * 1000000;
	rdsconn = redisConnectWithTimeout((char*)"127.0.0.1", 6379, tm);
	if (rdsconn->err)
	{
		errno = -100;	//conn fail
	}
}

RDSConnector::~RDSConnector()
{
	if (rdsreply != NULL) freeReplyObject(rdsreply);
	redisFree(rdsconn);
}

void RDSConnector::query(const char *format, ...)
{
	if (rdsreply != NULL)  freeReplyObject(rdsreply);
	va_list ap;
	va_start(ap, format);
	rdsreply = (redisReply *)redisvCommand(rdsconn, format, ap);	//not sure of the method va_list,need debug
	va_end(ap);
	replyCheck(rdsreply);
	printf("errno[%d]:%s\n", errno, errmsg.c_str());

}

/**
 * a). REDIS_REPLY_STATUS: 返回执行之后的状态，具体的状态信息可以通过reply->str获得，reply->str的长度保存在reply->len中
 * b). REDIS_REPLY_ERROR : 执行出错，具体错误信息同REDIS_REPLY_STATUS
 * c). REDIS_REPLY_INTEGER : 返回是一个整数，值保存在reply->interger,类型为long long
 * d). REDIS_REPLY_NIL : 表示什么都没有返回，没有任何信息
 * e). REDIS_REPLY_STRING : 长string返回，具体获得方法同 REDIS_REPLY_STATUS
 * f). REDIS_REPLY_ARRAY : 返回为长string数组类型，保存在 reply->elements 中，每个element都是一个reply类型的数据，通过下标 reply->element[..index..]
 * 得，
 */
void RDSConnector::replyCheck(redisReply* reply)
{
	if (NULL == reply)	//fatal err .need  handle tobe reset
	{
		errno = -200;
		return;
	}

	switch (reply->type) 	{

	case (REDIS_REPLY_STATUS) :
		errno = 0;
		errmsg = std::string(reply->str, reply->len);
		break;

	case (REDIS_REPLY_ERROR) :
		errno = -1; //command error
		errmsg = std::string(reply->str, reply->len);
		break;

	case (REDIS_REPLY_INTEGER) :
		errno = 0;
		char rep[REPLY_INT_MAX_LEN];
		sprintf(rep, "%d", reply->integer);
		rep[REPLY_INT_MAX_LEN - 1] = 0;
		errmsg = std::string(rep);
		break;

	case (REDIS_REPLY_NIL) :
		errno = 0;
		errmsg = "nothing replied!";
		break;

	case (REDIS_REPLY_STRING) :
		errno = 0;
		errmsg = "bulk data replied";
		data = std::string(reply->str, reply->len);
		break;

	case (REDIS_REPLY_ARRAY) :

		break;

	default:
		errno = -1; //unkown err
		errmsg = "unkown error";
		break;
	}
}

void RDSConnector::reset()
{

}






