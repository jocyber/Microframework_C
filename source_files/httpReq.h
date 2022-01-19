#ifndef HTTP
#define HTTP

#include <arpa/inet.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <string>
#include <unistd.h>
#include <fstream>

const std::string HTTP_HEADER = "HTTP/1.1 200 OK\r\n\n";
const std::string HTTP_ERROR = "HTTP/1.1 404 Not Found\r\n\n";

#define PORT 5200
#define BUFFSIZE 512
#define REQUEST_SIZE 256

extern void errexit(const std::string message);

namespace web {
	class app {
		int clientfd, sockfd;
		struct sockaddr_in addr;
		std::fstream file;
		char req[REQUEST_SIZE];

		public:
			app();
			~app();
			void run(std::string(*mainLogic)(const std::string &request));
			int handleGetRequest(const std::string &request, std::fstream &file, int &clientfd);

		private:
			std::string getRequestedFile(const std::string &request);
	};
}

#endif
