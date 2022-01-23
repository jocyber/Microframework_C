#include "httpReq.h"

bool web::app::was_modified(char date[], char time[], const std::string &lastTime) {
	if(lastTime.length() == 0)
		return true;

	//check year
	/*
	int file_year = std::stoi(date.substr(date.length() - 2, date.length()));
	int last_year = std::stoi(lastTime.substr(8, 10));

	if(file_year > last_year)
		return true;

	//check month
	std::string file_month = date.substr(3, 5);
	std::string last_month = lastTime.substr(3, 6);

	if(month_code[file_month] > month_code[last_month])
		return true;

	//check day
	int file_day = std::stoi(date.substr(0,2));
	int last_day = std::stoi(lastTime.substr(0,2));

	if(file_day > last_day)
		return true;
	
	//check hour
	

	//check minute
	

	//check second
	*/

	return false;
}
