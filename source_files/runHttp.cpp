#include "httpReq.h"
#include <thread> // use C++ threads instead of pthreads
#include <mutex>
#include <queue>

std::queue<int> producedData;
std::mutex mtx;

/********** RUN FUNCTION **************/
void web::app::run() {
	int clientfd;
	std::thread pool[POOLSIZE];

	//create array of C++ threads here
	for(unsigned int i = 0; i < POOLSIZE; ++i)  {
		pool[i] = std::thread(&app::worker_thread, this);//pass worker thread that calls handle_client
		pool[i].detach();
	}

	//semaphores have a busy-waiting problem, so use conditional variables to reduce
	//wasted CPU cycles
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

			mtx.lock();
			producedData.push(clientfd);
			mtx.unlock();

		}//end of try block
		catch(const char* message) {
			std::cerr << message << '\n';
			//close(clientfd);
		}
		catch(...) {
			std::cerr << "Unknown error occured.\n";
		}
	}//end of while
}

//worker thread
void web::app::worker_thread(void) {
	while(true) {
		int client_sock = -1;

		mtx.lock();
		//critical section
		if(!producedData.empty()) {
			client_sock = producedData.front();
			producedData.pop();
		}
		mtx.unlock();

		if(client_sock != -1)
			handle_client(client_sock);
	}
}
