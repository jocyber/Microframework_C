#include "httpReq.h"
#include <time.h>

//constructor
web::app::app() {
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		errexit("Could not connect to the socket.");

	memset((struct sockaddr_in *) &addr, 0, sizeof(addr));

	addr.sin_port = htons(PORT);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;

	if(bind(sockfd, (struct sockaddr *) &addr, sizeof(addr)) == -1)
		errexit("Could not bind the port to the address.");

	listen(sockfd, 1);
}

//destructor
web::app::~app() {
	if(close(sockfd) == -1)
		errexit("Could not close the socket connection.");

	if(close(clientfd) == -1)
		errexit("Could not close the client socket.");
}

int web::app::handleGetRequest(const std::string &name) {
		struct stat sb;
		file = open(name.c_str(), O_RDONLY);

		if(file == -1)
			return -1;

		fstat(file, &sb); //retrieve file metadata

		//if requested file is html, go ahead and send it
		if(name.length() > 5 && name.substr(name.length() - 5, name.length()).compare(".html") == 0)
			goto sendFile;

		char date[20]; // day-month-year
		char time[20]; 

		//check if file was modified after the last request. If not, send 304 response
		strftime(date, sizeof(date), "%d-%m-%y", localtime(&sb.st_ctime));
		//parse last-modified from http request

		//if the file was not modified and it's not an html file
		if(!was_modified(date, time)) {	
			if(write(clientfd, HTTP_IF_MODIFIED.c_str(), HTTP_IF_MODIFIED.length()) == -1)
				return errclose("Failed to write HTTP_IF_MODIFIED to client.", file);
		}
		else {
sendFile:
			//enable TCP_CORK
			int optval = 1;
			if(setsockopt(clientfd, IPPROTO_TCP, TCP_CORK, &optval, sizeof(optval)) == -1)
				return errclose("Failed to enable TCP_CORK.", file);

			//send the file data
			if(write(clientfd, HTTP_HEADER.c_str(), HTTP_HEADER.length()) == -1)
				return errclose("Failed to write HTTP_HEADER to client.", file);

			if(sendfile(clientfd, file, 0, sb.st_size) == -1)
				return errclose("Failed to send file contents.", file);

			//disable TCP_CORK
			optval = 0;
			if(setsockopt(clientfd, IPPROTO_TCP, TCP_CORK, &optval, sizeof(optval)) == -1)
				return errclose("Failed to disable TCP_CORK.", file);
		}

		if(close(file) == -1)
			return errclose("Failed to close the requested file.", file);

		return 0;
}

//compare modified time to last request time (stored in last_mode_date struct)
bool web::app::was_modified(char date[], char time[]){return true;}

std::string web::app::getRequestedFile(const std::string &request) const {
	std::string reqFile;

	for(unsigned int i = 5; request[i] != ' '; ++i)
		reqFile += request[i];

	//.html extension will not work here, this is for any other generic file type
	if(reqFile.find(".") != std::string::npos)
		return reqFile;

	if(reqFile.length() == 0)
		reqFile = "index";

	//if the file type is .html, this will return it
	return reqFile + ".html";	
}

//exit the program on critical error
void errexit(const std::string message) {
	std::cerr << message << '\n';
	exit(EXIT_FAILURE);
}

int errclose(const std::string message, int fd) {
	close(fd);

	std::cerr << message << '\n';
	return -1;
}
