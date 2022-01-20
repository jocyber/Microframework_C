#include <iostream>
#include <string>
#include "httpReq.h"

web::app App;

//create a series of functions that handles different routes
//framework will assume that all html files are in the 'templates' directory
//if there are any subdirectories, the user must specify
std::string mainLogic(void) { 
	if(App.getFileName().compare("index.html") == 0) {
		return "index.html";
	}
	else if(App.getFileName().compare("test.html") == 0) 
		return "test.html";
	else
		return "404.html";
}

int main(int argc, char **argv)
{
	//web application object
	App.run(mainLogic);
}
