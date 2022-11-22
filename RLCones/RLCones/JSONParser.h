#pragma once
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class JSONParser
{
public:
	JSONParser();
	json ReadFile(std::string filePath);
	void WriteFile(std::string filePath, std::string data);
};

