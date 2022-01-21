#include "httpReq.h"

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

		if(file == -1) {
			std::cerr << "File could not be opened.\n";
			return -1;
		}

		//copy file contents into string
		stat(name.c_str(), &sb); // retrieve file size

		//enable TCP_CORK
		int optval = 1;
		if(setsockopt(clientfd, IPPROTO_TCP, TCP_CORK, &optval, sizeof(optval)) == -1) {
			std::cerr << "Failed to enable TCP_CORK.\n";
			return -1;
		}

		//send the file data
		if(write(clientfd, HTTP_HEADER.c_str(), HTTP_HEADER.length()) == -1) {
			std::cerr << "Failed to write HTTP_HEADER to client.\n";
			return -1;
		}	

		if(sendfile(clientfd, file, 0, sb.st_size) == -1) {
			std::cerr << "Failed to send file contents.\n";
			return -1;
		}

		//disable TCP_CORK
		optval = 0;
		if(setsockopt(clientfd, IPPROTO_TCP, TCP_CORK, &optval, sizeof(optval)) == -1) {
			std::cerr << "Failed to disable TCP_CORK.\n";
			return -1;
		}

		if(close(file) == -1) {
			std::cerr << "Failed to close the requested file.\n";
			return -1;
		}

		return 0;
}

std::string web::app::getRequestedFile(const std::string &request) {
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
