#include "pch.h"
#include "JSONParser.h"

JSONParser::JSONParser() 
{

}

json JSONParser::ReadFile(std::string filePath) 
{
	std::ifstream f(filePath);
	json data = json::parse(f);
	return data;
}

void JSONParser::WriteFile(std::string filePath, std::string data) 
{
	std::ofstream stream(filePath);
	stream << data;
}