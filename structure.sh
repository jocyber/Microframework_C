mkdir templates

if [ ! -f "source_files/server.cpp" ];
then
	touch source_files/server.cpp
	echo "#include <iostream>
#include <string>
#include \"httpReq.h\"

std::string mainLogic(const std::string &request) {
	;
}

int main(int argc, char **argv)
{
	web::app App;
	App.run(mainLogic);
}" > source_files/server.cpp
fi

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
