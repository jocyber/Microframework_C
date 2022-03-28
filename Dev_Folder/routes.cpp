#include "routes.h"

//create or import functions for the routes here
std::string retrieveData(std::string &html_data, std::string file) {
	MYSQL *conn;
	MYSQL_RES *cursor;
	MYSQL_ROW row;

    Connection_Info sql_info;
    sql_info.user = "root";
    sql_info.password = "";
    sql_info.server = "localhost";
    sql_info.database = "Test";

	std::vector<std::vector<std::string>> data; 

	if((conn = sql_connect(sql_info)) != nullptr) {
		if((cursor = sql_exec_query(conn, "SELECT * FROM Stuff")) != nullptr) {
			while((row = mysql_fetch_row(cursor)) != nullptr) {
				std::vector<std::string> rows = {row[0], row[1]};
				data.push_back(rows);
			}
		}

		std::vector<std::string> cols;
		//retrieve column names
		if((cursor = sql_exec_query(conn, "SHOW COLUMNS FROM Stuff;")) != nullptr) {
			while((row = mysql_fetch_row(cursor)) != nullptr)
				cols.push_back(row[0]);
		}

		std::vector<std::string> lists = {"list"};
		getFileContents(file, html_data);
		generate_list(html_data, lists, data, cols);

		mysql_free_result(cursor);
		mysql_close(conn);
	}

	return "index.html";
}