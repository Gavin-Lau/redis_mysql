#ifndef _SAVER_H_
#define _SAVER_H_


#include "common.h"
#include "sqlconnector.h"
#include "redissql.h"

/**
 * if table 'tableName' has field named dirtyIdentity
 */
inline int saveDirtyRows(SQLConnector sHandle, RDSConnector rHandle, 
					const std::string& tableName, const std::string cacheKey,
					const std::string& dirtyIdentity = "dirty")
{
	int ret = -1;
	if (sHandle.getErrorNum() < 0 || rHandle.getErrno() < 0)
		return ErrGloble::ERR_SAVE_TABLE;
	//1.get redis all key
	std::string rdsquery =
			rHandle.query("keys %s", (tableName + CACHE_KEY_SPLITER + '*').c_str());
	if (rHandle.getErrno() < 0)
		return ErrGloble::ERR_SAVE_TABLE;
	std::vector<std::string> keyList;
	split(rdsquery, std::string(1,REDIS_QUERY_LIST_SPLITER), keyList);
	if (keyList.size() == 0)
		return ErrGloble::ERR_SAVE_TABLE;

	int rowNum = keyList.size();
	std::vector<std::string> dirtyRowList;
	std::vector<std::string> rowKeysList;	
	for (int i = 0; i < rowNum; ++i)
	{
		std::string queryStr = "UPDATE " + tableName + " SET ";
		
		rdsquery =
			rHandle.query("hexists %s %s", keyList[i].c_str(), dirtyIdentity.c_str());
		if (rHandle.getErrno() < 0)
			return ErrGloble::ERR_SAVE_TABLE;
		//2.construct sql query str.
		if ("0" != rdsquery) //key 'dirty' exists.
		{
			rdsquery = rHandle.query("hkeys %s", keyList[i].c_str());
			if (rHandle.getErrno() < 0)
				return ErrGloble::ERR_SAVE_TABLE;
			split(rdsquery, std::string(1, REDIS_QUERY_LIST_SPLITER), rowKeysList);
			if (keyList.size() == 0)
				return ErrGloble::ERR_SAVE_TABLE;

			std::string cacheVal;
			std::string value;
			for (int m = 0; m < rowKeysList.size(); ++m)
			{
				if (rowKeysList[m] == dirtyIdentity ) continue;
				value = rHandle.query("hget %s %s", keyList[i].c_str(), rowKeysList[m].c_str());
				if (rowKeysList[m] == cacheKey)
				{
					cacheVal = value;
					continue;
				}
				queryStr = queryStr + rowKeysList[m] + "=";
				queryStr = queryStr + value + ",";
			}
			queryStr.pop_back();
			queryStr = queryStr + " WHERE " + cacheKey + "=" + cacheVal + ";";
			sHandle.query(queryStr,SQLConnector::SQL_WRITE);
		}

	}
}


inline void saveByKey(SQLConnector sHandle, RDSConnector rHandle)
{
	std::vector<std::string> result;
	split("aaa,bbb,ccc.dd.", ",.", result);

}




#endif //_SAVER_H_