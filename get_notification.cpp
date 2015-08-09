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
		if(session->checkSession() == false){
			detail = detail + "unlogin";

		}else{

			char query_buf[1024] = {0};
			string user_id;
			user_id = session->getValue("user_id");

			snprintf(query_buf,sizeof(query_buf),"select b.no_id, b.type, a.user_id, a.username, b.additional_message, b.created_time from users a INNER JOIN (select * from notification  where rece_id = %d and state = 0) b on a.user_id = b.rece_id union  select b.no_id, b.type, b.send_id, a.username, b.additional_message, b.created_time from users a INNER JOIN (select * from notification  where send_id = %d and state = 1) b on a.user_id = b.send_id",atoi(user_id.c_str()),atoi(user_id.c_str()));
			string query(query_buf);
			int flag = db->dbQuery(query,query_result);
			if(flag ){
				result = "success" ;

			}else{
				detail = "数据库擦操作错误！";
			}
		}
		root["result"] = Json::Value(result);
		if(strcmp(result.c_str(),"success") == 0){
			Json::Value notification_list;
			for(size_t i = 0 ; i != query_result.size(); i++ ){
				Json::Value notification;
				notification["no_id"] = Json::Value(query_result[i]["no_id"]);
				notification["type"] = Json::Value(query_result[i]["type"]);
				notification["send_id"] = Json::Value(query_result[i]["send_id"]);
				notification["send_name"] = Json::Value(query_result[i]["username"]);
				notification["message"] = Json::Value(query_result[i]["additional_message"]);
				notification["created_time"] = Json::Value(query_result[i]["created_time"]);
				root["notification_list"].append(notification);
			}

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
