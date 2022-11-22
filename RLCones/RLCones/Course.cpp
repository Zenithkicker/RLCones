#include "pch.h"
#include "Course.h"

Course::Course(json data) 
{
	try {		
		json j_array(data["data"]);	

		for (json::iterator it = j_array.begin(); it != j_array.end(); ++it) {
			LOG("Creating CourseItem");
			CreateCourseItemFromJson(*it);
		}
		
		int sz = static_cast<int>(_courseItems.size());		
		LOG("Item Count: " + std::to_string(sz));
	}
	catch (std::exception& e) {
		LOG("{}", e.what());
	}
}

std::string Course::CourseItemListToJson() 
{
	json root;
	json dataArray = json::array();

	for (std::vector<CourseItem>::iterator it = _courseItems.begin(); it != _courseItems.end(); ++it) {
		json d = nlohmann::json{
			{"x", it->X},
			{"b", it->Y},
			{"c", it->Z},
			{"isBig", it->IsBig}
		};
		dataArray.push_back(d);
	}
	root["data"] = dataArray;
	return root.dump();
}

void Course::CreateCourseItemFromJson(const nlohmann::json& j) 
{
	CourseItem ci = CourseItem();	
	j.at("x").get_to(ci.X);
	j.at("y").get_to(ci.Y);
	j.at("z").get_to(ci.Z);
	j.at("isBig").get_to(ci.IsBig);
	_courseItems.push_back(ci);
}

std::vector<CourseItem> Course::GetCourseItems()
{
	return _courseItems;
}