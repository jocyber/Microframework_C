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
		bool isHTML = false;
		std::string file_name = getRequestedFile(req, req_method.length(), isHTML);	
		std::string request(req);

		if(!isHTML) {
			handleGetRequest(file_name, request, clientfd);
			close(clientfd);
			return;
		}

		Map form_data;
		if(req_method.compare("POST") == 0)//create unordered_map for post from name->value 
			handlePostRequest(request, form_data);//get content

		//determine which html file to send, then send it
		std::string html_data = "";
		file_name = mainLogic(file_name, form_data, html_data); //user returns the desired html file to render

		//send string contents and return
		//otherwise, send the file directly
		if(html_data.length() != 0) 
			writeToPacket(clientfd, html_data);
		else
			handleGetRequest(file_name, clientfd);

		close(clientfd);
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
	//enable TCP_CORK
	int optval = 1;
	setsockopt(clientfd, IPPROTO_TCP, TCP_CORK, &optval, sizeof(optval));

	if(write(clientfd, HTTP_HEADER.c_str(), HTTP_HEADER.length()) == -1)
		throw "Failed to write HTTP_HEADER to client.";

	write(clientfd, html_data.c_str(), html_data.length());

	//disable TCP_CORK
	optval = 0;
	setsockopt(clientfd, IPPROTO_TCP, TCP_CORK, &optval, sizeof(optval));
}
