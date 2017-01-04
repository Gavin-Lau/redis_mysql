#ifndef _LOADER_H_
#define _LOADER_H_

#include <iostream>
#include <string>

#include "common.h"

#include "sqlconnector.h"
#include "redissql.h"

/** cache mysql table 'tableName' to redis
 * 'tableName' must have field named 'cacheKey'
 * and 'cacheKey' must be unique key at least.
 */
inline int cacheTable(SQLConnector& sHandle, RDSConnector& rHandle,
						std::string tableName, std::string cacheKey)
{
	int ret = -1;
	if (sHandle.getErrorNum() < 0 || rHandle.getErrno() < 0)
		return ErrGloble::ERR_LOAD_TABLE;

	ret = sHandle.query("SELECT * FROM " + tableName + ";", SQLConnector::SQL_READ);
	if (ret < 0) return ErrGloble::ERR_LOAD_TABLE;
	SQLtable& table = sHandle.getTable();
	int rowCount = table.getRowNum();
	int colNum = table.getColNum();
	for (int row = 0; row < rowCount; ++row)
	{
		std::string rowKey = tableName + CACHE_KEY_SPLITER + table.get(row, cacheKey);
		for (int col = 0; col < colNum; ++col)
		{
			rHandle.query("hset %s %s %s", rowKey.c_str(), table.getColName(col).c_str(), table.get(row, col).c_str());
		}
	}
}

inline int cacheRow(SQLConnector& sHandle, RDSConnector& rHandle,
						const std::string tableName, const std::string cacheKey,
						const std::string rowKey)
{
	int ret = -1;
	if (sHandle.getErrorNum() < 0 || rHandle.getErrno() < 0)
		return ErrGloble::ERR_LOAD_TABLE;

	std::string queryStr = "SELECT * FROM " + tableName +
		" WHERE " + cacheKey + " = " + rowKey + ";";
	ret = sHandle.query(queryStr, SQLConnector::SQL_READ);
	if (ret < 0) return ErrGloble::ERR_LOAD_TABLE;

	SQLtable& table = sHandle.getTable();
	if (1 != table.getRowNum()) return ErrGloble::ERR_LOAD_TABLE; //must only 1 row
	int colNum = table.getColNum();

	std::string rowKey_ = tableName + CACHE_KEY_SPLITER + table.get(0, cacheKey);
	for (int col = 0; col < colNum; ++col)
	{
		rHandle.query("hset %s %s %s", rowKey_.c_str(), table.getColName(col).c_str(), table.get(0, col).c_str());
	}
}

/**
 * get the value of key 'key',
 * in row cacheKey = 'rowKey',in table 'tableName',
 */
inline std::string get(SQLConnector& sHandle, RDSConnector& rHandle,
					const std::string& tableName, const std::string cacheKey,
					const std::string& rowKey, const std::string key)
{
	int ret = -1;
	if (sHandle.getErrorNum() < 0 || rHandle.getErrno() < 0)
	{
		ErrGloble::errnum = ErrGloble::ERR_LOAD_TABLE;
		return "";
	}

	std::string result = rHandle.query("hget %s %s", (tableName + CACHE_KEY_SPLITER + rowKey).c_str(), key.c_str());
	if (rHandle.getErrno() == OK)
	{
		return result;
	}
	else if (rHandle.getErrno() == RDS_QUERIED_NOTHING)
	{
		// no error occured, but queried nothing.
		// then cache the row.
		rHandle.setErrno(OK);
		ret = cacheRow(sHandle, rHandle, tableName, cacheKey, rowKey);
		return rHandle.query("hget %s %s", (tableName + CACHE_KEY_SPLITER + rowKey).c_str(), key.c_str());
	}
	else if (rHandle.getErrno() < 0 || result.length() <= 0)
	{
		ErrGloble::errnum = ErrGloble::ERR_LOAD_TABLE;
		return "";
	} else {
		ErrGloble::errnum = UNKOWN_ERROR;
		return "";
	}
}

#endif //_LOADER_H_

