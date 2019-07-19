/*
 *  Sane Core.
 */

// Standard Libraries
#include <iostream>
#include <string>

// 3rd Party Libraries
//#include <curl/curl.h>

// Sane++ Project specific
#include "config.h"
#include "api_handler.h"


using namespace std;

int main(void)
{
	cout << "project name: " << PROJECT_NAME << " version1: " << PROJECT_VER << endl;
	string staticJsonResponse = sapiTestStaticJson();
	cout << staticJsonResponse << endl;
	return 0;
}
