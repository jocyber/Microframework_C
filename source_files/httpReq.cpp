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
		errexit("Could not the port to the address.");

	listen(sockfd, 1);
}

//destructor
web::app::~app() {
	if(close(sockfd) == -1 || close(clientfd) == -1)
		errexit("Could not close the socket connection.");
}

int web::app::handleGetRequest(const std::string &name) {
		struct stat sb;
		std::string response;
		file = fopen(name.c_str(), "r");

		if(file == nullptr) {
			std::cerr << "File could not be opened.\n";
			return -1;
		}

		//copy file contents into string
		stat(name.c_str(), &sb); // retrieve file size
		response.resize(sb.st_size); // resize the string
		size_t temp = fread(const_cast<char*>(response.data()), sb.st_size, 1, file);//fill the string
		fclose(file);	// close the file

		if(temp == 0) {
			std::cerr << "No data to read from {" + name + "}\n";
			return -1;
		}

		//prepend the http header
		response = HTTP_HEADER + response;

		//send response to client's browser
		if(send(clientfd, response.c_str(), response.length(), 0) == -1) {
			std::cerr << "Failed to send data back to the client.\n";
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
