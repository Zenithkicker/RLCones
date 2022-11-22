#pragma once
#include <nlohmann/json.hpp>
#include "CourseItem.h"
using json = nlohmann::json;

class Course
{
private:
	std::vector<CourseItem> _courseItems;

public:
	Course(json data);
	std::string CourseItemListToJson();
	void CreateCourseItemFromJson(const nlohmann::json& j);
	std::vector<CourseItem> GetCourseItems();
};

