#ifndef PATHS
#define PATHS

#include "../source_files/httpReq.h" //path to header file
#include <mysql.h>

struct Connection_Info {
    std::string server, user, password, database;
};

MYSQL* sql_connect(const struct Connection_Info &connect);
MYSQL_RES* sql_exec_query(MYSQL* conn, const char* query);
std::string retrieveIndex(void);

#endif