#include "redissql.h"
#include "common.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <vector>
#include <winsock.h>

RDSConnector::RDSConnector(std::string host = "127.0.0.1", 
						  unsigned short port = 6379, 
						  double timeout = 5.0)
	:rdsreply(NULL)
{
	struct timeval tm;
	tm.tv_sec = timeout;
	tm.tv_usec = (timeout - tm.tv_sec) * 1000000;
	rdsconn = redisConnectWithTimeout(host.c_str(), port, tm);
	if (rdsconn->err)
	{
		errnum = RDS_CONN_FAIL;	//conn fail
		errmsg = "conn error";
	} else {
		errnum = OK;
	}
}

RDSConnector::~RDSConnector()
{
	if (rdsreply != NULL) freeReplyObject(rdsreply);
	redisFree(rdsconn);
}

const std::string RDSConnector::query(const char *format, ...)
{
	errnum = UNKOWN_ERROR;
	errmsg = "unkown error";
	data = "";
	if (rdsreply != NULL)  freeReplyObject(rdsreply);
	va_list ap;
	va_start(ap, format);
	rdsreply = (redisReply *)redisvCommand(rdsconn, format, ap);	//not sure of the method va_list,need debug
	va_end(ap);
	replyCheck(rdsreply);
	printf("errno[%d]:%s\n", errnum, errmsg.c_str());
	freeReplyObject(rdsreply);
	rdsreply = NULL;
	return data;
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
		errnum = RDS_QUERY_FATAL;
		errmsg = "fatal err while query";
		return;
	}

	switch (reply->type) 	{

	case (REDIS_REPLY_STATUS) :
		errnum = OK;
		data = std::string(reply->str, reply->len);
		errmsg = "SUCCESS";
		break;

	case (REDIS_REPLY_ERROR) :
		errnum = RDS_QUERY_FAIL; //command error
		data =  std::string(reply->str, reply->len);
		errmsg = "query reply with errmsg:" + data;
		break;

	case (REDIS_REPLY_INTEGER) :
		errnum = OK;
		char rep[REPLY_INT_MAX_LEN] = { 0 };
		sprintf(rep, "%d", reply->integer);
		data = std::string(rep, strlen(rep));
		errmsg = "SUCCESS";
		break;

	case (REDIS_REPLY_NIL) :
		errnum = OK;
		errmsg = "nothing replied!";
		break;

	case (REDIS_REPLY_STRING) :
		errnum = OK;
		errmsg = "SUCCESS";
		data = std::string(reply->str, reply->len);
		break;

	case (REDIS_REPLY_ARRAY) :
		for (int j = 0; j < reply->elements; j++) {
			char rep[REPLY_INT_MAX_LEN] = {0};
			if (reply->element[j]->type == REDIS_REPLY_INTEGER) 
			{
				sprintf(rep, "%d", reply->integer);
				data += std::string(rep, strlen(rep)) + SPLITER;
			} else if (reply->element[j]->type == REDIS_REPLY_STRING) {
				data += std::string(reply->element[j]->str, reply->element[j]->len) + SPLITER;
			} else {
				errnum = RDS_TYPE_ERROR; // "error type";
				errmsg = "error type";
				return;
			}
		}
		errnum = OK;
		errmsg = "SUCCESS";
		break;

	default:
		errnum = UNKOWN_ERROR; //unkown err
		errmsg = "unkown error";
		break;
	}
}

/** not only reset the current conn, but change host/port */
void RDSConnector::reset(std::string host="127.0.0.1", 
						 unsigned short port=6379, 
						 double timeout=5.0)
{
	struct timeval tm;
	tm.tv_sec = timeout;
	tm.tv_usec = (timeout - tm.tv_sec) * 1000000;
	rdsconn = redisConnectWithTimeout((char*)host.c_str(), port, tm);
	if (rdsconn->err)
	{
		errnum = RDS_CONN_FAIL;	//conn fail
		errmsg = "conn error";
	} else {
		errnum = OK;
		errmsg = "";
	}
}

void RDSConnector::begin() { query("MULTI"); }
void RDSConnector::commit() { query("EXEC"); }
void RDSConnector::rollback() { query("DISCARD"); }





