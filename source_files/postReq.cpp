#include "httpReq.h"

 Map web::app::handlePostRequest(const std::string &header) const {
	unsigned int i = header.length();
	Map form_data;

	//go back to the beginning of the line
	while(header.at(i - 1) != '\n') {i--;}

	std::string key = "";
	std::string value = "";
	bool isKey = true;

	for(; i < header.length(); ++i) {//O(n)
		if(header[i] == '=') {
			isKey = false;
		}
		else if(header[i] == '&') {
			isKey = true;
			form_data[key] = value;
			key.clear();
			value.clear();
		}
		else {
			if(isKey)
				key += header[i];
			else
				value += header[i];
		}
	}

	return form_data;
}
