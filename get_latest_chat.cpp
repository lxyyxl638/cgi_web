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

			unordered_map<string,string> ans;
			char * method = getenv("REQUEST_METHOD");
			if ( strcmp(method,"POST") == 0) {
				char *contentLength = getenv("CONTENT_LENGTH");
				int len;
				if (contentLength != NULL) {
					len = strtol(contentLength, NULL, 10);
				} else {
					len = 0;
				}
				int i, ch;
				string post_val="";
				for (i = 0; i < len; i++) {
					if ((ch = getchar()) < 0) {
						break;
					}
					post_val = post_val + (char) ch ;
				}
				ParseParam(post_val,ans);

			} else if(strcmp(method,"GET")==0){
				char* str = getenv("QUERY_STRING");
				string Param(str);
				ParseParam(Param,ans);
			}
			int argu_count = 0;
			
			
			char query_buf[1024] = {0};
			int user_id;
			
			user_id = atoi(session->getValue("user_id").c_str());

			snprintf(query_buf,sizeof(query_buf),"select count(*) as num,send_id,nickname,username from p2p_messages inner join users on send_id= user_id where rece_id=%d and p2p_messages.state=0 group by send_id",user_id);
			string query(query_buf);
			int flag = db->dbQuery(query,query_result);
				
			if(flag ){
				result = "success" ;

			}else{
				detail = "数据库操作错误!!";
			}
		}
	
		root["result"] = Json::Value(result);
		if(strcmp(result.c_str(),"success") == 0){
			Json::Value team;
			for(size_t i = 0 ; i != query_result.size(); i++ ){
				team["is_online"] = Json::Value((session->getOnline(atoi(query_result[i]["send_id"].c_str()))? "1":"0"));
				team["friend_id"] = Json::Value(query_result[i]["send_id"]);
				team["friend_username"] = Json::Value(query_result[i]["username"]);
				team["friend_nickname"] = Json::Value(query_result[i]["nickname"]);
				team["num"] = Json::Value(query_result[i]["num"]);
				root["friend_list"].append(team);
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
