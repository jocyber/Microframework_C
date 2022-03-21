#include "httpReq.h"
#include <stdio.h>

//filename is the path to the file
std::string web::app::md5Hash(const std::string &filename) const {
	std::string command = "md5sum " + filename;
	FILE *fileptr = popen(command.c_str(), "r");

	if(fileptr == nullptr)
		throw "Unable to open pointer to file.";

	const unsigned int md5Length = 33;
	char buffer[md5Length];
	std::string result = "";

	//append buffer to result string
	fgets(buffer, md5Length, fileptr);
	result += buffer;

	pclose(fileptr);
	return result;
}

//get the etag cached by the browser
std::string web::app::getEtag(const std::string &req) const {
	std::string etag;
	std::size_t position;

	if((position = req.find("If-None-Match")) != std::string::npos) {
		position += 15;
		etag = req.substr(position, 32);
		return etag;
	}

	return "";
}

//add html code to a file
void web::app::generate_list(std::string &content, const std::vector<std::string> &listName, const std::vector<std::vector<std::string>> &data, const std::vector<std::string> &cols) const {
	std::size_t pos = 0;

	for(const int &x : listName) {
		std::string searchLoop = "{%for val in " + x + "%}";

		if((pos = content.find(searchLoop, pos)) != std::string::npos) {
			content.erase(pos, searchLoop.length());
			//copy lines in between this and endfor and output it for all rows in data

			for(unsigned int k = 0; k < data.length(); ++k) {
				for(unsigned int i = 0; i < cols.length(); ++i) {
					std::string columnVar = "val." + cols[i];

					std::size_t var_pos;
					if((var_pos = content.find(columnVar, pos)) != std::string::npos) {
						content.replace(var_pos, columnVar.length(), data[k][i]);
					}
				}
			}
		}
	}
}
