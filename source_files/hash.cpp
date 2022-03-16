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

	for(unsigned int i = 0; i != req.length(); ++i) {
		if(req[i] == 'I') {
			if(req.substr(i, 13).compare("If-None-Match") == 0) {
				i += 15;
				etag = req.substr(i, 32);
				return etag;
			}
		}
	}

	return "";
}
