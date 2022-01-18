mkdir app
mkdir templates

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
