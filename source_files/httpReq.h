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
#include <unordered_map>
//download SQLAPI++ header for database connection

const std::string HTTP_HEADER = "HTTP/1.1 200 OK\r\nCache-Control: no-cache";
const std::string HTTP_ERROR = "HTTP/1.1 404 Not Found\r\n\n";
const std::string HTTP_IF_MODIFIED = "HTTP/1.1 304 Not Modified\r\n\n";

typedef std::unordered_map<std::string, std::string> Map;

#define PORT 5000
#define BUFFSIZE 1000 // 1KB

extern void errexit(const std::string message);

//web app class
namespace web {
	class app {
		int clientfd, sockfd;
		struct sockaddr_in addr;
		char req[BUFFSIZE];

		public:
			app();
			~app();
			void run();

		private:
			std::string getRequestedFile(const char *request, unsigned int start) const;
			void handleGetRequest(const std::string &name, const std::string &request, const int &clientfd) const;
			Map handlePostRequest(const std::string &header) const;
			std::string md5Hash(const std::string &filename) const;
			std::string getEtag(const std::string &req) const;	

			void handle_client(void);
			//defined by the user
			std::string mainLogic(const std::string &method, const std::string &file, const Map &form) const;
	};
}

//custom exception object
class File_Close : std::exception {
	const char *message;
	int fd;

public:
	File_Close(const char* _message, const int _fd) : message(_message), fd(_fd) {}

	const char* what() const throw() {
		return message;
	}

	void closeFd() const {
		close(fd);
	}
};

#endif
