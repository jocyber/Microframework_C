#include "httpReq.h"
#include <thread> // use C++ threads instead of pthreads

/********** RUN FUNCTION **************/
void web::app::run() {
	//create thread pool here for better performance
	while(true) {
		try {
			if((clientfd = accept(sockfd, NULL, NULL)) == -1)
				throw "Could not accept the oncoming connection.";

			//receive http request
			if(recv(clientfd, req, BUFFSIZE, 0) == -1) {	
				if(close(clientfd) == -1)
					throw "Failed to close the client connection.";

				continue;//perform a jump back to the beginning of the loop
			}

			//use the on-demand model to create the threads for each client
			std::thread tid(&app::handle_client, this);
			tid.detach(); //continue execution without blocking the main thread
			
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
