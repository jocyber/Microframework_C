#include <iostream>
#include <string>
#include "httpReq.h"

//utilize function pointers to modify .run() behavior
std::string mainLogic(const std::string &request) {
	;
}

int main(int argc, char **argv)
{
	//web application object
	web::app App;
	App.run(mainLogic);
}
