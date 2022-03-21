#include "httpReq.h"

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
		if(file_name.length() > 5 && file_name.substr(file_name.length() - 5).compare(".html") == 0) {
			file_name = "templates/" + mainLogic(file_name, form_data, html_data); //user returns the desired html file to render

			if(html_data.length() != 0) {
				;//send string contents and return
				//otherwise, send the file directly
			}
			
		}

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
