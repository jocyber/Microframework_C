#include <arpa/inet.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/types.h>
#include <string>
#include <fstream>

#define unsigned int uint
#define PORT 5200
#define BUFFSIZE 512

void errexit(const std::string message);

int main(int argc, char **argv)
{
	std::string HTTP_OK = "HTTP/1.1 200 OK";
	std::string HTTP_ERROR = "HTTP/1.1 404 Not Found";
	int sockfd, clientfd;

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		errexit("Could not connect to the socket.");

	struct sockaddr_in addr;
	memset((struct sockaddr_in *) &addr, 0, sizeof(addr));

	addr.sin_port = htons(PORT);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;

	if(bind(sockfd, (struct sockaddr *) &addr, sizeof(addr)) == -1)
		errexit("Could not the port to the address.");

	listen(sockfd, 1);

	while(true) {
		if((clientfd = accept(sockfd, NULL, NULL)) == -1)
			errexit("Could not accept the oncoming connection.");
		
		send(clientfd, HTTP_OK.c_str(), BUFFSIZE, 0);

		if(close(clientfd) == -1)
			errexit("Failed to close the client connection.");
	}
}

void errexit(const std::string message) {
	std::cerr << message << '\n';
	exit(EXIT_FAILURE);
}
