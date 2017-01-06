#ifndef _COMMON_H_
#define _COMMON_H_

#include <iostream>
#include <vector>

#ifndef max
#define max(a, b) ((a) > (b) ? a : b)
#endif

#ifndef min
#define min(a, b) ((a) < (b) ? a : b)
#endif

#define  INT32_MAX_LENGTH 15 //POWER(2,32) = 4294967296
#define  CACHE_KEY_SPLITER ':'
#define  REDIS_QUERY_LIST_SPLITER '|'

#ifdef _WIN32

# include <windows.h>
inline void secSleep(int sec)
{
	Sleep(sec*1000);
}
#else
#include <unistd.h>
inline void secSleep(int sec)
{
	sleep(sec);
}

#endif

inline std::string int2str(int x)
{
	char str[15] = { 0 };
	sprintf(str, "%d", x);
	str[14] = 0;
	return str;
}

/**
 * splite string 'str', by chars in spliter.
 */
inline void split(const std::string&str, const std::string spliter, std::vector<std::string>& result)
{
	if (str.length() == 0 || spliter.length() == 0) return;

	int spliterNum = spliter.length();
	int rawLen = str.length();
	
	int rightLen = rawLen;
	for (;;)
	{
		std::string tmp = str.substr(rawLen - rightLen, rawLen);
		int pos = tmp.length();
		bool isEnd = false;
		for (int j = 0; j < spliterNum; ++j)
		{
			int posTmp = tmp.find(spliter[j]);
			isEnd = (isEnd == false) ? (isEnd || (posTmp == std::string::npos)) :
						(isEnd && (posTmp == std::string::npos));
			if (posTmp != std::string::npos && posTmp < pos)
				pos = posTmp;
		}
		if (isEnd)
		{
			if (tmp.length() > 0)
				result.push_back(tmp);
			break;
		}
		result.push_back(tmp.substr(0, pos));
		rightLen = tmp.length() - pos - 1; // the next char must be spliter.
	}
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
#define  SQL_CONN_LOST			-1008
#define  SQL_OUT_RANGE			-1009 //row or colum out of range


//redis errors
#define  RDS_CONN_FAIL			-2001
#define  RDS_QUERY_FAIL			-2002
#define  RDS_QUERY_FATAL		-2003	//need conn reseted
#define  RDS_QUERIED_NOTHING	-2004
#define  RDS_TRANSACTION_ERR	-2005
#define  RDS_TYPE_ERROR			-2006

namespace ErrGloble {
	const int ERR_LOAD_TABLE = -100;
	const int ERR_SAVE_TABLE = -101;
	const int ERR_ = -102;
	
	static int errnum;
}



#endif //_COMMON_H_
