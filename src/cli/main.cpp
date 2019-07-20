/*
 *  Sane CLI.
 */

// Standard Libraries
#include <iostream>
#include <string>

// 3rd Party Libraries
//#include <nlohmann/json.hpp>

// Sane++ Project specific
#include <api_handler.hpp>
#include <sapi_objs/sapi_test_static_json.hpp>

using json = nlohmann::json;
using namespace std;

int main()
{
    cout << "Sane CLI ~" << endl;

	// Instantiate the SapiTestStaticJson class.
	SapiTestStaticJson staticJsonResponse;
	
	// Assign it an object.
	staticJsonResponse = sapiTestStaticJsonResponse();

	//cout << staticJsonResponse << endl;
	staticJsonResponse.print();

    list <shared_ptr<YoutubeSubscription>> subscriptions;
    subscriptions = sapiGetSubscriptions();
    cout << "len(subs): " << subscriptions.size() << endl;

	return 0;
}
