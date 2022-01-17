#include <arpa/inet.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/types.h>
#include <string>
#include <fstream>
#include "httpReq.h"

#define unsigned int uint
#define PORT 5200
#define BUFFSIZE 512
#define REQUEST_SIZE 256

int main(int argc, char **argv)
{
	int sockfd, clientfd;
	std::fstream file;
	char req[REQUEST_SIZE];

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

		recv(clientfd, req, REQUEST_SIZE, 0);
		std::string request(req);
		
		//function for handling the type of request
		if(request.substr(0, 3).compare("GET") == 0) {
			if(handleGetRequest(request, file, clientfd) == -1) {
				if(close(clientfd) == -1)
					errexit("Failed to close the client connection.");

				//respond with 404 error
				continue;
			}
		}
		
		if(close(clientfd) == -1)
			errexit("Failed to close the client connection.");
	}
}
