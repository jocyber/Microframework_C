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

extern void errexit(const std::string message);

int handleGetRequest(const std::string &request, std::fstream &file, int &clientfd);
std::string getRequestedFile(const std::string &request);

#endif
