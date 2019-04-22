#include "web_tools.hpp"

std::string Tools::get_webdata(std::string website, std::string site) {
	std::list <std::string> p;
	
	return Tools::get_webdata(website, site, p);
}

std::string Tools::get_webdata(std::string website, std::string site, std::list <std::string> params) {
	sf::Http::Request request;
	sf::Http http(website);
	std::string parameters;
	
	for (auto it = params.begin(); it != params.end(); it++) {
		parameters += *it;
	}
	
	request.setMethod(sf::Http::Request::Get);
	request.setUri(site);
	request.setHttpVersion(1, 1); // HTTP 1.1
	request.setField("From", "sfml-radar");
	request.setField("Content-Type", "application/x-www-form-urlencoded");
	request.setBody(parameters);
	
	sf::Http::Response response = http.sendRequest(request);
	
	std::cout << "status: " << response.getStatus() << std::endl;
	std::cout << "HTTP version: " << response.getMajorHttpVersion() << "." << response.getMinorHttpVersion() << std::endl;
	std::cout << "Content-Type header:" << response.getField("Content-Type") << std::endl;
	std::cout << "body: " << response.getBody() << std::endl;
	
	return "";
}