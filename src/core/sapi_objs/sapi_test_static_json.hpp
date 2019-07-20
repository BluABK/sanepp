#ifndef SAPI_TEST_STATIC_JSON_HPP
#define SAPI_TEST_STATIC_JSON_HPP

// Standard libraries
#include <string>

// 3rd party libraries.
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class SapiTestStaticJson
{
public:
//		SapiTestStaticJson();

		void parse(json t_data);

		void print();

private:
		std::string m_one;
		std::string m_subitemOne;
		std::string m_subitemTwo;
		//std::string m_theList;  // FIXME: Lists aren't working.
		std::string m_three;
		std::string m_two;
};

#endif // Header guards