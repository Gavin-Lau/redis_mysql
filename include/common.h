#ifndef _COMMON_H_
#define _COMMON_H_

#ifdef max
# undef max 
# define max(a,b) ((a) > (b) ? a : b)
#endif

#ifdef min
# undef min
# define min(a, b) ((a) < (b) ? a : b)
#endif

/** error number define */
#define	OK					1
#define UNKOWN_ERROR		-1
//mysql errors
#define	SQL_CONN_FAIL		-1000
#define	SQL_QUERY_FAIL		-1001
#define	SQL_				-1000

//redis errors
#define	RDS_CONN_FAIL		-2000
#define RDS_QUERY_FAIL		-2001
#define RDS_QUERY_FATAL		-2002	//need conn reseted
#define RDS_TYPE_ERROR		-2003

#endif //_COMMON_H_
