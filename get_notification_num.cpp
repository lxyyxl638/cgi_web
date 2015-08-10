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
		vector<unordered_map<string,string> >   query_result;
		int num=0;
		if(session->checkSession() == false){
			detail = detail + "unlogin";

		}else{

			char query_buf[1024] = {0};
			string user_id;
			user_id = session->getValue("user_id");

			snprintf(query_buf,sizeof(query_buf),"(SELECT no_id,type,send_id,notification.state,username,nickname,created_time,additional_message FROM notification inner join users on users.user_id=send_id where rece_id=%d and notification.state=0) union (SELECT no_id,type,rece_id as send_id,notification.state,username,nickname,created_time,additional_message FROM notification inner join users on users.user_id=rece_id where send_id=%d and notification.state=1)",atoi(user_id.c_str()),atoi(user_id.c_str()));
			string query(query_buf);
		         num = db->dbQuery(query);
			result = "success";
		}
		root["result"] = Json::Value(result);
		if(strcmp(result.c_str(),"success") == 0){
			root["num"] = Json::Value(num);
		} else {
		}

		string str = fw.write(root);
		FCGI_printf("Content-type: application/json\r\n"
			"\r\n"
			"%s",str.c_str());
		
		
	}
	return 0;
}
