#ifndef PATHS
#define PATHS

#include "../source_files/httpReq.h" //path to header file
#include <mariadb/mysql.h>

struct Connection_Info {
    const std::string server, user, password, database;
};

std::string retrieveIndex(void);

#endif