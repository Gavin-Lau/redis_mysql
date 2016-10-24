#ifndef REDSQL_ODBCSQL_H
#define REDSQL_ODBCSQL_H


class ODBCEnv {
public:

	ODBCEnv(const std::string DSNname);
	SQLConnector* getConnector();
	const std::vector<std::string> getValidDSNs();

private:
	
	SQLHENV     hEnv;
	SQLHDBC     hDbc;
	std::vector<std::string> vsDSNs;

};

class SQLConnector {

private:	
	SQLHSTMT    hStmt = NULL;
};

class Table {

private:
	SQLHSTMT    hStmt = NULL;
};

class Column {

private:
	SQLHSTMT    hStmt = NULL;

};





#endif //REDSQL_ODBCSQL_H
