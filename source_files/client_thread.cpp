#include "httpReq.h"

void writeToPacket(int clientfd, const std::string &html_data);

void web::app::handle_client(int clientfd) {
	try {
		//receive http request
		char req[BUFFSIZE];

		if(recv(clientfd, req, BUFFSIZE, 0) == -1) {	
			if(close(clientfd) == -1)
				throw "Failed to close the client connection.";
		}
		
		std::string req_method;
		for(unsigned int i = 0; req[i] != ' '; ++i)
			req_method += req[i];

		//get the file that's being requested by the client
		//guarantees .html file extension if filetype isn't specified
		std::string file_name = getRequestedFile(req, req_method.length());	

		//parse the type of file that is being requested
		//should add code that gaurds against files names that are not large enough
		std::string request(req);
		Map form_data;

		if(req_method.compare("POST") == 0)//create unordered_map for post from name->value 
			handlePostRequest(request, form_data);//get content

		//determine which html file to send, then send it
		std::string html_data = "";
		if(file_name.substr(file_name.length() - 5, 5).compare(".html") == 0) {
			file_name = mainLogic(file_name, form_data, html_data); //user returns the desired html file to render
		}

		//send string contents and return
		//otherwise, send the file directly
		if(html_data.length() != 0) 
			writeToPacket(clientfd, html_data);
		else
			handleGetRequest(file_name, request, clientfd);

		if(close(clientfd) == -1)
			throw "Failed to close the client connection.";
	}
	catch(const File_Close &mess) {
		std::cerr << mess.what() << '\n';
		mess.closeFd();//close the file descriptor
	}
	catch(const char* message) {
		std::cerr << message << '\n';
	}
	catch(...) {
		std::cerr << "Unknown exception occured.\n";
	}
}

void writeToPacket(int clientfd, const std::string &html_data) {
	const std::string header = HTTP_HEADER + "\r\n\n";

	//enable TCP_CORK
	int optval = 1;
	if(setsockopt(clientfd, IPPROTO_TCP, TCP_CORK, &optval, sizeof(optval)) == -1)
		throw "Failed to enable TCP_CORK.";

	if(write(clientfd, header.c_str(), header.length()) == -1)
		throw "Failed to write HTTP_HEADER to client.";

	if(write(clientfd, html_data.c_str(), html_data.length()) == -1)
		throw "Failed to write file contents to client.";

	//disable TCP_CORK
	optval = 0;
	if(setsockopt(clientfd, IPPROTO_TCP, TCP_CORK, &optval, sizeof(optval)) == -1)
		throw "Failed to disable TCP_CORK.";
}
