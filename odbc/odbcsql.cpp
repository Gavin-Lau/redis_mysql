#ifdef _WIN32
# include <windows.h>
#endif

#include <iostream>
#include <vector>
#include <cstdio>
#include <stdlib.h>

#include <sql.h>
#include <sqlext.h>



using std::cout;
using std::endl;

/******************************************/
/* Structure to store information about   */
/* a column.
/******************************************/
typedef struct _SQLCOLUMN {
    SQLSMALLINT         cDisplaySize;			/* size to display  */
	unsigned char		szcolumName[100];		/* colume name*/
    unsigned char*		szBuffer;				/* display buffer   */
    SQLLEN              indPtr;					/* size or null     */
    bool                fChar;					/* character col?   */

} SQLCOLUMN;






/*****************************************/
/* Some constants                        */
/*****************************************/


#define DISPLAY_MAX 50          // Arbitrary limit on column width to display
#define DISPLAY_FORMAT_EXTRA 3  // Per column extra display bytes (| <data> )
#define DISPLAY_FORMAT      "%c %*.*s "
#define DISPLAY_FORMAT_C    "%c %-*.*s "
#define NULL_SIZE           6   // <NULL>
#define SQL_QUERY_SIZE      1000 // Max. Num characters for SQL Query passed in.

#define PIPE                '|'

SHORT   gHeight = 80;       // Users screen height

void CHECK_ERROR(SQLHANDLE  hHandle,
	SQLSMALLINT    hType,
	RETCODE        RetCode,
	const char*		exmsg);

std::vector<SQLCOLUMN*>*
allocateColumn(SQLHSTMT& hStmt, SQLLEN cRowCount);

int __cdecl wmain(int argc, _In_reads_(argc) CHAR **argv)
{
    SQLHENV     hEnv = NULL;
    SQLHDBC     hDbc = NULL;
    SQLHSTMT    hStmt = NULL;
    CHAR*      pwszConnStr;
    CHAR       szInput[SQL_QUERY_SIZE];
	SQLRETURN	retcode;
    // Allocate an environment

    if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) == SQL_ERROR)
    {
        printf("Unable to allocate an environment handle\n");
        exit(-1);
    }

    // Register this as an application that expects 3.x behavior,
    // you must register something if you use AllocHandle


	retcode = SQLSetEnvAttr(hEnv,
                SQL_ATTR_ODBC_VERSION,
                (SQLPOINTER)SQL_OV_ODBC3, 0);
	CHECK_ERROR(hEnv, SQL_HANDLE_ENV, retcode, "SQLSetEnvAttr:");

    // Allocate a connection
    retcode = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);
	CHECK_ERROR(hEnv, SQL_HANDLE_ENV, retcode, "SQLAllocHandle");

	SQLSetConnectAttr(hDbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)10, 0);
	printf("set hDbc return %d\n", retcode);

    // Connect to the driver.  Use the connection string if supplied
    // on the input, otherwise let the driver manager prompt for input.
	SQLCHAR OutConnStr[255];
	SQLSMALLINT OutConnStrLen;
	SQLCHAR szDNS[2048] = { 0 };
	//const char* InconnStr = "Driver={MySQL ODBC 5.3 ANSI Driver};Server=192.168.200.102;Port=3309;Database=test;Uid=gavin;Pwd=lgk;";
	const char* InconnStr = "mysql51x";

	retcode = SQLConnect(hDbc,
		(SQLCHAR*)InconnStr, strlen(InconnStr),
		(SQLCHAR*)"gavin", strlen("gavin"),
		(SQLCHAR*)"lgk", strlen("lgk"));

	CHECK_ERROR(hDbc, SQL_HANDLE_DBC, retcode, "SQLConnect");

	retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

	CHECK_ERROR(hDbc, SQL_HANDLE_DBC, retcode, "SQLAllocHandle");

    printf("Enter SQL commands, type (control)Z to exit\nSQL COMMAND>");
	char sql[1000] = "SELECT nodename FROM t_test;";
	retcode = SQLExecDirect(hStmt, (SQLCHAR*)sql, strlen(sql));

	CHECK_ERROR(hStmt, SQL_HANDLE_STMT, retcode, "SQLExecDirect");

	SQLSMALLINT sNumResults = 0;
	if (SQL_SUCCESS == retcode)
	{
		retcode = SQLNumResultCols(hStmt, &sNumResults);
		CHECK_ERROR(hStmt, SQL_HANDLE_STMT, retcode, "SQLNumResultCols");
	}

	if (sNumResults > 0)
	{
		std::vector<SQLCOLUMN*>* table = allocateColumn(hStmt, sNumResults);

		SQLLEN cRowCount;
		retcode = SQLRowCount(hStmt, &cRowCount);
		CHECK_ERROR(hStmt, SQL_HANDLE_STMT, retcode, "SQLRowCount");

		if (cRowCount >= 0)
		{
			printf("%Id %s affected\n",
				cRowCount,
				cRowCount == 1 ? "row" : "rows");
		}

		//fetch rows
		int count = cRowCount;
		do {
			retcode = SQLFetch(hStmt);
			CHECK_ERROR(hStmt, SQL_HANDLE_STMT, retcode, "SQLFetch");
		} while (--count >= 0);
	}





