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

int web::app::handleGetRequest(const std::string &request, std::fstream &file, int &clientfd) {
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

std::string web::app::getRequestedFile(const std::string &request) {
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

void web::app::run() {
	while(true) {
		if((clientfd = accept(sockfd, NULL, NULL)) == -1)
			errexit("Could not accept the oncoming connection.");

		recv(clientfd, req, REQUEST_SIZE, 0);
		std::string request(req);
		
		//function for handling the type of request
		if(request.substr(0, 3).compare("GET") == 0) {
			if(handleGetRequest(request, file, clientfd) == -1) {
				if(close(clientfd) == -1)
					errexit("Failed to close the client connection.");

				//respond with 404 error
				continue;
			}
		}
		
		if(close(clientfd) == -1)
			errexit("Failed to close the client connection.");
	}
}


//exit the program on critical error
void errexit(const std::string message) {
	std::cerr << message << '\n';
	exit(EXIT_FAILURE);
}
