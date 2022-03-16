#include "httpReq.h"

//constructor
web::app::app() {
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		errexit("Could not connect to the socket.");

	memset((struct sockaddr_in *) &addr, 0, sizeof(addr));
	memset(req, '\0', BUFFSIZE);

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
}

void web::app::handleGetRequest(const std::string &name, const std::string &request, const int &clientfd) const {
		int file = open(name.c_str(), O_RDONLY);
		struct stat sb;
		bool flag = false;

		if(file == -1) {
			if(write(clientfd, HTTP_ERROR.c_str(), HTTP_ERROR.length()) == -1)
				throw "Failed to write 404 error to socket.";
		}

		fstat(file, &sb); //retrieve file metadata
		std::string fileHash, header = HTTP_HEADER, etag;

		//if requested file is html, go ahead and send it
		if(name.substr(name.length() - 5, name.length()).compare(".html") == 0) {
			flag = true;
			goto sendFile;
		}

		//check if file was modified after the last request. If not, send 304 response
		//improve performace below with multithreading
		etag = getEtag(request);
		fileHash = md5Hash(name);//O(n) where n is the size of the file

		//if the file was not modified and it's not an html file
		if(fileHash.compare(etag) == 0) {	
			if(write(clientfd, HTTP_IF_MODIFIED.c_str(), HTTP_IF_MODIFIED.length()) == -1)
				throw File_Close("Failed to write HTTP_IF_MODIFIED to client.", file);
		}
		else {
sendFile:
			//enable TCP_CORK
			int optval = 1;
			if(setsockopt(clientfd, IPPROTO_TCP, TCP_CORK, &optval, sizeof(optval)) == -1)
				throw File_Close("Failed to enable TCP_CORK.", file);

			//send the file data
			if(flag)
				header += "\r\n\n";
			else
				header += ("\r\nEtag: " + fileHash + "\r\n\n");

			if(write(clientfd, header.c_str(), header.length()) == -1)
				throw File_Close("Failed to write HTTP_HEADER to client.", file);

			if(sendfile(clientfd, file, 0, sb.st_size) == -1)
				throw File_Close("Failed to send file contents.", file);

			//disable TCP_CORK
			optval = 0;
			if(setsockopt(clientfd, IPPROTO_TCP, TCP_CORK, &optval, sizeof(optval)) == -1)
				throw File_Close("Failed to disable TCP_CORK.", file);
		}

		if(close(file) == -1)
			throw "Failed to close the requested file.";
}

std::string web::app::getRequestedFile(const char *request, unsigned int start) const {
	std::string reqFile;

	for(unsigned int i = start + 2; request[i] != ' '; i++)
		reqFile += request[i];

	//this is for any generic file type. Will create a set that contains all supported file types, soon
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
