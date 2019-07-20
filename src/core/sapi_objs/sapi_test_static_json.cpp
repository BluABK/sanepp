// Standard libraries.
#include <iostream>
#include <string>

// 3rd party libraries.
#include <nlohmann/json.hpp>

// Project specfic libraries.
#include "sapi_test_static_json.hpp"

using json = nlohmann::json;
//using namespace std;

void SapiTestStaticJson::parse(json t_data)
{
	m_one = t_data[0]["one"];
	m_subitemOne = t_data[0]["sub_dict"]["subitem1"];
	m_subitemTwo = t_data[0]["sub_dict"]["subitem2"];
	//m_theList = t_data[0]["the list"]; // FIXME: Lists aren't working.
	m_three = t_data[0]["three"];
	m_two = t_data[0]["two"];
}

void SapiTestStaticJson::print()
{
	std::cout << "one: " << m_one << std::endl;
	std::cout << "two: " << m_two << std::endl;
	std::cout << "three: " << m_three << std::endl;
	std::cout << "subitem1: " << m_subitemOne << std::endl;
	std::cout << "subitem2: " << m_subitemTwo << std::endl;
//	std::cout << "the list: [";
	//for (std::string &text : m_theList)
//	for(auto & text : m_theList)
//		std::cout << text << ", ";
//	std::cout << "]" << std::endl;
}
