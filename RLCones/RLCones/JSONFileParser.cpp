#include "pch.h"
#include "JSONFileParser.h"

JSONFileParser::JSONFileParser()
{

}

json JSONFileParser::ReadFile(std::string filePath)
{
	try {
		LOG(filePath);
		std::ifstream f(filePath);
		LOG("file loaded. parsing...");
		json data = json::parse(f);
		LOG("file parsed: ");		
		return data;
	}
	catch (std::exception& e) {
		LOG("{}", e.what());
	}
	return json();
}

void JSONFileParser::WriteFile(std::string filePath, json jsonData)
{
	std::ofstream stream(filePath);
	stream << jsonData.dump();
}