Exit:

    // Free ODBC handles and exit
    if (hStmt)
    {
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }

    if (hDbc)
    {
        SQLDisconnect(hDbc);
        SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    }

    if (hEnv)
    {
        SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
    }

    wprintf(L"\nDisconnected.");

    return 0;
}


void CHECK_ERROR(SQLHANDLE      hHandle,
                SQLSMALLINT    hType,  
                RETCODE        RetCode,
				const char*		exmsg)
{
    SQLSMALLINT iRec = 0;
    SQLINTEGER  iError;
	SQLRETURN	ret;
	unsigned char	szMessage[SQL_MAX_MESSAGE_LENGTH + 1];
	unsigned char	szState[SQL_SQLSTATE_SIZE + 1];


    if (RetCode == SQL_INVALID_HANDLE)
    {
        printf("Invalid handle!\n");
        return;
    }

	do
	{
		ret = SQLGetDiagRec(hType, hHandle, ++iRec, szState, &iError, 
							szMessage, sizeof(szMessage), NULL);
		if (SQL_SUCCEEDED(ret))
			printf("[%5.5s] %s (%d)\n", szState, szMessage, iError);
	} while (ret == SQL_SUCCESS);
}

void listDrivers(SQLHENV  henv, const std::string driver)
{
	char attr[256];
	SQLSMALLINT attr_ret;
	SQLUSMALLINT direction = SQL_FETCH_FIRST;
	SQLRETURN retcode;
	SQLSMALLINT driver_ret;
	// Call SQLDrivers to return ODBC Drivers
	do 
	{
		retcode = SQLDrivers(henv, direction, (SQLCHAR*)driver.data(), driver.size(),
			&driver_ret, (SQLCHAR*)attr,
			sizeof(attr), &attr_ret);
		CHECK_ERROR(henv, SQL_NULL_HANDLE, retcode, "SQLDrivers()");
		direction = SQL_FETCH_NEXT;
		printf("%s - %s\n", driver, attr);
	} while (SQL_SUCCEEDED(retcode));
}

std::vector<SQLCOLUMN*>* 
allocateColumn(SQLHSTMT& hStmt, SQLLEN cRowCount)
{
	if (NULL == hStmt || cRowCount <= 0) return NULL;
	std::vector<SQLCOLUMN*> *colums = new std::vector<SQLCOLUMN*>;
	SQLSMALLINT iCol = 1;
	SQLLEN cchDisplay = 0, ssType;
	SQLRETURN retcode;
	SQLSMALLINT cchColumnNameLength;
	for (; iCol <= cRowCount; ++iCol)
	{
		SQLCOLUMN *icolObj = new SQLCOLUMN;
		// column length
		retcode = SQLColAttribute(hStmt, iCol, SQL_DESC_DISPLAY_SIZE,
			NULL, 0, NULL, &cchDisplay);
		CHECK_ERROR(hStmt, SQL_HANDLE_STMT, retcode, "SQLColAttribute:SQL_DESC_DISPLAY_SIZE");
		// column name
		retcode = SQLColAttribute(hStmt, iCol, SQL_DESC_NAME, icolObj->szcolumName,
			sizeof(icolObj->szcolumName), NULL, NULL);
		CHECK_ERROR(hStmt, SQL_HANDLE_STMT, retcode, "SQLColAttribute:SQL_DESC_NAME");

		// column type
		retcode = SQLColAttribute(hStmt, iCol, SQL_DESC_CONCISE_TYPE,
			NULL, 0, NULL, &ssType);
		CHECK_ERROR(hStmt, SQL_HANDLE_STMT, retcode, "SQLColAttribute:SQL_DESC_CONCISE_TYPE");

		icolObj->fChar = (ssType == SQL_CHAR ||
			ssType == SQL_VARCHAR ||
			ssType == SQL_LONGVARCHAR);

		icolObj->szBuffer = (unsigned char*)new char[cchDisplay + 1];

		retcode = SQLBindCol(hStmt, iCol, SQL_C_CHAR,
			(SQLPOINTER)icolObj->szBuffer, cchDisplay + 1,
			&icolObj->indPtr);
		CHECK_ERROR(hStmt, SQL_HANDLE_STMT, retcode, "SQLBindCol");

		retcode = SQLColAttribute(hStmt, iCol, SQL_DESC_NAME,
			NULL, 0, &cchColumnNameLength, NULL);
		CHECK_ERROR(hStmt, SQL_HANDLE_STMT, retcode, "SQLColAttribute:SQL_DESC_NAME");

		icolObj->cDisplaySize = max((SQLSMALLINT)cchDisplay, cchColumnNameLength);
		colums->push_back(icolObj);
	}
}