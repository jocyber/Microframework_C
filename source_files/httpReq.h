#ifndef HTTP
#define HTTP

#include <arpa/inet.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <netinet/tcp.h>
#include <stdexcept>

const std::string HTTP_HEADER = "HTTP/1.1 200 OK\r\nCache-Control: no-cache";
const std::string HTTP_ERROR = "HTTP/1.1 404 Not Found\r\n\n";
const std::string HTTP_IF_MODIFIED = "HTTP/1.1 304 Not Modified\r\n\n";

#define PORT 5000
#define BUFFSIZE 1024 // 1KB
#define REQUEST_SIZE 4096 // 4KB

using usint = unsigned short int;
extern void errexit(const std::string message);
extern int errclose(const std::string message, int fd);

namespace web {
	class app {
		int clientfd, sockfd;
		struct sockaddr_in addr;
		int file;
		char req[REQUEST_SIZE];

		std::string req_method = "", file_name = "";

		public:
			app();
			~app();
			void run(std::string(*mainLogic)(void));
			std::string getMethodType() const {return req_method;}
			std::string getFileName() const {return file_name;}

		private:
			std::string getRequestedFile(const std::string &request) const;
			int handleGetRequest(const std::string &name, const std::string &request);
			std::string md5Hash(const std::string &filename);
			std::string getEtag(const std::string &req);
	};
}

//custom exception object
class Custom_Error : std::exception {
	const char *message;

	Custom_Error(const char* _message) : message(_message) {}

	const char* what() const throw() {
		return message;
	}
};

#endif
