/*
 *  Sane CLI.
 */

// Standard Libraries
#include <iostream>
#include <string>

// Sane++ Project specific
#include <api_handler.hpp>
#include <sapi_objs/sapi_test_static_json.hpp>

using namespace sane;

int main() {
    std::cout << "Sane CLI ~" << std::endl;

    // Instantiate the SapiTestStaticJson class.
    SapiTestStaticJson staticJsonResponse;

    // Assign it an object.
    staticJsonResponse = sapiTestStaticJsonResponse();

    //cout << staticJsonResponse << endl;
    staticJsonResponse.print();

    std::list<std::shared_ptr<YoutubeSubscription>> subscriptions;
    subscriptions = sapiGetSubscriptions();
    std::cout << "len(subs): " << subscriptions.size() << std::endl;

    return 0;
}