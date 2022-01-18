#include "httpReq.h"

int handleGetRequest(const std::string &request, std::fstream &file, int &clientfd) {
		std::string reqFile = getRequestedFile(request);
		file.open(reqFile, std::ios::in);

		if(!file.is_open()) {
			std::cerr << "File could not be opened.\n";
			return -1;
		}

		//fill response string with file data		
		std::string response = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
		response = HTTP_HEADER + response;

		//send response to client's browser
		if(send(clientfd, response.c_str(), response.length(), 0) == -1) {
			std::cerr << "Failed to send data back to the client.\n";
			file.close();
			return -1;
		}

		file.close();
		return 0;
}

std::string getRequestedFile(const std::string &request) {
	std::string reqFile;

	for(unsigned int i = 5; request[i] != ' '; ++i)
		reqFile += request[i];

	//if css extension is found, then just return the directory path
	//.html extension will not work here, this is for any other generic file type
	if(reqFile.find(".") != std::string::npos)
		return reqFile;

	if(reqFile.length() == 0)
		reqFile = "index";

	//if the file type is .html, this will return it
	return "templates/" + reqFile + ".html";	
}

void errexit(const std::string message) {
	std::cerr << message << '\n';
	exit(EXIT_FAILURE);
}
