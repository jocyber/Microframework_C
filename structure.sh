#make the html pages directory
if [ ! -d "templates" ]
	mkdir templates
fi

#create the initial code for the main dev file
if [ ! -f "source_files/server.cpp" ];
then
	touch source_files/server.cpp
	echo "#include <unordered_map>
#include \"httpReq.h\"

//map for encoding html page names
std::unordered_map<std::string, unsigned short> codes = {
	{\"index.html\", 1}, {\"test.html\", 2}
};

//create a series of functions that handles different routes
//framework will assume that all html files are in the 'templates' directory
//if there are any subdirectories, the user must specify
std::string web::app::mainLogic(const std::string &method, const std::string &file) { 
	unsigned short page = 0;
	page = codes[file];

	//return html pages based on codes map
	switch(page) {
		case 1: 
			return \"index.html\";

		default:
			return \"404.html\";
	}
}

//create or import functions for the routes here


//main function to run the server
int main(void)
{
	//web application object
	web::app App;
	App.run();
}" > source_files/server.cpp
fi

#index.html page generation
if [ ! -f "templates/index.html" ];
then
	touch templates/index.html;
	echo "<!DOCTYPE html>

<html>
	<head>
	</head>
				
	<body>
	</body>
</html>" > templates/index.html
fi

#404 html page
if [ ! -f "templates/404.html" ];
then
	touch templates/404.html;
	echo "<!DOCTYPE html>

<html>
	<head>
	</head>
				
	<body>
	<p>404 Error</p>
	</body>
</html>" > templates/404.html
fi
