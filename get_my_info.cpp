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
#include "include/public.h"
int main() {
	Database *db = Database::getInstance();
	Session *session = Session::getInstance();

	while (FCGI_Accept() >= 0) {
		session->sessionInit();
		Json::FastWriter fw;
		Json::Value root;
		string result("fail");
		string detail("");
		vector<unordered_map<string,string> >   query_result;
		if(session->checkSession() == false){
			detail = detail + "unlogin";

		}else{
			result = "success";					
		}
		root["result"] = Json::Value(result);
		if(strcmp(result.c_str(),"success") == 0){
			
			root["user_id"] = Json::Value(session->getValue("user_id"));
			root["nickname"] = Json::Value(session->getValue("nickname"));
			root["username"] = Json::Value(session->getValue("username"));
			

		}else{
			root["detail"] = Json::Value(detail);
		}
		
		string str = fw.write(root);
		FCGI_printf("Content-type: application/json\r\n"
			"\r\n"
			"%s",str.c_str());
		
		
	}
	return 0;
}
