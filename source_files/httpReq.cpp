#include "httpReq.h"
#include <unordered_set>

//set of supported file types
const std::unordered_set<std::string> file_types = {
	{"css"}, {"png"}, {"jpg"}, {"ico"}, {"html"}, {"jpeg"}
};

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

void web::app::handleGetRequest(const std::string &name, const int &clientfd) const {
		int file = open(name.c_str(), O_RDONLY);
		struct stat sb;

		if(file == -1) {
			if(write(clientfd, HTTP_ERROR.c_str(), HTTP_ERROR.length()) == -1)
				throw "Failed to write 404 error to socket.";

			return;
		}

		fstat(file, &sb); //retrieve file metadata

		//enable TCP_CORK
		int optval = 1;
		setsockopt(clientfd, IPPROTO_TCP, TCP_CORK, &optval, sizeof(optval));

		if(write(clientfd, HTTP_HEADER.c_str(), HTTP_HEADER.length()) == -1)
			throw File_Close("Failed to write HTTP_HEADER to client.", file);

		sendfile(clientfd, file, 0, sb.st_size);

		//disable TCP_CORK
		optval = 0;
		setsockopt(clientfd, IPPROTO_TCP, TCP_CORK, &optval, sizeof(optval));

		close(file);
}

std::string web::app::getRequestedFile(const char *request, unsigned int start, bool &isHTML) const {
	std::string reqFile;

	for(unsigned int i = start + 2; request[i] != ' '; i++)
		reqFile += request[i];

	if(reqFile.length() == 0) {
		isHTML = true;
		return "index.html";
	}

	std::string type; std::size_t pos;
	if((pos = reqFile.find(".")) != std::string::npos) {
		type = reqFile.substr(pos + 1, reqFile.length());

		if(file_types.find(type) == file_types.end())
			throw "File type not supported.";

		if(type.compare("html") == 0)
			isHTML = true;
	}	
	else {
		isHTML = true;	
		return reqFile + ".html";
	}

	//if the file type is .html, this will return it
	return reqFile;	
}

//exit the program on critical error
void errexit(const std::string message) {
	std::cerr << message << '\n';
	exit(EXIT_FAILURE);
}

void web::app::handleGetRequest(const std::string &name, const std::string &request, const int &clientfd) const {
		int file = open(name.c_str(), O_RDONLY);
		struct stat sb;

		if(file == -1) {
			if(write(clientfd, HTTP_ERROR.c_str(), HTTP_ERROR.length()) == -1)
				throw "Failed to write 404 error to socket.";

			return;
		}

		fstat(file, &sb); //retrieve file metadata

		//check if file was modified after the last request. If not, send 304 response
		//improve performace below with coroutines
		const std::string etag = getEtag(request);
		const std::string fileHash = md5Hash(name);//O(n) where n is the size of the file

		//if the file was not modified and it's not an html file
		if(fileHash.compare(etag) == 0) {	
			if(write(clientfd, HTTP_IF_MODIFIED.c_str(), HTTP_IF_MODIFIED.length()) == -1)
				throw File_Close("Failed to write HTTP_IF_MODIFIED to client.", file);
		}
		else {
			//enable TCP_CORK
			int optval = 1;
			setsockopt(clientfd, IPPROTO_TCP, TCP_CORK, &optval, sizeof(optval));

			//send the file data
			const std::string header = HTTP_HEADER + "Etag: " + fileHash + "\r\n\n";

			if(write(clientfd, header.c_str(), header.length()) == -1)
				throw File_Close("Failed to write HTTP_HEADER to client for non-html file.", file);

			sendfile(clientfd, file, 0, sb.st_size);

			//disable TCP_CORK
			optval = 0;
			setsockopt(clientfd, IPPROTO_TCP, TCP_CORK, &optval, sizeof(optval));
		}

		close(file);
}
