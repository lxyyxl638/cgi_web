#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fcgiapp.h>
#include <string>
#include <cstring>
#include <unordered_map>
#include <fcgi_stdio.h>
#include "include/database.h"
#include "json/json.h"
#include "include/session.h"

int main() {
	Database *db = Database::getInstance();
	Session *session = Session::getInstance();

	while (FCGI_Accept() >= 0) {
		Json::FastWriter fw; 
		Json::Value root;
		string result("fail");
		string detail("");
		session->sessionInit();

		if (session->checkSession()) {
			session->destroySession();
			result = "success";
		} else {
			detail = "unlogin";
		}

		root["result"] = Json::Value(result.c_str());
		root["detail"] = Json::Value(detail.c_str());

		string str = fw.write(root);
		FCGI_printf("Content-type: application/json\r\n"
			"\r\n"
			"%s",str.c_str());
		
		
	}
	return 0;
}
