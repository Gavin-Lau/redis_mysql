#ifndef _COMMON_H_
#define _COMMON_H_

 
#define max(a, b) ((a) > (b) ? a : b)
#define min(a, b) ((a) < (b) ? a : b)


#define  INT32_MAX_LENGTH 15 //POWER(2,32) = 4294967296


inline std::string int2str(int x)
{
	char str[15] = { 0 };
	sprintf(str, "%d", x);
	str[14] = 0;
	return str;
}


/** error number define */
#define  OK						0
#define  UNKOWN_ERROR			-1
//mysql errors
#define  SQL_INIT_FAIL			-1001
#define  SQL_CONN_FAIL			-1002
#define  SQL_QUERY_ERROR		-1003
#define  SQL_QUERIED_NOTHING	-1004
#define  SQL_PARSE_TABLE_ERR	-1005
#define  SQL_TRANSACTION_ERR	-1006
#define  SQL_IMPROPER_ORDER		-1007 //"Commands were executed in an improper order"
#define  SQL_CONN_LOST			-1006


//redis errors
#define  RDS_CONN_FAIL			-2001
#define  RDS_QUERY_FAIL			-2002
#define  RDS_QUERIED_NOTHING	-2003
#define  RDS_TRANSACTION_ERR	-2004

#define  RDS_QUERY_FATAL		-2004	//need conn reseted
#define  RDS_TYPE_ERROR			-2005

#endif //_COMMON_H_
