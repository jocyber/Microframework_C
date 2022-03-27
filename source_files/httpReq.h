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
#include <vector>

const std::string HTTP_HEADER = "HTTP/1.1 200 OK\r\nCache-Control: no-cache";
const std::string HTTP_ERROR = "HTTP/1.1 404 Not Found\r\n\n";
const std::string HTTP_IF_MODIFIED = "HTTP/1.1 304 Not Modified\r\n\n";

typedef std::unordered_map<std::string, std::string> Map;

#define PORT 5000
#define BUFFSIZE 1000 // 1KB
#define POOLSIZE 5

extern void errexit(const std::string message);

//web app class
namespace web {
	class app {
		int sockfd;
		struct sockaddr_in addr;

		public:
			app();
			~app();
			void run();

		private:
			std::string getRequestedFile(const char *request, unsigned int start) const;
			void handleGetRequest(const std::string &name, const std::string &request, const int &clientfd) const;
			void handlePostRequest(const std::string &header, Map &form_data) const;
			std::string md5Hash(const std::string &filename) const;
			std::string getEtag(const std::string &req) const;	

			//defined by the user
			std::string mainLogic(const std::string &file, const Map &form, std::string &html_data) const;

			//working thread
			void worker_thread(void);
			void handle_client(int clientfd);
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
		if(close(fd) == -1)
			std::cerr << "Failed to close the file descriptor: {" << fd << "}\n";
	}
};

#endif
