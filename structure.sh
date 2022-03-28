#make the html pages directory
if [ ! -d "templates" ]
	mkdir templates
fi

#create the initial code for the main dev file
if [ ! -f "source_files/server.cpp" ];
then
	touch source_files/server.cpp
	echo "#include \"routes.h\"

//map for encoding html page names
std::unordered_map<std::string, unsigned short> codes = {
	{\"index.html\", 1}, {\"test.html\", 2}
};

//create a series of functions that handles different routes
//framework will assume that all html files are in the 'templates' directory
//if there are any subdirectories, the user must specify
std::string web::app::mainLogic(const std::string &file, const Map &form, std::string &html_data) const { 
	unsigned short page = 0;
	page = codes[file];

	//return html pages based on codes map
	switch(page) {
		case 1: 
			return \"templares/index.html\";

		default:
			return \"templates/404.html\";
	}
}

//main function to run the server
int main(void)
{
	//web application object
	web::app App;
	App.run();
}" > source_files/server.cpp
fi

#index.html page generation
if [ ! -f "templates/index.html" ];
then
	touch templates/index.html;
	echo "<!DOCTYPE html>

<html>
	<head>
	</head>
				
	<body>
	</body>
</html>" > templates/index.html
fi

#404 html page
if [ ! -f "templates/404.html" ];
then
	touch templates/404.html;
	echo "<!DOCTYPE html>

<html>
	<head>
	</head>
				
	<body>
	<p>404 Error</p>
	</body>
</html>" > templates/404.html
fi

mkdir Dev_Folder
if [ ! -f "Dev_Folder/routes.h" ] && [ ! -f "Dev_Folder/routes.cpp" ];
then
	touch Dev_Folder/routes.h && touch Dev_Folder/routes.cpp;
	echo "#include "routes.h"

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
}" > routes.cpp

	echo "#ifndef ROUTES
#define ROUTES

#include "../source_files/paths.h"

std::string retrieveData(std::string &html_data, std::string file);

#endif" > routes.h
fi
