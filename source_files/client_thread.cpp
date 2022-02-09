#include "httpReq.h"

void web::app::handle_client(void) {
	try {
		std::string req_method;
		for(unsigned int i = 0; req[i] != ' '; ++i)
			req_method += req[i];

		//get the file that's being requested by the client
		//guarantees .html file extension if filetype isn't specified
		std::string file_name = getRequestedFile(req);	

		//parse the type of file that is being requested
		//should add code that gaurds against files names that are not large enough
		if(file_name.length() > 5 && file_name.substr(file_name.length() - 5).compare(".html") == 0)
			file_name = "templates/" + mainLogic(req_method, file_name); //user returns the desired html file to render
		//otherwise, load the css or image file
			
		//handleGETRequest
		std::string request(req);

		//function for handling the type of request
		if(req_method.compare("GET") == 0) {
			//will need to modify this function
			handleGetRequest(file_name, request, clientfd);
		}

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
