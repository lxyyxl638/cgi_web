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

			snprintf(query_buf,sizeof(query_buf),"select b.send_id,a.username,b.message, b.send_time from users a INNER JOIN (select * from p2p_messages where rece_id = %d and state = 0) b on a.user_id = b.send_id;",atoi(user_id.c_str()));
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
			Json::Value message_list;
			for(size_t i = 0 ; i != query_result.size(); i++ ){
				Json::Value message;
				message["send_id"] = Json::Value(query_result[i]["send_id"]);
				message["send_name"] = Json::Value(query_result[i]["username"]);
				message["message"] = Json::Value(query_result[i]["message"]);
				message["created_time"] = Json::Value(query_result[i]["send_time"]);
				root["message_list"].append(message);
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
