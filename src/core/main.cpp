/*
 *  Sane Core.
 */

// Standard Libraries
#include <iostream>
#include <string>

// 3rd Party Libraries
#include <nlohmann/json.hpp>

// Sane++ Project specific
#include "config.hpp"
#include "api_handler.hpp"
#include <sapi_objs/sapi_test_static_json.hpp>

using json = nlohmann::json;
using namespace std;

int main(void)
{
	cout << PROJECT_NAME << " version: " << PROJECT_VER << endl;
	
	// Instantiate the SapiTestStaticJson class.
	SapiTestStaticJson staticJsonResponse;
	
	// Assign it an object.
	staticJsonResponse = sapiTestStaticJsonResponse();

	//cout << staticJsonResponse << endl;
	staticJsonResponse.print();
	return 0;
}
