#include "paths.h"

MYSQL* sql_connect(const struct Connection_Info &connect) {
    MYSQL *conn = mysql_init(nullptr);

    if(!mysql_real_connect(conn, connect.server.c_str(), connect.user.c_str(), connect.password.c_str(), connect.database.c_str(), 0, nullptr, 0)) {
        std::cerr << "Failed to establish connection to the database: " << mysql_error(conn) << '\n';
        return nullptr;
    }

    return conn;
}

MYSQL_RES* sql_exec_query(MYSQL* conn, const char* query) {
    if(mysql_query(conn, query)) { // returns true if error occurs
        std::cerr << "Error in executing query: {" << query << "}: " << mysql_error(conn) << '\n';
        return nullptr;
    }

    return mysql_use_result(conn);
}

//create or import functions for the routes here
std::string retrieveIndex(void) {
	MYSQL *conn;
	MYSQL_RES *cursor;
	MYSQL_ROW row;

	Connection_Info sql_info;
	sql_info.user = "root";
	sql_info.password = "";
	sql_info.server = "localhost";
	sql_info.database = "Test";

	if((conn = sql_connect(sql_info)) != nullptr) {
		if((cursor = sql_exec_query(conn, "SELECT * FROM Stuff;")) != nullptr) {
			while((row = mysql_fetch_row(cursor)) != nullptr)
				std::cout << row[0] << ' ' << row[1] << '\n';
		}

		mysql_free_result(cursor);
		mysql_close(conn);
	}

	return "index.html";
}