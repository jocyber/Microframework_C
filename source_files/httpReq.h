#ifndef HTTP
#define HTTP

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <iostream>
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

extern void errexit(const std::string message);

//web app class
namespace web {
	class app {
		int clientfd, sockfd;
		struct sockaddr_in addr;
		char req[REQUEST_SIZE];

		public:
			app();
			~app();
			void run(std::string(*mainLogic)(const std::string&, const std::string&));
			//friend void* handle_http_request(void* args);

		private:
			std::string getRequestedFile(const char request[]) const;
			int handleGetRequest(const std::string &name, const std::string &request, const int &clientfd) const;
			std::string md5Hash(const std::string &filename) const;
			std::string getEtag(const std::string &req) const;	
	};
}

//custom exception object
class File_Close : std::exception {
	const char *message;

public:
	File_Close(const char* _message) : message(_message) {}

	const char* what() const throw() {
		return message;
	}
};

#endif
