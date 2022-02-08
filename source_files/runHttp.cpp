#include "httpReq.h"
#include <thread> // use C++ threads instead of pthreads

/********** RUN FUNCTION **************/
void web::app::run(std::string(*mainLogic)(const std::string&, const std::string&)) {
	//create thread pool here for better performance
	while(true) {
		try {
			if((clientfd = accept(sockfd, NULL, NULL)) == -1)
				throw "Could not accept the oncoming connection.";

			//receive http request
			if(recv(clientfd, req, REQUEST_SIZE, 0) == -1) {	
				if(close(clientfd) == -1)
					throw "Failed to close the client connection.";

				continue;//perform a jump back to the beginning of the loop
			}

			//std::thread tid(handle_client_request);
			//fill the http request method for user to query
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
				if(handleGetRequest(file_name, request, clientfd) == -1) {
					if(close(clientfd) == -1)
						throw "Failed to close the client connection.";

					continue;
				}
			}
	
			if(close(clientfd) == -1)
				throw "Failed to close the client connection.";

		}//end of try block
		catch(const char* message) {
			std::cerr << message << '\n';
			close(clientfd);
		}
		catch(...) {
			std::cerr << "Unknown error occured.\n";
		}
	}//end of while
}
