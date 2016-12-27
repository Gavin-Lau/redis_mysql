## WHERETOUSE
Using redis as memory cached database,  Wihle using mysql as permanent disk database.
* if using data from memory cached database,finding out that is __NULL__,it will load the data from disk database,then cached it.  
* There also has a timer,whose interval was defined by programmer,to travel all memory cached data to find out what data have changed during the time interval,then write to disk permanently.

## INSTALL
#### Windows Platform([MSOpenTech/redis](https://github.com/MSOpenTech/redis))
build "project-dir/third/redis"(only hiredis,others based on your requirement),then copy the generated "*.lib"s,to "project-dir/../third/hiredis".

#### Linux Platform([hiredis](https://github.com/redis/hiredis))
build "project-dir/third/hiredis",then install the libs and headers into system position(like '/usr/include','/usr/lib').
## THE RULES MYSQL-->REDIS
##### mysql table requirement:
* every table has field named "id",which is unique key,and auto-increase.
* there must be a table to identify which table need to memory cached(if not, means all tables).

##### redis table
* using "table-name:id" as unique key;
* every "table-name:id" key has additional field to identify whether this key is modified,if so, this data of this key will write back to disk database by timer.
