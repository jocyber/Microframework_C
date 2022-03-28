#ifndef PATHS
#define PATHS

#include "../source_files/httpReq.h" //path to header file
#include <mysql.h>

struct Connection_Info {
    std::string server, user, password, database;
};

MYSQL* sql_connect(const struct Connection_Info &connect);
MYSQL_RES* sql_exec_query(MYSQL* conn, const char* query);
void generate_list(std::string &content, const std::vector<std::string> &listName, const std::vector<std::vector<std::string>> &data, const std::vector<std::string> &cols);
void getFileContents(const std::string &file, std::string &html_data);

#endif