#include "paths.h"
#include <fstream>

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

void getFileContents(const std::string &file, std::string &html_data) {
	std::fstream html_file(file, std::ios::in);

	if(html_file.is_open()) {
		html_data = std::string((std::istreambuf_iterator<char>(html_file)), std::istreambuf_iterator<char>());
	}
}

//add html code to a file
void generate_list(std::string &content, const std::vector<std::string> &listName, const std::vector<std::vector<std::string>> &data, const std::vector<std::string> &cols) {
	std::size_t pos = 0;

	for(const std::string &x : listName) {
		std::string searchLoop = "{%for val in " + x + "%}";

		if((pos = content.find(searchLoop, pos)) != std::string::npos) {
			content.replace(pos, searchLoop.length(), "");
			//copy lines in between this and endfor and output it for all rows in data
			std::size_t new_pos = content.find("{%endfor%}", pos);

			std::string html_item = "";
			std::size_t i = pos, var_pos;

			while(i < new_pos)
				html_item += content[i++];

			for(unsigned int k = 0; k < data.size(); ++k) {
				new_pos -= html_item.length();

				for(unsigned int j = 0; j < cols.size(); ++j) {
					std::string columnVar = "val." + cols[j];

					if((var_pos = content.find(columnVar, new_pos)) != std::string::npos)
						content.replace(var_pos, columnVar.length(), data[k][j]);
				}

				//insert html here
				if(k != data.size() - 1) {
					new_pos = content.find("{%endfor%}", var_pos);
					content.insert(new_pos, html_item);
				}
			}

			new_pos = content.find("{%endfor%}", var_pos);
			content.erase(new_pos, 10); //erase endfor
		}
	}
}