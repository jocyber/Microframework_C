#include "httpReq.h"

/********** RUN FUNCTION **************/
void web::app::run(std::string(*mainLogic)(void)) {

	while(true) {
		if((clientfd = accept(sockfd, NULL, NULL)) == -1)
			errexit("Could not accept the oncoming connection.");

		//receive http request
		recv(clientfd, req, REQUEST_SIZE, 0);
		std::string request(req);

		//get last-modified header
		//
		
		std::string findstr = "";
		/*
		for(unsigned int i = 0; i < request.size(); ++i) {
			if(request[i] == 'I') {
				if(request.substr(i, i + 17).compare("If-Modified-Since") == 0) {
					i += 24;
					findstr = request.substr(i, i + 20);
					break;
				}
			}
		}
		*/

		//fill the http request method for user to query
		req_method.clear();
		for(unsigned int i = 0; request[i] != ' '; ++i)
			req_method += request[i];

		//get the file that's being requested by the client
		//guarantees .html file extension
		file_name = getRequestedFile(request);	

		//parse the type of file that is being requested
		//should add code that gaurds against files names that are not large enough
		if(file_name.length() > 5 && file_name.substr(file_name.length() - 5).compare(".html") == 0)
			file_name = "templates/" + mainLogic(); //user returns the desired html file to render
		//otherwise, load the css or image file

		//handleGETRequest
		//function for handling the type of request
		if(req_method.compare("GET") == 0) {
			//will need to modify this function
			if(handleGetRequest(file_name, findstr) == -1) {
				if(close(clientfd) == -1)
					errexit("Failed to close the client connection.");

				continue;
			}
		}
		
		if(close(clientfd) == -1)
			errexit("Failed to close the client connection.");
	}
}
