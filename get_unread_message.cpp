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
			
			if(ans.find("request_uid") != ans.end())
				argu_count++;
			if(argu_count < 1) {
				detail = "参数错误！";
			}
			else
			{
				char query_buf[1024] = {0};
				int request_uid = atoi(ans["request_uid"].c_str());
				int my_uid = atoi(session->getValue("user_id").c_str());
				snprintf(query_buf,sizeof(query_buf),"select * from p2p_messages where send_id=%d and rece_id=%d and state=0 ",request_uid,my_uid);
				string query(query_buf);
				int flag = db->dbQuery(query,query_result);
				
				memset(query_buf,0,sizeof(query_buf));
				snprintf(query_buf,sizeof(query_buf),"update p2p_messages set state = 1 where send_id=%d and rece_id=%d and state=0 ",request_uid,my_uid);
				string query_update(query_buf);
				db->dbQuery(query_update);
					result = "success" ;

				
			} 
		}
		root["result"] = Json::Value(result);
		if(strcmp(result.c_str(),"success") == 0){
			for(size_t i = 0 ; i != query_result.size(); i++ ){
				Json::Value message;
				 message["message"] = Json::Value(query_result[i]["message"]);
				 message["send_time"] = Json::Value(query_result[i]["send_time"]);
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
