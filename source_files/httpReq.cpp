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
		file = fopen(name.c_str(), "r");

		if(file == nullptr) {
			std::cerr << "File could not be opened.\n";
			return -1;
		}

		//copy file contents into string
		stat(name.c_str(), &sb); // retrieve file size
		unsigned int string_size = HTTP_HEADER.length() + sb.st_size;

		//copy file contents into string
		char *buffer = (char*) malloc(string_size);
		strcpy(buffer, HTTP_HEADER.c_str());

		//should guard against empty file contents
		fread(buffer + HTTP_HEADER.length(), string_size, 1, file);
		fclose(file);	// close the file

		//send response to client's browser
		if(send(clientfd, buffer, string_size, 0) == -1) {
			free(buffer);
			std::cerr << "Failed to send data back to the client.\n";
			return -1;
		}

		free(buffer);
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
