#include <iostream>
#include <string>
#include "httpReq.h"

//utilize function pointers to modify .run() behavior

int main(int argc, char **argv)
{
	//web application object
	web::app App;
	App.run();
}
